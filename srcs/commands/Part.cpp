#include "../server/Server.hpp"


static void partSingleChannel(Channel & channel, const client & client)
{
	channel.deleteClient(client);
}

static std::vector<std::string> splitChannels(std::string channels)
{

}

void		Server::part(const client & client, std::string channels)
{
	
}
