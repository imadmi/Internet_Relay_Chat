/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otait-ta <otait-ta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 14:36:54 by otait-ta          #+#    #+#             */
/*   Updated: 2023/10/07 11:27:10 by otait-ta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "irc_header.hpp"
#include <map>

class Channel
{
private:
    std::string _name;
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
    /**
     * @brief get the name of the channel.
     *
     * @return std::string
     */
    std::string get_name();
    /**
     * @brief join a client to a channel.
     *
     * @param client to add.
     * @return status of the operation (0 if success).
     */
    int add_client(Client &new_client);
    /**
     * @brief remove a client from a channel.
     *
     * @param client to remove.
     * @return status of the operation (0 if success).
     */
    int remove(Client &client);
};
