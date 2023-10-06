#include "../headers/irc_header.hpp"


void Irc::add_new_client(int client_fd)
{
    Client new_client;

    _clients.insert(std::make_pair(client_fd , new_client));
}

void Irc::remove_client(int client_fd)
{
    _clients.erase(client_fd);
}
