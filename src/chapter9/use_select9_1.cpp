// #include <sys/types.h>
// #include "../../inc/include_set.h"
// #include "../../inc/mySocket.h"

// // select处理带外数据
// // 但是好像接受不到客户端发来的带外数据

// int main(int argc, char* argv[])
// {
//     if(argc <= 2)
//     {
//         std::cout << "usage: " << basename(argv[0]) << "ip_address port_number" << std::endl;
//         return 1;
//     }

//     const char* ip = argv[1];
//     int port = atoi(argv[2]);

//     int socketfd = getSocket(ip, port);
    
//     struct sockaddr_in client_address;
//     socklen_t client_addrlength = sizeof(client_address);
//     int connfd = accept(socketfd, (struct sockaddr*)&client_address, &client_addrlength);
//     if(connfd < 0)
//     {
//         std::cout << "errno is: " << errno << std::endl; 
//         close(connfd);
//     }
//     char buf[1024];
//     fd_set read_fds;
//     fd_set exception_fds;
//     FD_ZERO(&read_fds);
//     FD_ZERO(&exception_fds);
//     int ret = 0;

//     while(1)
//     {
//         std::memset(buf, '\0', sizeof(buf));
//         // 每次调用select前都要重新在read_fds和exception_fds中设置文件描述符connfd，因为事件发生后，文件描述符集合将被内核修改
//         FD_SET( connfd, &read_fds );
//         FD_SET( connfd, &exception_fds );

//         ret = select(connfd + 1, &read_fds, NULL, &exception_fds, NULL);
//         if(ret < 0)
//         {
//             std::cout << "selection failed" << std::endl;
//             break;
//         }
//         if(FD_ISSET(connfd, &read_fds))
//         {
//             ret = recv(connfd, buf, sizeof(buf) - 1, 0);
//             if(ret <= 0)
//             {
//                 break;
//             }
//             std::cout << "get " << ret << " bytes of normal data: " << buf << std::endl;
//         }

//         // 从客户端发送带外数据也好像一直无法接受带外数据（监视异常）
//         else if(FD_ISSET(connfd, &exception_fds))
//         {
//             ret = recv(connfd, buf, sizeof(buf) - 1, MSG_OOB);
//             if(ret <= 0)
//             {
//                 break;
//             }
//             std::cout << "get " << ret << " bytes of oob data: " << buf << std::endl;
//         }
//     }
    
//     close(connfd);
//     close(socketfd);
//     return 0;
// }