/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otait-ta <otait-ta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 15:46:19 by otait-ta          #+#    #+#             */
/*   Updated: 2023/10/09 09:59:21 by otait-ta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Channel.hpp"
#include "../../headers/irc_header.hpp"

Channel::Channel(std::string channel_name)
{
    _name = channel_name;
    _topic = "";
    _modes['i'] = '+';
    _modes['t'] = '+';
    _modes['k'] = '+';
    _modes['o'] = '+';
    _modes['l'] = '+';
}

Channel::~Channel()
{
}

std::string Channel::get_name()
{
    return _name;
}

std::map<std::string, Client> Channel::get_clients()
{
    return _clients;
}

int Channel::add_client(Client &new_client)
{
    if (this->_clients.find(new_client.get_socket_fd_str()) != this->_clients.end())
        return (-1);
    this->_clients.insert(std::pair<std::string, Client>(new_client.get_socket_fd_str(), new_client));
    return (0);
}

int Channel::remove_client(Client &client)
{
    if (this->_clients.find(client.get_socket_fd_str()) == this->_clients.end())
        return (-1);
    this->_clients.erase(client.get_socket_fd_str());
    if (this->_moderators.find(client.get_socket_fd_str()) != this->_moderators.end())
        this->_moderators.erase(client.get_socket_fd_str());
    return (0);
}

int Channel::set_topic(std::string topic)
{
    this->_topic = topic;
    return (0);
}

std::string Channel::get_topic()
{
    return (this->_topic);
}