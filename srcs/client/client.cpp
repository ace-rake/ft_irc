#include "client.hpp"
#include <arpa/inet.h>
#include <cstdlib>
#include <string>
#include <unistd.h>

// Constructor
client::client()
{
	_server_fd = -1;
	_client_fd = NULL;
}

// Destructor
client::~client(void)
{
}

void	client::send_message(std::string str)
{
	send(_server_fd, str.c_str(), str.size(), 0);
}

struct pollfd &	client::getFd()const
{
	return *_client_fd;
}

void	client::setFd(struct pollfd * fd)
{
	_client_fd = fd;
}
