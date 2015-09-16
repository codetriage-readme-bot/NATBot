#include <iostream>
#include "NATBot.h"

void handleRaw(std::string caller, std::string text)
{
	std::cout << "in raw " << caller << ": " << text << std::endl;
}

int main()
{
    NATBot tClient("TwitchPlays.cfg");
	tClient.hookRaw(handleRaw);
	tClient.start();
    tClient.stop(); puts("STOPPED");
}
