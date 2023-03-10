// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <assert.h>
// // #include <stdio.h>
// #include <unistd.h>
// #include <cstring>
// #include <string>
// #include <iostream>
// #include <libgen.h>
// #include <cstdlib>


// // 发送带外数据 紧急数据，该代码在客户端上实现

// int main(int argc, char* argv[])
// {

//     if (argc <= 2)
//     {
//         std::cout << "usage: " << basename(argv[0]) << "ip_address port_number" << std::endl;
//         return 1;
//     }

//     const char* ip = argv[1];
//     int port = atoi(argv[2]);

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

//     if(connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
//     {
//         std::cout << "connection failed" << std::endl;
//     }
//     else
//     {
//         std::string oob_data = "emergence!";
//         std::string normal_data = "123abc";
//         send(sockfd, normal_data.c_str(), sizeof(normal_data), 0);
//         send(sockfd, oob_data.c_str(), sizeof(oob_data), MSG_OOB);
//         send(sockfd, normal_data.c_str(), sizeof(normal_data), 0);
//     }

//     close(sockfd);

//     return 0;

// }