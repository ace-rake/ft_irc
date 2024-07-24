#ifndef CHANNEL_HPP
# define CHANNEL_HPP

// Custom class: Channel
#include <set>
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
		void	handleJoinRequest	(client & client, std::string psw = "");
		void	sendMsgToAll		(std::vector<std::string> args, client& client);
		std::vector<client> getClients(){return _clients;}
		int	deleteClient		(client client);
		client *	retrieveClientByNick(const std::string& name);
		client *	retrieveClientById(int id);
		std::vector<client>::const_iterator	findClient(const client & client)const;
		bool    clientIsOperator(client client);

		void	addInviteToList	(int id);
		bool	isInInviteList	(int id)const;
		void	removeIdFromList(int id);



	private:
		std::string		_channelName;
		std::vector<client>	_clients;
		std::string		_psw;
		std::vector<client>	_opList;
		std::set<int>		_inviteList; // List of userId's that were invited

	private:
		void	broadcastMsg		(std::string msg, client & sender);
		int	addClient		(client & client);
};

#endif

