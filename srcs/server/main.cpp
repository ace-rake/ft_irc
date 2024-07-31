#include "Server.hpp"
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

Server* serverInstance = NULL;

void    signalHandler(int signal)
{
    if (signal == SIGINT)
        if (serverInstance)
            serverInstance->shutdown();
    exit(EXIT_SUCCESS);
}

int main(int ac, char **av)
{
    if (ac < 3)
    {
        std::cout << "Invalid program call. Should be ./ircserv <port> <password>" << std::endl;
        return (0);
    }
	Server server(av);
    serverInstance = &server;

    std::signal(SIGINT, signalHandler);
	server.run();
	return (0);
}
