#include "Server.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

bool    Server::findSuitableIp(struct hostent *host)
{
	for (int i = 0; host->h_addr_list[i] != NULL; i++)
	{
		struct in_addr  *addr = (struct in_addr *)host->h_addr_list[i];
		if (strcmp(inet_ntoa(*addr), "127.0.0.1") != 0)
		{
			this->_address.sin_addr = *addr;
			std::cout << "Using IP address: " << inet_ntoa(*addr) << std::endl;
			return true;
		}
	}
	return false;
}

void    Server::noSuitableIpFound(void)
{
	this->_address.sin_addr.s_addr = INADDR_ANY;
	std::cerr << "No suitable IP address found, using 0.0.0.0 instead" << std::endl;
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

	if (!findSuitableIp(host))
		noSuitableIpFound();
}
