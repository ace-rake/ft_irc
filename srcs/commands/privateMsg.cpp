#include "../server/Server.hpp"
#include <iostream>

/*
void Server::privateMsg(std::vector<std::string> args, Client &sender)
{
	Client * recipient = getUser(NICK, args[1]);
	if (!recipient)
		return ; // Recipients doesnt exist
	std::string msg(args[0]);
	for (int i = 1; i < args.size(); ++i)
		msg +=  " " + args[i];
	std::cout << msg << std::endl;
	recipient->sendMessageToClient(msg);
}
*/

void Server::privateMsg(std::vector<std::string> args, Client &sender)
{
    if (args.size() < 3)
    {
        sender.sendMessageToClient("Error: Not enough parameters for private message");
        return;
    }

    Client* recipient = getUser(NICK, args[1]);
    if (!recipient)
    {
        sender.sendMessageToClient("Error: Recipient does not exist");
        return;
    }

    std::string msg = ":" + sender.getNickName() + "!" + sender.getUserName() + "@" + sender.getHostName() + " PRIVMSG " + recipient->getNickName() + " ";
    for (size_t i = 2; i < args.size(); ++i)
    {
        if (i != 2)
            msg += " ";
        msg += args[i];
    }

    std::cout << msg << std::endl;
    recipient->sendMessageToClient(msg);
}

