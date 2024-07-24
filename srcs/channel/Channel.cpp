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

int	Channel::deleteClient(Client c)
{
	std::vector<Client>::const_iterator it = findClient(c);
	if (it != _clients.end())
		_clients.erase(it);
	return (0);
}

Client* Channel::retrieveClientByNick(const std::string& name)
{
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getNickName() == name)
            return &(*it);
    }
    return NULL;
}

Client * Channel::retrieveClientById(int id)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		if (it->getId() == id)
			return &(*it);
	return NULL;
}

std::vector<Client>::const_iterator	Channel::findClient(const Client & findClient)const
{
	for (std::vector<Client>::const_iterator it = _clients.begin(); it < _clients.end(); ++it)
		if (*it == findClient)
			return it;
	return _clients.end();
}

bool    Channel::clientIsOperator(Client findClient)
{
	for (std::vector<Client>::iterator it = _opList.begin(); it != _opList.end(); it++)
        if (it->getId() == findClient.getId())
            return true;
    return false;
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
