
#include "../../headers/commands.hpp"
#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"

static std::string getCurrentTime()
{
    time_t currentTime;
    struct tm *timeInfo;
    char buffer[80];
    time(&currentTime);
    timeInfo = localtime(&currentTime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
    return std::string(buffer);
}
static bool checkForma(const std::string &username, std::string &user)
{
    std::istringstream iss(username);
    std::string word;
    int position = 1;
    while (iss >> word)
    {
        if (position == 1 || position == 4)
        {
            user = position == 1 ? word:user;
        }
        else if (position == 2)
        {
            if (word != "0")
            {
                return false;
            }
        }
        else if (position == 3)
        {
            if (word != "*")
            {
                return false;
            }
        }
        else
        {
            return false;
        }

        position++;
    }
    return position == 5;
}

void user(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> clients)
{
    std::stringstream ss;
    ss << client.get_fd();
    std::string str = ss.str();
    std::string username = command.substr(5, command.length() - 5);
    std::string user;
    if (client.is_authenticated() == false && client.get_pass() != "" && client.get_nickname() != "")
    {
        if (username.empty())
            send(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "USER").c_str(), ERR_NEEDMOREPARAMS(client.get_nickname(), "USER").length(), 0);
        else if (checkForma(username, user) == false)
            send(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "USER").c_str(), ERR_NEEDMOREPARAMS(client.get_nickname(), "USER").length(), 0);
        else
        {
            client.set_username(user);
            client.set_authenticated(true);
            client.add_buffer_to_send(RPL_WELCOME(str, client.get_nickname()));
            client.add_buffer_to_send(RPL_YOURHOST(client.get_nickname(), "1337-ft_irc"));
            client.add_buffer_to_send(RPL_CREATED(client.get_nickname(), getCurrentTime()));
            client.add_buffer_to_send(RPL_MYINFO(client.get_nickname(), "localhost", "1.1", "io", "kost", "k"));
        }
    }
}