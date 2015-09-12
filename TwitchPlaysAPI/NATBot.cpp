#include "NATBot.h"

//global mutex, this way threads dont talk over one another
std::mutex testMutex;

NATBot::NATBot(const char *filename): callbackRaw(NULL)
{
	commandHandler.AddCommand("msg", 2, &msgCommand);
	commandHandler.AddCommand("join", 1, &joinCommand);
	commandHandler.AddCommand("part", 1, &partCommand);
	commandHandler.AddCommand("ctcp", 2, &ctcpCommand);

	FILE *fi = fopen(filename, "r");
	if (fi == NULL)
	{
        char correct = 'n';
        while (tolower(correct) != 'y')
        {
            fprintf(stderr, "* Error: Cannot open config file: %s\n", filename);
            std::cout << "Generating one for you" << std::endl;
            
            std::cout << "bot username?" << std::endl;
            std::getline(std::cin, user);
            std::cout << "oauth?" << std::endl;
            std::getline(std::cin, password);
            std::cout << "default channel username to join?" << std::endl;
            std::getline(std::cin, channel);
            std::cout << "what you entered was\n"
            << "user name = " << user << "\n"
            << "oauth = "<< password<< "\n"
            << "default channel = "<< channel << "\n"
            << "is this correct? (y/n)"<< std::endl;
            
            std::cin >> correct;
            std::cout << tolower(correct);
            if (tolower(correct) == 'y')
            {
                std::ofstream config;
                config.open(filename);
                config << "host = irc.twitch.tv\n";
                config << "port = 6667\n";
                config << "nick = " + user + "\n";
                config << "user = " + user + "\n";
                config << "password = " + password + "\n";
                config << "channel = " + channel + "\n";
                
            }
            
        }

        
        return ;
	}
	char s[200], par[200], val[200];
	size_t len;
	while (fgets(s, 200, fi))
	{
		len = strlen(s);
		if (len == 0) continue;
		if (s[len - 1] == '\n') s[len - 1] = '\0';
		sscanf(s, "%s = %s", par, val);
		if (strcmp(par, "host") == 0)
		{
            
			host = new char[len + 1];
			strcpy(host, val);
		}
		else if (strcmp(par, "port") == 0)
			port = atoi(val);
		else if (strcmp(par, "nick") == 0)
			nick = val;
		else if (strcmp(par, "user") == 0)
			user = val;
		else if (strcmp(par, "password") == 0)
			password = val;
		else if (strcmp(par, "channel") == 0)
		{
			int i;
			len = strlen(val);
			for (i = 0; i < len; i++)
				val[i] = isalpha(val[i]) ? tolower(val[i]) : val[i];
			channel = val;
		}
	}
	printf("Finished reading config file: %s\n", filename);
}

bool NATBot::start()
{
	client.Debug(true);
	curTwitch = this;
    
    //making of threads, will run simultaneously
    std::thread iThread(inputThread, &client), TLThread(TwitchListener, this);

    iThread.join();
   // std::cout << "first " << std::endl;
    TLThread.join();
    //std::cout << "second " << std::endl;


	//w hile (!running) ; // still loading
    std::cout << "quitting" << std::endl;
	return true;
}

bool NATBot::stop()
{
	running = false;
	return true;
}

void NATBot::hookRaw(void (*cbRaw)(std::string, std::string))
{
	callbackRaw = client.callbackRaw = cbRaw;
}

void NATBot::sendMessage(std::string msg)
{
	client.SendIRC("PRIVMSG #" + channel + " :" + msg);
}

void * TwitchListener(void *arg)
{
	NATBot *twi = (NATBot *)arg;
	IRCClient &irc = twi->client;
	if (irc.InitSocket())
	{
        std::unique_lock<std::mutex> lock(testMutex);
        //lock.try_lock();
		 std::cout << "Socket initialized. Connecting..." << std::endl;
        lock.unlock();
		if (irc.Connect(twi->host, twi->port))
		{
            lock.try_lock();
			 std::cout << "Connected. Loggin in..." << std::endl;
            lock.unlock();
           // std::cout << "did i unlock it1?" << std::endl;

			if (irc.Login(twi->nick, twi->user, twi->password))
			{
                lock.try_lock();
			std::cout << "Logged in." << std::endl;
                lock.unlock();
              //  std::cout << "did i unlock it?2" << std::endl;

				twi->running = true;
				signal(SIGINT, signalHandler);
				irc.SendIRC("JOIN #" + twi->channel);
				while (irc.Connected() && twi->running)
				{
                    lock.try_lock();
					std::cout << "Waiting for input" << std::endl;
                    lock.unlock();
                 //   std::cout << "did i unlock it?3" << std::endl;

					irc.ReceiveData();
				}

			}

			if (irc.Connected())
            {
				irc.Disconnect();
            }

			 std::cout << "Disconnected." << std::endl;
		}
	}
	return NULL;
}

void msgCommand(std::string arguments, IRCClient* client)
{
	std::string to = arguments.substr(0, arguments.find(" "));
	std::string text = arguments.substr(arguments.find(" ") + 1);

	std::cout << "To " + to + ": " + text << std::endl;
	client->SendIRC("PRIVMSG " + to + " :" + text);
};

void joinCommand(std::string channel, IRCClient* client)
{
	if (channel[0] != '#')
		channel = "#" + channel;

	client->SendIRC("JOIN " + channel);
}

void partCommand(std::string channel, IRCClient* client)
{
	if (channel[0] != '#')
		channel = "#" + channel;

	client->SendIRC("PART " + channel);
}

void ctcpCommand(std::string arguments, IRCClient* client)
{

	//std::string to = arguments.substr(0, arguments.find(" "));
	//std::string text = arguments.substr(arguments.find(" ") + 1);

	//std::transform(text.begin(), text.end(), text.begin(), towupper);

	client->SendIRC("CTCP");
}

void signalHandler(int signal)
{
	curTwitch -> running = false;
}

void * inputThread(void *client)
{
    std::unique_lock<std::mutex> lock(testMutex);

	std::string command;
   // lock.try_lock();
    
	std::cout << "in this part" << std::endl;
    
    lock.unlock();
    std::cout << "did i unlock it?" << std::endl;

	while (true)
	{
		getline(std::cin, command);
		if (command == "")
			continue;

		if (command[0] == '/')
		{
			std::cout << "" << std::endl;
			commandHandler.ParseCommand(command, (IRCClient*)client);
		}
		else
			((IRCClient*)client)->SendIRC(command);

		if (command == "quit")
        {
            lock.lock();
            std::cout << "quiting" << std::endl;
            lock.unlock();
            break;
        }
	}
    std::cout << "at return"<< std::endl;
	return NULL;
}

bool ConsoleCommandHandler::AddCommand(std::string name, int argCount, void(*handler)(std::string /*params*/, IRCClient* /*client*/))
{
	CommandEntry entry;
	entry.handler = handler;
	std::transform(name.begin(), name.end(), name.begin(), towlower);
	_commands.insert(std::pair<std::string, CommandEntry>(name, entry));
	return true;
}

void ConsoleCommandHandler::ParseCommand(std::string command, IRCClient* client)
{
	if (_commands.empty())
	{
		std::cout << "No commands available." << std::endl;
		return;
	}

	if (command[0] == '/')
    {
		command = command.substr(1); // Remove the slash
    }
	std::string name = command.substr(0, command.find(" "));
	std::string args = command.substr(command.find(" ") + 1);
    
	long argCount = std::count(args.begin(), args.end(), ' ');

	std::transform(name.begin(), name.end(), name.begin(), towlower);

	std::map<std::string, CommandEntry>::const_iterator itr = _commands.find(name);
   // std::cout << "i made it here  "  << itr->first << std::endl;
	if (itr == _commands.end())
	{
		std::cout << "Command not found." << std::endl;
		return;
	}

	if (++argCount < itr->second.argCount)
	{
		std::cout << "Insuficient arguments. maybe? fuck if i know?" << std::endl;
		return;
	}

	(*(itr->second.handler))(args, client);
}
