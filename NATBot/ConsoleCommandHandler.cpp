//
//  ConsoleCommandHandler.cpp


#include "ConsoleCommandHandler.h"

bool ConsoleCommandHandler::AddCommand(std::string name, int argCount, void(*handler)(std::string /*params*/, IRCClient* /*client*/))
{
    CommandEntry entry;
    entry.argCount = argCount;
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
        command = command.substr(1); // Remove the slash
    
    std::string name = command.substr(0, command.find(" "));
    std::string args = command.substr(command.find(" ") + 1);
    size_t argCount = std::count(args.begin(), args.end(), ' ');
    std::transform(name.begin(), name.end(), name.begin(), towlower);
    
    std::map<std::string, CommandEntry>::const_iterator itr = _commands.find(name);
    if (itr == _commands.end())
    {
        std::cout << "Command not found." << std::endl;
        return;
    }
    if (++argCount < itr->second.argCount)
    {
        std::cout << "Insuficient arguments." << std::endl;
        return;
    }
    
    (*(itr->second.handler))(args, client);
    std::cout << "its over" << std::endl;
}
