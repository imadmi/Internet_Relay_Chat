/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imimouni <imimouni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 14:36:54 by otait-ta          #+#    #+#             */
/*   Updated: 2023/10/09 10:20:46 by imimouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "irc_header.hpp"
#include "Client.hpp"
#include <map>

class Channel
{
private:
    std::string _name;
    int _socket_fd;
    std::map<std::string, Client> _clients;

public:
    /**
     * @brief Construct a new Channel object
     *
     * @param channel_name
     */
    Channel(std::string channel_name);
    /**
     * @brief Destroy the Channel object
     *
     */
    ~Channel();
    void set_socket_fd(int socket_fd);
    int get_socket_fd();
    std::string get_name();
    /**
     * @brief join a client to a channel.
     *
     * @param client to add.
     * @return status of the operation (0 if success).
     */
    int add(Client &new_client);
    /**
     * @brief remove a client from a channel.
     *
     * @param client to remove.
     * @return status of the operation (0 if success).
     */
    int remove(Client &client);
    // asigne a new socket to the channel and add it to the pollfds
    void add_socket(int socket_fd, pollfd *fds);
    
};
