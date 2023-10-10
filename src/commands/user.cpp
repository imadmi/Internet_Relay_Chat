
#include "../../headers/commands.hpp"
#include "../../headers/irc_header.hpp"
#include "../../headers/Channel.hpp"


static std::string getCurrentTime() {
    time_t currentTime;
    struct tm* timeInfo;
    char buffer[80];

    // Get the current time as a Unix timestamp
    time(&currentTime);

    // Convert the timestamp to a struct tm
    timeInfo = localtime(&currentTime);

    // Format the time as a string
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);

    return std::string(buffer);
}
static bool checkForma(const std::string& username) {
    std::istringstream iss(username);
    std::string word;
    int position = 1;
    while (iss >> word) {
        if (position == 1 || position == 4) {
        } else if (position == 2) {
            if (word != "0") {
                return false;
            }
        } else if (position == 3) {
            if (word != "*") {
                return false;
            }
        } else {
            return false;
        }
        
        position++;
    }
    
    // Check if all required parts were found
    return position == 5;
}

void user(std::string command, Client &client, std::map<std::string, Channel> channels, std::map<int, Client> clients)
{
    std::stringstream ss;
    ss << client.get_fd();
    std::string str = ss.str();
    std::string username = command.substr(5, command.length() - 5);
    if (client.is_authenticated() == false && client.get_pass() != ""  && client.get_nickname() != "")
    {
        if (username.empty())
        {
            client.add_buffer_to_send(ERR_NEEDMOREPARAMS(client.get_nickname(), "USER"));
        }
        else if (checkForma(username) == false)
        {
            client.add_buffer_to_send(ERR_NEEDMOREPARAMS(client.get_nickname(), "USER"));
        }
        else
        {
            client.set_username(username);
            client.set_authenticated(true);
            client.add_buffer_to_send(RPL_WELCOME(str,client.get_nickname()));
            client.add_buffer_to_send(RPL_YOURHOST(client.get_nickname(), "1337-ft_irc"));
            client.add_buffer_to_send(RPL_CREATED(client.get_nickname(), getCurrentTime()));
            client.add_buffer_to_send(RPL_MYINFO(client.get_nickname(), "localhost", "1.1", "io", "kost", "k"));
        }
    }
}