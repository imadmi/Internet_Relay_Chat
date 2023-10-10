#pragma once

#include "Irc.hpp"
#include "Channel.hpp"

/**
 * @brief execute the command given by the client
 * @param command the command to execute
 * @param client the client who sent the command
 * @param channels the channels of the server
 * @return void
 */
void excute_command(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients);

/**
 * @brief join a channel
 * @param client the client who wants to join the channel
 * @param channels the channels of the server
 * @param channel_name the name of the channel to join
 * @return int 0 if success, 1 if error
 */
int join(Client &client, std::map<std::string, Channel> &channels, std::string &channel_name);