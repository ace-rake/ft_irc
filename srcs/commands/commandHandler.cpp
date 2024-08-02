#include "../server/Server.hpp"
#include <iostream>
#include "../irc.h"

bool starts_with(const std::string& str, const std::string& cmd)
{
	return (!str.compare(0, cmd.size(), cmd));
}

// Run checks on client before running certain commands
static bool validate_client(Client& client)
{
	// Check password
    if (!client.getPsw())
		return (std::cerr << "Client password incorrect\n", false);

	// Check nickname
    if (client.getNickName().empty())
		return (std::cerr << "Client nickname incorrect\n", false);

	// Check if user is invalid
	if (!client.isValid())
		return (std::cerr << "Client invalid\n", false);

	// Else user is valid
	else
		return (std::cout << "Client valid\n", true);
}

// TODO: Fix info messages
// TODO: Also inform user of anything that went wrong
void	Server::commandHandler(std::string command, Client & client)
{
	std::vector<std::string> args = split(command);

	std::cout << "enter commandHandler" << std::endl;
	std::cout << "Command to handle\n" << command << std::endl;

	if (starts_with(command,"CAP LS") || starts_with(command, "QUOTE "))
	{
		command.erase(0, command.find("\r\n") + 2);
		args = split(command);
	}

	if (starts_with(command, "PASS "))
	{
		std::cout << "enter PASS" << std::endl;
		if (args.size() >= 2)
		{
			if (args[1].compare(_serverPassword) == 0)
				client.setPsw(true);
		}

		else
		{
			std::cerr << "Wrong amount of args" << std::endl;
			client.sendMessageToClient("Not enough parameters");
		}

		command.erase(0, command.find("\r\n") + 2);
		args = split(command);

		if (!starts_with(command, "NICK "))
			return ;
	}

    if (starts_with(command, "NICK "))
	{
		std::cout << "enter NICK" << std::endl;
		if (args.size() < 2)
		{
			std::cerr << "Not enough parameters" << std::endl;
			client.sendMessageToClient("Not enough parameters");
		}
		else
			setNewNick(client, args[1]);

		command.erase(0, command.find("\r\n") + 2);
		args = split(command);

		if (!starts_with(command, "USER "))
			return ;
	}

	//TODO: Fix segfault with too few arguments
	//TODO: Actually handle the args
	if (starts_with(command, "USER "))
	{
		// Check password
	    if (!client.getPsw())
			return (std::cerr << "Client password incorrect\n", void());

		// Check nickname
    	if (client.getNickName().empty())
			return (std::cerr << "Client nickname incorrect\n", void());

		std::cout << "enter USER" << std::endl;
		client.setUserData(args);

		std::string welcomeMessage = ":serverhostname 001 " + client.getNickName() + " :Welcome to the IRC network, " + client.getNickName() + "!\r\n";
		client.sendMessageToClient(welcomeMessage);
	}

	else if (starts_with(command, "JOIN "))
	{
		if (!validate_client(client))
			return ;

		std::cout << "enter JOIN" << std::endl;
		joinHandler(args, client);
	}

    else if (starts_with(command, "PRIVMSG "))
	{
		if (!validate_client(client))
			return ;

		std::cout << "Enter PRIVMSG" << std::endl;
		if (args[1].at(0) == '#')
		{
			std::cout << "test: " << client.getNickName() << std::endl;

			Channel * channel = findChannel(args[1]);
			if (channel)
			{
                if (channel->findClient(ID, client.getId()) == channel->getClients().end())
                    client.sendMessageToClient("PRIVMSG: User not in channel");
                else
				    channel->sendMsgToAll(args, client);
			}

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

    else if (starts_with(command, "KICK "))
	{
		if (!validate_client(client))
			return ;

		std::cout << "enter KICK" << std::endl;
		kickHandler(args, client, _channels);
	}

    else if (starts_with(command, "INVITE "))
	{
		if (!validate_client(client))
			return ;

		std::cout << "enter INVITE" << std::endl;
		inviteToChannel(args[2], args[1], client);
	}

    else if (starts_with(command, "TOPIC "))
    {
		if (!validate_client(client))
			return ;

        std::cout << "enter TOPIC" << std::endl;
        topicHandler(args, _channels, client);
    }

    else if (starts_with(command, "MODE "))
    {
		if (!validate_client(client))
			return ;

        std::cout << "enter MODE" << std::endl;
        modeHandler(args, client, _channels);
    }

	else if (starts_with(command, "QUIT "))
        std::cout << "Client disconnecting\n";

    else
    {
		std::string message = "Command not found: " + command;
        std::cout << message << '\n';
        client.sendMessageToClient(message);
    }
}
    /*
    if (starts_with(command, "PART "))
    {
        std::cout << "enter PART" << std::endl;
        partHandler(args, _channels, client);
    }
    */
