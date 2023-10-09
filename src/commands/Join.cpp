/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otait-ta <otait-ta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 11:03:59 by otait-ta          #+#    #+#             */
/*   Updated: 2023/10/09 17:23:58 by otait-ta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/commands.hpp"
#include "../../headers/irc_header.hpp"
#include "../../headers/Channel.hpp"

int join(Client &client, std::map<std::string, Channel> &channels, std::string &channel_name)
{

    if (channels.find(channel_name) == channels.end())
    {
        Channel new_channel(channel_name);
        channels.insert(std::pair<std::string, Channel>(channel_name, new_channel));
        new_channel.add_client(client);
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
    return (0);
}