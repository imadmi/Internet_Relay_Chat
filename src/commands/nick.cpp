
#include "../../headers/commands.hpp"
#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"

// static bool hasInvalidCharacters(std::string nickname)
// {
//     if (nickname[0] == '$' || nickname[0] == ':' || nickname[0] == '#')
//         return (true);

//     for (size_t i = 0; i < nickname.size(); i++)
//     {
//         if ((nickname[i] == ' ' || nickname[i] == ',' || nickname[i] == '*' || nickname[i] == '?' || nickname[i] == '!' || nickname[i] == '@' || nickname[i] == '.'))
//             return (true);
//     }
//     return (false);
// }
bool hasInvalidCharacters(const std::string& name)
{
    // Check the first character for validity
    char firstChar = name[0];
    if (firstChar == '$' || firstChar == ':' || firstChar == '#')
    {
        return true;
    }

    // Check the rest of the string for invalid characters
    const char invalidChars[] = " ,*?!@.";
    for (size_t i = 0; i < name.size(); ++i)
    {
        for (size_t j = 0; j < sizeof(invalidChars) - 1; ++j)
        {
            if (name[i] == invalidChars[j])
            {
                return true;
            }
        }
    }

    // No invalid characters found
    return false;
}


void nick(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> clients)
{
    std::string nickname = filteredString(command.substr(5, command.length() - 5));
    if (client.is_authenticated() == false && client.get_pass() != "" && client.get_nickname() == "")
    {

        if (nickname.empty())
        {
            client.add_buffer_to_send(ERR_NONICKNAMEGIVEN(client.get_nickname()));
        }

        else if (client_already_exist(nickname, clients))
        {
            client.add_buffer_to_send(ERR_NICKNAMEINUSE(client.get_nickname(), nickname));
        }
        else if (hasInvalidCharacters(nickname))
        {
            client.add_buffer_to_send(ERR_ERRONEUSNICKNAME(client.get_nickname(), nickname));
        }
        else
        {
            client.set_nickname(nickname);
        }
    }
    else
    {
        if (client.is_authenticated() == true && !nickname.empty() && !client_already_exist(nickname, clients) && !hasInvalidCharacters(nickname))
        {
            client.set_old_nick(client.get_nickname());
            client.set_nickname(nickname);
            std::cout << "[Server] Nickname change registered. new nickname is now : " << client.get_nickname() << std::endl;
            client.set_buff_to_send(RPL_NICK(client.get_old_nick(), client.get_username(), nickname));
        }
        else if (client.is_authenticated() == true && nickname.empty())
        {
            client.add_buffer_to_send(ERR_NONICKNAMEGIVEN(client.get_nickname()));
        }
    }
}