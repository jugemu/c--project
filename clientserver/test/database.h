
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

  typedef pair<string, map<int, article> > newsgroup;
  typedef map<int, newsgroup> MapType;
  
public:

	// Creates a new instance of the Database
  static Database& instance();
  ödsaliglsbhgidlökjfgvbawödiskfjgahvabögfodsilfkjnvbahfsudi
  
  // Return a string with the listed newsgroups
  string listNewsgroups();
  
  // Create a new newsgroup and add it to the database
  int addNewsgroup(string title);
  
  // Deletes the newsgroup
  int delNewsgroup(int newsId);
  
  // Lists articles in newsgroup
  string listArticles(int newsId);
  
  // Adds article to newsgroup
  int addArticle(int newsId, string title, string author, string text);
  
  // Delete article in newsgroup
  int delArticle(int newsId, int artId);
  
  // Gets article from newgroup
  string getArticle(int newsId, int artId);
    
private:
  // Constructs an empty database
  Database(){};
  
  // Declares a Map map<int, pair<string, map<int, article> > >
  MapType newsDb;
};
#endif
