#include "database.h"
#include <sstream>
#include <protocol.h>
#include "netutils.h"
string
Database::listNewsgroups()
{
  ostringstream oss;
  writeNumber(oss, newsDb.size());
  for(MapType::iterator it = newsDb.begin(); it != newsDb.end(); ++it)
    {
      writeNumber(oss, it->first);
      writeString(oss, it->second.first);
    }
  return oss.str();
}

int
Database::addNewsgroup(string title)
{
  newsDb[newsDb.size() + 1] = make_pair(title, map<int, article>());
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
