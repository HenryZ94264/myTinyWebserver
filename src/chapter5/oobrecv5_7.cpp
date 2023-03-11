#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
// #include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <string>
#include <cstdlib>
#include <iostream>
#include <libgen.h>

#define BUF_SIZE 1024

// 带外数据的接受 服务器端。 多字节的情况下"abc"，只有最后一个字节"c"被当作带外数据。需要通过其他方式来实现紧急数据的传输

int main(int argc, char* argv[])
{
    if (argc <= 2)
    {
        std::cout << "usage: " << basename(argv[0]) << "ip_address port_number" << std::endl;
        return 1;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int sockfd;
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(bind(sockfd, (struct sockaddr*)&address, sizeof(address)) == -1)
    {
        std::cout << "bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(listen(sockfd, 2) == -1)
    {
        std::cout << "listen failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client;
    socklen_t client_length = sizeof(client);
    int connfd;
    if((connfd = accept(sockfd, (struct sockaddr*)&client, &client_length)) < 0)
    {
        std::cout << "errno is: " << errno << std::endl;
    }
    else
    {
        char buffer[BUF_SIZE];

        std::memset(&buffer, '\0', BUF_SIZE);
        int ret = recv(connfd, buffer, BUF_SIZE - 1, 0);    // 为什么-1?
        std::cout << "got " << ret << " bytes of oob data " << buffer << std::endl;

        std::memset(&buffer, '\0', BUF_SIZE);
        ret = recv(connfd, buffer, BUF_SIZE - 1, MSG_OOB);    // 为什么-1?
        std::cout << "got " << ret << " bytes of oob data " << buffer << std::endl;

        std::memset(&buffer, '\0', BUF_SIZE);
        ret = recv(connfd, buffer, BUF_SIZE - 1, 0);    // 为什么-1?
        std::cout << "got " << ret << " bytes of oob data " << buffer << std::endl;

        close(connfd);
    }
    close(sockfd);
    return 0;
}