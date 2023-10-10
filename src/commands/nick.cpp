
#include "../../headers/commands.hpp"
#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"

static bool isForbidden(char c)
{
    if (c == ' ' || c == ',' || c == '*' || c == '?' || c == '!' || c == '@' || c == '.')
        return (true);
    else
        return (false);
}

static bool containsInvalidCharacters(std::string nickname)
{
    if (nickname[0] == '$' || nickname[0] == ':' || nickname[0] == '#')
        return (true);

    for (size_t i = 0; i < nickname.size(); i++)
    {
        if (isForbidden(nickname[i]) == true)
            return (true);
    }
    return (false);
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
        else if (containsInvalidCharacters(nickname))
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
        if (client.is_authenticated() == true && !nickname.empty())
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