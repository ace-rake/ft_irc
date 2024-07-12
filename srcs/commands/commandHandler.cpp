#include "../server/Server.hpp"
#include <iostream>
#include "../irc.h"

bool starts_with(const std::string& str, const std::string& cmd) {
	return str.compare(0, cmd.size(), cmd) == 0;
}

void	Server::commandHandler(std::string command, client & client)
{
	std::cout << "enter commandHandler" << std::endl;
	if (starts_with(command, "JOIN "))
	{
		std::cout << "enter JOIN" << std::endl;
		joinHandler(split(command), client);
	}
	if (starts_with(command, "PRIVMSG "))
	{
		std::cout << "Enter PRIVMSG" << std::endl;
		std::vector<std::string> args = split(command);
		if (args[1].at(0) == '#')
		{
			Channel * channel = findChannel(args[1]);
			channel->sendMsgToAll(args);
			// Channel
		}
		else
		{
			// Private Message
		}
	}
}
