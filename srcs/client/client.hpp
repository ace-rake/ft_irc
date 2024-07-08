#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <netinet/in.h>
#include <string>
# ifndef PORT
#  define PORT 12345
# endif

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
		int _server_fd;
		struct sockaddr_in _serv_addr;
		char buffer[1024];
		int _port;
		/*_*/;

		void	idle();
		void	send_message(std::string);
};

#endif

