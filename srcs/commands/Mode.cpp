#include "../irc.h"
#include "../server/Server.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>

Channel* findChannelByNameMode(const std::string& channelName, std::vector<Channel>& channels)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
		if (it->getName() == channelName) {
			return &(*it);
		}
	}
	return NULL;
}

void    modeHandler(std::vector<std::string> args, Client &sender, std::vector<Channel> &channels)
{
    if (args.size() < 3)
    {
        std::cout << "Not enough parameters for MODE command" << std::endl;
        sender.sendMessageToClient("461 " + sender.getNickName() + " MODE :Not enough parameters");
        return ;
    }

    if (sender.getNickName() == args[1])
        return ;

    Channel*    wantedChannel = findChannelByNameMode(args[1], channels);

    if (!wantedChannel)
    {
        std::cerr << "Error: No channel found for MODE command." << std::endl;
        sender.sendMessageToClient("403 " + sender.getNickName() + " " + "No channel found with this name");
        return ;
    }

    if (wantedChannel->findClient(ID, sender.getId()) == wantedChannel->getClients().end())
    {
        std::cerr << "Error: Sender is not in the right channel for MODE command" << std::endl;
        std::string clientMessage = ":server 442 " + sender.getNickName() + " " + wantedChannel->getName() + " :You're not on that channel";
        sender.sendMessageToClient(clientMessage);
        return ;
    }

    if (!wantedChannel->clientIsOperator(sender))
    {
        std::cerr << "Error: Sender doesn't have OP rights." << std::endl;
        sender.sendMessageToClient("482 " + sender.getNickName() + " " + "You're not channel operator");
        return ;
    }

    if (args[2] == "+i")
        wantedChannel->setInviteOnly(sender);
    else if (args[2] == "-i")
        wantedChannel->removeInviteOnly(sender);
    else if (args[2] == "+t")
        wantedChannel->setTopicOperatorOnly(sender);
    else if (args[2] == "-t")
        wantedChannel->removeTopicOperatorOnly(sender);
    else if (args[2] == "+k")
        try {
            wantedChannel->setPassword(args[3], sender);
        } catch (const std::exception& e) {
            std::cout << "Password was not given" << std::endl;
            sender.sendMessageToClient("Password not given");
        }
    else if (args[2] == "-k")
        wantedChannel->removePassword(sender);
    else if (args[2] == "+l")
        try {
            wantedChannel->setUserLimit(std::atoi(args[3].c_str()), sender);
        } catch (const std::exception& e) {
            std::cout << "User limit not given" << std::endl;
            sender.sendMessageToClient("User limit not given");
        }
    else if (args[2] == "-l")
        wantedChannel->removeUserLimit(sender);
    else if (args[2] == "+o")
        try {
            Client  wanted = *wantedChannel->findClient(NICK, args[3]);
            wantedChannel->makeClientOperator(wanted, sender);
        } catch (const std::exception& e) {
            std::cout << "User doesnt exist in channel or is not given";
            sender.sendMessageToClient("User is not given or doesnt exist in channel");
        }
    else if (args[2] == "-o")
        try {
            Client  wanted = *wantedChannel->findOperator(NICK, args[3]);
            wantedChannel->removeClientOperator(wanted, sender);
        } catch (const std::exception& e) {
            std::cout << "User doesnt exist in channel or is not given";
            sender.sendMessageToClient("User is not given or doesnt exist in channel");
        }
    else
    {
        std::cout << "Flag isn't implemented for MODE command" << std::endl;
        sender.sendMessageToClient("Flag isn't implemented for MODE command");
    }
}
