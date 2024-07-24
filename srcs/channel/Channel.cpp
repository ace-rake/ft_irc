#include "Channel.hpp"
#include <iostream>
#include <sys/socket.h>
#include <vector>
#include "../irc.h"

// Constructor
Channel::Channel(std::string name, std::string psw): _channelName(name), _psw(psw)
{
    this->_channelTopic = "";
}

// Destructor
Channel::~Channel(void)
{
	std::cout << "Channel:\tdestroying object\n";
}

void	Channel::handleJoinRequest(Client & client, std::string psw)
{
	if (retrieveClientById(client.getId()) != NULL)
		return ;// Client already in channel
	// TODO: Check blacklist
	// TODO: Check join permission for channel
	if (_psw.compare(psw) != 0 && !_psw.empty() && !isInInviteList(client.getId()))
		return ;// Cannot Join
	addClient(client);
    client.addToClientChannelList(this);
}

int	Channel::addClient(Client & client)
{
	if (retrieveClientById(client.getId()) != NULL)
		return 1;// Client already in channel
	_clients.push_back(client);
    if (_opList.empty())
        _opList.push_back(client);
	return 0;
}

void Channel::changeTopic(const std::string& newTopic)
{
    this->_channelTopic = newTopic;

    // Notify all clients in the channel about the new topic
    std::string topicChangeMsg = ":server 332 " + _channelName + " :" + _channelTopic;
    broadcastMsg(topicChangeMsg);

    // Notify all clients in the channel that the topic has been changed
    std::string topicNotificationMsg = ":server TOPIC " + _channelName + " :" + _channelTopic;
    broadcastMsg(topicNotificationMsg);
}
