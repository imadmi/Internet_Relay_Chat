#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

bool is_multipe_words(std::string message)
{
    for (int i = 0; i < (int)message.length(); i++)
    {
        if (message[i] == ' ')
            return true;
    }
    return false;
}

void broadcastTochannel(Client client, std::string message, std::string channel, std::map<std::string, Channel> &channels)
{
    std::map<std::string, Channel>::iterator it = channels.find(channel);
    if (it != channels.end())
    {
        std::map<int, Client>::iterator it2 = it->second.get_clients().begin();
        while (it2 != it->second.get_clients().end())
        {
            send(it2->second.get_fd(), message.c_str(), message.length(), 0);
            ++it2;
        }
    }
    else
        send(client.get_fd(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel).c_str(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel).length(), 0);
}

bool is_on_channel(Client &client, Channel &channel)
{
    std::map<int, Client> clients = channel.get_clients();

    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->second.get_nickname() == client.get_nickname())
        {
            return true;
        }
    }

    return false;
}

void broadcastTochannel_1(Client client, std::string message, std::string channel, std::map<std::string, Channel> &channels)
{
    std::map<std::string, Channel>::iterator it = channels.find(channel);
    if (it != channels.end())
    {
        std::map<int, Client>::iterator it2 = it->second.get_clients().begin();
        while (it2 != it->second.get_clients().end())
        {
            if (it2->second.get_fd() != client.get_fd())
                send(it2->second.get_fd(), message.c_str(), message.length(), 0);
            ++it2;
        }
    }
    else
        send(client.get_fd(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel).c_str(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel).length(), 0);
}

std::string extractName(std::string message, int *spaces)
{
    std::string name;
    std::string::iterator it;
    // skip all spaces
    for (it = message.begin(); it != message.end(); ++it)
    {
        if (*it != ' ')
        {
            break;
        }
        *spaces += 1;
    }

    for (it = message.begin(); it != message.end(); ++it)
    {
        if (*it == ' ')
        {
            break;
        }
        name += *it;
    }
    return name;
}
std::string extract_channel_name(std::string message)
{
    std::string channel_name;
    std::string::iterator it;
    for (it = message.begin(); it != message.end(); ++it)
    {
        if (*it == ' ')
        {
            break;
        }
        channel_name += *it;
    }
    return channel_name;
}

void privmsg_user(std::string to_send, Client &client, std::map<int, Client> &clients, std::string receiver)
{
    std::map<int, Client>::iterator it2;
    if (to_send.empty())
        send(client.get_fd(), ERR_NOTEXTTOSEND(client.get_nickname()).c_str(), ERR_NOTEXTTOSEND(client.get_nickname()).length(), 0);
    else
    {
        for (it2 = clients.begin(); it2 != clients.end(); ++it2)
        {
            if (it2->second.get_nickname() == receiver)
            {
                send(it2->second.get_fd(), RPL_PRIVMSG(client.get_nickname(), client.get_username(), receiver, to_send).c_str(), RPL_PRIVMSG(client.get_nickname(), client.get_username(), receiver, to_send).length(), 0);
                break;
            }
        }
        if (it2 == clients.end())
        {
            send(client.get_fd(), ERR_NOSUCHNICK(client.get_nickname(), receiver).c_str(), ERR_NOSUCHNICK(client.get_nickname(), receiver).length(), 0);
        }
    }
}

void privmsg(std::string message, Client &client, std::map<int, Client> &clients, std::map<std::string, Channel> &channels)
{
    int spaces = 0;
    std::string receiver = extractName(message.substr(8, message.length() - 8), &spaces);
    std::string channel_name = receiver.find("#") == std::string::npos ? "" : receiver;
    std::string to_send = message.substr(8 + receiver.length() + 1 + spaces, message.length());

    std::map<int, Client>::iterator it2;

    to_send = (is_multipe_words(to_send) ? to_send.substr(1, to_send.length() - 1) : to_send);

    if (receiver.empty() && channel_name.empty())
        send(client.get_fd(), ERR_NORECIPIENT(client.get_nickname()).c_str(), ERR_NORECIPIENT(client.get_nickname()).length(), 0);
    else if (!receiver.empty() && channel_name.empty())
    {
        privmsg_user(to_send, client, clients, receiver);
    }
    else if (!channel_name.empty())
    {
        if (to_send.empty())
            send(client.get_fd(), ERR_NOTEXTTOSEND(client.get_nickname()).c_str(), ERR_NOTEXTTOSEND(client.get_nickname()).length(), 0);
        else if (is_on_channel(client, channels[channel_name]))
            broadcastTochannel_1(client, RPL_PRIVMSG(client.get_nickname(), client.get_username(), channel_name, to_send), channel_name, channels);
        else
            send(client.get_fd(), ERR_CANNOTSENDTOCHAN(client.get_nickname(), channel_name).c_str(), ERR_CANNOTSENDTOCHAN(client.get_nickname(), channel_name).length(), 0);
    }
}
