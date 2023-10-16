
#include "../../headers/commands.hpp"
#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"

bool hasInvalidCharacters(const std::string &name)
{
    char firstChar = name[0];
    if (firstChar == '$' || firstChar == ':' || firstChar == '#')
    {
        return true;
    }
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
    return false;
}

void nick(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> clients)
{
    (void)channels;
    std::string nickname = filteredString(command.substr(5, command.length() - 5));
    if (client.is_authenticated() == false && client.get_pass() != "" && client.get_nickname() == "")
    {

        if (nickname.empty())
        {
            send(client.get_fd(), ERR_NONICKNAMEGIVEN , sizeof(ERR_NONICKNAMEGIVEN), 0);
        }

        else if (client_already_exist(nickname, clients))
        {
            send(client.get_fd(), ERR_NICKNAMEINUSE(client.get_nickname(), nickname).c_str(), ERR_NICKNAMEINUSE(client.get_nickname(), nickname).length(), 0);
        }
        else if (hasInvalidCharacters(nickname))
        {
            send(client.get_fd(), ERR_ERRONEUSNICKNAME(client.get_nickname(), nickname).c_str(), ERR_ERRONEUSNICKNAME(client.get_nickname(), nickname).length(), 0);
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
            client.set_buff_to_send(RPL_NICK(client.get_old_nick(), client.get_username(), nickname));
        }
        else if (client.is_authenticated() == true && nickname.empty())
            send(client.get_fd(), ERR_NONICKNAMEGIVEN , sizeof(ERR_NONICKNAMEGIVEN), 0);
    }
}
