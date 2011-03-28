
#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include <string>

using namespace std;

class Database
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

	// Creates a new instance of the Database
  static Database& instance();
  
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
  // Constructs an empty database
 Database() : free(0)
    {};
  
  // Declares a Map map<int, pair<string, map<int, article> > >
  MapType newsDb;
  unsigned long free; //ful hack...
};
#endif
