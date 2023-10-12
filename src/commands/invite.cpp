#include "../../headers/commands.hpp"
#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"

int invite(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients)
{
    // INVITE <nickname> <channel>
    std::string nickname = command.substr(7, command.find(" ", 7) - 7);
    nickname = filteredString(nickname);
    std::string channel_name = command.substr(command.find(" ", 7) + 1, command.length() - command.find(" ", 7) - 1);
    channel_name = filteredString(channel_name);

    if (client_already_exist(nickname, clients))
    {
        client.add_buffer_to_send(ERR_NOSUCHNICK(client.get_nickname(), nickname));
        return (1);
    }
    if (channels.find(channel_name) == channels.end())
    {
        client.add_buffer_to_send(ERR_NOSUCHCHANNEL(client.get_nickname(), channel_name));
        return (1);
    }
    Channel &channel = channels[channel_name];
    std::cout << "channel name: " << channel.get_name() << std::endl;
    // client.add_buffer_to_send(RPL_INVITING(client.get_nickname(), nickname));
    return (0);
}