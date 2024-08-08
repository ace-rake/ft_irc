#include <cstddef>
#include <vector>
#include "../client/Client.hpp"
#include "../irc.h"
#include <iostream>
#include "../channel/Channel.hpp"

Channel* findChannelByNameKick(const std::string& channelName, std::vector<Channel>& channels)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
		if (it->getName() == channelName) {
			return &(*it);
		}
	}
	return NULL;
}

void    kickNoChannelFound(Client& sender)
{
	std::cerr << "Error: No channel found for KICK command." << std::endl;
	sender.sendMessageToClient("403 " + sender.getNickName() + " " + "No channel found with this name");
}

void    kickNoPerms(Client& sender)
{
	std::cerr << "Error: Sender doesn't have OP rights." << std::endl;
	sender.sendMessageToClient("482 " + sender.getNickName() + " " + "You're not channel operator");
}

void    kickNoVictim(Client& sender)
{
	std::cerr << "Error: No victim specified." << std::endl;
	sender.sendMessageToClient("461 " + sender.getNickName() + " KICK :Not enough parameters");
}

void    kickHandler(std::vector<std::string> args, Client& sender, std::vector<Channel>& channels)
{
	if (args.size() < 2) {
		return kickNoVictim(sender);
	}

	Channel* wantedChannel = findChannelByNameKick(args[1], channels);

	if (!wantedChannel) {
		return kickNoChannelFound(sender);
	}

	if (!wantedChannel->clientIsOperator(sender)) {
		return kickNoPerms(sender);
	}

	wantedChannel->kickUser(sender, args[2], args);
}
