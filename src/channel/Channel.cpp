/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imimouni <imimouni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 15:46:19 by otait-ta          #+#    #+#             */
/*   Updated: 2023/10/09 10:15:36 by imimouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Channel.hpp"
#include "../../headers/irc_header.hpp"

// Channel::Channel(std::string channel_name)
// {
//     _name = channel_name;
//     _socket_fd = -1;
// }

// Channel::~Channel()
// {
// }

// void Channel::set_socket_fd(int socket_fd)
// {
//     _socket_fd = socket_fd;
// }

// int Channel::get_socket_fd()
// {
//     return _socket_fd;
// }

// std::string Channel::get_name()
// {
//     return _name;
// }

// int Channel::add(Client &new_client)
// {
//     if (_clients.find(new_client.get_nickname()) != _clients.end())
//         return (1);
//     _clients.insert(std::pair<std::string, Client>(new_client.get_nickname(), new_client));
//     return (0);
// }
