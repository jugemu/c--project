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
/*

  typedef pair<string, map<int, article> > newsgroup;
  typedef map<int, newsgroup> MapType;
  
*/
string
Database::listArticles(int newsId)
{
	MapIterator it = newsDb.find(newsId);
	if(it != newsDb.end()) {
		
		ArticleIterator artBegin = it->second.second.begin;
		ArticleIterator artEnd = it->second.second.end;
		
		string articles;
		
		for(; artBegin != artEnd; ++artBegin) {
			articles = "\n" + articles + artBegin->second.title + "\n" + artBegin->second.author + "\n" + artBegin->second.text + "\n";
		}
		return articles;
	}
	return '0';
	
}

int
Database::addArticle(int newsId, string title, string author, string text)
{
	MapIterator it = newsDb.find(newsId);
	if(it != newsDb.end()){
		article art(title,author,text);
		it->second.second.insert(art);
		return 1;
	}
	return 0;
}

int delArticle(int newsId, int artId)
{
	MapIterator it = newsDb.find(newsId);
	if(it != newsDb.end()){
		ArticleIterator articleIt = it->second.second.find(artId);
		if(articleIt != it->second.second.end()){
			it->second.second.erase(articleIt);
		} 
		return 1;
	}
	return 0;
}

string getArticle(int newsId, int artId)
{
	MapIterator it = newsDb.find(newsId);
	if(it != newsDb.end()){
		rticleIterator articleIt = it->second.second.find(artId);
		string art;
		if(articleIt != it->second.second.end()){
			art = articleIt->title + "\n" + articleIt->author + "\n" articleIt->text;
		}
		return art;
	}
	return '0';
}

Database&
Database::instance()
{
  
  static Database db;
  return db;
}
