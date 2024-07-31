#include "../server/Server.hpp"
#include <iostream>
#include "../irc.h"

bool starts_with(const std::string& str, const std::string& cmd) {
	return str.compare(0, cmd.size(), cmd) == 0;
}

void	Server::commandHandler(std::string command, Client & client)
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
			else
			{
                client.sendMessageToClient("PRIVMSG: Channel doesn't exist");
				throw std::runtime_error("PRIVMSG: Error: Channel doesn't exist");
				//TODO: Handle channel doesnt exist
			}
		}
		else
		{
			privateMsg(args, client);
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
		inviteToChannel(args[2], args[1], client);
	}
	if (starts_with(command, "NICK"))
	{
		std::cout << "enter NICK" << std::endl;
        if (args.size() < 2)
        {
            std::cerr << "Not enough parameters" << std::endl;
            client.sendMessageToClient("Not enough parameters");
        }
        else
		    setNewNick(client, args[1]);
	}
    if (starts_with(command, "TOPIC"))
    {
        std::cout << "enter TOPIC" << std::endl;
        topicHandler(args, _channels, client);
    }
    if (starts_with(command, "MODE"))
    {
        std::cout << "enter MODE" << std::endl;
        modeHandler(args, client, _channels);
    }
    /*
    if (starts_with(command, "PART "))
    {
        std::cout << "enter PART" << std::endl;
        partHandler(args, _channels, client);
    }
    */
}
