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

	/* TESTING */
	// works like this as well, but problem is, we never get more than 1 address, despite using addrinfo instead of gethostbyname
	//struct addrinfo hints, *res, *p;
	//memset(&hints, 0, sizeof hints); //not allowed, but just testing

	//hints.ai_family = AF_UNSPEC;
	//hints.ai_socktype = SOCK_STREAM;
	//getaddrinfo(hostname, "6667", &hints, &res);

	//for (p = res; p != NULL; p = p->ai_next)
	//{
		//if (p->ai_family != AF_INET)
			//continue ;

		//struct sockaddr_in *addr = (struct sockaddr_in *)p->ai_addr;
		//if (strcmp(inet_ntoa(addr->sin_addr), "127.0.0.1"))
		////&& bind(this->_server.fd, p->ai_addr, p->ai_addrlen) >= 0)
		//{
			//this->_address = *addr;
			//std::cout << "Using IP address: " << inet_ntoa(addr->sin_addr) << '\n';

			//return (freeaddrinfo(res));
		//}
	//}
	//freeaddrinfo(res);
	//noSuitableIpFound();
}
