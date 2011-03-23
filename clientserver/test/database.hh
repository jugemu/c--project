#include <map>
#include <string>

using namespace std;

#ifndef DATABASE_H
#define DATABASE_H

class Database
{
  typedef struct {
    string title;
    string author;
    string text;
  } article;

  typedef pair<string, map<int, article> > newsgroup;
  
public:
  string listNewsgroups();
  int addNewsgroup(string title);
  int delNewsgroup(int newsId);
  string listArticles(int newsId);
  int addArticle(int newsId, string title, string author, string text);
  int delArticle(int newsId, int artId);
  string getArticle(int newsId, int artId);
  static Database& instance();
  
private:
  Database(){};
  map<int, newsgroup> database;
};
#endif
