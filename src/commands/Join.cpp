/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otait-ta <otait-ta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 11:03:59 by otait-ta          #+#    #+#             */
/*   Updated: 2023/10/11 09:48:26 by otait-ta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/commands.hpp"
#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"

int join(std::string command, Client &client, std::map<std::string, Channel> &channels)
{
    std::string channel_name = command.substr(5, command.length() - 5);
    if (channels.find(channel_name) == channels.end())
    {
        Channel new_channel(channel_name);
        channels.insert(std::pair<std::string, Channel>(channel_name, new_channel));
        if (new_channel.add_client(client))
        {
            client.add_buffer_to_send(ERR_USERONCHANNEL(client.get_nickname(), client.get_nickname(), channel_name));
            return (1);
        }
        new_channel.set_operator(client);
    }
    std::map<std::string, Channel>::iterator it = channels.find(channel_name);
    if (it != channels.end())
    {
        it->second.add_client(client);
        client.add_channel(it->second);
    }
    else
    {
        return (1);
    }
    client.add_buffer_to_send(RPL_JOIN(client.get_nickname(), channel_name));
    return (0);
}