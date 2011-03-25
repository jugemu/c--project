#include "netutils.h"
#include "protocol.h"

//#define DEBUG

using namespace std;
using namespace protocol;

void writeNumber(ostringstream &oss, int num)
{
  oss << Protocol::PAR_NUM;
#ifdef DEBUG
  oss << num << " ";
#else
  oss << num;
#endif
}

void writeString(ostringstream &oss, string str)
{
  oss << Protocol::PAR_STRING;
#ifdef DEBUG
  oss << str.size() << " " << str << " ";
#else
    oss << str.size() << str;
#endif
}

string htonl(unsigned input)
{
  string res;
  res.resize(sizeof(input));
  for(int i = 0; i < sizeof(input); ++i)
    {
    }
}
