// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <iostream>
// #include <unistd.h>
// #include <string>
// #include <errno.h>
// #include <libgen.h>
// #include <cstring>
// #include <cstdlib>
// #include "../../inc/mySocket.h"

// 将控制台的输出重定向到客户端的输出

// int main(int argc, char* argv[])
// {
//     if(argc <= 2)
//     {
//         std::cout << "usage: " << basename(argv[0]) << "ip_address port_number" << std::endl;
//     }
//         const char* ip = argv[1];
//         int port = atoi(argv[2]);

//         int sockfd = getSocket(ip, port);
        
//         if(sockfd == SOCKET)
//         {
//             std::cout << "socket fialed" << std::endl;
//             exit(EXIT_FAILURE);
//         }
//         else if(sockfd == BIND)
//         {
//             std::cout << "bind fialed" << std::endl;
//             exit(EXIT_FAILURE);
//         }
//         else if(sockfd == LISTEN)
//         {
//             std::cout << "listen fialed" << std::endl;
//             exit(EXIT_FAILURE);
//         }
//         else
//         {
//         }

//         struct sockaddr_in client;
//         socklen_t client_addrlength = sizeof(client);
//         int connfd = accept(sockfd, (struct sockaddr*)&client, &client_addrlength);
//         if(connfd < 0)
//         {
//             std::cout << "[errno]: " << errno << " error: " << strerror(errno) <<std::endl;
//         }
//         else
//         {
//             // 将输出重定向到connfd
//             dup2(connfd, STDOUT_FILENO);     // dup2可以指定？
//             std::cout << "abcd" << std::endl;            
//             close(connfd);

//             // close(STDOUT_FILENO);
//             // dup(connfd);    // dup总是返回系统最小的可用文件描述符
//             // std::cout << "abcd" << std::endl;
//             // close(connfd);
//         }
//         close(sockfd);
//         return 0;
// }



