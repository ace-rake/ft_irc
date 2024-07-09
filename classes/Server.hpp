#ifndef SERVER_HPP
# define SERVER_HPP

#include <netinet/in.h>
#include "../irc.h"

class   Server
{
    public:
        Server(void);

        ~Server(void);

        //void    run(void);

    private:
        int                 _serverFd;
        int                 _serverSocket;
        int                 _addrLen;
        int                 _fds[MAX_CLIENTS];
        struct sockaddr_in  _address;

    private:
        //void    idle(void);
        void    getIpAddress(void);
};

#endif
