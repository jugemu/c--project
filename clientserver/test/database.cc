#include "database.hh"

string
Database::listNewsgroups()
{}

int
Database::addNewsgroup(string title)
{}

int
Database::delNewsgroup(int newsId)
{}

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
