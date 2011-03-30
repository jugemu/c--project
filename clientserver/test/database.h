
#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include <string>

using namespace std;

class Database
{
 public:
   // Return a string with the listed newsgroups
  virtual string listNewsgroups() = 0;
  
  // Create a new newsgroup and add it to the database
  virtual string addNewsgroup(string title) = 0;
  
  // Deletes the newsgroup
  virtual string delNewsgroup(int newsId) = 0;
  
  // Lists articles in newsgroup
  virtual string listArticles(int newsId) = 0;
  
  // Adds article to newsgroup
  virtual string addArticle(int newsId, string title, string author, string text) = 0;
  
  // Delete article in newsgroup
  virtual string delArticle(int newsId, int artId) = 0;
  
  // Gets article from newgroup
  virtual string getArticle(int newsId, int artId) = 0;

  virtual ~Database(){};
};

class RamDatabase : public Database
{
  typedef struct {
    string title;
    string author;
    string text;
  } article;
  
  typedef map<int, article> ArticleMap;
  typedef pair<string, ArticleMap> Newsgroup;
  
  typedef map<int, Newsgroup> MapType;
  typedef map<int, Newsgroup>::const_iterator MapIterator;
  
  typedef ArticleMap::const_iterator ArticleIterator;
  
public:

 RamDatabase() : free(0)
    {};

  // Return a string with the listed newsgroups
  string listNewsgroups();
  
  // Create a new newsgroup and add it to the database
  string addNewsgroup(string title);
  
  // Deletes the newsgroup
  string delNewsgroup(int newsId);
  
  // Lists articles in newsgroup
  string listArticles(int newsId);
  
  // Adds article to newsgroup
  string addArticle(int newsId, string title, string author, string text);
  
  // Delete article in newsgroup
  string delArticle(int newsId, int artId);
  
  // Gets article from newgroup
  string getArticle(int newsId, int artId);
  
    
private:
   // Declares a Map map<int, pair<string, map<int, article> > >
  MapType newsDb;
  map<string, bool> NBnames, ARTnames;
  unsigned long free; //ful hack...
};
#endif
