#include "../server/Server.hpp"

void	Server::setNewNick(client & client, std::string newNick)
{
	/* :old_nick NICK new_nick */
	std::string oldNick = client.getNickName();
	client.setNickName(newNick);	
	client.sendMessageToClient(":" + oldNick + " NICK " + newNick);
}
