/* myserver.cc: sample server program */

#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "database.h"
#include "protocol.h"
#include "netutils.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>

#define INTSIZE sizeof(int)
#define delimiter '|'

using namespace protocol;
using namespace std;
using client_server::Server;
using client_server::Connection;
using client_server::ConnectionClosedException;

static RamDatabase db;
/*
 * Read an integer from a client.
 */
int readNumber(Connection* conn) {
    unsigned char byte1 = conn->read();
    unsigned char byte2 = conn->read();
    unsigned char byte3 = conn->read();
    unsigned char byte4 = conn->read();
    return (byte1 << 24) | (byte2 << 16) | 
        (byte3 << 8) | byte4;
}


string executeCommand(string &input)
{
  istringstream in(input);
  ostringstream response;
  string  result, name, author, text;
  char c;
  int newsID, artID; //identification number
  in >> c;
  switch (c)
    {
    case Protocol::COM_LIST_NG:
      response << Protocol::ANS_LIST_NG;
      response << db.listNewsgroups();
      break;
      
    case Protocol::COM_CREATE_NG:
      getline(in, name, delimiter);
      response << Protocol::ANS_CREATE_NG;
      result =  db.addNewsgroup(name);
      response << result;
      break;
      
    case Protocol::COM_DELETE_NG:
      in >> newsID;
      response << Protocol::ANS_DELETE_NG;
      response << db.delNewsgroup(newsID);
      break;

    case Protocol::COM_LIST_ART:
      in >> newsID;
      response << Protocol::ANS_LIST_ART;
      response << db.listArticles(newsID);
      break;
      
    case Protocol::COM_CREATE_ART:
      in >> newsID;
      in.get(); //throw away extra space
      getline(in, name, delimiter);
      getline(in, author, delimiter);
      getline(in, text, delimiter);
      response << Protocol::ANS_CREATE_ART;
      response << db.addArticle(newsID, name, author, text);
      break;

    case Protocol::COM_DELETE_ART:
      in >> newsID;
      in >> artID;
      response << Protocol::ANS_DELETE_ART;
      response << db.delArticle(newsID, artID);
      break;

    case Protocol::COM_GET_ART:
      in >> newsID;
      in >> artID;
      response << Protocol::ANS_GET_ART;
      response << db.getArticle(newsID, artID);
      break;

    default:
      cout << "Recived unknown command: " << c << endl;
      break;
    }
  
  response << Protocol::ANS_END;
  return response.str();
}

int main(int argc, char* argv[]){
    if (argc != 2) {
        cerr << "Usage: myserver port-number" << endl;
        exit(1);
    }
    
    Server server(atoi(argv[1]));
    if (! server.isReady()) {
        cerr << "Server initialization error" << endl;
        exit(1);
    }
    
    while (true) {
        Connection* conn = server.waitForActivity();
        if (conn != 0) {
            try {
	      string command = readCommand(conn, Protocol::COM_END);
	      string response = executeCommand(command);
 	      sendString(response, conn);
            }
            catch (ConnectionClosedException&) {
                server.deregisterConnection(conn);
                delete conn;
                cout << "Client closed connection" << endl;
            }
        }
        else {
            server.registerConnection(new Connection);
            cout << "New client connects" << endl;
        }
    }
}
