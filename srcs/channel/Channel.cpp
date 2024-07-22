#include "Channel.hpp"
#include <iostream>
#include <sys/socket.h>
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

void	Channel::sendMsgToAll(std::vector<std::string> args, client& sender)
{
	// Msg start from args[2].1;
	std::string msg = ":" + sender.getNickName() + "!" + sender.getUserName() + "@" + sender.getHostName() + " PRIVMSG " + _channelName + " " + args[2];
	for (int i = 3; i < args.size(); ++i)
		msg += " " +args[i];
	broadcastMsg(msg, sender);
}

void	Channel::broadcastMsg(std::string str, client & sender)// Send a msg to all members of a channel, except to the sender
{
	for (int i = 0; i < _clients.size(); ++i)
	{
		std::cout << "Sending " << str << " to client in channel:" << _channelName << std::endl;
		if (_clients[i].getId() != sender.getId())
			_clients[i].sendMessageToClient(str);
	}
}

void    Channel::broadcastMsg(std::string str)
{
    for (size_t i = 0; i < _clients.size(); ++i) 
    {
        std::cout << "Sending " << str << " to client in channel:" << _channelName << std::endl;
        _clients[i].sendMessageToClient(str);
    }
}

void	Channel::handleJoinRequest(client & client, std::string psw)
{
	if (retrieveClientById(client.getId()) != NULL)
		return ;// Client already in channel
	// TODO: Check blacklist
	// TODO: Check join permission for channel
	if (_psw.compare(psw) != 0 && !_psw.empty() && !isInInviteList(client.getId()))
		return ;// Cannot Join
	addClient(client);
}

int	Channel::addClient(client & client)
{
	if (retrieveClientById(client.getId()) != NULL)
		return 1;// Client already in channel
	_clients.push_back(client);
    if (_opList.empty())
        _opList.push_back(client);
	return 0;
}

int	Channel::deleteClient(client client)
{
	if (retrieveClientById(client.getId()) == NULL)
		return 1;// Client not in channel	
	_clients.erase(_clients.begin());
	return (0);
}

client* Channel::retrieveClientByNick(const std::string& name)
{
    for (std::vector<client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getNickName() == name)
            return &(*it);
    }
    return NULL;
}

client * Channel::retrieveClientById(int id)
{
	for (std::vector<client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		if (it->getId() == id)
			return &(*it);
	return NULL;
}

bool    Channel::clientIsOperator(client findClient)
{
	for (std::vector<client>::iterator it = _opList.begin(); it != _opList.end(); it++)
        if (it->getId() == findClient.getId())
            return true;
    return false;
}

void	Channel::addInviteToList(int id)
{
	// Dont need to check if id is already in list cuz set ignores dups
	_inviteList.insert(id);
}

bool	Channel::isInInviteList(int id)const
{
	return (_inviteList.find(id) != _inviteList.end());
}

void	Channel::removeIdFromList(int id)
{
	if (_inviteList.find(id) != _inviteList.end())
		_inviteList.erase(id);
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
