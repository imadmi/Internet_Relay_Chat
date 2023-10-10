
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
            client.add_buffer_to_send(ERR_NEEDMOREPARAMS(client.get_nickname(), "PASS"));
        }
        else if (pass != "pass")
        {
            client.add_buffer_to_send(ERR_PASSWDMISMATCH(client.get_nickname()));
        }
        else
            client.set_pass(pass);
    }
    return;
}