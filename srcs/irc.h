#ifndef IRC_H
# define IRC_H

/* Config Section */

#include <string>
#include <vector>
#include "client/Client.hpp"
#define MAX_CLIENTS 100 // Max allowed amount of clients
#define BUFFER_SIZE 1024
#define DEBUG       FALSE // Gives extra info

typedef enum
{
	USER,
	NICK,
	REAL,
	HOST,
	IP,
	ID
} userData;
/* -------------- */

std::vector<std::string> split(std::string str);
template<typename T>
Client * retrieveClientByNick(T t, std::string name);

Channel* findChannelByName(const std::string& channelName, std::vector<Channel>& channels);

// Kick.cpp
void    kickHandler(std::vector<std::string> args, Client &sender, std::vector<Channel>& channels);
void    topicHandler(std::vector<std::string> args, std::vector<Channel>& channels, Client &sender);
void    partHandler(std::vector<std::string> args, std::vector<Channel>& channels, Client &sender);
void    modeHandler(std::vector<std::string> args, Client &sender, std::vector<Channel>& channels);

#endif
