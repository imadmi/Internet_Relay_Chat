#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

static bool is_in_channel(Client client, Channel channel)
{

    std::map<int, Client>::iterator it;
    for (it = channel.get_clients().begin(); it != channel.get_clients().end(); ++it)
    {
        if (it->second.get_nickname() == client.get_nickname())
            return true;
    }
    return false;
}

static bool is_operator(Client &client, Channel &channel)
{
    std::map<int, Client> operators = channel.get_operators();
    
    for (std::map<int, Client>::iterator it = operators.begin(); it != operators.end(); ++it)
    {
        if (it->second.get_nickname() == client.get_nickname())
        {
            return true;
        }
    }
    
    return false;
}


static std::string extract_channel_name(std::string command)
{
    std::string channel_name = "";
    int start = 0;
    while (command[start] != '#' && start < (int)command.length())
        start++;
    for (int i = start; i < (int)command.length(); i++)
    {
        if (command[i] == ' ')
            break;
        channel_name += command[i];
    }
    return channel_name;
}

static std::string extract_topic(std::string command)
{
    std::istringstream iss(command);
    std::string word;

    // Skip the first word (channel name)
    iss >> word;

    // Read the rest of the string as the topic
    std::string topic;
    while (iss >> word)
    {
        if (!topic.empty())
        {
            topic += ' ';
        }
        topic += word;
    }

    return topic;
}

void topic(std::string &command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients)
{
    std::string channel_name = filteredString(extract_channel_name(command));
    std::string topic = extract_topic(command.substr(5 + channel_name.length(), command.length() - 5 - channel_name.length()));
    if (!channel_name.empty() && channels.find(channel_name) != channels.end())
    {
        if (is_in_channel(client, channels[channel_name]) == false)
        {
            send(client.get_fd(), ERR_NOTONCHANNEL(client.get_nickname(), channel_name).c_str(), ERR_NOTONCHANNEL(client.get_nickname(), channel_name).length(), 0);
            return;
        }
        else if (!topic.empty() && is_operator(client, channels[channel_name]))
        {
            channels[channel_name].set_topic(topic);
            std::cout << "channel: " <<  channels[channel_name].get_topic() << std::endl;
            send(client.get_fd(), RPL_TOPIC(client.get_nickname(), channel_name, topic).c_str(), RPL_TOPIC(client.get_nickname(), channel_name, topic).length(), 0);
        }
        else if (topic.empty())
        {
            send(client.get_fd(), RPL_TOPIC(client.get_nickname(), channel_name, channels[channel_name].get_topic()).c_str(), RPL_TOPIC(client.get_nickname(), channel_name, channels[channel_name].get_topic()).length(), 0);
            return;
        }
    }
    else if (channels.find(channel_name) == channels.end())
        send(client.get_fd(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel_name).c_str(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel_name).length(), 0);
    else
        send(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "TOPIC").c_str(), ERR_NEEDMOREPARAMS(client.get_nickname(), "TOPIC").length(), 0);
    std::cout<< YELLOW<< "is operator :"<<is_operator(client, channels[channel_name])<<std::endl;
}
