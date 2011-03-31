#include "netutils.h"
#include <iostream>
#include "protocol.h"

//#define DEBUG
#define delimiter '|'
#define INTSIZE sizeof(int)

using namespace std;
using namespace protocol;
using namespace client_server;

string htonl(unsigned input)
{
  string res;
  int const size = INTSIZE;
  res.resize(size);
  for(int i = 0; i < size; ++i)
    {
      res[i] = ((input >> (8 * (size - i - 1))) & 0xFF);
    }
  return res;
}

unsigned ntohl(const char *input)
{
  //int val;
  int val = 0;
  for(int i = 0; i < INTSIZE; ++i)
    {
      val |= (input[i]) << 8 * (INTSIZE - i - 1);
    }
  return val;
}

string ntohs(const char *input)
{
  ostringstream oss;
  unsigned size = ntohl(input);
  if(size == 0)
    {
      cout << "SIZE IS Zero" << endl;
    }
  const char *start = input + INTSIZE;
  for(unsigned i = 0; i < size; ++i)
    {
      oss << start[i];
    }
  return oss.str();
}
  
string conToNetInt(int num)
{
  ostringstream oss;
  oss << Protocol::PAR_NUM;
#ifdef DEBUG
  oss << num << " ";
#else
  oss << htonl(num);
#endif
  return oss.str();
}

string conToNetString(string str)
{
  ostringstream oss;
  oss << Protocol::PAR_STRING;
#ifdef DEBUG
  oss << str.size() << " " << str << " ";
#else
  oss << htonl(str.size()) << str;
#endif
  return oss.str();
}


/*
 * Send the string 's' to a client.
 */
void sendString(const string& s, Connection* conn) {
  for (size_t i = 0; i < s.size(); ++i)
    {
      conn->write(s[i]);
    }
}

string readCommand(Connection* conn, char endbyte)
{
  ostringstream iss;
  ostringstream oss;
  oss << conn->read(); //command type
  char tmp = conn->read();
  
  while(tmp != endbyte)
    {
      iss << tmp;
      if(tmp == Protocol::PAR_NUM || tmp == Protocol::PAR_STRING)
	{
	  //hack to get around the fact that a number byte might be equal to 8 and
	  //therfore equal to COM_END
	  iss << conn->read();
	  iss << conn->read();
	  iss << conn->read();
	  iss << conn->read();
	}
      tmp = conn->read();
    }
  string input = iss.str();
  for(unsigned i = 0; i < input.size(); ++i)
    {
      if(input[i] == Protocol::PAR_NUM)
	{
	  oss << ntohl(&input[i + 1]) << " ";
	  i += INTSIZE;
	}
      else if(input[i] == Protocol::PAR_STRING)
	{
	  string tmp = ntohs(&input[i + 1]);
	  oss << tmp << delimiter;
	  i += tmp.size();
	}
    }
  oss << delimiter; //insert final delimiter so that we now where text in an article ends
  return oss.str();
}
