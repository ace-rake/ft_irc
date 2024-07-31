#include "../server/Server.hpp"
#include <iostream>
#include <sstream>

#define ERR_NICKINUSE 443

void	Server::nickInUseError(Client & client)
{
	nickInUseError(client, client.getNickName());
}

void	Server::nickInUseError(Client & client, std::string nickName)
{
	std::stringstream converter;
	converter << ERR_NICKINUSE;

	std::string errorMessage = ":" + std::string("server_name") + " " + converter.str() + " * " + nickName + " :Nickname is already in use\r\n";
	client.sendMessageToClient(errorMessage);
}

bool	Server::nickInUse(std::string nickToCheck, int clientId)const
{
	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (_clients[i].getFd().fd == -1)
			continue ;
		if (_clients[i].getId() == clientId)
			continue ;
		if (_clients[i].getNickName().compare(nickToCheck) == 0)
			return true;
	}
	return false;
}

void	Server::setNewNick(Client & client, std::string newNick)
{
	/* :old_nick NICK new_nick */

	if (nickInUse(newNick, client.getId()))
	{
		std::cerr << "Server::setNewNick: Error: Nick already in use" << std::endl;
		nickInUseError(client, newNick);
		return ;
	}
	std::string oldNick = client.getNickName();
	client.setNickName(newNick);	
	client.sendMessageToClient(":" + oldNick + " NICK " + newNick);
}
