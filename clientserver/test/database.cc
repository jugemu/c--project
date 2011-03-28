#include "database.h"
#include <sstream>
#include <protocol.h>
#include "netutils.h"
#include <iostream>
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
{}

int
Database::addArticle(int newsId, string title, string author, string text)
{}

int delArticle(int newsId, int artId)
{}

string getArticle(int newsId, int artId)
{}

Database&
Database::instance()
{
  
  static Database db;
  return db;
}
