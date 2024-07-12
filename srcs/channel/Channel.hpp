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
		Channel	(	std::string name, std::string psw = ""	);

		// Destructor
		~Channel	(void);

		std::string getName()const{return _channelName;}
		void	handleJoinRequest	(client client, std::string psw = "");


	private:
		std::string		_channelName;
		std::vector<client>	_clients;
		std::string		_psw;

	private:
		void	broadcastMsg		(std::string msg);
		int	addClient		(client client);
		client *	retrieveClientByNick	(std::string name);
		client *	retrieveClientById(int id);
		int	deleteClient		(client client);
};

#endif

