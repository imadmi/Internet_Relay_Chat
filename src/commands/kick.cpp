#include "../../headers/commands.hpp"
#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"

void kick(std::string command, Client &client, std::map<std::string, Channel> &channels)
{
    std::string channel_name = command.substr(5, command.find(" ", 5) - 5);
    std::string nickname = command.substr(command.find(" ", 5) + 1, command.length() - command.find(" ", 5) - 1);
    nickname = nickname.substr(0, nickname.find(" "));
    std::string comment = command.substr(command.find(":", 5) + 1, command.length() - command.find(":", 5) - 1);
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
    while (it != clients.end())
    {
        if (it->second.get_nickname() == nickname)
        {
            broadcastTochannel(client, RPL_KICK(client.get_nickname(), channel_name, nickname, comment), channel_name, channels);
            channel.remove_client(it->second);
            return;
        }
        it++;
    }
    client.add_buffer_to_send(ERR_NOSUCHNICK(client.get_nickname(), channel_name));
}