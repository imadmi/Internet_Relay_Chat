/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imimouni <imimouni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 11:51:53 by otait-ta          #+#    #+#             */
/*   Updated: 2023/10/09 15:14:33 by imimouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/irc_header.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

bool client_already_exist(std::string nickname, std::map<int, Client> clients) {
    std::map<int, Client>::const_iterator it;
    for (it = clients.begin(); it != clients.end(); ++it) {
        Client  cl = it->second;
        std::cout<< nickname <<std::endl;
        if (cl.get_nickname() == nickname){
            return true; // Nickname already exists
        }
    }
    return false; // Nickname does not exist
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

void excute_command(std::string command, Client &client, std::map<std::string, Channel> &channels , std::map<int, Client> &clients)
{
    (void)channels;
    // if command is #NICK <nickname>

    if (command.substr(0, 4) == "PASS")
        pass(command, client, channels, clients);
    // else
    // {
    //         std::cout<< "inser the pass first "<<std::endl;
    // }
     if (command.substr(0, 4) == "NICK")
            nick(command, client, channels, clients);
     if (command.substr(0, 4) == "USER")
     {
        user(command, client, channels, clients);
     }
     if (client.is_authenticated())
        send(client.get_fd(), client.get_buff_to_send().c_str(), client.get_buff_to_send().length(), 0);
    //  if (command.substr(0, 4) == "USER")
    //     {
    //         std::string username_rel = command.substr(5, command.length() - 5);
    //         if (checkForma(username_rel) == false)
    //             std::cout << "wrong format" << std::endl;
            
    //         client.set_username(username_rel);
    //         std::cout << "username set to " << username_rel << std::endl;
    //         client.set_authenticated(true);
    //         std::cout << "authenticated" << std::endl;
    //     }
   // }
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
}