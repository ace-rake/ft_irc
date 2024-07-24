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

void    topicNotEnoughParam(client& sender)
{
    std::cerr << "Error: Not enough parameters for TOPIC command." << std::endl;
    
    std::string clientMessage = ":server 416 " + sender.getNickName() + " TOPIC :Not enough parameters";

    sender.sendMessageToClient(clientMessage);
}

void    topicNoChannelFound(client& sender)
{
    std::cerr << "Error: No channel found for TOPIC command." << std::endl;
    sender.sendMessageToClient("TOPIC: No channel found with this name");
}

void    topicUserNotInChannel(client& sender, Channel* channel)
{
    std::cerr << "Error: Sender is not in the right channel for TOPIC command" << std::endl;

    std::string clientMessage = ":server 442 " + sender.getNickName() + " " + channel->getName() + " :You're not on that channel";

    sender.sendMessageToClient(clientMessage);
}

void    noTopicSet(client& sender, Channel* channel)
{
    std::string clientMessage = ":server 331 " + sender.getNickName() + " " + channel->getName() + " :No topic is set";
    std::cout << clientMessage << std::endl;
    sender.sendMessageToClient(clientMessage);
}

void    topicDisplay(client& sender, Channel* channel, std::string topic)
{
    std::string clientMessage = ":server 332 " + sender.getNickName() + " " + channel->getName() + " :" + topic;
    std::cout << clientMessage << std::endl;
    sender.sendMessageToClient(clientMessage);
}

void    topicHandler(std::vector<std::string> args, std::vector<Channel>& channels, client& sender)
{
    if (args.size() < 2)
        return ((void)topicNotEnoughParam(sender));

    Channel*    wantedChannel = getChannel(args, channels);

    if (!wantedChannel)
        return ((void)topicNoChannelFound(sender));
    else
    {
        if (!wantedChannel->isInClientList(sender))
            return ((void)topicUserNotInChannel(sender, wantedChannel));
        else
        {
            if (args.size() == 2)
            {
                std::string currentTopic = wantedChannel->getTopic();

                // this currently doesn't display because we need to send it as a private message in the channel the user is in.

                if (currentTopic.empty())
                    return ((void)noTopicSet(sender, wantedChannel));
                else
                    return ((void)topicDisplay(sender, wantedChannel, currentTopic));

                // end
            }
            else
                changeTopic(wantedChannel, args);
        }
    }
}
