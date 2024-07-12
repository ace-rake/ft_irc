#include "Server.hpp"


void	Server::joinChannel(std::string name, client client)
{
	joinChannel(name, client, "");
}

void	Server::joinChannel(std::string name, client client, std::string psw)
{
	// Check if channel exists
	// if not create channel
	// add user to channel
	Channel * channel = findChannel(name);
	if (!channel)
		channel = createChannel(name, psw);
	channel->handleJoinRequest(client, psw);	
}

Channel *	Server::createChannel	(std::string name, std::string psw)
{
	Channel * new_channel = new Channel(name, psw);
	_channels.push_back(*new_channel);
	return new_channel;
}

Channel *	Server::findChannel(std::string name)
{
	for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
		if (it->getName().compare(name) == 0)
			return &(*it);
	return NULL;
}
