/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otait-ta <otait-ta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 15:46:19 by otait-ta          #+#    #+#             */
/*   Updated: 2023/10/08 17:06:20 by otait-ta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Channel.hpp"
#include "../../headers/irc_header.hpp"

Channel::Channel(std::string channel_name)
{
    _name = channel_name;
}

Channel::~Channel()
{
}

std::string Channel::get_name()
{
    return _name;
}

int Channel::add_client(Client &new_client)
{
    if (_clients.find(new_client.get_nickname()) != _clients.end())
        return (-1);
    _clients.insert(std::pair<std::string, Client>(new_client.get_nickname(), new_client));
    return (0);
}

int Channel::remove_client(Client &client)
{
    if (_clients.find(client.get_nickname()) == _clients.end())
        return (-1);
    _clients.erase(client.get_nickname());
    return (0);
}