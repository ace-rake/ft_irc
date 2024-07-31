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
    std::cout << "testing" << std::endl;
    Channel*    wantedChannel = findChannelByNameMode(args[1], channels);

    if (!wantedChannel)
    {
        std::cout << "Channel not found" << std::endl;
        return ;
    }
    else
        std::cout << "Channel Name: " << wantedChannel->getName() << std::endl;

    if (!wantedChannel->clientIsOperator(sender))
    {
        std::cout << "You are not a operator for this channel" << std::endl;
        return ;
    }

    if (args[2] == "+i")
        wantedChannel->setInviteOnly();
    else if (args[2] == "-i")
        wantedChannel->removeInviteOnly();
    else if (args[2] == "+t")
        wantedChannel->setTopicOperatorOnly();
    else if (args[2] == "-t")
        wantedChannel->removeTopicOperatorOnly();
    else if (args[2] == "+k")
        try {
            wantedChannel->setPassword(args[3]);
        } catch (const std::exception& e) {
            std::cout << "Password was not given" << std::endl;
        }
    else if (args[2] == "-k")
        wantedChannel->removePassword();
    else if (args[2] == "+l")
        try {
            wantedChannel->setUserLimit(std::atoi(args[3].c_str()));
        } catch (const std::exception& e) {
            std::cout << "User limit not given" << std::endl;
        }
    else if (args[2] == "-l")
        wantedChannel->removeUserLimit();
    else if (args[2] == "+o")
        try {
            Client  wanted = *wantedChannel->findClient(NICK, args[3]);
            wantedChannel->makeClientOperator(wanted);
        } catch (const std::exception& e) {
            std::cout << "User doesnt exist in channel or is not given";
        }
    else if (args[2] == "-o")
        try {
            Client  wanted = *wantedChannel->findOperator(NICK, args[3]);
            wantedChannel->removeClientOperator(wanted);
        } catch (const std::exception& e) {
            std::cout << "User doesnt exist in channel or is not given";
        }
    else
        std::cout << "Flag isn't implemented for MODE command" << std::endl;
}
