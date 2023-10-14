#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"


static bool is_operator(Client & client,  Channel& channel)
{
    std::map<int, Client>::iterator it;
    for (it = channel.get_operators().begin(); it != channel.get_operators().end(); ++it)
    {
        if (it->second.get_nickname() == client.get_nickname())
            return true;
    }
    return false;
}

bool client_already_exist(std::string nickname, std::map<int, Client> clients)
{
    std::map<int, Client>::iterator it;
    for (it = clients.begin(); it != clients.end(); ++it)
    {
        Client cl = it->second;
        if (cl.get_nickname() == nickname)
            return true;
    }
    return false;
}

std::string filteredString(std::string str)
{
    std::string filteredString;
    for (int i = 0; i < (int)str.length(); i++)
    {
        if (str[i] != '\n' && str[i] != '\r' && str[i] != '\t' && str[i] != '\v' && str[i] != '\f' && str[i] != ' ')
        {
            filteredString += str[i];
        }
    }
    return filteredString;
}

void excute_command(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients)
{
    if (command.substr(0, 4) == "PASS")
        pass(command, client, channels, clients);
    else if (command.substr(0, 4) == "NICK")
        nick(command, client, channels, clients);
    else if (command.substr(0, 4) == "USER")
        user(command, client, channels, clients);
    else if (command.substr(0, 4) == "JOIN")
    {
        join(command, client, channels);
    }
    else if (command.substr(0, 4) == "KICK")
    {
        kick(command, client, channels);
    }
    // INVITE
    if (command.substr(0, 6) == "INVITE")
    {
        invite(command, client, channels, clients);
    }
    if (command.substr(0, 4) == "MODE")
    {
        mode(command, client, channels);
    }
    if (client.is_authenticated())
    {
        send(client.get_fd(), client.get_buff_to_send().c_str(), client.get_buff_to_send().length(), 0);
        client.set_buff_to_send("");
    }

    if (command.substr(0, 7) == "PRIVMSG" && client.is_authenticated())
    {
        privmsg(command, client, clients, channels);
    }
    if (command.substr(0, 7) == "LOGTIME" && client.is_authenticated())
    {
        Irc::handleLogTime(client);
    }
    if (command.substr(0, 6) == "QUOTES" && client.is_authenticated())
    {
        Irc::handleQuotes(client);
    }
    if (command.substr(0, 3) == "BOT" && client.is_authenticated())
    {
        // pthread_t dccThread;
        // if (pthread_create(&dccThread, NULL, dccFileTransfer, &client) != 0)
        // {
        //     std::cerr << "Error creating DCC thread" << std::endl;
        //     return ;
        // }
        // if (pthread_detach(dccThread) != 0)
        // {
        //     std::cerr << "Error detaching DCC thread" << std::endl;
        //     return ;
        // }
        Irc::handleBot(client);
        // pthread_join(dccThread, NULL);
    }
    // if (command.substr(0, 3) == "BOT")
    // {
    //     std::string file_path = "/goinfre/imimouni/zoro.jpeg";

    //     char buffer[1000];
    //     FILE *fd = fopen(file_path.c_str(), "rb");
    //     std::string buff;
    //     while (!feof(fd))
    //     {
    //         int size = fread(&buffer, 1, 1000, fd);
    //         if (size < 0)
    //             break;
    //         buff.append(buffer, size);
    //     }



    //     std::string dccRequest = "DCC SEND";
    //     dccRequest += file_path;
    //     dccRequest += " 0 0";

    //     std::string bot = "#ch";


    //     // std::string privmsgCommand = "PRIVMSG " + client.get_nickname() + " :" + '\x01' + "DCC SEND " + "/goinfre/imimouni/zoro.jpeg" + " 0 6667 " + std::to_string(buff.size()) + '\x01';
    //     std::string privmsgCommand = "PRIVMSG " + bot  + " " + '\x01' + "DCC SEND " + "/goinfre/imimouni/zoro.jpeg" + " 10.12.11.2 6667 " + std::to_string(buff.size()) + '\x01';
    //     // std::string privmsgCommand = "PRIVMSG " + bot  + " DCC SEND " + "/goinfre/imimouni/zoro.jpeg" + " 10.12.11.2 6667 " + std::to_string(buff.size());


    // // PRIVMSG imad \x01 DCC SEND /goinfre/imimouni/zoro.jpeg 10.12.11.2 6667 420782\x01
    // // PRIVMSG #ch \x01 DCC SEND /goinfre/imimouni/zoro.jpeg 10.12.11.2 6667 420782\x01

    //     std::cout << privmsgCommand << std::endl;


    //     //send(client.get_fd(), privmsgCommand.c_str(), strlen(privmsgCommand.c_str()), 0);
    //     privmsg(privmsgCommand,client, clients, channels);
    // }
}