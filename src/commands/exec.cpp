#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

bool client_already_exist(std::string nickname, std::map<int, Client> clients)
{
    std::map<int, Client>::const_iterator it;
    for (it = clients.begin(); it != clients.end(); ++it)
    {
        Client cl = it->second;
        std::cout << nickname << std::endl;
        if (cl.get_nickname() == nickname)
        {
            return true; // Nickname already exists
        }
    }
    return false; // Nickname does not exist
}

// static bool checkForma(const std::string &username)
// {
//     std::istringstream iss(username);
//     std::string word;
//     int position = 1;
//     while (iss >> word)
//     {
//         if (position == 1 || position == 4)
//         {
//         }
//         else if (position == 2)
//         {
//             if (word != "0")
//             {
//                 return false;
//             }
//         }
//         else if (position == 3)
//         {
//             if (word != "*")
//             {
//                 return false;
//             }
//         }
//         else
//         {
//             return false;
//         }

//         position++;
//     }

//     // Check if all required parts were found
//     return position == 5;
// }

std::string filteredString(std::string str)
{
    std::string filteredString;
    for (int i = 0; i < (int)str.length(); i++)
    {
        if (str[i] != '\n' && str[i] != '\r' && str[i] != '\t' && str[i] != '\v' && str[i] != '\f')
        {
            filteredString += str[i];
        }
    }
    return filteredString;
}

void excute_command(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients)
{

    if (command.substr(0, 4) == "PASS")
        pass(command, client, channels, clients);
    if (command.substr(0, 4) == "NICK")
        nick(command, client, channels, clients);
    if (command.substr(0, 4) == "USER")
    {
        user(command, client, channels, clients);
    }
    if (command.substr(0, 4) == "JOIN")
    {
        join(command, client, channels);
    }
    if (command.substr(0, 4) == "KICK")
    {

        kick(command, client, channels);
    }
    // if (client.is_authenticated())
    // {
    send(client.get_fd(), client.get_buff_to_send().c_str(), client.get_buff_to_send().length(), 0);
    client.set_buff_to_send("");
    // }
}