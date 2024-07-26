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
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

bool    Server::findSuitableIp(struct addrinfo *res)
{
	for (struct addrinfo *p = res; p != NULL; p = p->ai_next)
	{
		if (p->ai_family != AF_INET)
			continue ;

		struct sockaddr_in *addr = (struct sockaddr_in *)p->ai_addr;
		if (strcmp(inet_ntoa(addr->sin_addr), "127.0.0.1"))
		{
			std::cout << "Resovled IP: " << inet_ntoa(addr->sin_addr) << '\n';
			this->_fall_back_address = *addr;

			createSocket();
			setupPolling();

			if (bind(this->_server.fd, p->ai_addr, p->ai_addrlen) < 0)
			{
				std::cerr << "Bind failed for " << inet_ntoa(addr->sin_addr) << '\n';
				continue ;
			}

			std::cout << "Using IP address: " << inet_ntoa(addr->sin_addr) << '\n';
			this->_address = *addr;

			return (freeaddrinfo(res), true);
		}
	}
	return (freeaddrinfo(res), false);
}

void    Server::noSuitableIpFound(void)
{
	this->_address.sin_addr.s_addr = INADDR_ANY;
	std::cerr << "No suitable IP address found, using 0.0.0.0 instead" << std::endl;

	createSocket();

	setupPolling();

	bindSocketToAddress();
}

void    Server::getIpAddress(void)
{
	char    hostname[256];
	if (gethostname(hostname, sizeof(hostname)))
	{
		perror("gethostname");
		exit(EXIT_FAILURE);
	}

	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if (int status = getaddrinfo(hostname, "6667", &hints, &res))
	{
		std::cerr << "Error: getaddrinfo: " << gai_strerror(status) << '\n';
		exit(EXIT_FAILURE);
	}

	if (!findSuitableIp(res))
		noSuitableIpFound();
}
