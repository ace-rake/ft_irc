#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <netinet/in.h>
#include <string>
#include "../irc.h"

// Custom class: client
class	client
{
	public:
		// Constructor
		client	(	int	);

		// Copy constructor
		client	(	const client& other	);

		// Copy assignment operator overload
		client& operator = (const client& other);

		// Destructor
		~client	(void);


		void	run	(	void	);

	protected:
		/*_*/;

	private:
		int			_server_fd;
		int			_client_fd;
		struct sockaddr_in	_serv_addr;
		char			_buffer[BUFFER_SIZE];
		int			_port;
		/*_*/;

		void	idle();
		void	send_message(std::string);
};

#endif

