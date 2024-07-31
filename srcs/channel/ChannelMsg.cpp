#include "Channel.hpp"
#include <iostream>

void	Channel::sendMsgToAll(std::vector<std::string> args, Client& sender)
{
	// Msg start from args[2].1;
	std::string msg = ':' + sender.getNickName() + '!' + sender.getUserName() + '@' + sender.getHostName() + " PRIVMSG " + _channelName + ' ' + args[2];
	for (size_t i = 3; i < args.size(); ++i)
		msg += ' ' +args[i];
	broadcastMsg(msg, sender);
}

void	Channel::broadcastMsg(std::string str, Client & sender)// Send a msg to all members of a channel, except to the sender
{
	for (size_t i = 0; i < _clients.size(); ++i)
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
