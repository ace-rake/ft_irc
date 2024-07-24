#include "Channel.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

int	Channel::deleteClient(Client c)
{
	std::vector<Client>::const_iterator it = findClient(USER, c.getUserName());
	if (it != _clients.end())
		_clients.erase(it);
	return (0);
}

void    Channel::kickUser(Client sender, std::string victimName, std::vector<std::string>args)
{
    if (findClient(USER, victimName) == _clients.end())
    {
        std::cerr << "Error: Victim is not found in channel" << std::endl;
        sender.sendMessageToClient("KICK: User doesn't exists in channel");
        return ;
    }

    //deleteClient(Client c) -- dont think we have a function for this yet

    std::string kickMessage = ":" + sender.getNickName() + " KICK " + getName() + " " + victimName + " :";

    for (size_t i = 4; i < args.size(); ++i)
		kickMessage += " " + args[i];

    sender.sendMessageToClient(kickMessage);
}

/* Client* Channel::retrieveClientByNick(const std::string& name) */
/* { */
/*     for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) { */
/*         if (it->getNickName() == name) */
/*             return &(*it); */
/*     } */
/*     return NULL; */
/* } */
/*  */
/* Client * Channel::retrieveClientById(int id) */
/* { */
/* 	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++) */
/* 		if (it->getId() == id) */
/* 			return &(*it); */
/* 	return NULL; */
/* } */
/*  */
/* std::vector<Client>::const_iterator	Channel::findClient(const Client & findClient)const */
/* { */
/* 	for (std::vector<Client>::const_iterator it = _clients.begin(); it < _clients.end(); ++it) */
/* 		if (*it == findClient) */
/* 			return it; */
/* 	return _clients.end(); */
/* } */
std::vector<Client>::iterator	Channel::findClient(userData field, int id)
{
	if (field != ID)
	{
		std::cerr << "wrong field" << std::endl;
		return _clients.end();
	}
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->getId()== id)
			return it;
	}
	return _clients.end();
}

std::vector<Client>::iterator	Channel::findClient(userData field, const std::string data)
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
		{
			throw std::runtime_error("find client default, You need to add your field to the switch case");
		}
	}
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it.*funcptr)() == data)
			return it;
	}
	return _clients.end();
}

bool    Channel::clientIsOperator(Client findClient)
{
	for (std::vector<Client>::iterator it = _opList.begin(); it != _opList.end(); it++)
        if (it->getId() == findClient.getId())
            return true;
    return false;
}