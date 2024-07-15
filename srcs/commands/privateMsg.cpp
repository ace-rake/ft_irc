#include "../server/Server.hpp"
#include <iostream>

void Server::privateMsg(std::vector<std::string> args, client &sender)
{
	client * recipient = getUser(NICK, args[1]);
	if (!recipient)
		return ; // Recipients doesnt exist
	std::string msg(args[0]);
	for (int i = 1; i < args.size(); ++i)
		msg +=  " " + args[i];
	std::cout << msg << std::endl;
	recipient->sendMessageToClient(msg);
}
