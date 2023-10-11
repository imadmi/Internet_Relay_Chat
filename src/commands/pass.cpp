
#include "../../headers/commands.hpp"
#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"


void pass( std::string command, Client &client, std::map<std::string, Channel> &channels , std::map<int, Client> &clients)
{
    std::string pass = filteredString (command.substr(5, command.length() - 5)) ;
    if (client.is_authenticated() == false && client.get_pass() == "")
    {
        if (pass.empty())
        {
            send(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "PASS").c_str(), ERR_NEEDMOREPARAMS(client.get_nickname(), "PASS").length(), 0);
        }
        else if (pass != "pass")
            send(client.get_fd(), ERR_PASSWDMISMATCH(client.get_nickname()).c_str(), ERR_PASSWDMISMATCH(client.get_nickname()).length(), 0);
        else
            client.set_pass(pass);
    }
}