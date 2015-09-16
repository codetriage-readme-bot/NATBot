#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <map>
#include <algorithm>
#include <signal.h>
#include <mutex>
#include <thread>
#include <chrono>
#include <fstream>
#include "IRCClient.h"


#ifndef _TWITCH_PLAYS
#define _TWITCH_PLAYS

class NATBot;
class ConsoleCommandHandler;

class NATBot
{
public:
    

    IRCClient client;
    char *host;
    int port;
    std::string nick, user;
    std::string password;
    std::string channel;
    
    volatile bool running;
    
    void (*callbackRaw)(std::string, std::string);
    
    NATBot(const char *filename = "TwitchPlays.cfg");
    bool start();
    bool stop();
    void hookRaw(void (*cbRaw)(std::string, std::string));
    void sendMessage(std::string msg);
};

class ConsoleCommandHandler
{
public:
    bool AddCommand(std::string name, int argCount, void (*handler)(std::string /*params*/, IRCClient* /*client*/));
    void ParseCommand(std::string command, IRCClient* client);
    
private:
    struct CommandEntry
    {
        int argCount;
        void (*handler)(std::string /*arguments*/, IRCClient* /*client*/);
    };
    
    std::map<std::string, CommandEntry> _commands;
};

void msgCommand(std::string arguments, IRCClient* client);
void joinCommand(std::string channel, IRCClient* client);
void partCommand(std::string channel, IRCClient* client);
void ctcpCommand(std::string arguments, IRCClient* client);
void signalHandler(int signal);
void * inputThread(void *client);

static ConsoleCommandHandler commandHandler;
static NATBot *curTwitch;
void * TwitchListener(void *);

#endif
