// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <netinet/in.h>
// #include <cstring>
// #include <cstdlib>
// #include <iostream>
// #include <errno.h>
// #include <unistd.h>

// #define BUFFER_SIZE 1024

// // 修改接受缓冲区的服务器程序

// int main(int argc, char* argv[])
// {
//     const char* ip = argv[1];
//     int port = atoi(argv[2]);
//     int recvbuf = 50;

//     struct sockaddr_in address;
//     std::memset(&address, 0, sizeof(address));
//     address.sin_family = AF_INET;
//     inet_pton(AF_INET, ip, &address.sin_addr);
//     address.sin_port = htons(port);

//     int sockfd;
//     if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
//     {
//         std::cout << "socket failed" << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     int len = sizeof(recvbuf);
//     setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &recvbuf, len);
//     getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &recvbuf, (socklen_t*)&len);
//     std::cout << "the tcp receive buffer size after setting is " << recvbuf << std::endl;

//     if(bind(sockfd, (struct sockaddr*)&address, sizeof(address)) == -1)
//     {
//         std::cout << "bind failed" << std::endl;
//         std::cout << "errno: " << errno << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     if(listen(sockfd, 5) == -1)
//     {
//         std::cout << "listen failed" << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     struct sockaddr_in client;
//     socklen_t client_addrlength = sizeof(client);

//     int connfd = accept(sockfd, (struct sockaddr*)&address, &client_addrlength);
//     if(connfd < 0)
//     {
//         std::cout << "errno is: " << errno << std::endl;
//     }
//     else
//     {
//         char buffer[BUFFER_SIZE];
//         std::memset(buffer, '\0', BUFFER_SIZE);
//         while(
//         recv(connfd, &buffer, BUFFER_SIZE - 1, 0) > 0 )
//         {
//             std::cout << buffer << std::endl;
//         }
//         close(connfd);

//     }

//     close(sockfd);
//     return 0;

// }