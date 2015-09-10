#include <iostream>
#include "TwitchPlaysAPI.h"
#include <thread>
#include <future>
using namespace std;

void handleRaw(string caller, string text)
{
	cout << "in raw " << caller << ": " << text << endl;
}

//void test (int i)
//{
    //cout << " int hre"<< i << endl;
//}
int main(int argc, char* argv[])
{
    //thread t1(test,1), t2(test,2);
    //async(test(1));
    //t1.join();
    //t2.join();
	
    TwitchPlays tClient("TwitchPlays.cfg");
	tClient.hookRaw(handleRaw);
    
	tClient.start();
    cout << "ending" << endl;
    
    //tClient.sendMessage("H");
	tClient.stop(); puts("STOPPED");
    cout << "stopped" << endl;
	//while(1);
}
