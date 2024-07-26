#include "Server.hpp"
#include <iostream>

std::string Server::handleClientMessage(Client & client)
{
	char buffer[BUFFER_SIZE];
	int clientFd = client.getFd().fd;
	int valread = recv(clientFd, buffer, BUFFER_SIZE, 0);
	if (valread == 0)
	{
		close(clientFd);
		client.clear();
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
