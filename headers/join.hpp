/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otait-ta <otait-ta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 11:05:14 by otait-ta          #+#    #+#             */
/*   Updated: 2023/10/07 13:50:00 by otait-ta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "irc_header.hpp"
#include "channel.hpp"

int join(Client &client, std::map<std::string, Channel> &channels, std::string &channel_name);