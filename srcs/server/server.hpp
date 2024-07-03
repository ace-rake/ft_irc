#ifndef SERVER_HPP
# define SERVER_HPP

# include <netinet/in.h>
#include <vector>
#define PORT 12345

// Custom class: server
class	server
{
	public:
		// Constructor
		server	(	int	);

		// Copy constructor
		server	(	const server& other	);

		// Copy assignment operator overload
		server& operator = (const server& other);

		// Destructor
		~server	(void);

		void	run();

	protected:
		/*_*/;

	private:
		int			_server_fd;
		int			_server_socket;
		struct sockaddr_in	_address;
		int			_addrlen;
		std::vector<int>	_clients;
		/*_*/;

		void handleNewConnection(){};
		void handleClientMessage(int){};
};

#endif

