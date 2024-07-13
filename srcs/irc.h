#ifndef IRC_H
# define IRC_H

/* Config Section */

#include <string>
#include <vector>
#include "client/client.hpp"
#include "channel/Channel.hpp"
#define MAX_CLIENTS 100 // Max allowed amount of clients
#define PORT        6667 // 6667 is the most used port for irc
#define BUFFER_SIZE 1024
#define DEBUG       FALSE // Gives extra info

/* -------------- */

std::vector<std::string> split(std::string str);
template<typename T>
client * retrieveClientByNick(T t, std::string name);

// Kick.cpp
void    kickHandler(std::vector<std::string> args, client &executor, std::vector<Channel>& channels);

#endif
