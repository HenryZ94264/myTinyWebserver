#include "../../inc/mySocket.h"
#include <cstring>


int getSocket(const char* ip, const int& port, bool reuse)
{
    struct sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        return SOCKET;
    }
    if(reuse)
    {
        int reuse = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    }

    int ret = bind(sockfd, (struct sockaddr*)&address, sizeof(address));
    if(ret == -1)
    {
        return BIND;
    }

    ret = listen(sockfd, 5);
    if(ret == -1)
    {
        return LISTEN;
    }
    return sockfd;
}