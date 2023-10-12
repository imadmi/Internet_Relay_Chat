#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

Irc::Irc(int port, char *password)
{
    _passWord = password;
    _port = port;
    _serverName = ":MSN ";

    createSocket();
    settingsockopt();
    nonBlockFd();
    bindSocket();
    listeningToClients();

    std::cout << BLACK << "IRC Server is running on port : " << _port << RESET << std::endl;
}

void Irc::createSocket()
{
    _serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_serverSocket < 0)
        printc("ERROR : socket", RED, 1);

    _server_addr.sin_addr.s_addr = INADDR_ANY;
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_port = htons(_port);
}

void Irc::settingsockopt()
{
    int opt = 1;
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        printc("ERROR : setsockopt", RED, 1);
}

void Irc::nonBlockFd()
{
    if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1)
        printc("ERROR : fcntl", RED, 1);
}

void Irc::bindSocket()
{

    if (bind(_serverSocket, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) < 0)
        printc("ERROR : bind", RED, 1);
}

void Irc::listeningToClients()
{
    if (listen(_serverSocket, MAX_CLIENTS) < 0)
        printc("ERROR : listen", RED, 1);
}

void Irc::runServer()
{
    pollfd serverPoll;
    serverPoll.fd = _serverSocket;
    serverPoll.events = POLLIN;
    _pollfds.push_back(serverPoll);

    while (true)
    {
        if (poll(&_pollfds[0], _pollfds.size(), -1) == -1)
            printc("ERROR : poll", RED, 1);

        if (_pollfds[0].revents & POLLIN)
            addClient();

        Handle_activity();
    }
}

Client::Client(int fd)
{
    _fd = fd;
    _is_authenticated = false;
    _is_registered = false;
    _pwd = 0;

    _nickname = "";
    _username = "";
    _buffer = "";

    struct timeval time;
    gettimeofday(&time, NULL);
    _start = time.tv_sec;
}


void Irc::handleLogTime(Client &client)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    long end = time.tv_sec - client.getStart();
    long minutes = end / 60;
    end %= 60;

    std::ostringstream oss;
    oss << minutes << " minutes and " << end << " seconds";
    std::string str = oss.str();
    std::string msg = ": 001 logtime : " + str + "\n";

    send(client.get_fd(), msg.c_str(), strlen(msg.c_str()), 0);
}

void Irc::addClient()
{
    _newSocket = accept(_serverSocket, NULL, NULL);
    if (_newSocket < 0)
        printc("Error accepting client connection", RED, 1);

    Client new_client(_newSocket);
    pollfd client_pollfd = {_newSocket, POLLIN, 0};
    _pollfds.push_back(client_pollfd);
    _clients.insert(std::pair<int, Client>(_newSocket, new_client));
    std::cout << GREEN << "[Server] Added client #" << _newSocket \
    << " successfully" << RESET << std::endl;
}

void Irc::printc(std::string msg, std::string color, int ex)
{
    std::cout << color << msg << RESET << std::endl;
    if (ex)
        exit(EXIT_SUCCESS);
}

void Irc::Handle_activity()
{
    for (int i = 1; i < _pollfds.size(); i++)
    {
        if (_pollfds[i].revents & POLLIN)
        {
            char buffer[BUFFER_SIZE];
            bzero(buffer, sizeof(buffer));
            int count = recv(_pollfds[i].fd, buffer, BUFFER_SIZE - 1, 0);
            if (count == 0)
            {
                std::cout << YELLOW << "Client " << _pollfds[i].fd << " disconnected.\n";
                std::cout << PURPLE << "Total clients : " << _pollfds.size() - 2 << RESET << std::endl;
            }
            else if (count < 0)
            {
                std::cerr << RED << "Error reading from client " << _pollfds[i].fd << "\n";
                std::cout << PURPLE << "Total clients : " << _pollfds.size() - 2 << RESET << std::endl;
            }
            else
            {
                std::string message(buffer);
                it = _clients.find(_pollfds[i].fd);

                if (it != _clients.end())
                    recvClientsMsg(it->second, message);
                if (it->second.get_buffer().find('\n') != std::string::npos)
                {
                    excute_command(it->second.get_buffer(), it->second, _channels, _clients);
                    std::cout << BLUE << "Client [" << it->second.get_fd() << "] : " \
                    << it->second.get_buffer()  << RESET << std::flush;
                    it->second.set_buffer("");
                }
            }
            if (_pollfds[i].revents & POLLIN && count <= 0)
            {
                close(_pollfds[i].fd);
                _clients.erase(_pollfds[i].fd);
                _pollfds.erase(_pollfds.begin() + i);
            }
        }
    }
}

void Irc::recvClientsMsg(Client &client, std::string buffer)
{
    client.addt_buffer(buffer);
}

void Irc::handleQuotes(Client &client)
{
    std::map<int, std::string> quotes;

    quotes[1] = "Success is not final; failure is not fatal: It is the courage to continue that counts.";
    quotes[2] = "It is better to fail in originality than to succeed in imitation.";
    quotes[3] = "The road to success and the road to failure are almost exactly the same.";
    quotes[4] = "Success usually comes to those who are too busy looking for it.";
    quotes[5] = "Develop success from failures. Discouragement and failure are two of the surest stepping stones to success.";
    quotes[6] = "Nothing in the world can take the place of Persistence. Talent will not; nothing is more common than unsuccessful men with talent. Genius will not; unrewarded genius is almost a proverb. Education will not; the world is full of educated derelicts. The slogan 'Press On' has solved and always will solve the problems of the human race.";
    quotes[7] = "There are three ways to ultimate success: The first way is to be kind. The second way is to be kind. The third way is to be kind.";
    quotes[8] = "Success is peace of mind, which is a direct result of self-satisfaction in knowing you made the effort to become the best of which you are capable.";
    quotes[9] = "I never dreamed about success. I worked for it.";
    quotes[10] = "Success is getting what you want, happiness is wanting what you get.";
    quotes[11] = "The pessimist sees difficulty in every opportunity. The optimist sees opportunity in every difficulty.";
    quotes[12] = "Don’t let yesterday take up too much of today.";
    quotes[13] = "You learn more from failure than from success. Don’t let it stop you. Failure builds character.";
    quotes[14] = "If you are working on something that you really care about, you don’t have to be pushed. The vision pulls you.";
    quotes[15] = "Experience is a hard teacher because she gives the test first, the lesson afterwards.";
    quotes[16] = "To know how much there is to know is the beginning of learning to live.";
    quotes[17] = "Concentrate all your thoughts upon the work in hand. The sun's rays do not burn until brought to a focus.";
    quotes[18] = "Either you run the day or the day runs you.";
    quotes[19] = "I’m a greater believer in luck, and I find the harder I work the more I have of it.";
    quotes[20] = "When we strive to become better than we are, everything around us becomes better too.";
    quotes[21] = "Opportunity is missed by most people because it is dressed in overalls and looks like work.";
    quotes[22] = "Setting goals is the first step in turning the invisible into the visible.";
    quotes[23] = "Women challenge the status quo because we are never it.";
    quotes[24] = "We don’t just sit around and wait for other people. We just make, and we do.";
    quotes[25] = "Think like a queen. A queen is not afraid to fail. Failure is another stepping stone to greatness.";
    quotes[26] = "The strongest actions for a woman is to love herself, be herself and shine amongst those who never believed she could.";
    quotes[27] = "Whenever you see a successful woman, look out for three men who are going out of their way to try to help her.";
    quotes[28] = "Some women choose to follow men, and some choose to follow their dreams. If you’re wondering which way to go, remember that your career will never wake up and tell you that it doesn’t love you anymore.";
    quotes[29] = "The thing women have yet to learn is nobody gives you power. You just take it.";
    quotes[30] = "No woman wants to be in submission to a man who isn’t in submission to God!";
    quotes[31] = "A witty woman is a treasure; a witty beauty is a power.";
    quotes[32] = "When a woman becomes her own best friend life is easier.";
    quotes[33] = "If you want something said, ask a man; if you want something done, ask a woman.";
    quotes[34] = "We need women at all levels, including the top, to change the dynamic, reshape the conversation, to make sure women’s voices are heard and heeded, not overlooked and ignored.";
    quotes[35] = "It took me quite a long time to develop a voice, and now that I have it, I am not going to be silent.";
    quotes[36] = "Women must learn to play the game as men do.";
    quotes[37] = "I swear, by my life and my love of it, that I will never live for the sake of another man, nor ask another man to live for mine.";

    struct timeval time;

    gettimeofday(&time, NULL);
    long nbr = time.tv_sec ;
    nbr %= quotes.size();
    
    if (quotes.find(nbr) != quotes.end())
    {
        std::string msg = ": 001 quotes : " + quotes[nbr] + "\n";
        send(client.get_fd(), msg.c_str(), strlen(msg.c_str()), 0);
    }
}