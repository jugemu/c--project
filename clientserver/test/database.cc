#include "database.h"
#include "protocol.h"
#include "netutils.h"
#include <sstream>
#include <map>
#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <cstdlib>
#include <fstream>
#include <string.h>
using namespace std;
using namespace protocol;


string
RamDatabase::listNewsgroups()
{
  ostringstream oss;
  oss << conToNetInt(newsDb.size());
  for(MapType::iterator it = newsDb.begin(); it != newsDb.end(); ++it)
    {
      oss << conToNetInt(it->first);
      oss << conToNetString(it->second.first);
    }
  return oss.str();
}

string
RamDatabase::addNewsgroup(string title)
{
  ostringstream oss;
  if(ARTnames.find(title) != ARTnames.end())
    {
      oss << Protocol::ANS_NAK << Protocol::ERR_NG_ALREADY_EXISTS;
    }
  else {
    ARTnames[title];
    newsDb[free] = make_pair(title, map<int, article>());
    //replace with hashing function later
    ++free;
    oss << Protocol::ANS_ACK;
  }
  return oss.str();
}

string
RamDatabase::delNewsgroup(int newsId)
{
  ostringstream oss;
  MapType::iterator tmp = newsDb.find(newsId);
  if(tmp != newsDb.end())
    {
      NBnames.erase(tmp->second.first);//delete the name
       newsDb.erase(tmp);
      oss << Protocol::ANS_ACK;
    }
  else
    {
      oss << Protocol::ANS_NAK << Protocol::ERR_NG_DOES_NOT_EXIST;
    }
  return oss.str();
}

string
RamDatabase::listArticles(int newsId)
{
	// find returns an iterator, iterator is NULL if it points to the last element in the map
	ostringstream oss;
	string articles;
	MapType::iterator it = newsDb.find(newsId);
	if(it != newsDb.end()) {
		string newsgroupName = it->second.first;
		ArticleIterator mapItEnd = it->second.second.end();
		
		oss << Protocol::ANS_ACK;
		oss << conToNetInt(it->second.second.size());
		
		for(ArticleIterator mapItBegin = it->second.second.begin(); mapItBegin != mapItEnd; ++mapItBegin) {			
		  // COM_LIST_ART num_p COM_END
		  // ANS_LIST_ART [ANS_ACK num_p [num_p string_p]* |
		  // ANS_NAK ERR_NG_DOES_NOT_EXIST] ANS_END
		  oss << conToNetInt(mapItBegin->first);
		  oss << conToNetString(mapItBegin->second.title);
		}
		return oss.str();
	}
	
	oss << Protocol::ANS_NAK;
	oss << Protocol::ERR_NG_DOES_NOT_EXIST;
	return oss.str();
}

string
RamDatabase::addArticle(int newsId, string title, string author, string text)
{
	ostringstream oss;
	article art = {title, author, text};
	if(newsDb.find(newsId) != newsDb.end()) {
	
		// COM_CREATE_ART num_p string_p string_p string_p COM_END
		// ANS_CREATE_ART [ANS_ACK | ANS_NAK ERR_NG_DOES_NOT_EXIST] ANS_END
		newsDb.find(newsId)->second.second[free] = art;
		++free;
		oss << Protocol::ANS_ACK;
		return oss.str();
	}
	oss << Protocol::ANS_NAK;
	oss << Protocol::ERR_NG_DOES_NOT_EXIST;
	return oss.str();
}

string 
RamDatabase::delArticle(int newsId, int artId)
{
	ostringstream oss;
	
	if(newsDb.find(newsId) != newsDb.end()) {
		if(newsDb.find(newsId)->second.second.find(artId) != newsDb.find(newsId)->second.second.end()) {
		
			// IF WE ARE HERE ARTICLE AND NEWSGROUP EXIST
			// LETS DELETE THE ARTICLE
			
			// COM_DELETE_ART num_p num_p COM_END
			// ANS_DELETE_ART [ANS_ACK |
			// ANS_NAK [ERR_NG_DOES_NOT_EXIST | ERR_ART_DOES_NOT_EXIST]] ANS_END
			newsDb.find(newsId)->second.second.erase(artId);
			oss << Protocol::ANS_ACK;
			return oss.str();
		} else {
				oss << Protocol::ANS_NAK;
				oss << Protocol::ERR_ART_DOES_NOT_EXIST;
				return oss.str();
		}
	} 
	oss << Protocol::ANS_NAK;
	oss << Protocol::ERR_NG_DOES_NOT_EXIST;
	return oss.str();
}

string
RamDatabase::getArticle(int newsId, int artId)
{
	ostringstream oss;
	string art;
	
	if(newsDb.find(newsId) != newsDb.end()) {
			if(newsDb.find(newsId)->second.second.find(artId) != newsDb.find(newsId)->second.second.end()) {
		
				// IF WE ARE HERE ARTICLE AND NEWSGROUP EXIST
				// GET ARTICLE
				
				// COM_GET_ART num_p num_p COM_END
				// ANS_GET_ART [ANS_ACK string_p string_p string_p |
				// ANS_NAK [ERR_NG_DOES_NOT_EXIST | ERR_ART_DOES_NOT_EXIST]] ANS_END

				article temp = newsDb.find(newsId)->second.second.find(artId)->second;
				oss << Protocol::ANS_ACK;			
      	oss << conToNetString(temp.title);
      	oss << conToNetString(temp.author);
      	oss << conToNetString(temp.text);
      	
				return oss.str();
		} else {
				oss << Protocol::ANS_NAK;
				oss << Protocol::ERR_ART_DOES_NOT_EXIST;
				return oss.str();
		}
	} 
	oss << Protocol::ANS_NAK;
	oss << Protocol::ERR_NG_DOES_NOT_EXIST;
	return oss.str();
}

const char* DiskDatabase::rootpath = "/tmp/diskdatabase";

DiskDatabase::DiskDatabase()
{
  mkdir(rootpath, 0777);
}

string
DiskDatabase::listNewsgroups()
{
  DIR* root = opendir(rootpath);
  ostringstream oss, osstmp;
  struct dirent* dirp;
  int count = 0;
  int id;
  string name;
  while((dirp = readdir(root)))
    {
      if(dirp->d_name[0] == '.')
	 continue;

      ++count;
      istringstream iss(dirp->d_name);
      iss >> id;
      iss.get(); //throw away space
      getline(iss, name);
      osstmp << conToNetInt(id) << conToNetString(name);
    }
  closedir(root);
  oss << conToNetInt(count) << osstmp.str();
  return oss.str();
}

string
DiskDatabase::addNewsgroup(string title)
{
  ostringstream path, oss;
  struct dirent* tmp;
  while((tmp = findNewsgroup(free)) != NULL)
    {
      ++free;
      delete tmp;
    }

  if(isNgNameTaken(title))
    {
      oss << Protocol::ANS_NAK << Protocol::ERR_NG_ALREADY_EXISTS;
      return oss.str();
    }
  
  path << rootpath << '/' << free << " " << title;
  ++free;
  mkdir(path.str().c_str(), 0777);
  oss << Protocol::ANS_ACK;
  return oss.str();}


string
DiskDatabase::delNewsgroup(int newsId)
{
  struct dirent* dirp = findNewsgroup(newsId);
  ostringstream cmd, oss;
  if(dirp == NULL)
    {
      oss << Protocol::ANS_NAK << Protocol::ERR_NG_DOES_NOT_EXIST;
      return oss.str();
    }
  
  cmd << "rm -r ";
  cmd << "\"" << rootpath << '/' << dirp->d_name << "\"";
  system(cmd.str().c_str());
  delete dirp;
  oss << Protocol::ANS_ACK;
  return oss.str();
}

string
DiskDatabase::listArticles(int newsId)
{
  struct dirent* ng = findNewsgroup(newsId);
  ostringstream path, osstmp, oss;
  if(ng == NULL)
    {
      oss << Protocol::ANS_NAK << Protocol::ERR_NG_DOES_NOT_EXIST;
      return oss.str();
    }
  path << rootpath << '/' << ng->d_name;
  DIR* root = opendir(path.str().c_str());
  struct dirent* tmp;
  int count = 0, id;
  string name;
  while((tmp = readdir(root)))
    {
      if(tmp->d_name[0] == '.')
	 continue;
      
      ++count;
      istringstream iss(tmp->d_name);
      iss >> id;
      iss.get(); //throw away space
      getline(iss, name);
      osstmp << conToNetInt(id) << conToNetString(name);
    }
  closedir(root);
  oss << Protocol::ANS_ACK;
  oss << conToNetInt(count) << osstmp.str();
  delete ng;
  return oss.str();
}

string
DiskDatabase::addArticle(int newsId, string title, string author, string text)
{
  struct dirent* ng = findNewsgroup(newsId);
  ostringstream path, oss;
  if(ng == NULL)
    {
      oss << Protocol::ANS_NAK << Protocol::ERR_NG_DOES_NOT_EXIST;
      return oss.str();
    }
  
  while(findArt(ng, free) != NULL)
    ++free;

  string tmp(ng->d_name);
  path << rootpath << '/' << tmp << '/' << free << " " << title;
  ofstream of(path.str().c_str(), ofstream::out);
  of << author << "\n" << text;
  of.close();
  oss << Protocol::ANS_ACK;
  delete ng;
  return oss.str();
}

string
DiskDatabase::delArticle(int newsId, int artId)
{
  ostringstream path, oss, cmd;
  struct dirent* ng = findNewsgroup(newsId);
  if(ng == NULL)
    {
      oss << Protocol::ANS_NAK << Protocol::ERR_NG_DOES_NOT_EXIST;
      return oss.str();
    }
  
  struct dirent* art = findArt(ng, artId);
  if(art == NULL)
    {
      oss << Protocol::ANS_NAK << Protocol::ERR_ART_DOES_NOT_EXIST;
      return oss.str();
    }
  path << "\"" << rootpath << '/' << ng->d_name << '/' << art->d_name << "\"";
  cmd << "rm " << path.str();
  system(cmd.str().c_str());
  oss << Protocol::ANS_ACK;
  delete ng;
  delete art;
  return oss.str();
}

string
DiskDatabase::getArticle(int newsId, int artId)
{
  ostringstream oss;
  struct dirent* ng = findNewsgroup(newsId);
  if(ng == NULL)
    {
      oss << Protocol::ANS_NAK << Protocol::ERR_NG_DOES_NOT_EXIST;
      return oss.str();
    }
  struct dirent* art = findArt(ng, artId);
  if(art == NULL)
    {
      oss << Protocol::ANS_NAK << Protocol::ERR_ART_DOES_NOT_EXIST;
      return oss.str();
    }
  ostringstream path;
  path << rootpath << '/' << ng->d_name << '/' << art->d_name;
  ifstream artData(path.str().c_str(), ifstream::in);
  string title, author, text;
  istringstream iss(art->d_name);
  int id;
  iss >> id;
  iss.get();//throw away space
  getline(iss, title);
  getline(artData, author);
  getline(artData, text, '|');
  artData.close();
  oss << Protocol::ANS_ACK;
  oss << conToNetString(title) << conToNetString(author) << conToNetString(text);
  delete ng;
  delete art;
  return oss.str();
}

struct dirent*
DiskDatabase::findNewsgroup(int newsId)
{
  DIR* root = opendir(rootpath);
  struct dirent* dirp;
  int id;
  while((dirp = readdir(root)))
    {
      if(dirp->d_name[0] == '.')
	 continue;

      istringstream iss(dirp->d_name);
      iss >> id;
      if(id == newsId)
	  break;
    }
  if(dirp == NULL)
    {
      closedir(root);
      return NULL;
    }

  struct dirent* safereturn = new struct dirent;
  *safereturn = *dirp;
  closedir(root);
  return safereturn;
}
      
struct dirent*
DiskDatabase::findArt(struct dirent* ng, int artId)
{
  if(ng == NULL)
      return NULL;
  
  ostringstream path;
  char tmp[256];
  strcpy(tmp, ng->d_name);
  path << rootpath << '/'  << tmp;
  DIR* root = opendir(path.str().c_str());
  struct dirent* dirp;
  while((dirp = readdir(root)))
    {
      if(dirp->d_name[0] == '.')
	 continue;
      
      istringstream fullpath(dirp->d_name);
      int id;
      fullpath >> id;
      if(id == artId)
	{
	  break;
	}
    }
  if(dirp == NULL)
    {
      closedir(root);
      return NULL;
    }
  
  struct dirent* safereturn = new struct dirent;
  *safereturn = *dirp;
  closedir(root);
  return safereturn;
}

bool
DiskDatabase::isNgNameTaken(string title)
{
  DIR* root = opendir(rootpath);
  struct dirent* dirp;
  int id;
  string name;
  while((dirp = readdir(root)))
    {
      if(dirp->d_name[0] == '.')
	 continue;

      istringstream iss(dirp->d_name);
      iss >> id;
      iss.get();//throw away space
      getline(iss, name);
      if(name == title)
	  break;
    }
  bool returnval = dirp != NULL;
  closedir(root);
  return returnval;
}

bool
DiskDatabase::isArtNameTaken(struct dirent* ng, string title)
{
  ostringstream path;
  char tmp[256];
  strcpy(tmp, ng->d_name);
  path << rootpath << '/'  << tmp;
  DIR* root = opendir(path.str().c_str());
  struct dirent* dirp;
  int id;
  string name;
  while((dirp = readdir(root)))
    {
      if(dirp->d_name[0] == '.')
	 continue;
      
      istringstream fullpath(dirp->d_name);
      fullpath >> id;
      fullpath.get();//throw away space
      getline(fullpath, name);
      if(name == title)
	{
	  break;
	}
    }
  bool returnval = dirp != NULL;
  closedir(root);
  return returnval;
}
