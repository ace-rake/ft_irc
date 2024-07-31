#include "Server.hpp"
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

Server* serverInstance = NULL;

void    signalHandler(int signal)
{
	if (signal == SIGINT)
		std::cout << ": Received interuption signal\n";
	else if (signal == SIGQUIT)
		std::cout << ": Received termination signal\n";

	if (serverInstance)
		serverInstance->shutdown();

    exit(EXIT_SUCCESS);
}

int main(int ac, char **av)
{
    if (ac < 3)
		std::cout << "Invalid program call. Should be ./ircserv <port> <password>\n";

	Server server(av);
    serverInstance = &server;

    std::signal(SIGINT, signalHandler);
    std::signal(SIGQUIT, signalHandler);

	server.run();
}
