#include "../server/Server.hpp"


void	Server::joinHandler(std::vector<std::string> args, Client & client)
{
	if (args.size() == 2)
		joinChannel(args[1], client);
	else
		joinChannel(args[1], client, args[2]);
}

void	Server::joinChannel(std::string name, Client & client)
{
	joinChannel(name, client, "");
}

void	Server::joinChannel(std::string name, Client & client, std::string psw)
{
	// Check if channel exists
	// if not create channel
	// add user to channel
	Channel * channel = findChannel(name);
	if (!channel)
		channel = createChannel(name, psw);
	/* channel = findChannel(name); */
	channel->handleJoinRequest(client, psw);	
}

Channel *	Server::createChannel	(std::string name, std::string psw)
{
	Channel * new_channel = new Channel(name, psw);
	_channels.push_back(*new_channel);
	return findChannel(name);
}

Channel *	Server::findChannel(std::string name)
{
	for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
		if (it->getName().compare(name) == 0)
			return &(*it);
	return NULL;
}
