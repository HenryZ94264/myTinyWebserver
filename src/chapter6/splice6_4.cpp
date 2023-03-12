// #include "../../inc/mySocket.h"
// #include "../../inc/include_set.h"

// // 将客户端telnet输出回射给客户端，客户端必须很快地输入才能看到效果。如何保持？
// // splice 是系统调用中与管道相关的函数，在两个fd之间移动数据

// int main(int argc, char* argv[])
// {
//     if(argc <= 2)
//     {
//         std::cout << "Usage: " << basename(argv[0]) << " ip_address port_number" << std::endl;
//         return 1;
//     }

//     const char* ip = argv[1];
//     int port = atoi(argv[2]);

//     int sockfd = getSocket(ip, port, true);
    

//     struct sockaddr_in client;
//     socklen_t client_addrlength = sizeof(client);

//     int connfd = accept(sockfd, (struct sockaddr*)&client, &client_addrlength);
//     if(connfd < 0)
//     {
//         LOG("accept failed");
//     }
//     else
//     {
//         int pipefd[2]; // pipefd[1]是写端，pipefd[0]是读端
//         if(pipe(pipefd) == -1)
//         { 
//             LOG("pipe failed");
//             exit(EXIT_FAILURE);
//         }
//         int ret = splice(connfd, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE);
//         if(ret == -1) 
//         {       LOG("splice read failed");
//             exit(EXIT_FAILURE);

//         }

//         ret = splice(pipefd[0], NULL, connfd, NULL, 32768, SPLICE_F_MORE);
//         if(ret == -1) 
//         {
//             LOG("splice write failed");
//             exit(EXIT_FAILURE);

//         }
//         close(connfd);
//         close(pipefd[0]);
//         close(pipefd[1]);
//     }
//     close(sockfd);
//     return 0;
// }