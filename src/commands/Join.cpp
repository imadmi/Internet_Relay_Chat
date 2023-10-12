#include "../../headers/commands.hpp"
#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"

int join(std::string command, Client &client, std::map<std::string, Channel> &channels)
{
    std::string channel_name = command.substr(5, command.length() - 5);
    channel_name = filteredString(channel_name);
    std::cout << channel_name << std::endl;
    if (channels.find(channel_name) == channels.end())
    {
        Channel new_channel(channel_name);
        channels.insert(std::pair<std::string, Channel>(channel_name, new_channel));
        new_channel.set_operator(client);
    }
    std::map<std::string, Channel>::iterator it = channels.find(channel_name);
    if (it != channels.end())
    {
        if (it->second.add_client(client))
        {
            client.add_buffer_to_send(ERR_USERONCHANNEL(client.get_nickname(), client.get_nickname(), channel_name));
            return (1);
        }
    }
    else
    {
        return (1);
    }

    client.add_buffer_to_send(RPL_JOIN(client.get_nickname(), channel_name));
    broadcastTochannel(client, RPL_JOIN(client.get_nickname(), channel_name), channel_name, channels);
    return (0);
}