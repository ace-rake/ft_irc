#include "server.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/poll.h>
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
	if ((_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}
	_fds[0].fd = _server_fd;
	_fds[0].events = POLLIN;
	for (int i = 1; i < MAX_CLIENTS; ++i)
		_fds[i].fd = -1;
	if (bind(_server_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
	{
		perror("bind failed");
		close(_server_fd);
		exit(EXIT_FAILURE);
	}

	if (listen(_server_fd, 1) < 0)
	{
		perror("listen");
		close(_server_fd);
		exit(EXIT_FAILURE);
	}

	std::cout << "Server listening on port " << PORT << std::endl;

	idle();
}

void server::idle()
{
	while (1)
	{
		int retval = poll(_fds, MAX_CLIENTS, -1);

		if (retval > 0)
		{
			if (_fds[0].revents & POLLIN)
				handleNewConnection();
			for (int i = 1; i < MAX_CLIENTS; ++i)
				if (_fds[i].fd != -1 && (_fds[i].revents & POLLIN))
					handleClientMessage(&(_fds[i].fd));
		}
	}
}

//TODO what to do if max clients
void server::handleNewConnection()
{
	int new_connection = accept(_server_fd, (struct sockaddr *) &_address, (socklen_t *)&_addrlen);
	for (int i = 1; i < MAX_CLIENTS ; ++i)
		if (_fds[i].fd == -1)
		{
			_fds[i].fd = new_connection;
			_fds[i].events = POLLIN;
			std::cout << "Client connected" << std::endl;
			break;
		}
}

void server::handleClientMessage(int *fd)
{
	char buffer[BUFFER_SIZE];
	int valread = read(*fd, buffer, BUFFER_SIZE);
	if (valread == 0)
	{
		close(*fd);
		*fd = -1;
		std::cout << "Client disconnected" <<std::endl;
	}
	else
	{
		buffer[valread] = '\0';
		std::cout << "Received:\t" << buffer << std::endl;
	}

}
