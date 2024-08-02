#include "../irc.h"
#include "./Channel.hpp"
#include <iostream>
#include <string>
#include <sstream>

void    Channel::setInviteOnly(Client &sender)
{
    std::cout << "Set channel " << getName() << " to invite only" << std::endl;
    sender.sendMessageToClient("You set Channel " + getName() + " to invite only");
    _settings.inviteOnly = true;
}

void    Channel::removeInviteOnly(Client &sender)
{
    std::cout << "set channel " << getName() << " to public" << std::endl;
    sender.sendMessageToClient("You set " + getName() + " to public");
    _settings.inviteOnly = false;
}

void    Channel::setPassword(std::string psw, Client &sender)
{
    if (psw.empty())
    {
        sender.sendMessageToClient("No Password given");
        return ;
    }

    _psw = psw;
    std::cout << "Set channel " << getName() << " password to " << _psw << std::endl;
    sender.sendMessageToClient("You set " + psw + " as the password for " + getName());
}

void    Channel::removePassword(Client &sender)
{
    _psw = "";
    std::cout << "Remove channel " << getName() << " password" << std::endl;
    sender.sendMessageToClient("You removed the password for " + getName());
}

void    Channel::setTopicOperatorOnly(Client &sender)
{
    std::cout << "Set channel " << getName() << " to topic change only by operator" << std::endl;
    _settings.plebsCanChangeTopic = false;
    sender.sendMessageToClient("You made it so only operators can change the topic for " + getName());
}

void    Channel::removeTopicOperatorOnly(Client &sender)
{
    std::cout << "Set channel " << getName() << " to topic change can be by everyone" << std::endl;
    _settings.plebsCanChangeTopic = true;
    sender.sendMessageToClient("You made it so anybody can change topic for " + getName());
}

void    Channel::setUserLimit(int amount, Client &sender)
{
    _settings.userLimit = amount;
    std::cout << "Set channel " << getName() << " user limit to " << _settings.userLimit << std::endl;

	std::stringstream converter;
	converter << amount;

    sender.sendMessageToClient("You get the user limit for " + getName() + " to " + converter.str());
}

void    Channel::removeUserLimit(Client &sender)
{
    std::cout << "Removed channel " << getName() << " user limit" << std::endl;
    _settings.userLimit = MAX_CLIENTS;
    sender.sendMessageToClient("Removed user limit for " + getName());
}

void    Channel::makeClientOperator(Client& victim, Client &sender)
{
    this->_opList.push_back(victim);
    sender.sendMessageToClient("You made " + victim.getNickName() + " operator to " + getName());
}

void    Channel::removeClientOperator(Client& victim, Client &sender)
{
    if (findOperator(NICK, victim.getNickName()) != _opList.end())
	    this->_opList.erase(Channel::findOperator(NICK, victim.getNickName()));
    sender.sendMessageToClient("You removed " + victim.getNickName() + " operator rights to " + getName());
}

bool    Channel::everyoneCanChangeTopic()
{
    return (this->_settings.plebsCanChangeTopic);
}
