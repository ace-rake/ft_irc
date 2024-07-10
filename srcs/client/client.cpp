#include "client.hpp"
#include <arpa/inet.h>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

// Constructor
client::client( int port)
{
	_port = port;
	_server_fd = -1;
	_serv_addr.sin_family = AF_INET;
	_serv_addr.sin_port = htons(port);
	_serv_addr.sin_addr = "10.12.2.5";
}

// Copy constructor
client::client(const client& other)
{
	*this = other;
}

// Copy assignment operator overload
client& client::operator = (const client& other)
{
	if (this != &other)
	{
		/* this->_=other._ */
	}
	return (*this);
}

// Destructor
client::~client(void)
{
}

void	client::run()
{
	if ((_server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cerr << "Socket creation error" << std::endl;
		throw std::exception();
	}
	if (inet_pton(AF_INET, "127.0.0.1", &_serv_addr.sin_addr) <= 0) {
		std::cerr << "Invalid address/ Address not supported" << std::endl;
		throw std::exception();
	}
	if (connect(_server_fd, (struct sockaddr *)&_serv_addr, sizeof(_serv_addr)) < 0) {
		std::cerr << "Connection failed" << std::endl;
		throw std::exception();
	}
	receiveClientFd();
	this->idle();
}

void	client::receiveClientFd()
{
	char buffer[BUFFER_SIZE];
	int valread = read(_server_fd, buffer, BUFFER_SIZE);
	if (valread == 0)
		throw std::exception();
	else
		_client_fd = std::atoi(buffer);
	std::cout << "I am " << _client_fd << std::endl;
}

void	client::idle()
{
	std::string str;
	while (true)
	{
		std::getline(std::cin, str);
		if (std::cin.eof())
			break;
		this->send_message(str);
		str.clear();
	}
}

void	client::send_message(std::string str)
{
	send(_server_fd, str.c_str(), str.size(), 0);
	std::cout << "Sent \"" << str << "\" to server" << std::endl;
}

int	client::getClientFd()
{
	return _client_fd;
}
