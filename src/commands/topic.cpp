#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"


// static std::string extract_channel_name(std::string command)
// {
//     std::string channel_name;
//     std::string::iterator it;
//     int spaces = 0;
//     for (it = command.begin(); it != command.end(); ++it)
//     {
//         if (*it == ' ')
//             spaces++;
//         if (spaces == 0)
//             channel_name += *it;
//     }
//     return channel_name;
// }

// static std::string extract_topic(std::string command)
// {
//     std::string topic;
//     std::string::iterator it;
//     int spaces = 0;
//     for (it = command.begin(); it != command.end(); ++it)
//     {
//         if (*it == ' ')
//             spaces++;
//         if (spaces == 1)
//             topic += *it;
//     }
//     return topic;
// }
// void topic( std::string command, Client &client, std::map<std::string, Channel> &channels , std::map<int, Client> &clients)
// {
//     std::string channel_name;
//     std::string topic;
//     std::string::iterator it;
//     int spaces = 0;
//     channel_name = extract_channel_name(command.substr(5, command.length() - 5));
//     std::cout<<RED << channel_name << RESET << std::endl;
//     topic = extract_topic(command.substr(5 + channel_name.length() + 1, command.length() - 5 - channel_name.length() - 1));
//     std::cout<<YELLOW << topic << RESET << std::endl;
//     if (channel_name.empty())
//     {
//         send(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "TOPIC").c_str(), ERR_NEEDMOREPARAMS(client.get_nickname(), "TOPIC").length(), 0);
//         return;
//     }
//     if (topic.empty())
//     {
//         send(client.get_fd(), RPL_NOTOPIC(client.get_nickname(), channel_name).c_str(), RPL_NOTOPIC(client.get_nickname(), channel_name).length(), 0);
//         return;
//     }
//     if (channels.find(channel_name) == channels.end())
//     {
//         send(client.get_fd(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel_name).c_str(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel_name).length(), 0);
//         return;
//     }
//     if (channels[channel_name].get_topic() == topic)
//     {
//         send(client.get_fd(), ERR_TOPIC(client.get_nickname(), channel_name).c_str(), ERR_TOPIC(client.get_nickname(), channel_name).length(), 0);
//         return;
//     }
//     if (channels[channel_name].get_topic().empty())
//     {
//         send(client.get_fd(), RPL_NOTOPIC(client.get_nickname(), channel_name).c_str(), RPL_NOTOPIC(client.get_nickname(), channel_name).length(), 0);
//         return;
//     }
//     if (channels[channel_name].get_topic().empty())
//     {
//         send(client.get_fd(), RPL_NOTOPIC(client.get_nickname(), channel_name).c_str(), RPL_NOTOPIC(client.get_nickname(), channel_name).length(), 0);
//         return;
//     }
//     if (channels[channel_name].get_topic().empty())
//     {
//         send(client.get_fd(), RPL_NOTOPIC(client.get_nickname(), channel_name).c_str(), RPL_NOTOPIC(client.get_nickname(), channel_name).length(), 0);
//         return;
//     }
//     channels[channel_name].set_topic(topic);
// }


//version 2
// static bool is_operator(Client client, Channel channel)
// {
//     std::map<int, Client>::iterator it;
//     for (it = channel.get_operators().begin(); it != channel.get_operators().end(); ++it)
//     {
//         if (it->second.get_nickname() == client.get_nickname())
//             return true;
//     }
//     return false;
// }
// static std::string extract_channel_name(std::string command)
// {
//     std::istringstream iss(command);
//     std::string word;
    
//     // Split the input string by space and get the first word
//     iss >> word;
    
//     return word;
// }

// static std::string extract_topic(std::string command)
// {
//     std::istringstream iss(command);
//     std::string word;

//     // Skip the first word (channel name)
//     iss >> word;

//     // Read the rest of the string as the topic
//     std::string topic;
//     while (iss >> word) {
//         if (!topic.empty()) {
//             topic += ' ';
//         }
//         topic += word;
//     }

//     return topic;
// }

// void topic(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients)
// {
//     std::string channel_name = extract_channel_name(command.substr(5));
//     std::string topic = extract_topic(command.substr(5 + channel_name.length()));
    
//     std::cout << RED << channel_name << RESET << std::endl;
//     std::cout << YELLOW << topic << RESET << std::endl;

//     if (channel_name.empty() || (channel_name.empty() &&  topic.empty()))
//     {
//         // Handle missing parameters
//         send(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "TOPIC").c_str(), ERR_NEEDMOREPARAMS(client.get_nickname(), "TOPIC").length(), 0);
//         return;
//     }
//     else if (channels.find(channel_name) == channels.end())
//     {
//         // Handle channel not found
//         send(client.get_fd(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel_name).c_str(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel_name).length(), 0);
//         return;
//     }

//     else if (channels[channel_name].get_topic().empty() && topic.empty())
//     {
//         // Handle topic already set
//         send(client.get_fd(), ERR_TOPIC(client.get_nickname(), channel_name).c_str(), ERR_TOPIC(client.get_nickname(), channel_name).length(), 0);
//         return;
//     }
//     else if (is_operator(client , channels[channel_name]))
//         channels[channel_name].set_topic(topic);
//     else
//         send(client.get_fd(), RPL_TOPIC(client.get_nickname(), channel_name, topic).c_str(), RPL_TOPIC(client.get_nickname(), channel_name, topic).length(), 0);

// }
//version 3

static bool is_operator(Client & client,  Channel& channel)
{
    std::map<int, Client>::iterator it;
    for (it = channel.get_operators().begin(); it != channel.get_operators().end(); ++it)
    {
        if (it->second.get_nickname() == client.get_nickname())
            return true;
    }
    return false;
}

static std::string extract_channel_name(std::string command)
{
    std::istringstream iss(command);
    std::string word;

    // Split the input string by space and get the first word
    iss >> word;

    return word;
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

void topic(std::string& command, Client& client, std::map<std::string, Channel>& channels, std::map<int, Client>& clients)
{
    std::string channel_name = extract_channel_name(command.substr(5));
    std::string topic = extract_topic(command.substr(5 + channel_name.length()));

    std::cout << RED << channel_name << RESET << std::endl;
    std::cout << YELLOW << topic << RESET << std::endl;

    if (channel_name.empty() || topic.empty())
    {
        // Handle missing parameters
        send(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "TOPIC").c_str(), ERR_NEEDMOREPARAMS(client.get_nickname(), "TOPIC").length(), 0);
        return;
    }
    else if (channels.find(channel_name) == channels.end())
    {
        // Handle channel not found
        send(client.get_fd(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel_name).c_str(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel_name).length(), 0);
        return;
    }
    else if (channels[channel_name].get_topic().empty() && topic.empty())
    {
        // Handle topic already set
        send(client.get_fd(), ERR_TOPIC(client.get_nickname(), channel_name).c_str(), ERR_TOPIC(client.get_nickname(), channel_name).length(), 0);
        return;
    }
    else if (is_operator(client, channels[channel_name]))
    {
        channels[channel_name].set_topic(topic);
    }
    else
    {
        send(client.get_fd(), RPL_TOPIC(client.get_nickname(), channel_name, topic).c_str(), RPL_TOPIC(client.get_nickname(), channel_name, topic).length(), 0);
    }
}
