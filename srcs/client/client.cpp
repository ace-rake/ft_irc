#include "client.hpp"
#include <arpa/inet.h>
#include <cstdlib>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

// Constructor
client::client()
{
	static int id = 0;
	_server_fd = -1;
	_client_fd = NULL;
	_clientId = id++;
}

// Destructor
client::~client(void)
{
}

void	client::send_message(std::string str)
{
	send(_server_fd, str.c_str(), str.size(), 0);
}

struct pollfd &	client::getFd()const
{
	return *_client_fd;
}

void	client::setFd(struct pollfd * fd)
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

void	client::create(std::string userData)
{
	//CAP LS 
	//NICK vdenisse 
	//USER vdenisse vdenisse 10.12.4.5 :Victor Denissen
	//     (user)   host     server     realname
	std::vector<std::string> words = split(userData);	

	while (words.front().compare("NICK") != 0)
		words.erase(words.begin()); //TODO check if these can delete things we dont wanna delete
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

const bool client::operator==(const client & other)const
{
	return this->_clientId == other._clientId;
}

std::ostream& operator << (std::ostream &os,const client & client)
{

	os << "Host name " << client.getHostName() << std::endl;
	os << "real name " << client.getRealName() << std::endl;
	os << "nick name " << client.getNickName() << std::endl;
	os << "user name " << client.getUserName() << std::endl;
	os << "id " << client.getId() << std::endl;
	os << "ip " << client.getIp() << std::endl;
	return os;
}
