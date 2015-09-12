# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=g++ #-pthread 
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
#CFLAGS=-c -Wall -Wno-write-strings -Wno-unused -std=c++11 #-pthread ME := $(shell uname)
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    CFLAGS=-c -Wall -Wno-write-strings -Wno-unused -std=c++0x
else
    CFLAGS=-c -Wall -Wno-write-strings -Wno-unused -std=c++11
endif
all: hello

hello: IRCClient.o IRCHandler.o IRCSocket.o NATBot.o Main.o
	$(CC) IRCClient.o IRCHandler.o IRCSocket.o NATBot.o Main.o -o NATBot

Main.o: Main.cpp
	$(CC) $(CFLAGS) Main.cpp

IRCClient.o: IRCClient.cpp
	$(CC) $(CFLAGS) IRCClient.cpp

IRCHandler.o: IRCHandler.cpp
	$(CC) $(CFLAGS) IRCHandler.cpp

IRCSocket.o: IRCSocket.cpp
	$(CC) $(CFLAGS) IRCSocket.cpp

NATBot.o: NATBot.cpp
	$(CC) $(CFLAGS) NATBot.cpp

clean:
	rm *o NATBot
 
