#include "database.h"
#include <sstream>
#include <protocol.h>
#include "netutils.h"
#include <map>
#include <iostream>

using namespace std;


string
Database::listNewsgroups()
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

int
Database::addNewsgroup(string title)
{
  if(title.size() != 0)
    {
        newsDb[newsDb.size() + 1] = make_pair(title, map<int, article>());
    }
  else
    {
      cout << "RECIVED EMPTY TITLE!!!!" << endl;
    }
  return 0;
}

int
Database::delNewsgroup(int newsId)
{
  newsDb.erase(newsId);
  return 0;
}

string
Database::listArticles(int newsId)
{
	// find returns an iterator, iterator is NULL if it points to the last element in the map
	string articles;
	if(newsDb.find(newsId) != newsDb.end()) {
		
		string newsgroupName = newsDb.find(newsId)->second.first;
		
		ArticleIterator mapItEnd = newsDb.find(newsId)->second.second.end();
		
		for(ArticleIterator mapItBegin = newsDb.find(newsId)->second.second.begin(); mapItBegin != mapItEnd; ++mapItBegin) {			
			articles = articles + "\n" + mapItBegin->second.title + "\n" + mapItBegin->second.author + "\n" + mapItBegin->second.text;
		}
	}
	return articles;
}

int
Database::addArticle(int newsId, string title, string author, string text)
{
	article art = {title, author, text};

	if(newsDb.find(newsId) != newsDb.end()) {
		(newsDb[newsId].second)[newsDb[newsId].second.size() + 1] = art;
	}
	return 0;
}

int 
Database::delArticle(int newsId, int artId)
{
	if(newsDb.find(newsId) != newsDb.end()) {
		if(newsDb.find(newsId)->second.second.find(artId) != newsDb.find(newsId)->second.second.end()) {
		
			// IF WE ARE HERE ARTICLE AND NEWSGROUP EXIST
			// LETS DELETE THE ARTICLE
			newsDb.find(newsId)->second.second.erase(artId);
		}
	}
	return 0;
}

string
Database::getArticle(int newsId, int artId)
{
	string art;
	if(newsDb.find(newsId) != newsDb.end()) {
			if(newsDb.find(newsId)->second.second.find(artId) != newsDb.find(newsId)->second.second.end()) {
		
				// IF WE ARE HERE ARTICLE AND NEWSGROUP EXIST
				// GET ARTICLE
				article temp = newsDb.find(newsId)->second.second.find(artId)->second;
				art += "\n" + temp.title + "\n" + temp.author + "\n" + temp.text;
				return art;
		}
	}
}

Database&
Database::instance()
{
  
  static Database db;
  return db;
}
