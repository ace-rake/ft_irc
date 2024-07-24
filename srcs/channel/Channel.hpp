#ifndef CHANNEL_HPP
# define CHANNEL_HPP

// Custom class: Channel
#include <set>
#include <string>
#include <vector>
#include "../client/Client.hpp"

class   Client;

class	Channel
{
	public:
		// Constructor
		Channel	(	std::string name, std::string psw = ""	);

		// Destructor
		~Channel	(void);

		std::string getName()const{return _channelName;}
		std::vector<Client> getClients(){return _clients;}
		std::string getTopic()const{return _channelTopic;}

		Client *	retrieveClientByNick(const std::string& name);
		Client *	retrieveClientById(int id);
		std::vector<Client>::const_iterator	findClient(const Client & client)const;

		void	handleJoinRequest	(Client & client, std::string psw = "");
		void	sendMsgToAll		(std::vector<std::string> args, Client& client);
		int	deleteClient		(Client client);
		bool    clientIsOperator(Client client);


		void	addInviteToList	(int id);
		bool	isInInviteList	(int id)const;
		void	removeIdFromList(int id);

		void    changeTopic		(const std::string& newTopic);
		bool    isInClientList		(Client client);
		void	broadcastMsg		(std::string msg, Client & sender);
		void    broadcastMsg		(std::string str);




	private:
		std::string		_channelName;
		std::vector<Client>	_clients;
		std::string		_psw;
		std::vector<Client>	_opList;
		std::set<int>		_inviteList; // List of userId's that were invited
		std::string     _channelTopic;

	private:
		int	addClient		(Client & client);
};

#endif

