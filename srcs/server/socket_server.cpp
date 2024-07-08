#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>

#define PORT 12345
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int	main(void)
{
	int server_fd, new_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	struct pollfd fds[MAX_CLIENTS];

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	fds[0].fd = server_fd;
	fds[0].events = POLLIN;
	for (int i = 1; i < MAX_CLIENTS; ++i)
		fds[i].fd = -1;

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 1) < 0)
	{
		perror("listen");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	

	std::cout << "Server is listening on port: " << PORT << std::endl;

	while (1)
	{
		int retval = poll(fds, MAX_CLIENTS, -1);

		if (retval > 0)
		{
			if (fds[0].revents & POLLIN)
			{
				new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
				for (int i = 1; i < MAX_CLIENTS; ++i)
				{
					if (fds[i].fd == -1)
					{
						fds[i].fd = new_socket;
						fds[i].events= POLLIN;
						std::cout << "Client connected" << std::endl;
						break;
					}
				}
			}

			for (int i = 1; i < MAX_CLIENTS; ++i)
			{
				if (fds[i].fd != -1 && (fds[i].revents & POLLIN))
				{
					char buffer[BUFFER_SIZE];
					int valread = read(fds[i].fd, buffer, BUFFER_SIZE);
					if (valread == 0)
					{
						close(fds[i].fd);
						fds[i].fd = -1;
						std::cout << "Client disconnected" <<std::endl;
					}
					else
					{
						buffer[valread] = '\0';
						std::cout << "Received:\t" << buffer << std::endl;
					}
				}
			}
		}
	}
	close(new_socket);
	close(server_fd);
}

/* old */
/* int	main(void) */
/* { */
/* 	int server_fd, new_socket; */
/* 	struct sockaddr_in address; */
/* 	int addrlen = sizeof(address); */
/*  */
/* 	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) */
/* 	{ */
/* 		perror("Socket creation failed"); */
/* 		exit(EXIT_FAILURE); */
/* 	} */
/*  */
/* 	address.sin_family = AF_INET; */
/* 	address.sin_addr.s_addr = INADDR_ANY; */
/* 	address.sin_port = htons(PORT); */
/*  */
/* 	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) */
/* 	{ */
/* 		perror("bind failed"); */
/* 		close(server_fd); */
/* 		exit(EXIT_FAILURE); */
/* 	} */
/*  */
/* 	if (listen(server_fd, 1) < 0) */
/* 	{ */
/* 		perror("listen"); */
/* 		close(server_fd); */
/* 		exit(EXIT_FAILURE); */
/* 	} */
/*  */
/* 	std::cout << "Server is listening on port: " << PORT << std::endl; */
/*  */
/* 	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) */
/* 	{ */
/* 		perror("accept"); */
/* 		close(server_fd); */
/* 		exit(EXIT_FAILURE); */
/* 	} */
/*  */
/* 	std::string message; */
/* 	while (true) */
/* 	{ */
/* 		std::getline(std::cin, message); */
/* 		if (std::cin.eof()) */
/* 			break; */
/* 		send(new_socket, message.c_str(), static_cast<size_t>(message.size()), 0); */
/* 		std::cout << "Message sent to socket" << std::endl; */
/* 	} */
/*  */
/* 	close(new_socket); */
/* 	close(server_fd); */
/* } */
