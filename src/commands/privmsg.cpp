#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

std::string parseMessage(std::string message)
{
    std::string parsedMessage;
    std::string::iterator it;
    bool flag = false;
    for (it = message.begin(); it != message.end(); ++it)
    {
        if (*it == '\r' || *it == '\n')
        {
            break;
        }
        if (*it == ':')
        {
            flag = true;
            continue;
        }
        if (flag == true)
            parsedMessage += *it;
    }
    return parsedMessage;
}

std::string extractName(std::string message)
{
    std::string name;
    std::string::iterator it;
    for (it = message.begin(); it != message.end(); ++it)
    {
        if (*it == '!')
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

void privmsg(std::string message, Client &client , std::map<int, Client> &clients, std::map<std::string, Channel> &channels)
{
    std::string receiver = filteredString(  extractName(message.substr(7, message.length() - 8)) ) ;
   // std::string channel_name =extract_channel_name( message.substr(message.find("#"), message.length() - message.find("#") -2 )) ;
    //std::map<std::string, Channel>::iterator it = channels.find(channel_name);
    
    std::map<int, Client>::iterator it2;
    if (receiver != "")
    {
        int flag = 0;
        for (it2 = clients.begin(); it2 != clients.end(); ++it2)
        {
            if (it2->second.get_nickname() == receiver)
            {
                send(it2->second.get_fd(), RPL_PRIVMSG(client.get_nickname(), client.get_username(), receiver,  parseMessage(message)).c_str(), RPL_PRIVMSG(client.get_nickname(), client.get_username(), receiver,  parseMessage(message)).length(), 0);
                flag = 1;
                break;
            }
    }
        if (flag == 0)
        {
            send(client.get_fd(), ERR_NOSUCHNICK(client.get_nickname(), receiver).c_str(), ERR_NOSUCHNICK(client.get_nickname(), receiver).length(), 0);
            std::cout << RED << "zok" ;
        }   
    }
    // else if (it != channels.end())
    // {
    //     std::map<int, Client>::iterator it3 = it->second.get_clients().begin();
    //     if (it3 == it->second.get_clients().end())
    //     {
    //         std::cout<<"no clients in channel"<<std::endl;
    //     }
    //     else
    //     {

    //     while (it3 != it->second.get_clients().end())
    //     {
    //         send(it3->second.get_fd(), parseMessage(message).c_str(), parseMessage(message).length(), 0);
    //         ++it3;
    //     }
    //     } 
    // }
}