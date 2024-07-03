#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 12345

int	main(void)
{
	int server_fd, new_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

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

	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
	{
		perror("accept");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	std::string message;
	while (true)
	{
		std::getline(std::cin, message);
		if (std::cin.eof())
			break;
		send(new_socket, message.c_str(), static_cast<size_t>(message.size()), 0);
		std::cout << "Message sent to socket" << std::endl;
	}

	close(new_socket);
	close(server_fd);
}
