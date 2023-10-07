struct cmd_struct {
    std::string command;
    std::string message;
};
class Client {
private:
    int client_fd;
    std::string nickname;
    std::string username;

public:
    Client(int fd, const std::string& nick, const std::string& user);

    int getClientFd() const;
    std::string getNickname() const;
    std::string getUsername() const;
};

class Channel {
private:
    std::string name;
    std::vector<std::string> bannedUsers;
    std::vector<std::string> kickedUsers;
    std::vector<std::string> operators;
    std::vector<std::string> voicedUsers;
    std::map<int, Client*> clients; // Maps client file descriptors to client objects

public:
    Channel(const std::string& channelName);

    std::string getName() const;
    bool isUserBanned(const std::string& username) const;
    bool isUserKicked(const std::string& username) const;
    bool isOperator(const std::string& username) const;
    bool isVoiced(const std::string& username) const;

    void addClient(int client_fd, Client* client);
    void removeClient(int client_fd);
    std::map<int, Client*> getClientList() const;

    void broadcast(Server* server, int sender_fd, const std::string& message) const;
};

static void broadcastToChannel(Server* server, int client_fd, cmd_struct cmd_infos) {
    std::string target, msg_to_send;
    
    if (!parseMessage(cmd_infos.message, target, msg_to_send, client_fd, server)) {
        return;
    }

    if (target[0] == '#') {
        handleChannelMessage(server, client_fd, target, msg_to_send);
    } else {
        handlePrivateMessage(server, client_fd, target, msg_to_send);
    }
}

static bool parseMessage(const std::string& message, std::string& target, std::string& msg_to_send, int client_fd, Server* server) {
    size_t delimiter = message.rfind(":");
    
    if (delimiter == std::string::npos) {
        addToClientBuffer(server, client_fd, ERR_NOTEXTTOSEND(server->getClientNickname(client_fd)));
        return false;
    }
    
    target = message.substr(1, delimiter - 1);
    
    if (target.empty()) {
        addToClientBuffer(server, client_fd, ERR_NORECIPIENT(server->getClientNickname(client_fd)));
        return false;
    }
    
    msg_to_send = message.substr(delimiter);
    size_t spacePos = target.find(" ");
    
    if (spacePos != std::string::npos) {
        target.erase(spacePos);
    }
    
    return true;
}

static void handleChannelMessage(Server* server, int client_fd, const std::string& target, const std::string& msg_to_send) {
    Channel* channel = server->findChannel(target.substr(1)); // Remove '#' to find channel
    
    if (!channel) {
        addToClientBuffer(server, client_fd, ERR_NOSUCHNICK(server->getClientNickname(client_fd), target));
    } else if (channel->isUserBanned(server->getClientNickname(client_fd))) {
        std::cout << "[Server] " << server->getClientNickname(client_fd) << " is banned from the channel and can't send messages anymore" << std::endl;
    } else if (channel->isUserKicked(server->getClientNickname(client_fd))) {
        std::cout << "[Server] " << server->getClientNickname(client_fd) << " is kicked from the channel and can't send messages anymore" << std::endl;
    } else if (!channel->canUserSpeak(server->getClientNickname(client_fd))) {
        addToClientBuffer(server, client_fd, ERR_CANNOTSENDTOCHAN(server->getClientNickname(client_fd), target));
    } else {
        channel->broadcast(server, client_fd, msg_to_send);
    }
}

// Function to handle the PRIVMSG command
void privmsg(Server* server, int client_fd, cmd_struct cmd_infos) {
    std::map<const int, Client> &client_list = server->getClients();
    std::map<std::string, Channel> &channel_list = server->getChannels();
    std::map<const int, Client>::iterator it_client = client_list.find(client_fd);

    std::string target;
    std::string msg_to_send;

    // Parsing message
    size_t delimiter = cmd_infos.message.rfind(":");
    if (delimiter == std::string::npos) {
        addToClientBuffer(server, client_fd, ERR_NOTEXTTOSEND(it_client->second.getNickname()));
        return;
    }
    target = cmd_infos.message.substr(1, delimiter - 1);
    if (target.empty()) {
        addToClientBuffer(server, client_fd, ERR_NORECIPIENT(it_client->second.getNickname()));
        return;
    }
    msg_to_send = cmd_infos.message.substr(delimiter);

    delimiter = target.find(" ");
    if (delimiter != std::string::npos) {
        target.erase(delimiter);
    }

    // Channel case
    if (target[0] == '#') {
        Channel* channel = server->findChannel(target.substr(1)); // Remove '#' to find the channel

        if (!channel) {
            addToClientBuffer(server, client_fd, ERR_NOSUCHNICK(it_client->second.getNickname(), target));
        } else {
            broadcastToChannel(server, client_fd, it_client, channel, msg_to_send);
        }
    }
    // User case
    else {
        Client* targetUser = server->getClientByUsername(target);

        if (!targetUser) {
            if (target == "chatGTP") {
                bot(server, client_fd, it_client, msg_to_send);
            } else {
                addToClientBuffer(server, client_fd, ERR_NOSUCHNICK(it_client->second.getNickname(), target));
            }
        } else {
            addToClientBuffer(server, targetUser->getClientFd(),
                              RPL_PRIVMSG(it_client->second.getNickname(), it_client->second.getUsername(), target, msg_to_send));
        }
    }
}
