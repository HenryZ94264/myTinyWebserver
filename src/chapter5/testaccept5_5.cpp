#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <errno.h>
#include <cstring>
#include <stdio.h>

// socket 接受一个异常连接，远端socket连接到端口后，立刻断开连接，服务器会显示CLOSE_WAIT，但是依然能够接受这个异常的连接


// int main(int argc, char* argv[])
// {
//     if(argc <= 2)
//     {
//         std::cout << "usage: " << basename(argv[0]) << " ip_address port_number" << std::endl;
//         return 1;
//     }

//     const char* ip = argv[1];
//     int port = atoi(argv[2]);

//     // IPv4 socket addr
//     struct sockaddr_in address;
//     std::memset(&address, 0, sizeof(address));
//     address.sin_family = AF_INET;
//     inet_pton(AF_INET, ip, &address.sin_addr);
//     address.sin_port = htons(port);

//     int socket_fd;
//     // 创建socket
//     if((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
//     {
//         perror("socket failed");
//         exit(EXIT_FAILURE);
//     }

//     // socket fd 绑定 address
//     if(bind(socket_fd, (struct sockaddr*) &address, sizeof(address)) < 0)
//     {
//         perror("bind failed");
//         exit(EXIT_FAILURE);
//     }

//     // 监听
//     if(listen(socket_fd, 5) < 0)
//     {
//         perror("listen");
//         exit(EXIT_FAILURE);
//     }

//     //暂停20秒，等待客户端连接和相关操作（掉线或退出）
//     sleep(20);
//     // 获取远端socket地址
//     /* 结论：accetp只是从监听队列中取出连接，而不论连接处于何种状态，更不关心任何网络状况的变化*/
//     struct sockaddr_in client;
//     socklen_t client_addrlength = sizeof(client);
//     int connfd;
//     if((connfd = accept(socket_fd, (struct sockaddr*)&client, &client_addrlength)) < 0)
//     {
//         std::cout << "errno is:" << errno << std::endl;
//     }
//     else
//     {
//         char remote[INET_ADDRSTRLEN];
//         std::cout << "connected with ip: " << inet_ntop(AF_INET, &client.sin_addr, remote, INET6_ADDRSTRLEN)
//         << " and port: " << ntohs(client.sin_port) << std::endl;
//         close(connfd);
//     }

//     close(socket_fd);
//     return 0;
// }