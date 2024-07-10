#include "Channel.hpp"
#include <iostream>
#include <sys/socket.h>

// Constructor
Channel::Channel()
{
	std::cout << "Channel:\tcreating object\n";
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

void	Channel::handleJoinRequest(client client)
{

}
