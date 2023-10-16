#include "../headers/Irc.hpp"

void signal_handler(int signal)
{
    (void)signal;
    std::cout << RED << "\nBYE BYE" << RESET << std::endl;
    exit(EXIT_SUCCESS);
}

static int IsDigit(char *argv)
{
    for (int i = 0; argv[i] != '\0'; i++)
    {
        if (!std::isdigit(argv[i]))
            return (1);
    }
    return (0);
}

int main(int ac, char *argv[])
{
    if (ac != 3 || IsDigit(argv[1]) || argv[2] == NULL || !strlen(argv[2]))
    {
        std::cerr << RED << "try ./ircserv [port] [password]" << RESET << std::endl;
        exit(EXIT_FAILURE);
    }
    signal(SIGINT, signal_handler);

    try
    {
        Irc irc(std::atoi(argv[1]), argv[2]);
        irc.runServer();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return (EXIT_SUCCESS);
}
