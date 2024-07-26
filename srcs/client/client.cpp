#include "Client.hpp"
#include <algorithm>
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
	_channelNames.clear();
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
	//CAP LS 
	//NICK vdenisse 
	//USER vdenisse vdenisse 10.12.4.5 :Victor Denissen
	//     (user)   host     server     realname
	std::vector<std::string> words = split(userData);	

	while (words.front().compare("NICK") != 0)
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
		realName += " ";
		realName += *words.begin();
		words.erase(words.begin());
	}
	realName.erase(0, 1);
	_realName = realName;
}
//FUCK

void    Client::sendMessageToClient(std::string message) const
{
    std::string buffer = message + "\r\n";
    if (send(this->getFd().fd, buffer.c_str(), buffer.size(), 0) < 0)
        throw std::runtime_error("Error while sending message to the client");
}

void    Client::addToClientChannelList(Channel* channel)
{
    removeFromClientChannelList(channel);
    _channelNames.push_back(channel->getName());
}

void    Client::removeFromClientChannelList(Channel* channel)
{
    _channelNames.erase(std::remove(_channelNames.begin(), _channelNames.end(), channel->getName()), _channelNames.end());
}

const bool Client::operator==(const Client & other)const
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
