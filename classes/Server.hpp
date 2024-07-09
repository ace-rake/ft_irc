#ifndef SERVER_HPP
# define SERVER_HPP

#include <netinet/in.h>
#include <sys/poll.h>
#include "../irc.h"

class   Server
{
    public:
        Server(void);

        ~Server(void);

        void    run(void);

    private:
        int                 _serverFd;
        int                 _serverSocket;
        int                 _addrLen;
        struct pollfd       _fds[MAX_CLIENTS];
        struct sockaddr_in  _address;

    private:
        void    idle(void);
        bool    findSuitableIp(struct hostent *host);
        void    noSuitableIpFound(void);
        void    getIpAddress(void);
        void    handleNewConnection(void);
        void    handleClientMessage(int*);
};

#endif
