#ifndef SERVER_HPP
# define SERVER_HPP

# include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <vector>
#include "../irc.h"
#include "../channel/Channel.hpp"
#include "../client/Client.hpp"


// Custom class: Server
class	Server
{
	public:
		// Constructor
		Server	(void);

		// Destructor
		~Server	(void);

		void	run(void);
		Client * getClients(){return _clients;}

		// Logging purposes TODO delete this after
		void logCommand(std::string);


	protected:
		/*_*/;

	protected:
		int			_server_socket;
		struct sockaddr_in	_address;
		int			_addrlen;
		struct pollfd &		_server;

		struct pollfd		_fds[MAX_CLIENTS + 1];
		Client			_clients[MAX_CLIENTS];

		std::vector<Channel>	_channels;


	protected:
		void		idle			(void);
		void		handleNewConnection	(void);
		std::string	handleClientMessage	(Client & client);

		// ServerIp.cpp
		bool		findSuitableIp		(struct hostent *host);
		void		noSuitableIpFound	(void);
		void		getIpAddress		(void);

		// ServerSetup.cpp
		void        createSocket		(void);
		void        setupPolling		(void);
		void        bindSocketToAddress		(void);
		void        listenIncomingConnections	(void);

		std::string	receiveUserData	(struct pollfd client);
		std::string	readUserData	(int &fd);
		void		createNewClient	(Client & client);

		// Commands
		void		commandHandler	(std::string command, Client & client);
		Channel *	findChannel	(std::string name);
		void		joinHandler	(std::vector<std::string> args, Client &client);
		void		joinChannel	(std::string name, Client & client);
		void		joinChannel	(std::string name, Client & client, std::string psw);
		Channel *	createChannel	(std::string name, std::string psw = "");

		void		inviteToChannel	(std::string ChannelName, std::string nickName);

		void		setNewNick(Client & client, std::string newNick);

		/* void		part(const client & client, std::string channel); */

		void		broadCastMsg	(std::string msg);
		void		privateMsg	(std::vector<std::string> args, Client & sender);

		Client *	getUser		(userData, std::string);
		Channel *	getChannel(std::string name);
};

#endif

