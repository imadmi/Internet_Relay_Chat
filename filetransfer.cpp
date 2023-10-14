
// #include "../headers/Irc.hpp"

// av 1 = cliebt fd 
// av 2 = cliebt nicknam


#include <ctime>
#include <sys/time.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include <map>
#include <poll.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <fcntl.h>

int main(int argc, const char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: ./DwnloadBot <client_fd> <client_nickname>" << std::endl;
        std::string a;

        return 1;
    }
    std::string nick(argv[2]);

    std::string file_path = "/Users/imimouni/Downloads/imimouni.jpeg";

    std::string msg;
    char buffer[1024];
    std::string file;
    FILE *fd = fopen(file_path.c_str(), "rb");
    if (fd == NULL)
    {
        // std::string(argv[2]) nick = 
        msg = "PRIVMSG " + nick + " :Error opening file\r\n";
        send(std::atoi(argv[1]), msg.c_str(), msg.size(), 0);
        return 1;
    }
    while (!feof(fd))
    {
        int size = fread(buffer, 1, 1024, fd);
        if (size < 0)
            break;
        file.append(buffer, size);
    }
    fclose(fd);

    int send_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (send_fd < 0)
    {
        std::cout << "Error creating socket" << std::endl;
        return 1;
    }
    int opt = 1;
    if (setsockopt(send_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        std::cout << "Error setting socket options" << std::endl;
        return 1;
    }
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    memset(&addr, 0, addrlen);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(send_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        std::cout << "Error binding socket here" << std::endl;
        return 1;
    }
    if (listen(send_fd, 1) < 0)
    {
        std::cout << "Error listening on socket" << std::endl;
        return 1;
    }
    int new_socket = accept(send_fd, (struct sockaddr *)&addr, &addrlen);
    if (new_socket < 0)
    {
        std::cout << "Error accepting connection" << std::endl;
        return 1;
    }
    send(new_socket, file.c_str(), file.size(), 0);
    close(new_socket);
    close(send_fd);

	msg = "PRIVMSG " + nick + " :" + '\x01' + "DCC SEND " + file_path + " 0 9999 " + std::to_string(file.size()) + '\x01';
	msg += "\r\n";
    
    send(std::atoi(argv[1]), msg.c_str(), msg.size(), 0);

    return 0;
}


// main(int ac, char *av)
//
// {
//     std::string file_path = "/Users/imimouni/Downloads/imimouni.jpeg";

// 	std::string msg;
// 	char buffer[1024];
// 	std::string file;
// 	FILE *fd = fopen(file_path.c_str(), "rb");
// 	if (fd == NULL)
// 	{
// 		msg = "PRIVMSG " + std::to_string(av[2]) + " :" + "Error opening file" + "\r\n";
// 		send(std::atoi(av[1]), msg.c_str(), msg.size(), 0);
// 		return 1 ;
// 	}
// 	while (!feof(fd))
// 	{
// 		int size = fread(&buffer, 1, 1024, fd);
// 		if (size < 0)
// 			break;
// 		file.append(buffer, size);
// 	}
// 	fclose(fd);


//     int send_fd = socket(AF_INET, SOCK_STREAM, 0);
//     if (send_fd < 0)
//    
// {
//         std::cout << "Error creating socket" << std::endl;
//         return 1 ;
//     }
//     int opt = 1;
//     if (setsockopt(send_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
//    
// {
//         std::cout << "Error setting socket options" << std::endl;
//         return 1 ;
//     }
//     struct sockaddr_in addr;
//     int addrlen = sizeof(addr);
//     memset(&addr, 0, addrlen);
//     addr.sin_family = AF_INET;
//     addr.sin_port = htons(9999);
//     addr.sin_addr.s_addr = INADDR_ANY;

//     if (bind(send_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
//    
// {
//         std::cout << "Error binding socket hire" << std::endl;
//         return 1 ;
//     }
//     if (listen(send_fd, 1) < 0)
//    
// {
//         std::cout << "Error listening on socket" << std::endl;
//         return 1 ;
//     }
//     int new_socket = accept(send_fd, (struct sockaddr *)&addr, (socklen_t *)&addrlen);
//     if (new_socket < 0)
//    
// {
//         std::cout << "Error accepting connection" << std::endl;
//         return 1 ;
//     }
//     send(new_socket, file.c_str(), file.size(), 0);
//     close(new_socket);
//     close(send_fd);
//     exit(1);


// 	msg = "PRIVMSG " + std::to_string(av[2]) + " :" + '\x01' + "DCC SEND " + file_path + " 0 9999 " + std::to_string(file.size()) + '\x01';
// 	msg += "\r\n";
// 	send(std::atoi(av[1]), msg.c_str(), msg.size(), 0);
// }