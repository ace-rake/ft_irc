#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <netinet/in.h>
#include <ostream>
#include <string>
#include <sys/poll.h>
#include <vector>

class   Channel;

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
		void	create(std::string userData);

		void	run	(	void	);

		const bool operator == (const client & other)const;

		std::string	getHostName()const{return _hostName;}
		std::string	getNickName()const{return _nickName;}
		void		setNickName(std::string newNick){_nickName = newNick;}
		std::string	getUserName()const{return _userName;}
		std::string	getRealName()const{return _realName;}
		std::string	getIp()const{return _ip;}
		int		getId()const{return _clientId;}
		void		sendMessageToClient(std::string) const;
        void        addToClientChannelList(Channel* channel);
        void        removeFromClientChannelList(Channel* channel);
        void        printClientChannels() const;

	protected:
		/*_*/;

	private:
		int			_server_fd;
		struct pollfd *		_client_fd;
		int			_clientId;

		/* User data */
		std::string 		_hostName;
		std::string 		_userName;
		std::string 		_nickName;
		std::string 		_realName;
		std::string		_ip;//TODO Do we need to get ip from server or client, rn its whatever the initial handshake gives
        std::vector<std::string>    _channelNames;
		/*_*/;

		void	idle();
		void	send_message(std::string);
		void	receiveClientFd();
};

std::ostream& operator << (std::ostream& os, const client& client);

#endif
