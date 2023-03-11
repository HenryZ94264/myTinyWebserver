#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <cstring>
#include <iostream>

// 以下代码段为SO_REUSEADDR 的socket选项的使用demo

namespace demo_resuseaddr {
    void demo()
    {

        int port = 12345;
        const char* ip_address = "ipaddress";

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    
    if(sock < 0)
    {
        std::cout << "error" << std::endl;
    }
    int reuse = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    struct sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip_address, &address.sin_addr);
    address.sin_port = htons(port);

    int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
    }

}