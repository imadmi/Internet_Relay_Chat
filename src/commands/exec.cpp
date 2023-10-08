/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otait-ta <otait-ta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 11:51:53 by otait-ta          #+#    #+#             */
/*   Updated: 2023/10/08 17:46:49 by otait-ta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/irc_header.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

bool client_already_exist(std::string nickname, std::map<std::string, Client> &clients)
{
    if (clients.find(nickname) != clients.end())
        return (true);
    return (false);
}

void excute_command(std::string command, Client &client, std::map<std::string, Channel> &channels , std::map<std::string, Client> &clients)
{
    // if command is #NICK <nickname>
     if (command.substr(0, 5) == "/NICK")
        {
            std::string nickname = command.substr(6, command.length() - 6);
            //check if nickname is valid
            //check if nickname is already taken
            if (client_already_exist(nickname, clients) == true)
            {
                std::cout << "nickname already taken" << std::endl;
                return;
            }
            client.set_nickname(nickname);
            std::cout << "nickname set to " << nickname << std::endl;
        }
    if (client.is_authenticated() == false  && client.get_nickname() != "")
    {
        if (command.substr(0, 5) == "/PASS")
        {
            std::string password = command.substr(6, command.length() - 6);
            if (password == "pass")
            {
                client.set_authenticated(true);
                std::cout << "authenticated" << std::endl;
            }
            else
            {
                std::cout << "wrong password" << std::endl;
            }
        }
        else
        {
            std::cout << "please set a nickname first" << std::endl;
        }
        clients.insert(std::pair<std::string, Client>(client.get_nickname(), client));
    }
    // if commade is #JOIN <channel>
    if (command.substr(0, 5) == "/JOIN")
    {
        std::string channel_name = command.substr(6, command.length() - 6);
        join(client, channels, channel_name);
        std::cout << "joined channel " << channel_name << std::endl;
        PRINT("LIST OF CHANNELS:")
        for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
        {
            std::cout << it->first << std::endl;
        }
    }
    // exit

    if (command.substr(0, 5) == "/EXIT")
    {

        // close(_pollfds[i].fd);
        // _pollfds.erase(_pollfds.begin() + i);
        std::cerr << RED << "exit\n";
        exit(1);
    }
}