#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345

int main() {
	int sock = 0;
	struct sockaddr_in serv_addr;
	//char buffer[1024] = {0}; //Unused variable

	// Creating socket file descriptor
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cerr << "Socket creation error" << std::endl;
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		std::cerr << "Invalid address/ Address not supported" << std::endl;
		return -1;
	}

	// Connect to the server
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		std::cerr << "Connection failed" << std::endl;
		return -1;
	}

	// Send message to the server
	while (1)
	{
		std::string str;
		std::getline(std::cin, str);
		if (std::cin.eof())
			break;
		send(sock, str.c_str(), str.size(), 0);
		std::cout << "Sent \"" << str << "\" to server" << std::endl;
		str.clear();
	}

	// Close the connection
	close(sock);

	return 0;
}

