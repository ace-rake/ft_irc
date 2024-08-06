#include "Server.hpp"
#include <iostream>

std::string Server::handleClientMessage(Client & client)
{
	char buffer[BUFFER_SIZE];
	int clientFd = client.getFd().fd;

	if (int valread = recv(clientFd, buffer, BUFFER_SIZE, 0))
	{
		buffer[valread] = '\0';
		std::string msg(buffer);

		std::cout << "Received from " << clientFd << ":\t" << msg << std::endl;
		/* logCommand(msg); */ //This is for logging commands

		try
		{
			commandHandler(msg, client);
		}

		catch (std::runtime_error &e)
		{
			std::cout << e.what() << std::endl;
		}

		return msg;
	}

	close(clientFd);
	clearClient(client);

	std::cout << "Client disconnected" << std::endl;

	return (std::string());
}

void	Server::broadCastMsg(std::string msg)
{
	for (int i = 0; i < MAX_CLIENTS && _clients[i].getFd().fd != -1; ++i)
	{
		std::cout << "sending msg..." << std::endl;
		_clients[i].sendMessageToClient(msg);
	}
}
