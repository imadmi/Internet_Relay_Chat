

#pragma once

#include "Irc.hpp"
#include "Client.hpp"
#include <map>

class Client;
class Channel
{
private:
    std::string _name;
    std::map<int, Client> _clients;
    std::string _topic;
    std::map<char, char> _modes;
    std::map<int, Client> _operators;
    std::vector<std::string> _invitees;


public:
    Channel();
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
    int remove_client(Client &client);
    /**
     * @brief get the clients of the channel.
     * @return std::map<std::string, Client>
     */
    std::map<int, Client> &get_clients();

    /**
     * set the topic of the channel.
     * @param topic to set.
     * @return status of the operation (0 if success).
     */
    void set_topic(std::string topic);
    /**
     * @brief get the topic of the channel.
     *
     * @return std::string
     */
    std::string get_topic();
    /**
     * @brief set the mode of the channel.
     * @param mode to set [i-t-k-o-l].
     * @param param of the mode (+ , -).
     */
    int set_mode(char mode, char sign);
    /**
     * @brief get the mode of the channel.
     *
     * @return std::map<char, char>
     */
    std::map<char, char> get_modes();
    /**
     * @brief get the sign of the mode.
     *
     * @param mode to get the sign.
     * @return  the signe of the mode.
     */
    char get_signe_mode(char mode);
    /**
     * @brief set the operator of the channel.
     *
     * @param client to set as operator.
     * @return status of the operation (0 if success).
     */
    int set_operator(Client &client);
    /**
     * @brief get the operator of the channel.
     *
     * @return std::map<int, Client>
     */
    std::map<int, Client> get_operators();
    void print_members();
    /**
     * Adds a user to the list of invitees for the channel.
     * @param nickname The nickname of the user to be added.
     */
    void add_invitee(std::string nickname);
    /**
     * Returns a vector of strings containing the usernames of all users who have been invited to the channel.
     * @return A vector of strings containing the usernames of all users who have been invited to the channel.
     */
    std::vector<std::string> get_invitees();

    /**
     * Returns a vector of strings containing the list of clients nick currently in the channel.
     *
     * @return A vector of strings containing the list of clients currently in the channel.
     */
    std::string get_clients_nick();
};
