#ifndef SERVER_HPP
# define SERVER_HPP

# include <netinet/in.h>
#include <sys/poll.h>
#define PORT 12345
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

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
		struct pollfd		_fds[MAX_CLIENTS];
		/*_*/;

		void	idle();
		void handleNewConnection();
		void handleClientMessage(int*);
};

#endif

