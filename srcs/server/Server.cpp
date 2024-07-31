#include "Server.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "../channel/Channel.hpp"

// Destructor
Server::~Server(void)
{
	close(_server.fd);
}

void Server::idle()
{
	int retval = 1;
	while (retval > 0)
	{
		try
		{
			retval = poll(_fds, MAX_CLIENTS, -1);

			if (retval > 0)
			{
				if (_server.revents & POLLIN)
					handleNewConnection();

				for (int i = 0; i < MAX_CLIENTS; ++i)
					if (_clients[i].getFd().fd != -1 && (_clients[i].getFd().revents & POLLIN))
						handleClientMessage(_clients[i]);
			}
		}

		catch (std::runtime_error &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}

Client*	Server::getUser(userData field, std::string data)
{
	std::string (Client::*funcptr)()const;
	switch (field){
		case (NICK):
			{
				funcptr = &Client::getNickName;
				break;
			}
		case (USER):
			{
				funcptr = &Client::getUserName;
				break;
			}
		default:
			return (NULL);
	}
	for (int i = 0; i < MAX_CLIENTS; ++i)
		if ((_clients[i].*funcptr)() == data)
			return &_clients[i];
	return NULL;
}

Channel *	Server::getChannel(std::string name)
{
	std::vector<Channel>::iterator it = _channels.begin();

	for (;it != _channels.end(); it++)
		if (it->getName().compare(name) == 0)
			return &*it;
	return NULL;
}


void	Server::logCommand(std::string str)
{
	std::ofstream out;
	out.open("Commands.txt", std::ios_base::app);
	out << str << std::endl;;
	out.close();
}

void    Server::shutdown(void)
{
    std::cout << "Shutting down server...\n";

	for (std::vector<Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
		delete &*it;
}
