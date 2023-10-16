
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define BUFFER_SIZE 1000

int main(int ac, const char *av[])
{
    if (ac != 4)
    {
        std::string a;

        return (EXIT_FAILURE);
    }

    int clientFd = std::atoi(av[1]);
    if (clientFd < 3 || clientFd > 1000)
    {
        return (EXIT_FAILURE);
    }
    std::string nick(av[2]);
    if (nick.empty())
    {
        return (EXIT_FAILURE);
    }
    std::string file_path(av[3]);
    if (file_path.empty())
    {
        return (EXIT_FAILURE);
    }
    std::string msg;
    char buffer[BUFFER_SIZE];
    std::string file;
    FILE *fd = fopen(file_path.c_str(), "rb");
    if (fd == NULL)
    {
        return (EXIT_FAILURE);
    }
    while (!feof(fd))
    {
        int size = fread(buffer, 1, BUFFER_SIZE, fd);
        if (size < 0)
            break;
        file.append(buffer, size);
    }
    fclose(fd);

    int send_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (send_fd < 0)
    {
        return (EXIT_FAILURE);
    }
    int opt = 1;
    if (setsockopt(send_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        return (EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    bzero(&addr, addrlen);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(send_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        return (EXIT_FAILURE);
    }

    if (listen(send_fd, 1) < 0)
    {
        return (EXIT_FAILURE);
    }

    int new_socket = accept(send_fd, (struct sockaddr *)&addr, &addrlen);
    if (new_socket < 0)
    {
        return (EXIT_FAILURE);
    }

    send(new_socket, file.c_str(), file.size(), 0);
    close(new_socket);
    close(send_fd);

    return (EXIT_SUCCESS);
}
