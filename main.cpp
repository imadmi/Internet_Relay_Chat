#include "irc_header.hpp"

int main(int argc, char const *argv[])
{
	if (argc == 3)
	{

		return (0);
	}
	else
	{
		std::cout << "Try ./ircserv [port] [password] :)" << std::endl;
		return (1);
	}
    return (0);
}
