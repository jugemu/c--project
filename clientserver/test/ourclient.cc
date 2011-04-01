#include "connection.h"
#include <stdlib.h>
#include <iostream>
#include "protocol.h"
#include <sstream>
#include "netutils.h"

using namespace std;
using namespace client_server;
using namespace protocol;

string prettifyResponse(const string &input)
{
  istringstream iss(input);
  ostringstream oss;
  char respType;
  iss >> respType;
  iss >> respType;
  string tmp;
  while(iss.good())
    {
      getline(iss, tmp, '|');
      oss << tmp << endl;
    }

  return oss.str();
}

int main(int argc, char** argv)
{
  
  char* host = argv[1];
  int port = atoi(argv[2]);
  Connection conn(host, port);

  if(!conn.isConnected())
    {
      cout << "Failed to connect";
      return 1;
    }
  static const string cmdhelp = "listng -- no parameters returns a list of all avalible newsgroups\ncreateng $name -- creates a newsgroup with the title $name\ndelng $id -- deletes the newsgroup with #id\ncreateart $id $title $author $text -- creates an article in newsgroup #id\nlistart $id -- list all articles in the newsgroup #id\n getart $newsId $artId -- gets article #artId from newsgroup #newsId\n";
  string response;
  while(true)
    {
      cout << endl << "> ";
      string cmdline;
      getline(cin, cmdline);
      istringstream iss(cmdline);
      string cmd;
      iss >> cmd;
      ostringstream ncmd;

      if(cmd == "listng")
	ncmd << Protocol::COM_LIST_NG;

      else if(cmd == "createng")
	{
	  string title;
	  iss >> title;
	  ncmd << Protocol::COM_CREATE_NG;
	  ncmd << conToNetString(title);
	}
      else if(cmd == "delng")
	{
	  int newsId;
	  iss >> newsId;
	  ncmd << Protocol::COM_DELETE_NG;
	  ncmd << conToNetInt(newsId);
	}
      else if(cmd == "createart")
	{
	  int newsId;
	  string title, author, text;
	  iss >> newsId;
	  iss >> title;
	  iss >> author;
	  getline(iss, text);
	  ncmd << Protocol::COM_CREATE_ART;
	  ncmd << conToNetInt(newsId) << conToNetString(title)
	       << conToNetString(author) << conToNetString(text);
	}
      else if(cmd == "listart")
	{
	  int newsId;
	  iss >> newsId;
	  ncmd << Protocol::COM_LIST_ART;
	  ncmd << conToNetInt(newsId);
	}
      else if(cmd == "getart")
	{
	  int newsId, artId;
	  iss >> newsId;
	  iss >> artId;
	  ncmd << Protocol::COM_GET_ART;
	  ncmd << conToNetInt(newsId) << conToNetInt(artId);
	}
      else if(cmd == "help")
	{
	  cout << cmdhelp;
	  continue;
	}
      else
	{
	  cout << "Invalid command";
	  continue;
	}
      
      ncmd << Protocol::COM_END;
      string netcommand = ncmd.str();
      sendString(ncmd.str(), &conn);
      response = readCommand(&conn, Protocol::ANS_END);
      cout << prettifyResponse(response);
    }
  return 0;
}
