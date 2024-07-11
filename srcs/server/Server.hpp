#ifndef SERVER_HPP
# define SERVER_HPP

# include <netinet/in.h>
#include <sys/poll.h>
#include <vector>
#include "../irc.h"
#include "../channel/Channel.hpp"

// Custom class: Server
class	Server
{
	public:
		// Constructor
		Server	(void);

		// Destructor
		~Server	(void);

		void	run(void);

	protected:
		/*_*/;

	private:
		int			_server_socket;
		struct sockaddr_in	_address;
		int			_addrlen;
		struct pollfd &		_server;

		struct pollfd		_fds[MAX_CLIENTS + 1];
		client			_clients[MAX_CLIENTS];

		std::vector<Channel>	_channels;


	private:
		void		idle();
		void		handleNewConnection();
		std::string	handleClientMessage(int & fd, bool silent = false);//TODO this need to be changed to actually forward the msg to a client or channel
		
        // ServerIp.cpp
        bool		findSuitableIp(struct hostent *host);
		void		noSuitableIpFound(void);
		void		getIpAddress(void);

        // ServerSetup.cpp
        void        createSocket(void);
        void        setupPolling(void);
        void        bindSocketToAddress(void);
        void        listenIncomingConnections(void);

		std::string	receiveUserData(struct pollfd client);
		void		createNewClient(struct pollfd client);
};

#endif

