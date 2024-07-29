/*
#include "../server/Server.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

std::vector<std::string> channelSplit(const std::string& str)
{
    std::vector<std::string> words;
    std::string word;
    std::stringstream stream(str);

    while (std::getline(stream, word, ','))
        words.push_back(word);

    return words;
}

Channel* findChannelByNamePart(const std::string& channelName, std::vector<Channel>& channels)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
		if (it->getName() == channelName) {
			return &(*it);
		}
	}
	return NULL;
}

static Channel*    getChannel(std::string channelName, std::vector<Channel>& channels)
{
    if (channelName[0] == ':')
        channelName = channelName.substr(1);

    if (channelName[0] != '#')
        channelName = "#" + channelName;

    return (findChannelByNamePart(channelName, channels));
}

void    partUserNotInChannel(Client& sender, Channel* channel)
{
    std::cerr << "Error: Sender is not in the right channel for PART command" << std::endl;

    std::string clientMessage = ":server 442 " + sender.getNickName() + " " + channel->getName() + " :You're not on that channel";

    sender.sendMessageToClient(clientMessage);
}

void    partNoChannelFound(Client& sender)
{
    std::cerr << "Error: No channel found for PART command." << std::endl;
    sender.sendMessageToClient("403 " + sender.getNickName() + " " + "No channel found with this name");
}

void    leaveChannel(Channel& channel, Client& sender)
{
    if (channel.findClient(ID, sender.getId()) == channel.getClients().end())
        return ((void)partUserNotInChannel(sender, &channel));
    std::cout << "Client " << sender.getNickName() << " parts from channel " << channel.getName() << std::endl;

    channel.deleteClient(sender);
}

void    partHandler(std::vector<std::string> args, std::vector<Channel> &channels, Client &sender)
{

    if (args.size() == 2)
    {
        Channel*    wantedChannel = getChannel(args[1], channels);
        
        if (!wantedChannel)
            partNoChannelFound(sender);
        else
            leaveChannel(*wantedChannel, sender);
    }

    if (args.size() < 3)
        return ;
    
    std::vector<std::string>    channelsToPart = channelSplit(args[2]);
    
    for (std::vector<std::string>::iterator it = channelsToPart.begin(); it != channelsToPart.end(); ++it)
    {
        const std::string&  channelName = *it;
        Channel*    wantedChannel = getChannel(channelName, channels);
    
        if (!wantedChannel)
            partNoChannelFound(sender);
        else
            leaveChannel(*wantedChannel, sender);
    }
}
*/
