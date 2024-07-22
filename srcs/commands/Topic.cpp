#include "../irc.h"
#include <vector>
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

void    displayTopic(Channel* referencedChannel)
{
    std::cout << "Topic: " << referencedChannel->getTopic() << std::endl;
}

void    changeTopic(Channel* referencedChannel, std::vector<std::string>args)
{
    std::string newTopicName = "";

    for (size_t i = 3; i < args.size(); ++i)
        newTopicName += args[i] + " ";

    newTopicName = newTopicName.substr(0, newTopicName.size() - 1);

    referencedChannel->changeTopic(newTopicName);
    displayTopic(referencedChannel);
}

void    topicHandler(std::vector<std::string> args, std::vector<Channel>& channels)
{
    if (args.size() < 3)
    {
        std::cerr << "Error: Not enough parameters for TOPIC command." << std::endl;
        return;
    }

    std::string channelName = "#" + args[2].substr(1);
    std::cout << channelName << std::endl;
    Channel*    wantedChannel = findChannelByNameTopic(channelName, channels);

    if (!wantedChannel)
    {
        std::cerr << "Error: Channel does not exist." << std::endl;
        return;
    }

    if (args.size() == 3)
        displayTopic(wantedChannel);
    else
        changeTopic(wantedChannel, args);
}
