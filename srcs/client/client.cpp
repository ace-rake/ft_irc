#include "client.hpp"
#include <arpa/inet.h>
#include <cstdlib>
#include <string>
#include <unistd.h>

// Constructor
client::client(int port)
{
	_server_fd = -1;
}

// Destructor
client::~client(void)
{
}

void	client::send_message(std::string str)
{
	send(_server_fd, str.c_str(), str.size(), 0);
}

int	client::getClientFd()
{
	return _client_fd.fd;
}
