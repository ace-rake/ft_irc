#include "../classes/Server.hpp"
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <sys/poll.h>
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

void    Server::run(void)
{
    if ((this->_serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    this->_fds[0].fd = this->_serverFd;
    this->_fds[0].events = POLLIN;
    
    for (int i = 1; i < MAX_CLIENTS; ++i)
        this->_fds[i].fd = -1;

    if (bind(this->_serverFd, (struct sockaddr *)&this->_address, sizeof(this->_address)) < 0)
    {
        perror("bind failed");
        close(this->_serverFd);
        exit(EXIT_FAILURE);
    }

    if (listen(this->_serverFd, 1) < 0)
    {
        perror("listen");
        close(this->_serverFd);
        exit(EXIT_FAILURE);
    }

    idle();
}

void   Server::idle(void)
{
    while (1)
    {
        int retval = poll(this->_fds, MAX_CLIENTS, -1);

        if (retval > 0)
        {
            if (this->_fds[0].revents & POLLIN)
                handleNewConnection();
            for (int i = 1; i < MAX_CLIENTS; ++i)
                if (this->_fds[i].fd != -1 && (this->_fds[i].revents & POLLIN))
                    handleClientMessage(&(this->_fds[i].fd));
        }
    }
}

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

void    Server::handleNewConnection(void)
{
    int     new_connection = accept(this->_serverFd, (struct sockaddr *)&this->_address, (socklen_t *)&this->_addrLen);
    for (int i = 1; i < MAX_CLIENTS; ++i)
    {
        if (this->_fds[i].fd == -1)
        {
            this->_fds[i].fd = new_connection;
            this->_fds[i].events = POLLIN;
            std::cout << "Client connected" << std::endl;
            break;
        }
    }
}

void    Server::handleClientMessage(int *fd)
{
    char    buffer[BUFFER_SIZE];
    int     valread = read(*fd, buffer, BUFFER_SIZE);
    if (valread == 0)
    {
        close(*fd);
        *fd = -1;
        std::cout << "Client disconnected" << std::endl;
    }
    else
    {
        buffer[valread] = '\0';
        std::cout << "Received:\t" << buffer << std::endl;
    }
}

Server::~Server(void)
{
    std::cout << "Destroyed server" << std::endl;
}
