#include "Client.hpp"
#include <arpa/inet.h>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include "../channel/Channel.hpp"
#include <iostream>

// Constructor
Client::Client()
{
	static int id = 0;
	_server_fd = -1;
	_client_fd = NULL;
	_clientId = id++;
	_psw = false;
}

// Destructor
Client::~Client(void)
{
}

void	Client::clear()
{
	_client_fd->fd = -1;
	_hostName = "";
	_userName = "";
	_nickName = "";
	_realName = "";
	_ip = "";
	_psw = false;
}

bool	Client::isValid()
{	
	if (_nickName.empty())
		return false;
	if (_hostName.empty())
		return false;
	if (_realName.empty())
		return false;
	if (_userName.empty())
		return false;
	if (_ip.empty())
		return false;
	return _psw;
}

void	Client::setUserData(std::vector<std::string> args)
{
	_hostName = args[1];
	_userName = args[2];
	_realName = args[4];
	_ip = args[3];
}

void	Client::setPsw(bool wow)
{
	_psw = wow;
}

void	Client::send_message(std::string str)
{
	send(_server_fd, str.c_str(), str.size(), 0);
}

struct pollfd &	Client::getFd()const
{
	return *_client_fd;
}

void	Client::setFd(struct pollfd * fd)
{
	_client_fd = fd;
}

std::vector<std::string> split(std::string str)
{
	std::vector<std::string> words;
	std::string word;
	std::stringstream stream(str);

	while (stream >> word)
		words.push_back(word);
	return words;
}

void	Client::create(std::string userData)
{
	std::vector<std::string> words = split(userData);	

	while (words.front().compare("NICK ") != 0)
		words.erase(words.begin());
	words.erase(words.begin());
	_nickName = *words.begin();

	words.erase(words.begin());
	words.erase(words.begin());
	_userName = *words.begin();

	words.erase(words.begin());
	_hostName = *words.begin();

	words.erase(words.begin());
	_ip = *words.begin();
	words.erase(words.begin());

	std::string realName = *words.begin();
	words.erase(words.begin());
	while (!words.empty())
	{
		realName += ' ';
		realName += *words.begin();
		words.erase(words.begin());
	}
	realName.erase(0, 1);
	_realName = realName;
}

void    Client::sendMessageToClient(std::string message) const
{
	std::string buffer = message + "\r\n";
	if (send(this->getFd().fd, buffer.c_str(), buffer.size(), 0) < 0)
		throw std::runtime_error("Error while sending message to the client");
}

bool Client::operator==(const Client & other)const
{
	return this->_clientId == other._clientId;
}

std::ostream& operator << (std::ostream &os,const Client & client)
{

	os << "Host name " << client.getHostName() << std::endl;
	os << "real name " << client.getRealName() << std::endl;
	os << "nick name " << client.getNickName() << std::endl;
	os << "user name " << client.getUserName() << std::endl;
	os << "id " << client.getId() << std::endl;
	os << "ip " << client.getIp() << std::endl;
	return os;
}
