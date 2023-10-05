#pragma once

/* includes */
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <cstring>
# include <cstdlib>
# include <vector>
# include <map>
# include <poll.h>
# include <unistd.h>
# include <sstream>
# include <string>
# include <cerrno>
# include <ctime>

/* colors */
# define RESET "\033[0m"
# define BLACK "\033[0;30m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define PURPLE "\033[0;35m"
# define WHITE "\033[0;37m"

# define BUFFER_SIZE 1000
# define  MAX_CLIENTS 10

class irc
{
    private:

    public:

};