/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otait-ta <otait-ta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 11:03:59 by otait-ta          #+#    #+#             */
/*   Updated: 2023/10/07 11:29:46 by otait-ta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Join.hpp"

int join(Client &client, std::map<std::string, Channel> &channels, std::string channel_name)
{
    if (channels.find(channel_name) == channels.end())
    {
        Channel *new_channel = new Channel(channel_name);
        channels.insert(std::pair<std::string, Channel>(channel_name, *new_channel));
    }
    if (client.join_channel(channels[channel_name]) == 1)
        return (1);
    return (0);
}