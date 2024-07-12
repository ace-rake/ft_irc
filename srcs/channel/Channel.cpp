#include "Channel.hpp"
#include <iostream>
#include <sys/socket.h>

// Constructor
Channel::Channel(std::string name, std::string psw): _channelName(name), _psw(psw)
{
}

// Destructor
Channel::~Channel(void)
{
	std::cout << "Channel:\tdestroying object\n";
}

void	Channel::broadcastMsg(std::string str)// Send a msg to all members of a channel
{
	for (std::vector<client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{

	}

}

void	Channel::handleJoinRequest(client client, std::string psw)
{
	if (retrieveClientById(client.getId()) != NULL)
		return ;// Client already in channel
	// TODO: Check blacklist
	// TODO: Check join permission for channel
	if (_psw.compare(psw) != 0 || _psw.empty())
		return ;// Wrong Password
	addClient(client);
}

int	Channel::addClient(client client)
{
	if (retrieveClientById(client.getId()) != NULL)
		return 1;// Client already in channel
	_clients.push_back(client);
	return 0;
}

int	Channel::deleteClient(client client)
{
	if (retrieveClientById(client.getId()) == NULL)
		return 1;// Client not in channel	
	_clients.erase(_clients.begin());
	return (0);
}

// Returns client with nickname "name"
// Returns NULL if no matching client found
client * Channel::retrieveClientByNick(std::string name)
{
	for (std::vector<client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		if (it->getNickName().compare(name) == 0)
			return &(*it);
	return NULL;
}

client * Channel::retrieveClientById(int id)
{
	for (std::vector<client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		if (it->getId() == id)
			return &(*it);
	return NULL;
}
