#include "mock_server.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

void mock_Server::mock()
{
	setupPolling();
	_clients[0].create("CAP LS NICK vdenisse USER vdenisse vdenisse 10.12.4.5 :Victor Denissen");
	_clients[1].create("CAP LS NICK vdenisse1 USER vdenisse1 vdenisse1 10.12.4.5 :Victor Denissen1");
	_clients[2].create("CAP LS NICK vdenisse2 USER vdenisse2 vdenisse2 10.12.4.5 :Victor Denissen2");

	commandHandler(createCommand("JOIN", {"#channel"}), _clients[0]);

	try {
		commandHandler(createCommand("INVITE", {"vdenisse1", "#channel"}), _clients[0]);
	}
	catch (std::runtime_error &e)
	{
		std::cout << "caught runtime error because the send of invite fails" << std::endl;
	}
}



std::string mock_Server::createCommand(std::string command, std::vector<std::string> argv)
{
	std::string retval = command;
	for (std::string arg: argv)
		retval += " " + arg;
	return retval;
}
