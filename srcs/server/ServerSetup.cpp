#include "Server.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Constructor
Server::Server(void): _server(_fds[0])
{
	_addrlen = sizeof(_address);
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(PORT);

	getIpAddress(); // Gets the first available ip address
}

void    Server::createSocket(void)
{
    if ((_server.fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}
}

void    Server::setupPolling(void)
{		
    _fds[0].fd = _server.fd;
	_fds[0].events = POLLIN;
	_server.events = POLLIN;
	for (int i = 1; i < MAX_CLIENTS + 1; ++i)// The +1 offset is to compensate for the server fd being a t_fds[0]
	{
		_fds[i].fd = -1;
		_fds[i].events = POLLIN;
		_clients[i - 1].setFd(&_fds[i]);
	}
}

void    Server::bindSocketToAddress(void)
{
	size_t timeout = 0;
	while (bind(_server.fd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
	{
		if (!timeout)
		{
			std::cerr << "Bind failed for 0.0.0.0\nRetrying to bind " << inet_ntoa(this->_fall_back_address.sin_addr) <<  " (this may take a while)\n";
			this->_address = this->_fall_back_address;
		}

		else if (timeout > 179)
		{
			std::cerr << "Error: Unable to bind: Timeout\n";
			close(this->_server.fd);
			exit(EXIT_FAILURE);
		}

		sleep(1);
		timeout++;
	}
	std::cout << "Successfully bound to " << inet_ntoa(this->_address.sin_addr) << '\n';
}

void    Server::listenIncomingConnections(void)
{
	if (listen(_server.fd, 1) < 0)
	{
		perror("listen");
		close(_server.fd);
		exit(EXIT_FAILURE);
	}
}

// Setup the server and then goes in idle state
void    Server::run()
{
    listenIncomingConnections();
	std::cout << "Server listening on port " << PORT << std::endl;

	idle();
}
