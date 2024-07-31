#include "../irc.h"
#include <iostream>
#include "../channel/Channel.hpp"

Channel* findChannelByNameTopic(const std::string& channelName, std::vector<Channel>& channels)
{
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
        if (it->getName() == channelName) {
            return &(*it);
        }
    }
    return NULL;
}

void    changeTopic(Channel* referencedChannel, std::vector<std::string>args, Client &sender)
{
    std::string newTopicName = args[2];

    for (size_t i = 3; i < args.size(); ++i)
        newTopicName += " " +  args[i];

    if (!newTopicName.empty() && newTopicName[0] == ':') 
        newTopicName.erase(0, 1);
    
    referencedChannel->changeTopic(newTopicName, sender);
}

void    topicNoChannelFound(Client& sender)
{
    std::cerr << "Error: No channel found for TOPIC command." << std::endl;
    sender.sendMessageToClient("TOPIC: No channel found with this name");
}

void    topicUserNotInChannel(Client& sender, Channel* channel)
{
    std::cerr << "Error: Sender is not in the right channel for TOPIC command" << std::endl;

    std::string clientMessage = ":server 442 " + sender.getNickName() + " " + channel->getName() + " :You're not on that channel";

    sender.sendMessageToClient(clientMessage);
}

void    topicUserNotOperator(Client& sender)
{
    std::cerr << "Error: Sender does not have operator privileges for TOPIC command." << std::endl;

    std::string clientMessage = "You are not an channel operator";

    sender.sendMessageToClient(clientMessage);
}

void    topicHandler(std::vector<std::string> args, std::vector<Channel> &channels, Client &sender)
{
    Channel*    wantedChannel = findChannelByNameTopic(args[1], channels);

    if (!wantedChannel)
        return ((void)topicNoChannelFound(sender));

    if (wantedChannel->findClient(ID, sender.getId()) == wantedChannel->getClients().end())
        return ((void)topicUserNotInChannel(sender, wantedChannel));

    if (!wantedChannel->clientIsOperator(sender) && !wantedChannel->everyoneCanChangeTopic())
        return ((void)topicUserNotOperator(sender));

    if (args.size() > 2)
        changeTopic(wantedChannel, args, sender);
}
