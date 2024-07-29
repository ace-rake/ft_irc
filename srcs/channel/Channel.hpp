#ifndef CHANNEL_HPP
# define CHANNEL_HPP

// Custom class: Channel
#include <set>
#include <string>
#include <vector>
#include "../client/Client.hpp"
#include "../irc.h"

#define ERR_INVITEONLYCHAN 473
#define ERR_WRONGCHANKEY 475
#define ERR_CHANNELISFULL 471

class   Client;

typedef struct s_settings
{
	bool		inviteOnly;
	bool		plebsCanChangeTopic;
	unsigned short	userAmount;
	unsigned short	userLimit;
	std::string	creator;
}		t_settings;

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

		/* Client *	retrieveClientByNick(const std::string& name); */
		/* Client *	retrieveClientById(int id); */
		/* std::vector<Client>::const_iterator	findClient(const Client & client)const; */
		std::vector<Client>::iterator	findClient(userData, const std::string);
		std::vector<Client>::iterator	findClient(userData, int);

		void	handleJoinRequest	(Client & client, std::string psw = "");
		void	inviteOnlyErr		(Client & client);
		void	wrongPswErr		(Client & client);
		void	isFullErr		(Client & client);

		void	sendMsgToAll		(std::vector<std::string> args, Client& client);
		int	deleteClient		(Client & client);
		int	deleteClient		(std::vector<Client>::iterator);
		bool    clientIsOperator(Client client);


		void	addInviteToList	(int id);
		bool	isInInviteList	(int id)const;
		void	removeIdFromList(int id);

		void    changeTopic		(const std::string& newTopic, Client &sender);
		bool    isInClientList		(Client client);
		void	broadcastMsg		(std::string msg, Client & sender);
		void    broadcastMsg		(std::string str);

        void    kickUser(Client sender, std::string victimName, std::vector<std::string>args);

		bool	isFull			()const;

        void    setInviteOnly();
        void    removeInviteOnly();

        void    setPassword(std::string psw);
        void    removePassword();

        void    setTopicOperatorOnly();
        void    removeTopicOperatorOnly();

        void    setUserLimit(int amount);
        void    removeUserLimit();

        void    makeClientOperator(Client & client);
        void    removeClientOperator(Client & client);

        bool    everyoneCanChangeTopic();

	private:
		std::string		_channelName;
		std::vector<Client>	_clients;
		std::string		_psw;
		std::vector<Client>	_opList;
		std::set<int>		_inviteList; // List of userId's that were invited
		std::string		_channelTopic;
		t_settings		_settings;

	private:
		int	addClient		(Client & client);
};

#endif

