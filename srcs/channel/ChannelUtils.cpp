#include "Channel.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

int	Channel::deleteClient(std::vector<Client>::iterator client)
{
	_clients.erase(client);
	return (0);
}

int	Channel::deleteClient(Client & c)
{
	std::vector<Client>::iterator it = findClient(USER, c.getUserName());
	if (it != _clients.end())
	{   
		std::string partMessage = ":";
		partMessage += c.getNickName() + "!";
		partMessage += c.getUserName() + "@";
		partMessage += c.getHostName();
		partMessage += " PART ";
		partMessage += getName();
		partMessage += " :Reason";
		broadcastMsg(partMessage);

		deleteClient(it);
		_settings.userAmount--;
	}
	return (0);
}

void    Channel::kickUser(Client sender, std::string victimName, std::vector<std::string>args)
{
	if (findClient(NICK, victimName) == _clients.end())
	{
		std::cerr << "Error: Victim is not found in channel" << std::endl;
		sender.sendMessageToClient("The person you are trying to kick can't be found");
		return ;
	}

	std::string kickMessageToClient = ":" + sender.getNickName() + " KICK " + getName() + " " + victimName;

	std::string kickMessage;

	for (size_t i = 3; i < args.size(); ++i)
		kickMessage += " " + args[i];

	kickMessageToClient += kickMessage;

	Client  wanted = *findClient(NICK, victimName);

	wanted.sendMessageToClient(kickMessageToClient);

	wanted.sendMessageToClient("You have been kicked from " + getName() + " for the reason" + kickMessage);

	broadcastMsg(kickMessageToClient);

	deleteClient(findClient(NICK, victimName));
}

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

std::vector<Client>::iterator	Channel::findOperator(userData field, const std::string data)
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
	for (std::vector<Client>::iterator it = _opList.begin(); it != _opList.end(); ++it)
	{
		if ((*it.*funcptr)() == data)
			return it;
	}
	return _opList.end();
}

bool    Channel::clientIsOperator(Client findClient)
{
	for (std::vector<Client>::iterator it = _opList.begin(); it != _opList.end(); it++)
		if (it->getId() == findClient.getId())
			return true;
	return false;
}

bool	Channel::isFull()const
{
	if (_settings.userAmount >= _settings.userLimit)
		return true;
	return false;
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
