#include <sstream>
#include <string>
#include "connection.h"

std::string conToNetInt(int number);
std::string conToNetString(std::string theString);
unsigned ntohl(const char *input);
std::string ntohs(const char *input);
std::string hotnl(unsigned input);
void sendString(const std::string& s, client_server::Connection* conn);
std::string readCommand(client_server::Connection* conn, char endbyte);
