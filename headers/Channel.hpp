/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otait-ta <otait-ta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 14:36:54 by otait-ta          #+#    #+#             */
/*   Updated: 2023/10/06 15:15:14 by otait-ta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "irc_header.hpp"

class Channel
{
private:
    std::string _name;

public:
    Channel(std::string channel_name);
    ~Channel();
};
