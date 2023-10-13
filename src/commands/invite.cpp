#include "../../headers/commands.hpp"
#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"

Client &get_client_by_nickname(std::string &nick, std::map<int, Client> &clients)
{
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->second.get_nickname() == nick)
        {
            return it->second;
        }
    }

    return clients.begin()->second;
}

int invite(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients)
{
    std::string nickname = command.substr(7, command.find(" ", 7) - 7);
    nickname = filteredString(nickname);
    std::string channel_name = command.substr(command.find(" ", 7) + 1, command.length() - command.find(" ", 7) - 1);
    channel_name = filteredString(channel_name);

    if (client_already_exist(nickname, clients) == false)
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
    if (client_already_exist(client.get_nickname(), channel.get_clients()) == false)
    {
        client.add_buffer_to_send(ERR_NOTONCHANNEL(client.get_nickname(), channel_name));
        return (1);
    }
    if (client_already_exist(nickname, channel.get_clients()) == true)
    {
        client.add_buffer_to_send(ERR_USERONCHANNEL(client.get_nickname(), nickname, channel_name));
        return (1);
    }
    channel.add_invitee(nickname);
    Client &invited_client = get_client_by_nickname(nickname, clients);
    std::string message = RPL_INVITING(client.get_nickname(), nickname, channel_name);
    broadcastTochannel(client, message, channel_name, channels);
    message = RPL_AWAY(client.get_nickname(), nickname, "You have been invited to join the channel");
    send(invited_client.get_fd(), message.c_str(), message.length(), 0);

    return (0);
}