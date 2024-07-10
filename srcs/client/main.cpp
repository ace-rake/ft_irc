#include "client.hpp"
#include <iostream>
#include <string>


int main(void)
{
	std::string str;
	std::getline(std::cin, str);
	
	client client(PORT);
	client.run();
}
