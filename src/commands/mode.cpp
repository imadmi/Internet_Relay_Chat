#include "../../headers/commands.hpp"
#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"

bool is_valid_mode(Channel &channel, std::string mode, Client client)
{
    if (mode.find_first_not_of("+-itkol") != std::string::npos)
    {
        client.set_buff_to_send(":" + client.get_nickname() + " 472 " + client.get_nickname() + " " + mode + " :is an unknown mode character to me\r\n");
        return false;
    }

    if (mode[0] == '+')
    {
        for (int i = 1; i < (int)mode.length(); i++)
        {
            channel.set_mode(mode[i], '+');
        }
    }
    else if (mode[0] == '-')
    {
        for (int i = 1; i < (int)mode.length(); i++)
        {

            channel.set_mode(mode[i], '-');
        }
    }
    else
        return false;
    return true;
}

void mode(std::string command, Client &client, std::map<std::string, Channel> &channels)
{
    // the first string after the channel name is the mode
    size_t spacePos = command.find(" ", 5);
    std::string channel_name = command.substr(5, spacePos - 5);
    channel_name = filteredString(channel_name);
    if (channels.find(channel_name) == channels.end())
    {
        client.set_buff_to_send(":" + client.get_nickname() + " 403 " + client.get_nickname() + " " + channel_name + " :No such channel\r\n");
        return;
    }
    Channel &channel = channels[channel_name];

    if (channel.get_operators().find(client.get_socket_fd()) == channel.get_operators().end())
    {
        client.set_buff_to_send(":" + client.get_nickname() + " 482 " + client.get_nickname() + " " + channel_name + " :You're not channel operator\r\n");
        return;
    }
    std::string parameter = command.substr(spacePos + 1);
    size_t spaceWithinParamPos = parameter.find(" ");
    std::string modePart = parameter.substr(0, spaceWithinParamPos);
    modePart = filteredString(modePart);
    std::string valuePart = parameter.substr(spaceWithinParamPos + 1);
    valuePart = filteredString(valuePart);

    modePart = filteredString(modePart);
    if (is_valid_mode(channel, modePart, client) == false)
        return;
    if (modePart == "+k" && !valuePart.empty() && modePart != valuePart)
    {
        channel.set_mode('k', '+');
        channel.set_key(valuePart);
    }
}