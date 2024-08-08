#include "../server/Server.hpp"
#include <iostream>

static std::string generateMsg(Client & client, std::string invitee, std::string channelName)
{
	std::string msg = ":"; 
	msg += client.getNickName();// This is what the invitee sees as who has invited them
	msg += "!";
	msg += client.getUserName();
	msg += "@";
	msg += client.getHostName();
	msg += " INVITE ";
	msg += invitee;
	msg += " :";
	msg += channelName;
	return msg;
}

void Server::inviteToChannel(std::string channelName, std::string inviteeNick, Client & sender)
{
	// Check if invitee exists
	Client * client = getUser(NICK, inviteeNick);
	if (!client)
	{
		std::cerr << "No client by such a nick" << std::endl;
		return ;
	}
	
	// Check if channel exists
	Channel *channel = findChannel(channelName);
	if (!channel)
	{
		std::cerr << "No channel by such a name" << std::endl;
		return ;
	}

	// get invitee id
	int clientId = client->getId();

	// Check if invitee is not already in channel
	if (channel->findClient(ID, clientId) != channel->getClients().end())
	{
		std::cerr << "Client already in channel" << std::endl;
	}

	// Check if invitee is not already invited
	if (channel->isInInviteList(clientId))
	{
		std::cerr << "Client already invited" << std::endl;
		return ;
	}

	// Add id to inviteList from channel
	channel->addInviteToList(clientId);

	// send invitee msg to let them know they are invited
	std::string msg = generateMsg(sender, inviteeNick, channelName);
	client->sendMessageToClient(msg);
}
