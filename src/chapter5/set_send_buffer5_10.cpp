// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <netinet/in.h>
// #include <cstring>
// #include <cstdlib>
// #include <iostream>
// #include <unistd.h>

// #define BUFFER_SIZE 512

// // 修改发送缓冲区的客户端程序

// int main()
// {
//     const char* ip = "172.28.241.110";
//     int port = 12345;
//     int sendbuf = 2000;

//     struct sockaddr_in server_address;
//     std::memset(&server_address, 0, sizeof(server_address));
//     server_address.sin_family = AF_INET;
//     inet_pton(AF_INET, ip, &server_address.sin_addr);
//     server_address.sin_port = htons(port);

//     int sockfd;
//     if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
//     {
//         std::cout << "socket failed" << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     int len = sizeof(sendbuf);
//     setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sendbuf, len);
//     getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sendbuf, (socklen_t*)&len);
//     std::cout << "the tcp send buffer size after setting is " << sendbuf << std::endl;

//     if(connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) != -1)
//     {
//         char buffer[BUFFER_SIZE];
//         std::memset(buffer, 'a', BUFFER_SIZE);
//         send(sockfd, buffer, BUFFER_SIZE, 0);
//     }

//     close(sockfd);
//     return 0;

// }