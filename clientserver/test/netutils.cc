#include "netutils.h"
#include <iostream>
#include "protocol.h"

//#define DEBUG
#define INTSIZE sizeof(int)

using namespace std;
using namespace protocol;

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


