#include "../irc.h"
#include "./Channel.hpp"
#include <iostream>

void    Channel::setInviteOnly()
{
    std::cout << "Set channel " << getName() << " to invite only" << std::endl;
    _settings.inviteOnly = true;
}

void    Channel::removeInviteOnly()
{
    std::cout << "set channel " << getName() << " to public" << std::endl;
    _settings.inviteOnly = false;
}

void    Channel::setPassword(std::string psw)
{
    _psw = psw;
    std::cout << "Set channel " << getName() << " password to " << _psw << std::endl;
}

void    Channel::removePassword()
{
    _psw = "";
    std::cout << "Remove channel " << getName() << " password" << std::endl;
}

void    Channel::setTopicOperatorOnly()
{
    std::cout << "Set channel " << getName() << " to topic change only by operator" << std::endl;
    _settings.plebsCanChangeTopic = false;
}

void    Channel::removeTopicOperatorOnly()
{
    std::cout << "Set channel " << getName() << " to topic change can be by everyone" << std::endl;
    _settings.plebsCanChangeTopic = true;
}

void    Channel::setUserLimit(int amount)
{
    _settings.userLimit = amount;
    std::cout << "Set channel " << getName() << " user limit to " << _settings.userLimit << std::endl;
}

void    Channel::removeUserLimit()
{
    std::cout << "Removed channel " << getName() << " user limit" << std::endl;
    _settings.userLimit = MAX_CLIENTS;
}

void    Channel::makeClientOperator(Client & client)
{
    _opList.push_back(client);
}

void    Channel::removeClientOperator(Client & client)
{
    std::cout << "Remove user if he exists" << std::endl;
}

bool    Channel::everyoneCanChangeTopic()
{
    if (_settings.plebsCanChangeTopic == true)
        return true;
    else
        return false;
}
