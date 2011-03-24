#include "database.h"
#include <sstream>

string
Database::listNewsgroups()
{
  ostringstream oss;
  oss << newsDb.size() - 1;
  for(MapType::iterator it = newsDb.begin(); it != newsDb.end(); ++it)
    {
      oss << it->first << " " << it->second.first.size() << " " << it->second.first << " ";
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
