#ifndef CHANNEL_HPP
# define CHANNEL_HPP

// Custom class: Channel
#include <string>
#include <vector>
#include "../client/client.hpp"
class	Channel
{
	public:
		// Constructor
		Channel	(	void	);

		// Destructor
		~Channel	(void);


	private:
		std::string		_channelName;
		std::vector<client>	_clients;
		std::string		_psw;

	private:
		void	broadcastMsg		(std::string msg);
		void	handleJoinRequest	(client client);
};

#endif

