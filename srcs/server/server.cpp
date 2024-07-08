#include "server.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

// Constructor
server::server(int port)
{
	_addrlen = sizeof(_address);

	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(port);



}

// Copy constructor
server::server(const server& other)
{
	std::cout << "server:\tcopying object\n";
	*this = other;
}

// Copy assignment operator overload
server& server::operator = (const server& other)
{
	std::cout << "server:\tcopying object (assignment)\n";
	if (this != &other)
	{
		/* this->_=other._ */
	}
	return (*this);
}

// Destructor
server::~server(void)
{
	close(_server_fd);
}

void server::run() {
}
