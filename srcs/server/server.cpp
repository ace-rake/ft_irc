#include "server.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

// Constructor
server::server(int port)
{
	_addrlen = sizeof(_address);

	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(port);

	if ((_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	if (bind(_server_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
	{
		perror("bind failed");
		close(_server_fd);
		exit(EXIT_FAILURE);
	}

	if (listen(_server_fd, 1) < 0)
	{
		perror("listen");
		close(_server_fd);
		exit(EXIT_FAILURE);
	}

	std::cout << "Server is listening on port: " << port << std::endl;


}

// Copy constructor
server::server(const server& other)
{
	std::cout << "server:\tcopying object\n";
	*this = other;
}

// Copy assignment operator overload
server& server::operator = (const server& other)
{
	std::cout << "server:\tcopying object (assignment)\n";
	if (this != &other)
	{
		/* this->_=other._ */
	}
	return (*this);
}

// Destructor
server::~server(void)
{
	close(_server_fd);
	for(int client : _clients)
		close(client);
}

void server::run() {
    fd_set readfds;
    int max_sd, sd, activity;

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(_server_fd, &readfds);
        max_sd = _server_fd;

        for (int client_socket : _clients) {
            sd = client_socket;
            if (sd > 0) FD_SET(sd, &readfds);
            if (sd > max_sd) max_sd = sd;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            std::cerr << "select error" << std::endl;
        }

        if (FD_ISSET(_server_fd, &readfds)) {
            handleNewConnection();
        }

        for (int i = 0; i < _clients.size(); ++i) {
            sd = _clients[i];
            if (FD_ISSET(sd, &readfds)) {
                handleClientMessage(sd);
            }
        }
    }
}
