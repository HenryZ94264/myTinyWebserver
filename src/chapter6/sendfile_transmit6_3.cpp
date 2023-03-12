// #include "../../inc/mySocket.h"
// #include <cstdlib>
// #include <cstring>
// #include <cstdio>
// #include <errno.h>
// #include <iostream>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <sys/sendfile.h>
// #include <fcntl.h>
// #include <unistd.h>

// // 使用sendfile直接将文件内容传给客户端，效率高，不用切换成用户态

// int main(int argc, char* argv[])
// {
//     if(argc <= 2)
//     {
//         std::cout << "Usage: " << basename(argv[0]) << " ip_address port_number" << std::endl;
//         return 1;
//     }

//     const char* ip = argv[1];
//     int port = atoi(argv[2]);
//     const char* filename("resource/test_cannot_read");

//     int sockfd = getSocket(ip, port);
//     int filefd = open(filename, O_RDONLY);
//     struct stat stat_buf;
//     fstat(filefd, &stat_buf);   // 获取该文件描述符的相关信息

//     struct sockaddr_in client;
//     socklen_t client_addrlength = sizeof(client);
//     int connfd = accept(sockfd, (struct sockaddr*)&client, &client_addrlength);
//     if(connfd < 0)
//     {
//         LOG("accept error");
//     }
//     else
//     {
//         sendfile(connfd, filefd, NULL, stat_buf.st_size);
//         LOG("SEND");
//         close(connfd);
//     }

//     close(sockfd);
//     return 0;
// }