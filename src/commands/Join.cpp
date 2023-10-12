#include "../../headers/commands.hpp"
#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"

int join(std::string command, Client &client, std::map<std::string, Channel> &channels)
{
    std::cout << "the client " << client.get_socket_fd() << client.get_nickname() << " wants to join a channel" << std::endl;
    std::string channel_name = command.substr(5, command.length() - 5);
    // remove the \r\n if it exists or both
    channel_name = filteredString(channel_name);
    if (channels.find(channel_name) == channels.end())
    {
        Channel new_channel(channel_name);
        channels.insert(std::pair<std::string, Channel>(channel_name, new_channel));
        if (new_channel.add_client(client))
        {
            client.add_buffer_to_send(ERR_USERONCHANNEL(client.get_nickname(), client.get_nickname(), channel_name));
            return (1);
        }
        new_channel.set_operator(client);
    }
    std::map<std::string, Channel>::iterator it = channels.find(channel_name);
    if (it != channels.end())
    {
        it->second.add_client(client);
    }
    else
    {
        return (1);
    }
    client.add_buffer_to_send(RPL_JOIN(client.get_nickname(), channel_name));
    return (0);
}