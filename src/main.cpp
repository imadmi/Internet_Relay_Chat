#include "../headers/irc_header.hpp"

void signal_handler(int signal)
{
    (void)signal;
    std::cout << RED << "\nBYE BYE" << RESET << std::endl;
    exit(EXIT_SUCCESS);
}

int main(int ac, char *argv[])
{
    if (ac != 3)
    {
        std::cerr << RED << "try ./ircserv [port] [password]" << RESET << std::endl;
        exit(EXIT_FAILURE);
    }
    signal(SIGINT, signal_handler);

    try
    {
        Irc irc(std::stoi(argv[1]), argv[2]);
        irc.runServer();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return (EXIT_SUCCESS);
}
