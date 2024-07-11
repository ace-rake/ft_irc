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

// Destructor
Server::~Server(void)
{
	close(_server.fd);
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
	if ((_server.fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}
	_fds[0].fd = _server.fd;
	_fds[0].events = POLLIN;
	_server.events = POLLIN;
	for (int i = 1; i < MAX_CLIENTS + 1; ++i)// The +1 offset is to compensate for the server fd being a t_fds[0]
	{
		_fds[i].fd = -1;
		_fds[i].events = POLLIN;
		_clients[i - 1].setFd(&_fds[i]);
	}
	if (bind(_server.fd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
	{
		perror("bind failed");
		close(_server.fd);
		exit(EXIT_FAILURE);
	}

	if (listen(_server.fd, 1) < 0)
	{
		perror("listen");
		close(_server.fd);
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
			if (_server.revents & POLLIN)
				handleNewConnection();
			for (int i = 0; i < MAX_CLIENTS; ++i)
				if (_clients[i].getFd().fd != -1 && (_clients[i].getFd().revents & POLLIN))
					handleClientMessage(_fds[i].fd);
		}
	}
}

//TODO what to do if max clients
//TODO send client fd back to the client
void Server::handleNewConnection()
{
	int new_connection = accept(_server.fd, (struct sockaddr *) &_address, (socklen_t *)&_addrlen);
	for (int i = 0; i < MAX_CLIENTS ; ++i)
		if (_clients[i].getFd().fd == -1)
		{
			_clients[i].getFd().fd = new_connection;
			_clients[i].getFd().events = POLLIN;
			std::string number = std::to_string(new_connection);
			std::cout << "Client connected with fd " << new_connection << std::endl;
			createNewClient(_clients[i].getFd());

			break;
		}
}
std::string	Server::receiveUserData(struct pollfd client)
{
	std::string buffer;
	std::string str;
	bool user_received = false;

	while (!user_received) {
		buffer += handleClientMessage(client.fd, true);

		size_t pos;
		while ((pos = buffer.find("\r\n")) != std::string::npos) {
			str += buffer.substr(0, pos); // Extract the complete message
			str += " ";
			buffer.erase(0, pos + 2); // Remove the processed message

			std::cout << "Current message: " << str << std::endl;

			if (str.find("USER") != std::string::npos) {
				user_received = true;
				break;
			}
		}

		if (user_received) {
			std::cout << "Full USER command received: " << str << std::endl;
		}
	}
	return str;
}

void	Server::createNewClient(struct pollfd client)
{
	receiveUserData(client);
}

std::string Server::handleClientMessage(int &fd, bool silent)
{
	char buffer[BUFFER_SIZE];
	int valread = recv(fd, buffer, BUFFER_SIZE, 0);
	if (valread == 0)
	{
		close(fd);
		fd = -1;
		if (!silent)
			std::cout << "Client disconnected" <<std::endl;
	}
	else
	{
		buffer[valread] = '\0';
		std::string msg(buffer);
		if (!silent)
			std::cout << "Received from " << fd << ":\t" << msg << std::endl;
		return msg;
	}
	return ("");
}
