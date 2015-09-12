#include <iostream>
#include "NATBot.h"
//#include <thread>
//#include <future>

void handleRaw(std::string caller, std::string text)
{
	std::cout << "in raw " << caller << ": " << text << std::endl;
}

//void test (int i)
//{
    //cout << " int hre"<< i << endl;
//}
int main()
{
    
    //thread t1(test,1), t2(test,2);
    //async(test(1));
    //t1.join();
    //t2.join();
    //std::cout << argc << "   r   " << argv << std::endl;
    
    NATBot tClient("TwitchPlays.cfg");
	tClient.hookRaw(handleRaw);
    
	tClient.start();
    std::cout << "ending" << std::endl;
    
    //tClient.sendMessage("H");
	tClient.stop(); puts("STOPPED");
    std::cout << "stopped" << std::endl;
	//while(1);
}
