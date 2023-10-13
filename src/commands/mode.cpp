#include "../../headers/commands.hpp"
#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"

void mode(std::string command, Client &client, std::map<std::string, Channel> &channels)
{
    std::string channel_name = command.substr(5, command.find(" ", 5) - 5);
    std::string mode = command.substr(command.find(" ", 5) + 1, command.length() - command.find(" ", 5) - 1);
    mode = filteredString(mode);

    if (mode.find_first_not_of("+-itkol") != std::string::npos)
    {
        client.set_buff_to_send(":" + client.get_nickname() + " 472 " + client.get_nickname() + " " + mode + " :is an unknown mode character to me\r\n");
        return;
    }
    if (channels.find(channel_name) == channels.end())
    {
        client.set_buff_to_send(":" + client.get_nickname() + " 403 " + client.get_nickname() + " " + channel_name + " :No such channel\r\n");
        return;
    }
    Channel &channel = channels[channel_name];
    if (mode[0] == '+')
    {
        for (int i = 1; i < (int)mode.length(); i++)
        {
            if (channel.get_signe_mode(mode[i]) == '-')
                channel.set_mode(mode[i], '+');
        }
    }
    else if (mode[0] == '-')
    {
        for (int i = 1; i < (int)mode.length(); i++)
        {
            if (channel.get_signe_mode(mode[i]) == '+')
                channel.set_mode(mode[i], '-');
        }
    }
}