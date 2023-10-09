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

std::string filteredString(std::string str)
{
    std::string filteredString;
    for (int i = 0; i < (int)str.length(); i++)
    {
        if (str[i] != '\n' &&  str[i] != '\r' && str[i] != '\t' &&  str[i] != '\v' && str[i] != '\f')
        {
            filteredString += str[i];
        }
    }
    return filteredString;
}

void excute_command(std::string command, Client &client, std::map<std::string, Channel> &channels , std::map<std::string, Client> &clients)
{
    (void)channels;
    // if command is #NICK <nickname>
    if (command.substr(0, 4) == "PASS" && client.is_authenticated() == false && client.get_pass() == "")
    {
            std::string password = filteredString (command.substr(5, command.length() - 5)) ;

            std::string str("pass");
            std::cout <<"******" <<password;
            if (password == str)
            {
                std::cout << "password is correct" << std::endl;
                client.set_pass(password);
            }
            else
            {
                std::cout << "wrong password" << std::endl;
            }
    }
    // else
    // {
    //         std::cout<< "inser the pass first "<<std::endl;
    // }
    else if (client.is_authenticated() == false && client.get_pass() != ""  && client.get_nickname() == ""  )
    {
     if (command.substr(0, 4) == "NICK")
        {
            std::string nickname = filteredString( command.substr(5, command.length() - 5));
            if (client_already_exist(nickname, clients) == true)
            {
                std::cout << "nickname already taken" << std::endl;
                return;
            }
            client.set_nickname(nickname);
            std::cout << "nickname set to " << nickname << std::endl;
            
        }
    }
    else if  (client.is_authenticated() == false  &&  client.get_pass() != "" && client.get_nickname() != ""  && client.get_username() == "")
    {
        if (command.substr(0, 4) == "USER")
        {
            std::string username = filteredString( command.substr(5, command.length() - 5));
            client.set_username(username);
            std::cout << "username set to " << username << std::endl;
            client.set_authenticated(true);
            std::cout << "authenticated" << std::endl;
        }
    }
    // else if  (client.is_authenticated() == false  &&  client.get_pass() != "" && client.get_nickname() != ""  && client.get_username() != ""
    // {
    //     std::cout<< "im in"<<std::endl;
    //     if (command.substr(0, 4) == "USER")
    //     {
    //         // std::string password = command.substr(6, command.length() - 6);
    //         // std::cout<<password.find("\n")<<std::endl;
    //         // std::string str("pass\n");
    //         // std::cout <<"******" <<password;
    //         // std::cout <<"******" <<str;
    //         // if (password == str)
    //         // {
    //         //     client.set_authenticated(true);
    //         //     std::cout << "authenticated" << std::endl;
    //         // }
    //         // else
    //         // {
    //         //     std::cout << "wrong password" << std::endl;
    //         // }
    //         client.set_authenticated(true);
    //         std::cout<<"wlah ma ana"<<std::endl;
    //     }
    //     else
    //     {
    //         std::cout<< "ana  "<<std::endl;
    //     }
    // }
    std::cout<<(client.is_authenticated() == false  &&  client.get_pass() != "" && client.get_nickname() != "")<<std::endl;
    // if commade is #JOIN <channel>
    // if (command.substr(0, 5) == "/JOIN")
    // {
    //     std::string channel_name = command.substr(6, command.length() - 6);
    //     join(client, channels, channel_name);
    //     std::cout << "joined channel " << channel_name << std::endl;
    //     PRINT("LIST OF CHANNELS:")
    //     for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
    //     {
    //         std::cout << it->first << std::endl;
    //     }
    // }
    // // exit

    // if (command.substr(0, 5) == "/EXIT")
    // {

    //     // close(_pollfds[i].fd);
    //     // _pollfds.erase(_pollfds.begin() + i);
    //     std::cerr << RED << "exit\n";
    //     exit(1);
    // }
    std::cout<<client.is_authenticated()<<std::endl;
}