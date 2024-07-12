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
	close(_server.fd);
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
					handleClientMessage(_clients[i]);
		}
	}
}

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
			createNewClient(_clients[i]);
			break;
		}
}

//TODO delete client at some point
std::string	Server::receiveUserData(struct pollfd client)
{
	std::string buffer;
	std::string str;
	bool user_received = false;

	while (!user_received) {
		buffer += readUserData(client.fd);

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
std::string Server::readUserData(int &fd)
{
	char buffer[BUFFER_SIZE];
	int valread = recv(fd, buffer, BUFFER_SIZE, 0);
	if (valread == 0)
	{
		close(fd);
		fd = -1;
		std::cout << "Client disconnected" <<std::endl;
	}
	else
	{
		buffer[valread] = '\0';
		std::string msg(buffer);
		return msg;
	}
	return ("");
}

void	Server::createNewClient(client & client)
{
	std::string userData = receiveUserData(client.getFd());
	client.create(userData);
	std::cout << "create user\n" << client << std::endl;

	// Welcome handshake
	std::string welcomeMessage = ":serverhostname 001 " + client.getNickName() + " :Welcome to the IRC network, " + client.getNickName() + "!\r\n";
	client.sendMessageToClient(welcomeMessage);
	client.sendMessageToClient("Anotha one");
	broadCastMsg("Fuck you");
}

std::string Server::handleClientMessage(client & client)
{
	char buffer[BUFFER_SIZE];
	int clientFd = client.getFd().fd;
	int valread = recv(clientFd, buffer, BUFFER_SIZE, 0);
	if (valread == 0)
	{
		close(clientFd);
		clientFd = -1;
		std::cout << "Client disconnected" <<std::endl;
	}
	else
	{
		buffer[valread] = '\0';
		std::string msg(buffer);
		std::cout << "Received from " << clientFd << ":\t" << msg << std::endl;
		commandHandler(msg, client);
		return msg;
	}
	return ("");
}

void	Server::broadCastMsg(std::string msg)
{
	for (int i = 0; i < MAX_CLIENTS && _clients[i].getFd().fd != -1; ++i)
	{
		std::cout << "sending msg..." << std::endl;
		_clients[i].sendMessageToClient(msg);
	}

}
