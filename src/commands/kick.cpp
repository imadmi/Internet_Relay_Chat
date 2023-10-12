#include "../../headers/commands.hpp"
#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"

void kick(std::string command, Client &client, std::map<std::string, Channel> &channels)
{
    std::string channel_name = command.substr(5, command.find(" ", 5) - 5);
    std::cout << "channel_name : " << channel_name << std::endl;
    std::string nickname = command.substr(command.find(" ", 5) + 1, command.length() - command.find(" ", 5) - 1);
    std::cout << "nickname : " << nickname << std::endl;
    nickname = nickname.substr(0, nickname.find(" "));
    std::cout << "nickname : " << nickname << std::endl;
    // std::string comment = command.substr(command.find(":", 5) + 1, command.length() - command.find(":", 5) - 1);
    // std::cout << "comment : " << comment << std::endl;
    // print the first channel in the map
    if (channels.find(channel_name) == channels.end())
    {
        client.add_buffer_to_send(ERR_NOSUCHCHANNEL(client.get_nickname(), channel_name));
        return;
    }
    Channel &channel = channels.find(channel_name)->second;
    if (channel.get_clients().find(client.get_fd()) == channel.get_clients().end())
    {
        client.add_buffer_to_send(ERR_NOTONCHANNEL(client.get_nickname(), channel_name));
        return;
    }
    if (channel.get_operators().find(client.get_fd()) == channel.get_operators().end())
    {
        client.add_buffer_to_send(ERR_CHANOPRIVSNEEDED(client.get_nickname(), channel_name));
        return;
    }
    std::map<int, Client> &clients = channel.get_clients();
    std::map<int, Client>::iterator it = clients.begin();
    std::cout << "channel_name : " << std::endl;
    while (it != clients.end())
    {
        std::cout << "channel_name : " << std::endl;
        if (it->second.get_nickname() == nickname)
        {
            channel.remove_client(it->second);
            // TODO : send a private message to the client

            // it->second.add_buffer_to_send(RPL_KICK(it->second.get_nickname(), channel_name));
            return;
        }
        it++;
    }
    std::cout << "im out " << std::endl;
    client.add_buffer_to_send(ERR_NOSUCHNICK(client.get_nickname(), channel_name));
}