#include "Server.hpp"
#include <iostream>
#include <fstream>

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

std::string Server::handleClientMessage(Client & client)
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
		logCommand(msg);
		try{
			commandHandler(msg, client);
		}
		catch (std::runtime_error &e)
		{
			std::cout << e.what();
		}
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

Client*	Server::getUser(userData field, std::string data)
{
	std::string (Client::*funcptr)()const;
	switch (field){
		case (NICK):
		{
			funcptr = &Client::getNickName;
			break;
		}
		case (USER):
		{
			funcptr = &Client::getUserName;
			break;
		}
		default:
			return (NULL);
	}
	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if ((_clients[i].*funcptr)() == data)
			return &_clients[i];

	}
	return NULL;
}


void	Server::logCommand(std::string str)
{
	std::ofstream out;
	out.open("Commands.txt", std::ios_base::app);
	out << str << std::endl;;
	out.close();
}

