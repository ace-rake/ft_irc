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

    Client  *victim = channel->retrieveClientByNick(victimName);
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
