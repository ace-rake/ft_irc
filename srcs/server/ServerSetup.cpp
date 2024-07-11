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
Server::Server(void)
{
	_addrlen = sizeof(_address);
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(PORT);

	getIpAddress();
}

void    Server::createSocket(void)
{
    if ((_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
}

void    Server::setupPolling(void)
{
    _fds[0].fd = _server_fd;
	_fds[0].events = POLLIN;
	for (int i = 1; i < MAX_CLIENTS; ++i)
		_fds[i].fd = -1;
}

void    Server::bindSocketToAddress(void)
{
    if (bind(_server_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
    {
        perror("bind failed");
        close(_server_fd);
        exit(EXIT_FAILURE);
    }
}

void    Server::listenIncomingConnections(void)
{
    if (listen(_server_fd, 1) < 0)
    {
        perror("listen");
        close(_server_fd);
        exit(EXIT_FAILURE);
    }
}

// Setup the server and then goes in idle state
void    Server::run()
{
    createSocket();

    setupPolling();
	
    bindSocketToAddress();

    listenIncomingConnections();

	std::cout << "Server listening on port " << PORT << std::endl;

	idle();
}
