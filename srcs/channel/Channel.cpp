#include "Channel.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <vector>
#include "../irc.h"


// Constructor
Channel::Channel(std::string name, std::string psw): _channelName(name), _psw(psw)
{
    this->_channelTopic = "";
    _settings.inviteOnly = false;
    _settings.plebsCanChangeTopic = false;
    _settings.userLimit = MAX_CLIENTS;
    _settings.userAmount = 0;
}

// Destructor
Channel::~Channel(void)
{
	std::cout << "Channel:\tdestroying object\n";
}

void	Channel::handleJoinRequest(Client & client, std::string psw)
{
	if (findClient(ID, client.getId()) != _clients.end())
		return ;// Client already in channel
	// TODO: Check blacklist
	if (isFull())
		return (void) isFullErr(client);
	if (isInInviteList(client.getId())) // Check if invited
		return (void) addClient(client);
	if (_settings.inviteOnly)
		return (void) inviteOnlyErr(client) ;// Cannot join
	if (_psw.compare(psw) != 0 && !_psw.empty()) // Check psw
		return (void) wrongPswErr(client);// Cannot Join
	addClient(client);
}

static std::string baseError(std::string channelName, Client client, int errCode)
{
	std::stringstream converter;
	converter << errCode;

	return (converter.str() + ' ' + client.getNickName() + ' ' + channelName + " :Cannot join channel");
}

void	Channel::inviteOnlyErr(Client & client)
{
	std::string errMsg = baseError(_channelName, client, ERR_INVITEONLYCHAN) + " (+i)";
	client.sendMessageToClient(errMsg);
}

void	Channel::wrongPswErr(Client & client)
{
	std::string errMsg = baseError(_channelName, client, ERR_WRONGCHANKEY) + " (+k)";
	client.sendMessageToClient(errMsg);
}

void	Channel::isFullErr(Client & client)
{
	std::string errMsg = baseError(_channelName, client, ERR_CHANNELISFULL) + " (+l)";
	client.sendMessageToClient(errMsg);
}

int	Channel::addClient(Client & client)
{
	if (findClient(ID, client.getId()) != _clients.end())
		return 1;// Client already in channel
	_clients.push_back(client);
	if (_opList.empty())
		_opList.push_back(client);
	client.addToClientChannelList(this);
	_settings.userAmount++;
	return 0;
}

void Channel::changeTopic(const std::string& newTopic, Client& sender)
{
    this->_channelTopic = newTopic;

    // Notify all clients in the channel about the new topic
    std::string topicChangeMsg = ":server 332 " + _channelName + " :" + _channelTopic;
    broadcastMsg(topicChangeMsg);

    // Notify all clients in the channel that the topic has been changed
    std::string topicNotificationMsg = ":" + sender.getNickName() + "!" + sender.getUserName() + "@" + sender.getHostName() + " TOPIC " + _channelName + " :" + _channelTopic;
    broadcastMsg(topicNotificationMsg);
}
