#include <cstddef>
#include <vector>
#include "../client/Client.hpp"
#include "../irc.h"
#include <iostream>
#include "../channel/Channel.hpp"

Channel* findChannelByName(const std::string& channelName, std::vector<Channel>& channels)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
		if (it->getName() == channelName) {
			return &(*it);
		}
	}
	return NULL;
}

/*
   void    kickHandler(std::vector<std::string> args, Client &executor, std::vector<Channel>& channels)
   {
   if (args.size() < 3)
   {
   std::cerr << "Error: Not enough parameters for KICK command." << std::endl;
   return;
   }

   std::string channelName = args[1];
   std::string victimName = args[2];

   Channel*    channel = findChannelByName(channelName, channels);
   if (!channel)
   {
   std::cerr << "Error: Channel does not exist." << std::endl;
   return;
   }

   if (!channel->clientIsOperator(executor))
   {
   std::cerr << "Error: You don't have permissions to kick a user." << std::endl;
   return;
   }

   Client  *victim = &(*(channel->findClient(NICK, victimName)));
   if (!victim)
   {
   std::cerr << "Error: Victim not found in the channel." << std::endl;
   return;
   }

   channel->deleteClient(*victim);

   std::string kickMessage = ":" + executor.getNickName() + " KICK " + channelName + " " + victimName + " :";

   for (size_t i = 3; i < args.size(); ++i)
   kickMessage += args[i] + " ";

   kickMessage = kickMessage.substr(0, kickMessage.size() - 1);
   kickMessage += "\r\n";

   send(victim->getFd().fd, kickMessage.c_str(), kickMessage.size(), 0);
   }
   */

Channel* findChannelByNameKick(const std::string& channelName, std::vector<Channel>& channels)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
		if (it->getName() == channelName) {
			return &(*it);
		}
	}
	return NULL;
}

static Channel*    getChannelKick(std::vector<std::string> args, std::vector<Channel>& channels)
{
	std::string channelName = args[1];

	if (channelName[0] != '#')
		channelName = "#" + channelName;

	return (findChannelByNameKick(channelName, channels));
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

    Channel* wantedChannel = getChannelKick(args, channels);
    if (!wantedChannel) {
        return kickNoChannelFound(sender);
    }

    if (!wantedChannel->clientIsOperator(sender)) {
        return kickNoPerms(sender);
    }

    wantedChannel->kickUser(sender, args[2], args);
}
