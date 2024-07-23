#include "../irc.h"
#include <iostream>

void partHandler(std::vector<std::string> args, std::vector<Channel> &channels, client &sender)
{
    if (args.size() < 2)
    {
        std::cerr << "Error: Not enough parameters for PART command." << std::endl;
        return;
    }

    std::string channelName = args[1];

    // Ensure the channel name starts with '#'
    if (channelName[0] == ':')
    {
        channelName[0] = '#';
    }
    else if (channelName[0] != '#')
    {
        channelName = "#" + channelName;
    }

    Channel* channel = findChannelByName(channelName, channels);

    if (!channel)
    {
        std::cerr << "Error: No such channel exists." << std::endl;
        return;
    }
    
    if (!channel->isInClientList(sender))
    {
        std::cerr << "Error: Client is not in the specified channel." << std::endl;
        return;
    }
    
    // Notify other clients in the channel
    std::string partMessage = ":" + sender.getNickName() + " PART " + channelName;
    channel->broadcastMsg(partMessage, sender);

    // Remove the client from the channel and the channel from the client's list
    sender.removeFromClientChannelList(channel);
    channel->deleteClient(sender);

    // Optionally send a confirmation message to the client
    std::string confirmationMessage = "PART " + channelName;
    sender.sendMessageToClient(confirmationMessage);
}
