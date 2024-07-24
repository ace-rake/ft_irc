#include "Channel.hpp"

int	Channel::deleteClient(Client c)
{
	std::vector<Client>::const_iterator it = findClient(c);
	if (it != _clients.end())
		_clients.erase(it);
	return (0);
}

Client* Channel::retrieveClientByNick(const std::string& name)
{
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getNickName() == name)
            return &(*it);
    }
    return NULL;
}

Client * Channel::retrieveClientById(int id)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		if (it->getId() == id)
			return &(*it);
	return NULL;
}

std::vector<Client>::const_iterator	Channel::findClient(const Client & findClient)const
{
	for (std::vector<Client>::const_iterator it = _clients.begin(); it < _clients.end(); ++it)
		if (*it == findClient)
			return it;
	return _clients.end();
}

bool    Channel::clientIsOperator(Client findClient)
{
	for (std::vector<Client>::iterator it = _opList.begin(); it != _opList.end(); it++)
        if (it->getId() == findClient.getId())
            return true;
    return false;
}
