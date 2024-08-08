#include "Server.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

// Constructor
Server::Server(char **av, int argc): _server(_fds[0])
{
	_addrlen = sizeof(_address);

	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;

    int port = std::atoi(av[1]);
    if (1 > port || port > 65535)
	{
		errno = EINVAL;

        perror("Error: Port number");
		std::cerr << "Info: Should be between 1 and 65535\n";

        exit(errno);
    }

    _address.sin_port = htons(port);

    std::cout << "Port provided: " << port << std::endl; // Print the port number provided

	getIpAddress(av[1]); // Gets the first available ip address

    if (argc == 3)
	    _serverPassword = av[2];
    else
	    _serverPassword.clear();
}

void    Server::createSocket(void)
{
    if ((_server.fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("Error: Socket creation failed");
		exit(errno);
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
	uint8_t timeout = 0;
	while (bind(_server.fd, reinterpret_cast<struct sockaddr*>(&_address), sizeof(_address)) < 0)
	{
		if (!timeout)
		{
			std::cerr << "Bind failed for 0.0.0.0\nRetrying to bind " << inet_ntoa(this->_fall_back_address.sin_addr) <<  " (this may take a while)\n";
			this->_address = this->_fall_back_address;
		}

		else if (timeout > 35)
		{
			errno = ETIMEDOUT;
			perror("Error: Unable to bind");

			close(this->_server.fd);
			exit(errno);
		}

		sleep(5);
		timeout++;
	}

	std::cout << "Using IP address: " << inet_ntoa(this->_address.sin_addr) << '\n';
}

void    Server::listenIncomingConnections(void)
{
	if (listen(_server.fd, 1) < 0)
	{
		perror("Error: Listening to incoming connections");

		close(_server.fd);
		exit(errno);
	}
}

// Setup the server and then goes in idle state
void    Server::run(void)
{
    listenIncomingConnections();
	std::cout << "Server listening on port " << ntohs(_address.sin_port) << std::endl;

	idle();
}
