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

void    changeTopic(Channel* referencedChannel, std::vector<std::string>args)
{
    std::string newTopicName = args[3];

    for (size_t i = 4; i < args.size(); ++i)
        newTopicName += " " +  args[i];

    referencedChannel->changeTopic(newTopicName);
}

void    topicHandler(std::vector<std::string> args, std::vector<Channel>& channels, client& sender)
{
    if (args.size() < 2)
    {
        std::cerr << "Error: Not enough parameters for TOPIC command." << std::endl;
        
        std::string clientMessage = ":server 416 " + sender.getNickName() + " TOPIC :Not enough parameters";

        sender.sendMessageToClient(clientMessage);
        return;
    }

    std::string channelName = args[1];

    if (channelName[0] != '#')
        channelName = "#" + channelName;

    std::cout << "ChannelName: " << channelName << std::endl;
    Channel*    wantedChannel = findChannelByNameTopic(channelName, channels);
    if (!wantedChannel)
    {
        std::cerr << "Error: No channel found for TOPIC command." << std::endl;
        sender.sendMessageToClient("TOPIC: No channel found with this name");
        return;
    }
    else
    {
        if (!wantedChannel->isInClientList(sender))
        {
            std::cerr << "Error: Sender is not in the right channel for TOPIC command" << std::endl;

            std::string clientMessage = ":server 442 " + sender.getNickName() + " " + wantedChannel->getName() + " :You're not on that channel";

            sender.sendMessageToClient(clientMessage);

            return;
        }
        else
        {
            std::cout << "Arg size: " << args.size() << std::endl;
            if (args.size() == 2)
            {
                std::string currentTopic = wantedChannel->getTopic();
                std::string clientMessage;

                // this currently doesn't display because we need to send it as a private message in the channel the user is in.

                if (currentTopic.empty())
                {
                    std::string clientMessage = ":server 331 " + sender.getNickName() + " " + wantedChannel->getName() + " :No topic is set";
                    std::cout << clientMessage << std::endl;
                    sender.sendMessageToClient(clientMessage);
                    return;
                }
                else
                {
                    std::string clientMessage = ":server 332 " + sender.getNickName() + " " + wantedChannel->getName() + " :" + currentTopic;
                    std::cout << clientMessage << std::endl;
                    sender.sendMessageToClient(clientMessage);
                    return;
                }

                // end
            }
            else
                changeTopic(wantedChannel, args);
        }
    }
}
