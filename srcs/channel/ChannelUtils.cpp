#include "Channel.hpp"
#include <iostream>

int	Channel::deleteClient(Client c)
{
	std::vector<Client>::const_iterator it = findClient(USER, c.getUserName());
	if (it != _clients.end())
		_clients.erase(it);
	return (0);
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
			std::cerr << "find client default, You need to add your field to the switch case" << std::endl;
			return _clients.end();
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
