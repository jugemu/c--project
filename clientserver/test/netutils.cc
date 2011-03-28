#include "netutils.h"
#include "protocol.h"
#include <iostream>

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
      res[i] = '0' + ((input >> (4 * (size - i - 1))) & 0xFF);
    }
  return res;
}

unsigned ntohl(const char *input)
{
  int val;
  for(int i = INTSIZE; i > -1; --i)
    {
      val |= input[i] << 4 * i;
    }
  return val;
}

string ntohs(const char *input)
{
  ostringstream oss;
  unsigned size = ntohl(input);
  const char *start = input + INTSIZE;
  for(unsigned i = 0; i < size; ++i)
    {
      oss << start[i];
    }
  return oss.str();
}
  
void writeNumber(ostringstream &oss, int num)
{
  oss << Protocol::PAR_NUM;
#ifdef DEBUG
  oss << num << " ";
#else
  oss << htonl(num);
#endif
}

void writeString(ostringstream &oss, string str)
{
  oss << Protocol::PAR_STRING;
#ifdef DEBUG
  oss << str.size() << " " << str << " ";
#else
  oss << htonl(str.size()) << str;
#endif
}


