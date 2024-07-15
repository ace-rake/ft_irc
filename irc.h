#ifndef IRC_H
# define IRC_H

#include <string>
#include ""
/* Config Section */

#define MAX_CLIENTS 100 // Max allowed amount of clients
#define PORT        6667 // 6667 is the most used port for irc
#define BUFFER_SIZE 1024
#define DEBUG       FALSE // Gives extra info

/* -------------- */
template<typename T>
client * retrieveClientByNick(T t, std::string name);

#endif
