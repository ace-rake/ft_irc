#include "server.hpp"

int main(void)
{
	server server(12345);
	server.run();
	return (0);
}
