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

// Destructor
Server::~Server(void)
{
	close(_server_fd);
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
					handleClientMessage(_fds[i].fd);
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
			std::cout << "Client connected with fd " << new_connection << std::endl;
			createNewClient(_fds[i]);

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
