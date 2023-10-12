#pragma once

#include "Irc.hpp"
#include "Channel.hpp"

#define ERR_NONICKNAMEGIVEN(client) (":localhost 431 " + client + " :There is no nickname.\r\n")
#define ERR_ERRONEUSNICKNAME(client, nickname) (":localhost 432 " + client + " " + nickname + " :Erroneus nickname\r\n")
#define RPL_NICK(oclient, uclient, client) (": " + oclient + "!" + uclient + "@localhost NICK " + client + "\r\n")
#define ERR_NICKNAMEINUSE(client, nickname) (":localhost 433 " + client + " " + nickname + " :Nickname is already in use.\r\n")
#define RPL_WELCOME(user_id, nickname) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")
#define RPL_YOURHOST(client, servername) (":localhost 002 " + client + " :Your host is " + servername + "\r\n")
#define RPL_CREATED(client, datetime) (":localhost 003 " + client + " :This server was created " + datetime + "\r\n")
// channels
#define RPL_JOIN(nickname, channel) (":" + nickname + " JOIN " + channel + "\r\n")
#define ERR_USERONCHANNEL(client, nick, channel) (":localhost 443 " + client + " " + nick + " " + channel + " is already on channel\r\n")
#define ERR_NOSUCHCHANNEL(client, channel) (":localhost 403 " + client + " " + channel + " :No such channel\r\n")
#define ERR_NOTONCHANNEL(client, channel) (":localhost 442 " + client + " " + channel + " :You're not on that channel\r\n")
#define ERR_CHANOPRIVSNEEDED(client, channel) (":localhost 482 " + client + " " + channel + " :You're not channel operator\r\n")
#define RPL_KICK(client, channel) (":localhost 441 " + client + " " + channel + " :You have been kicked from the channel\r\n")
#define ERR_NOSUCHNICK(client, channel) (":localhost 401 " + client + " " + channel + " :No such nick/channel\r\n")
/// end channels
#define ERR_PASSWDMISMATCH(client) (":localhost 464 " + client + " :Password incorrect\r\n")
#define RPL_MYINFO(client, servername, version, user_modes, chan_modes, chan_param_modes) (":localhost 004 " + client + " " + servername + " " + version + " " + user_modes + " " + chan_modes + " " + chan_param_modes + "\r\n")
#define ERR_NEEDMOREPARAMS(client, command) (":localhost 461 " + client + " " + command + " :Not enough parameters.\r\n")

//privmsg
# define ERR_NORECIPIENT(client) ("411 " + client + " :No recipient given PRIVMSG\r\n")
# define ERR_NOTEXTTOSEND(client) ("412 " + client + " :No text to send\r\n")
# define RPL_PRIVMSG(nick, username, target, message) (":" + nick + "!" + username + "@localhost PRIVMSG " + target + " " + message + "\r\n")

/**
 * @brief execute the command given by the client
 * @param command the command to execute
 * @param client the client who sent the command
 * @param channels the channels of the server
 * @return void
 */
void excute_command(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients);
void privmsg(std::string message, Client &client , std::map<int, Client> &clients, std::map<std::string, Channel> &channels);
void broadcastTochannel(Client client, std::string message, std::string channel, std::map<std::string, Channel> &channels);
/**
 * @brief join a channel
 * @param client the client who wants to join the channel
 * @param channels the channels of the server
 * @param channel_name the name of the channel to join
 * @param command the command received from the client
 * @return int 0 if success, 1 if error
 */

int join(std::string command, Client &client, std::map<std::string, Channel> &channels);
/**
 * Checks if a client with the given nickname already exists in the map of clients.
 * @param nickname The nickname to check for.
 * @param clients The map of clients to search in.
 * @return True if a client with the given nickname already exists in the map of clients, false otherwise.
 */
bool client_already_exist(std::string nickname, std::map<int, Client> clients);
/**
 * @brief Filters a string by removing any unwanted characters.
 *
 * @param str The string to be filtered.
 * @return The filtered string.
 */
std::string filteredString(std::string str);
void nick(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> clients);
void user(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> clients);
void pass(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients);

/**
 * Kicks a client from a channel.
 *
 * @param command The kick command to execute.
 * @param client The client who executed the command.
 * @param channels The map of channels in the server.
 * @param clients The map of clients in the server.
 */
void kick(std::string command, Client &client, std::map<std::string, Channel> &channels);