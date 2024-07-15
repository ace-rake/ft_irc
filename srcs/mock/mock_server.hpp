#ifndef MOCK_SERVER_HPP
# define MOCK_SERVER_HPP

#include "../server/Server.hpp"

// Custom class: mock_Server
class	mock_Server : public Server
{
	public:

		void mock();
		std::string createCommand(std::string, std::vector<std::string> argv);

	protected:
		/*_*/;

	private:
		/*_*/;
};

#endif

