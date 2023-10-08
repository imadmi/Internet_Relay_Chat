/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otait-ta <otait-ta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 11:51:53 by otait-ta          #+#    #+#             */
/*   Updated: 2023/10/08 17:09:20 by otait-ta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/irc_header.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

void excute_command(std::string command, Client &client, std::map<std::string, Channel> &channels)
{
    // if commade is #JOIN <channel>
    if (command.substr(0, 5) == "#JOIN")
    {
        std::string channel_name = command.substr(6, command.length() - 6);
        join(client, channels, channel_name);
        std::cout << "joined channel " << channel_name << std::endl;

        for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
        {
            std::cout << it->first << std::endl;
        }
    }
}