//
//  ConsoleCommandHandler.h

#ifndef ConsoleCommandHandler_h
#define ConsoleCommandHandler_h

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <map>
#include <algorithm>
#include <signal.h>
#include "IRCClient.h"

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

#endif
