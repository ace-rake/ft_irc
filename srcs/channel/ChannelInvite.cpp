#include "Channel.hpp"

void	Channel::addInviteToList(int id)
{
	// Dont need to check if id is already in list cuz set ignores dups
	_inviteList.insert(id);
}

bool	Channel::isInInviteList(int id)const
{
	return (_inviteList.find(id) != _inviteList.end());
}

bool    Channel::isInClientList(Client findClient)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		if (it->getId() == findClient.getId())
			return true;
	return false;
}

void	Channel::removeIdFromList(int id)
{
	if (_inviteList.find(id) != _inviteList.end())
		_inviteList.erase(id);
}
