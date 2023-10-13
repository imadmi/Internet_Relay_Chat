#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
    const char* serverIP = "127.0.0.1";
    const int serverPort = 6667;

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        std::cerr << "Failed to create socket" << std::endl;
        std::exit(1);
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cerr << "Failed to connect to the server" << std::endl;
        std::exit(1);
    }

    std::cout << "Connected to the IRC server at " << serverIP << " on port " << serverPort << std::endl;


    while(1)
    {
        
    }

    close(clientSocket);

    return 0;
}
