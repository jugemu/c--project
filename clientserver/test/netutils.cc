#include "netutils.h"
#include <arpa/inet.h>
#include "protocol.h"
using namespace std;

void writeNumber(ostringstream &oss, int num)
{
  oss << Protocol::PAR_NUM << " ";
  oss << htonl(num) << " ";
}

void writeString(ostringstream &oss, string str)
{
  oss << Protocol::PAR_STRING << " " << htonl(str.size()) << " " << str << " ";
}
