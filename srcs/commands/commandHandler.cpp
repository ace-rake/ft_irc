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

	return (true);
}

void	Server::commandHandler(std::string command, Client & client)
{
	std::vector<std::string> args = split(command);

	if (starts_with(command,"CAP LS") || starts_with(command, "QUOTE"))
	{
		command.erase(0, command.find("\r\n") + 2);
		args = split(command);
	}

	if (starts_with(command, "PASS"))
	{
		if (args.size() >= 2)
		{
			if (args[1].compare(_serverPassword) == 0)
            {
				client.setPsw(true);
                client.sendMessageToClient("The password you entered is correct");
            }
            else
                client.sendMessageToClient("Wrong password entered");
		}

		else
		{
			std::cerr << "Wrong amount of args" << std::endl;
            client.sendMessageToClient("461 " + client.getNickName() + " PASS :Not enough parameters");
		}

		command.erase(0, command.find("\r\n") + 2);
		args = split(command);

		if (!starts_with(command, "NICK"))
			return ;
	}

    if (starts_with(command, "NICK"))
	{
		if (args.size() < 2)
		{
			std::cerr << "Not enough parameters" << std::endl;
            client.sendMessageToClient("461 " + client.getNickName() + " NICK :Not enough parameters");
		}
		else
			setNewNick(client, args[1]);

		command.erase(0, command.find("\r\n") + 2);
		args = split(command);

		if (!starts_with(command, "USER "))
			return ;
	}

	if (starts_with(command, "USER"))
	{
		// Check argument count
		if (args.size() < 5)
        {
            client.sendMessageToClient("461 " + client.getNickName() + " USER :Not enough parameters");
			return (std::cerr << "Incomplete command: USER\n", void());
        }

		// Check password
	    if (!client.getPsw())
			return (std::cerr << "Client password incorrect\n", void());

		// Check nickname
    	if (client.getNickName().empty())
			return (std::cerr << "Client nickname incorrect\n", void());

		client.setUserData(args);

		std::string welcomeMessage = ":serverhostname 001 " + client.getNickName() + " :Welcome to the IRC network, " + client.getNickName() + "!\r\n";
		client.sendMessageToClient(welcomeMessage);
	}

	else if (starts_with(command, "JOIN"))
	{
		if (!validate_client(client))
			return ;

		joinHandler(args, client);
	}

    else if (starts_with(command, "PRIVMSG"))
	{
        if (args.size() < 3)
        {
            std::cerr << "Error: No enough parameters." << std::endl;
            client.sendMessageToClient("461 " + client.getNickName() + " PRIVMSG :Not enough parameters");
            return ;
        }

		if (!validate_client(client))
			return ;

		if (args[1].at(0) == '#')
		{
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
			}
		}

		else
		{
			privateMsg(args, client);
		}
	}

    else if (starts_with(command, "KICK"))
	{
		if (!validate_client(client))
			return ;

		kickHandler(args, client, _channels);
	}

    else if (starts_with(command, "INVITE"))
	{
		if (!validate_client(client))
			return ;

		if (args.size() < 3)
        {
            client.sendMessageToClient("461 " + client.getNickName() + " INVITE :Not enough parameters");
			return (std::cerr << "Incomplete command: INVITE\n", void());
        }

		inviteToChannel(args[2], args[1], client);
	}

    else if (starts_with(command, "TOPIC"))
    {
		if (!validate_client(client))
			return ;

        topicHandler(args, _channels, client);
    }

    else if (starts_with(command, "MODE"))
    {
		if (!validate_client(client))
			return ;

        modeHandler(args, client, _channels);
    }

	else if (!starts_with(command, "PING") && !starts_with(command, "QUIT"))
    {
        std::cerr << "Error: Command not found: " << command << std::endl;
        client.sendMessageToClient("Command not found: " + command);
    }
}
/*
    if (starts_with(command, "PART "))
    {
        std::cout << "enter PART" << std::endl;
        partHandler(args, _channels, client);
    }
*/
