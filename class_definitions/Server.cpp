#include "../classes/Server.hpp"
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

Server::Server(void)
{
    this->_addrLen = sizeof(this->_address);

    this->_address.sin_family = AF_INET; // Puts it as an IPv4
    this->_address.sin_port = htons(PORT);

    getIpAddress(); // Gets the first available ip address
    std::cout << "Listening to port: " << htons(this->_address.sin_port) << std::endl;
}

void    Server::getIpAddress(void)
{
    char    hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == -1)
    {
        perror("gethostname");
        exit(EXIT_FAILURE);
    }

    struct hostent  *host = gethostbyname(hostname);
    if (host == NULL)
    {
        perror("gethostbyname");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; host->h_addr_list[i] != NULL; i++)
    {
        struct in_addr  *addr = (struct in_addr *)host->h_addr_list[i];
        if (strcmp(inet_ntoa(*addr), "127.0.0.1") != 0)
        {
            this->_address.sin_addr = *addr;
            std::cout << "Using IP address: " << inet_ntoa(*addr) << std::endl;
            return;
        }
    }

    this->_address.sin_addr.s_addr = INADDR_ANY;
    std::cerr << "No suitable IP address found, using 0.0.0.0" << std::endl;
}

Server::~Server(void)
{
    std::cout << "Destroyed server" << std::endl;
}
