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

using namespace protocol;
using namespace std;
using client_server::Server;
using client_server::Connection;
using client_server::ConnectionClosedException;

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

/*
 * Send the string 's' to a client.
 */
void writeString(const string& s, Connection* conn) {
    for (size_t i = 0; i < s.size(); ++i)
      {
	conn->write(s[i]);
	
	if(s[i] == Protocol::PAR_NUM)
	  {
	    ++i;
	    unsigned tmp = i + sizeof(int);
	    for(; i < tmp; ++i)
	      {
		conn->write(s[i] - '0');
	      }
	  }
      }
}

string readCommand(Connection* conn)
{
  ostringstream iss;
  ostringstream oss;
  oss << conn->read(); //command type
  char tmp = conn->read();
  
  while(tmp != Protocol::COM_END)
    {
      iss << tmp;
      tmp = conn->read();
    }
  string input = iss.str();
  for(unsigned i = 0; i < input.size(); ++i)
    {
      if(input[i] == Protocol::PAR_NUM)
	{
	  oss << " " << ntohl(&input[i + 1]);
	  i += INTSIZE;
	}
      else if(input[i] == Protocol::PAR_STRING)
	{
	  string tmp = ntohs(&input[i + 1]);
	  oss << " " << tmp;
	  i += tmp.size();
	}
    }
  cout << oss.str() << endl;
  return oss.str();
}

string executeCommand(string &input)
{
  istringstream in(input);
  ostringstream response;
  string  result, name;
  char c, type, length;
  int id; //identification number
  in >> c;
  in >> type;
  in >> length; //this isn't the right length because we cut off stuff
  switch (c)
    {
    case Protocol::COM_LIST_NG:
      in >> id;
      response << Protocol::ANS_LIST_NG;
      result = Database::instance().listNewsgroups();
      response << result;
      break;
      
    case Protocol::COM_CREATE_NG:
      in >> name;
      response << Protocol::ANS_CREATE_NG;
      Database::instance().addNewsgroup(name);
      response << Protocol::ANS_ACK;
      break;
      
    case Protocol::COM_DELETE_NG:
      in >> id;
      response << Protocol::ANS_DELETE_NG << " ";
      Database::instance().delNewsgroup(id);
      response << Protocol::ANS_ACK << " ";
      break;

    case Protocol::COM_LIST_ART:
      break;
      
    case Protocol::COM_CREATE_ART:
      break;

    case Protocol::COM_DELETE_ART:
      break;

    case Protocol::COM_GET_ART:
      break;

    default:
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
	      cout << "Recived data\n";
	      string command = readCommand(conn);
	      string test = "this is a test";
	      string response = executeCommand(command);
 	      writeString(response, conn);
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
