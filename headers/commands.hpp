#pragma once

#include "Irc.hpp"
#include "Channel.hpp"

# define ERR_NONICKNAMEGIVEN(client) (":localhost 431 " + client + " :There is no nickname.\r\n")
# define ERR_ERRONEUSNICKNAME(client, nickname) (":localhost 432 " + client + " " + nickname + " :Erroneus nickname\r\n")
# define RPL_NICK(oclient, uclient, client) (": " + oclient + "!" + uclient + "@localhost NICK " +  client + "\r\n")
# define ERR_NICKNAMEINUSE(client, nickname) (":localhost 433 " + client + " " + nickname + " :Nickname is already in use.\r\n")
# define RPL_WELCOME(user_id, nickname) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")
# define RPL_YOURHOST(client, servername) (":localhost 002 " + client + " :Your host is " + servername + "\r\n")
# define RPL_CREATED(client, datetime) (":localhost 003 " + client + " :This server was created " + datetime + "\r\n")

# define RPL_MYINFO(client, servername, version, user_modes, chan_modes, chan_param_modes) (":localhost 004 " + client + " " + servername + " " + version + " " + user_modes + " " + chan_modes + " " + chan_param_modes + "\r\n")
# define ERR_NEEDMOREPARAMS(client, command) (":localhost 461 " + client + " " + command + " :Not enough parameters.\r\n")

# define ERR_PASSWDMISMATCH(client) (":localhost 464 " + client + " :Password incorrect.\r\n")
/**
 * @brief execute the command given by the client
 * @param command the command to execute
 * @param client the client who sent the command
 * @param channels the channels of the server
 * @return void
 */
void excute_command(std::string command, Client &client, std::map<std::string, Channel> &channels , std::map<int, Client> &clients);

/**
 * @brief join a channel
 * @param client the client who wants to join the channel
 * @param channels the channels of the server
 * @param channel_name the name of the channel to join
 * @return int 0 if success, 1 if error
 */

int join(Client &client, std::map<std::string, Channel &> &channels, std::string &channel_name);
bool client_already_exist(std::string nickname, std::map<int, Client> clients);
std::string filteredString(std::string str);
void nick(std::string command, Client &client, std::map<std::string, Channel> channels, std::map<int, Client> clients);
void user(std::string command, Client &client, std::map<std::string, Channel> channels, std::map<int, Client> clients);
void pass( std::string command, Client &client, std::map<std::string, Channel> &channels , std::map<int, Client> &clients);