/*
 * Copyright (C) 2011 Fredi Machado <https://github.com/Fredi>
 * IRCClient is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * http://www.gnu.org/licenses/lgpl.html 
 */
//http://stackoverflow.com/questions/6649936/c-compiling-on-windows-and-linux-ifdef-switch
//http://stackoverflow.com/questions/18884251/getaddrinfo-i-am-not-getting-any-canonname
//http://manpages.courier-mta.org/htmlman3/getaddrinfo.3.html
#include <cstring>
#include <fcntl.h>
#include "IRCSocket.h"
//#include <winsock2.h>
//#include <WS2tcpip.h>
#define MAXDATASIZE 4096
#define _WINSOCK_DEPRECATED_NO_WARNINGS 

bool IRCSocket::Init()
{
    #ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData))
    {
        ;// std::cout << "Unable to initialize Winsock." << std::endl;
        return false;
    }
    #endif

    if ((_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        ;// std::cout << "Socket error." << std::endl;
        #ifdef _WIN32
        WSACleanup();
        #endif
        return false;
    }

    int on = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (char const*)&on, sizeof(on)) == -1)
    {
        ;// std::cout << "Invalid socket." << std::endl;
        #ifdef _WIN32
        WSACleanup();
        #endif
        return false;
    }

    #ifdef _WIN32
    u_long mode = 0;
    ioctlsocket(_socket, FIONBIO, &mode);
    #else
    fcntl(_socket, F_SETFL, O_NONBLOCK);
    fcntl(_socket, F_SETFL, O_ASYNC);
    #endif

    return true;
}

bool IRCSocket::Connect(char const* host, int port)
{
//    hostent* he;
	int res;
	char * portss = "6667";
	struct addrinfo hints, *servinfo;
	hints.ai_family = AF_UNSPEC;    //IPv4 or IPv6 doesnt matter
	hints.ai_socktype = SOCK_STREAM;        //TCP stream sockets
	memset(&hints, 0, sizeof(hints));
	if ((res = getaddrinfo("irc.twitch.tv", portss, &hints, &servinfo)) != 0)
    {
		//system("pause");
        ;// std::cout << "Could not resolve host: " << host << std::endl;
        #ifdef _WIN32
        WSACleanup();
        #endif
        return false;
    }
	//int mySocket;
	//setup socket
	if ((_socket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)))
	{
		perror("client: socket");

	}

	//Connect
	if (connect(_socket, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
	{
		closesocket(_socket);
        perror("Client Connected");
		return false;
		
	}

    _connected = true;
    return true;
}

void IRCSocket::Disconnect()
{
    if (_connected)
    {
        #ifdef _WIN32
        shutdown(_socket, 2);
        #endif
        closesocket(_socket);
        _connected = false;
    }
}

bool IRCSocket::SendData(char const* data)
{
    if (_connected)
        if (send(_socket, data, strlen(data), 0) == -1)
            return false;

    return true;
}

std::string IRCSocket::ReceiveData()
{
    char buffer[MAXDATASIZE];

    memset(buffer, 0, MAXDATASIZE);

    int bytes = recv(_socket, buffer, MAXDATASIZE - 1, 0);

	if (bytes > 0)
	{
		std::cout << std::string(buffer) << std::endl;
		return std::string(buffer);
	}
    else
        Disconnect();

    return "";
}
