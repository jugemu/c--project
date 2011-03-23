/* myserver.cc: sample server program */

#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "database.h"
#include "protocol.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>

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
        conn->write(s[i]);
    conn->write('$');   // used to indicate end of the string
}

string readCommand(Connection* conn)
{
  ostringstream oss;
  char tmp = conn->read();; 
  while(tmp != Protocol::COM_END)
    {
      oss << tmp;
      tmp = conn->read();
    }  
  return oss.str();
}

string executeCommand(string &input)
{
  istringstream in(input);
  string c, answer, name;
  int newsId;
  in >> c;
  switch (c[0])
    {
    case Protocol::COM_LIST_NG:
      in >> newsId;
      answer = Database::instance().listNewsgroups();
      break;
      
    case Protocol::COM_CREATE_NG:
      in >> name;
      answer = Database::instance().addNewsgroup(name);
      break;
      
    case Protocol::COM_DELETE_NG:
      in >> newsId;
      answer = Database::instance().delNewsgroup(newsId);
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
  
  return answer;
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
	      printf("Recived data\n");
	      string command = readCommand(conn);
	      writeString(executeCommand(command), conn);
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
