#ifndef SERVER_HPP
# define SERVER_HPP

# include <netinet/in.h>
#include <sys/poll.h>
#include <vector>
#include "../irc.h"
#include "../channel/Channel.hpp"
#include "../client/client.hpp"

typedef enum
{
	USER,
	NICK,
	REAL,
	HOST,
	IP
} userData;

// Custom class: Server
class	Server
{
	public:
		// Constructor
		Server	(void);

		// Destructor
		~Server	(void);

		void	run(void);
		client * getClients(){return _clients;}

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
		client			_clients[MAX_CLIENTS];

		std::vector<Channel>	_channels;


	protected:
		void		idle			(void);
		void		handleNewConnection	(void);
		std::string	handleClientMessage	(client & client);

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
		void		createNewClient	(client & client);

		// Commands
		void		commandHandler	(std::string command, client & client);
		Channel *	findChannel	(std::string name);
		void		joinHandler	(std::vector<std::string> args, client &client);
		void		joinChannel	(std::string name, client & client);
		void		joinChannel	(std::string name, client & client, std::string psw);
		Channel *	createChannel	(std::string name, std::string psw = "");

		void		inviteToChannel	(std::string ChannelName, std::string nickName);

		void		broadCastMsg	(std::string msg);

		client *	getUser		(userData, std::string);
};

#endif

