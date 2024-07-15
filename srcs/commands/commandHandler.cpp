#include "../server/Server.hpp"
#include <iostream>
#include "../irc.h"

bool starts_with(const std::string& str, const std::string& cmd) {
	return str.compare(0, cmd.size(), cmd) == 0;
}

void	Server::commandHandler(std::string command, client & client)
{
	std::vector<std::string> args = split(command);
	std::cout << "enter commandHandler" << std::endl;
	if (starts_with(command, "JOIN "))
	{
		std::cout << "enter JOIN" << std::endl;
		joinHandler(args, client);
	}
	if (starts_with(command, "PRIVMSG "))
	{
		std::cout << "Enter PRIVMSG" << std::endl;
		if (args[1].at(0) == '#')
		{
			std::cout << "test: " << client.getNickName() << std::endl;
			Channel * channel = findChannel(args[1]);
			if (channel)
				channel->sendMsgToAll(args, client);
			// Channel
		}
		else
		{
			// Private Message
		}
	}
	if (starts_with(command, "KICK "))
	{
		std::cout << "enter KICK" << std::endl;
		kickHandler(args, client, _channels);
	}
	if (starts_with(command, "INVITE"))
	{
		std::cout << "enter INVITE" << std::endl;
		inviteToChannel(args[2], args[1]);
	}
}
