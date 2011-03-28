#include "database.h"
#include "protocol.h"
#include "netutils.h"
#include <sstream>
#include <map>
#include <iostream>

using namespace std;
using namespace protocol;


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

string
Database::addNewsgroup(string title)
{
  ostringstream oss;
  newsDb[free] = make_pair(title, map<int, article>());
  //replace with hashing function later
  ++free;
  oss << Protocol::ANS_ACK;
  return oss.str();
}

string
Database::delNewsgroup(int newsId)
{
  ostringstream oss;
  int response = newsDb.erase(newsId);
  if(response != 0)
    {
      oss << Protocol::ANS_ACK;
    }
  else
    {
      oss << Protocol::ANS_NAK << Protocol::ERR_NG_DOES_NOT_EXIST;
    }
  return oss.str();
}

string
Database::listArticles(int newsId)
{
	// find returns an iterator, iterator is NULL if it points to the last element in the map
	ostringstream oss;
	string articles;

	if(newsDb.find(newsId) != newsDb.end()) {
		string newsgroupName = newsDb.find(newsId)->second.first;
		ArticleIterator mapItEnd = newsDb.find(newsId)->second.second.end();
		
		oss << Protocol::ANS_ACK;
		oss << conToNetInt(newsDb.find(newsId)->second.second.size());
		
		for(ArticleIterator mapItBegin = newsDb.find(newsId)->second.second.begin(); mapItBegin != mapItEnd; ++mapItBegin) {			
			// COM_LIST_ART num_p COM_END
			// ANS_LIST_ART [ANS_ACK num_p [num_p string_p]* |
			// ANS_NAK ERR_NG_DOES_NOT_EXIST] ANS_END
			oss << mapItBegin->first;
			oss << mapItBegin->second.title;
		}
		return oss.str();
	}
	
	oss << Protocol::ANS_NAK;
	oss << Protocol::ERR_NG_DOES_NOT_EXIST;
	return oss.str();
}

string
Database::addArticle(int newsId, string title, string author, string text)
{
	ostringstream oss;
	article art = {title, author, text};

	if(newsDb.find(newsId) != newsDb.end()) {
	
		// COM_CREATE_ART num_p string_p string_p string_p COM_END
		// ANS_CREATE_ART [ANS_ACK | ANS_NAK ERR_NG_DOES_NOT_EXIST] ANS_END
		newsDb.find(newsId)->second.second[free] = art;

		oss << Protocol::ANS_ACK;
		return oss.str();
	}
	oss << Protocol::ANS_NAK;
	oss << Protocol::ERR_NG_DOES_NOT_EXIST;
	return oss.str();
}

string 
Database::delArticle(int newsId, int artId)
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
Database::getArticle(int newsId, int artId)
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

Database&
Database::instance()
{
  
  static Database db;
  return db;
}
