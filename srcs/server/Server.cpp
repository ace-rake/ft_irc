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


	getIpAddress(); // Gets the first available ip address

}

// Destructor
Server::~Server(void)
{
	close(_server_fd);
}

bool    Server::findSuitableIp(struct hostent *host)
{
	for (int i = 0; host->h_addr_list[i] != NULL; i++)
	{
		struct in_addr  *addr = (struct in_addr *)host->h_addr_list[i];
		if (strcmp(inet_ntoa(*addr), "127.0.0.1") != 0)
		{
			this->_address.sin_addr = *addr;
			std::cout << "Using IP address: " << inet_ntoa(*addr) << std::endl;
			return true;
		}
	}
	return false;
}

void    Server::noSuitableIpFound(void)
{
	this->_address.sin_addr.s_addr = INADDR_ANY;
	std::cerr << "No suitable IP address found, using 0.0.0.0 instead" << std::endl;
}

void    Server::getIpAddress(void)
{
	char    hostname[256];
	if (gethostname(hostname, sizeof(hostname)) == -1)
	{
		perror("gethostname");
		exit(EXIT_FAILURE);
	}

	struct hostent  *host = gethostbyname(hostname);
	if (host == NULL)
	{
		perror("gethostbyname");
		exit(EXIT_FAILURE);
	}

	if (!findSuitableIp(host))
		noSuitableIpFound();
}

void Server::run() {
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

void Server::idle()
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
//TODO send client fd back to the client
void Server::handleNewConnection()
{
	int new_connection = accept(_server_fd, (struct sockaddr *) &_address, (socklen_t *)&_addrlen);
	for (int i = 1; i < MAX_CLIENTS ; ++i)
		if (_fds[i].fd == -1)
		{
			_fds[i].fd = new_connection;
			_fds[i].events = POLLIN;
			std::string number = std::to_string(new_connection);
			send(new_connection, number.c_str(), number.size(), 0);
			std::cout << "Client connected" << std::endl;
			break;
		}
}

void Server::handleClientMessage(int *fd)
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
