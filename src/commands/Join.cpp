#include "../../headers/commands.hpp"
#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"

int join(std::string command, Client &client, std::map<std::string, Channel> &channels)
{
    // join #cha1 key
    size_t spacePos = command.find(" ", 5);
    std::string channel_name = command.substr(5, spacePos - 5);
    channel_name = filteredString(channel_name);
    if (channels.find(channel_name) == channels.end())
    {
        Channel new_channel(channel_name);
        channels.insert(std::pair<std::string, Channel>(channel_name, new_channel));
    }
    std::map<std::string, Channel>::iterator it = channels.find(channel_name);

    if (it != channels.end())
    {
        if (it->second.get_signe_mode('k') == '+')
        {
            // JOIN #cha1 key
            size_t spacePos = command.find(" ", 5);
            std::string key = command.substr(spacePos + 1);
            key = filteredString(key);
            if (it->second.get_key() != key)
            {
                client.add_buffer_to_send(ERR_BADCHANNELKEY(client.get_nickname(), channel_name));
                return (1);
            }
        }
        if (it->second.add_client(client))
        {
            client.add_buffer_to_send(ERR_USERONCHANNEL(client.get_nickname(), client.get_nickname(), channel_name));
            return (1);
        }
        if (it->second.get_clients().size() == 1)
        {
            it->second.set_operator(client);
        }
    }
    else
    {
        return (1);
    }

    // client.add_buffer_to_send(RPL_JOIN(client.get_nickname(), channel_name));
    broadcastTochannel(client, RPL_JOIN(client.get_nickname(), channel_name), channel_name, channels);
    std::string nick_list = it->second.get_clients_nick();
    broadcastTochannel(client, RPL_NAMREPLY(client.get_nickname(), channel_name, nick_list), channel_name, channels);
    return (0);
}