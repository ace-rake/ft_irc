#include "../irc.h"
#include <iostream>

Channel* findChannelByNameTopic(const std::string& channelName, std::vector<Channel>& channels)
{
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
        if (it->getName() == channelName) {
            return &(*it);
        }
    }
    return NULL;
}

static Channel*    getChannel(std::vector<std::string> args, std::vector<Channel>& channels)
{
    std::string channelName = args[1];

    if (channelName[0] != '#')
        channelName = "#" + channelName;

    return (findChannelByNameTopic(channelName, channels));
}

void    changeTopic(Channel* referencedChannel, std::vector<std::string>args)
{
    std::string newTopicName = args[3];

    for (size_t i = 4; i < args.size(); ++i)
        newTopicName += " " +  args[i];

    referencedChannel->changeTopic(newTopicName);
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

void    topicHandler(std::vector<std::string> args, std::vector<Channel> &channels, Client &sender)
{
    Channel*    wantedChannel = getChannel(args, channels);

    if (!wantedChannel)
        return ((void)topicNoChannelFound(sender));

    if (wantedChannel->findClient(sender) != wantedChannel->getClients().end())
        return ((void)topicUserNotInChannel(sender, wantedChannel));

    if (args.size() > 2)
        changeTopic(wantedChannel, args);
}
