#ifndef __MYSOCKET__
#define __MYSOCKET__

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>

#define LOG(x) std::cout << x << std::endl

enum SOCKET_FAILED_TYPE :int
{
    SOCKET = -1, BIND = -2, LISTEN = -3
};

int getSocket(const char* ip, const int& port, bool reuse =false);


#endif