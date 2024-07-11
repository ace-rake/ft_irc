#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <netinet/in.h>
#include <string>
#include <sys/poll.h>
#include "../irc.h"

// Custom class: client
class	client
{
	public:
		// Constructor
		client	(	void	);

		// Destructor
		~client	(void);


		struct pollfd &	getFd 	(	void	)const;
		void	setFd	(struct pollfd * fd);

		void	run	(	void	);

	protected:
		/*_*/;

	private:
		int			_server_fd;
		struct pollfd *		_client_fd;

		/* User data */
		std::string 		_hostName;
		std::string 		_userName;
		std::string 		_nickName;
		std::string 		_realName;
		std::string		_ip;
		/*_*/;

		void	idle();
		void	send_message(std::string);
		void	receiveClientFd();
};

#endif

