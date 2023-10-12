#include "../../headers/commands.hpp"
#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"

int invite(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients)
{
    std::cout << "the client " << client.get_socket_fd() << client.get_nickname() << " wants to invite a client" << std::endl;
    std::string nickname = command.substr(7, command.length() - 7);
    std::cout << "the nickname is " << nickname << std::endl;
    // remove the \r\n if it exists or both
    nickname = filteredString(nickname);
    if (client_already_exist(nickname, clients))
    {
        client.add_buffer_to_send(ERR_NOSUCHNICK(client.get_nickname(), nickname));
        return (1);
    }
    if (channels.find(nickname) == channels.end())
    {
        client.add_buffer_to_send(ERR_NOSUCHCHANNEL(client.get_nickname(), nickname));
        return (1);
    }
    std::map<std::string, Channel>::iterator it = channels.find(nickname);
    if (it != channels.end())
    {
        it->second.add_client(client);
    }
    else
    {
        return (1);
    }
    // client.add_buffer_to_send(RPL_INVITING(client.get_nickname(), nickname));
    return (0);
}