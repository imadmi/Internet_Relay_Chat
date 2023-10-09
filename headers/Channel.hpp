

#pragma once

#include "irc_header.hpp"
#include "Client.hpp"
#include <map>

class Client;
class Channel
{
private:
    std::string _name;
    std::map<int, Client> _clients;
    std::map<int, Client> _moderators;
    std::string _topic;
    std::map<char, char> _modes;

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
    int remove_client(Client &client);
    /**
     * @brief get the clients of the channel.
     *
     * @return std::map<std::string, Client>
     */
    std::map<int, Client> get_clients();
    /**
     * set the topic of the channel.
     * @param topic to set.
     * @return status of the operation (0 if success).
     */
    int set_topic(std::string topic);
    /**
     * @brief get the topic of the channel.
     *
     * @return std::string
     */
    std::string get_topic();
};
