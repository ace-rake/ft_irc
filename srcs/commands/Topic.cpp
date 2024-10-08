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

void displayTopic(Channel* referencedChannel, Client &sender)
{
	std::string topic = referencedChannel->getTopic();
	std::string channelName = referencedChannel->getName();
	std::string nickName = sender.getNickName();

	if (topic.empty())
	{
		std::string noTopicMessage = ":server_name 331 " + nickName + " " + channelName + " :No topic is set";
		sender.sendMessageToClient(noTopicMessage);
		std::cout << "No topic is set for channel: " << channelName << std::endl;
	}
	else
	{
		std::string topicMessage = ":server_name 332 " + nickName + " " + channelName + " :" + topic;
		sender.sendMessageToClient(topicMessage);
		std::cout << "Display Topic: " << topic << " for channel: " << channelName << std::endl;
	}
}

void    topicNoChannelFound(Client& sender)
{
	std::cerr << "Error: No channel found for KICK command." << std::endl;
	sender.sendMessageToClient("403 " + sender.getNickName() + " " + "No channel found with this name");
}

void    topicUserNotInChannel(Client& sender, Channel* channel)
{
	std::cerr << "Error: Sender is not in the right channel for TOPIC command" << std::endl;

	std::string clientMessage = ":server 442 " + sender.getNickName() + " " + channel->getName() + " :You're not on that channel";

	sender.sendMessageToClient(clientMessage);
}

void    topicUserNotOperator(Client& sender)
{
	std::cerr << "Error: Sender doesn't have OP rights." << std::endl;
	sender.sendMessageToClient("482 " + sender.getNickName() + " " + "You're not channel operator");
}

void    topicNotEnoughArguments(Client& sender)
{
	std::cerr << "Error: No victim specified." << std::endl;
	sender.sendMessageToClient("461 " + sender.getNickName() + " TOPIC :Not enough parameters");
} 

void    topicHandler(std::vector<std::string> args, std::vector<Channel> &channels, Client &sender)
{
	if (args.size() < 2)
		return ((void)topicNotEnoughArguments(sender));

	Channel*    wantedChannel = findChannelByNameTopic(args[1], channels);

	if (!wantedChannel)
		return ((void)topicNoChannelFound(sender));

	if (wantedChannel->findClient(ID, sender.getId()) == wantedChannel->getClients().end())
		return ((void)topicUserNotInChannel(sender, wantedChannel));

	if (!wantedChannel->clientIsOperator(sender) && !wantedChannel->everyoneCanChangeTopic())
		return ((void)topicUserNotOperator(sender));

	if (args.size() > 2)
		changeTopic(wantedChannel, args, sender);
	else
		displayTopic(wantedChannel, sender);
}
