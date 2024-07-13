#include <cstddef>
#include <vector>
#include "../client/client.hpp"
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

bool    hasKickPermissions(client &executor, const std::string& channelName)
{
    // if client is in the op list of the channel we should be able to kick a player
    // for now hardcoded to true
    return true;
}

void    kickHandler(std::vector<std::string> args, client &executor, std::vector<Channel>& channels)
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

    if (!hasKickPermissions(executor, channelName))
    {
        std::cerr << "Error: You don't have permissions to kick a user." << std::endl;
        return;
    }

    client  *victim = channel->retrieveClientByNick(victimName);
    if (!victim)
    {
        std::cerr << "Error: Victim not found in the channel." << std::endl;
        return;
    }

    channel->deleteClient(*victim);

    std::string kickMessage = ":" + executor.getNickName() + " KICK " + channelName + " " + victimName + " :";

    for (size_t i = 3; i < args.size(); ++i)
        kickMessage += args[i] + " ";

    kickMessage = kickMessage.substr(0, kickMessage.size() - 1); // Remove trailing space
    kickMessage += "\r\n";

    send(victim->getFd().fd, kickMessage.c_str(), kickMessage.size(), 0);
}
