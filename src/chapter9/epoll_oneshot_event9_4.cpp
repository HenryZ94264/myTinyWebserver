// // #include "../../inc/include_set.h"
// // #include "../../inc/mySocket.h"
// // #include <sys/epoll.h>
// // #include <pthread.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <assert.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <errno.h>
// #include <string.h>
// #include <fcntl.h>
// #include <stdlib.h>
// #include <sys/epoll.h>
// #include <pthread.h>
// #include <iostream>
// #include <cstring>

// #define MAX_EVENT_NUMBER 1024
// #define BUFFER_SIZE 1024

// // TODO
// // something error with the code epoll failure with [ERROR]:4 error is: Interrupted system call

// struct fds
// {
//     int epollfd;
//     int sockfd;
// };

// int setnonblocking(int fd)
// {
//     int old_option = fcntl(fd, F_GETFL);
//     int new_option = old_option | O_NONBLOCK;
//     fcntl(fd, F_SETFL, new_option);
//     return old_option;
// }

// // 将fd上的EPOLLIN和EPOLLET事件注册到epollfd指示的内核epoll事件表中，参数oneshot指定是否注册fd上的EPOLLONESHOT事件
// void addfd(int epollfd, int fd, bool oneshot)
// {
//     epoll_event event;
//     event.data.fd = fd;
//     event.events = EPOLLIN | EPOLLET;
//     if(oneshot)
//     {
//         event.events |= EPOLLONESHOT;
//     }
//     epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
//     setnonblocking(fd);
//     // int nonblock = fd & O_NONBLOCK;
//     // std::cout << nonblock << std::endl;
// }

// // 重置fd上的事件。这样操作后，尽管fd上的EPOLLONESHOT事件被注册，但是操作系统仍然会触发fd上的EPOLLIN事件，且只触发一次
// void reset_oneshot(int epollfd, int fd)
// {
//     epoll_event event;
//     event.data.fd = fd;
//     event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
//     epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
//     // setnonblocking(fd);
//     // int nonblock = fd & O_NONBLOCK;
//     // std::cout << nonblock << std::endl;
// }

// // 工作线程
// void* worker(void* arg)
// {
//     int sockfd = ((fds*)arg)->sockfd;
//     int epollfd = ((fds*)arg)->epollfd;

//     std::cout << "starting new thread to receive data on fd: " << sockfd << std::endl;
//     char buf[BUFFER_SIZE];
//     std::memset(buf, '\0', BUFFER_SIZE);
//     // 循环读取sockfd上的数据，直到遇到EAGAIN错误
//     while(1)
//     {
//         int ret = recv(sockfd, buf, BUFFER_SIZE-1, 0);
//         if(ret == 0)
//         {
//             close(sockfd);
//             std::cout << "foreigner closed the connection" << std::endl;
//             break;
//         }
//         else if(ret < 0)
//         {
//             if(errno == EAGAIN)
//             {
//                 reset_oneshot(epollfd, sockfd);
//                 std::cout << "read later" << std::endl;
//                 break;
//             }
//         }
//         else
//         {
//             std::cout << "get content " << buf << std::endl;
//             // 模拟线程处理过程
//             sleep(5);
//         }
//     }
//     std::cout << "end thread receiving data on fd: " << sockfd << std::endl;
// }

// int main(int argc, char* argv[])
// {
//     if(argc <= 2)
//     {
//         std::cout << "usage: " << argv[0] << "ip_address port_number" << std::endl;
//         return 1;
//     }

//     const char* ip = argv[1];
//     int port = atoi(argv[2]);

//     // int listenfd = getSocket(ip, port);

//     struct sockaddr_in address;
//     bzero(&address, sizeof(address));
//     address.sin_family = AF_INET;
//     inet_pton(AF_INET, ip, &address.sin_addr);
//     address.sin_port = htons(port);

//     int listenfd = socket(AF_INET, SOCK_STREAM, 0);
//     assert(listenfd != -1);
    
//     int ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
//     assert(ret != -1);

//     ret = listen(listenfd, 5);
//     assert(ret != -1);
    
//     epoll_event events[MAX_EVENT_NUMBER];
//     int epollfd = epoll_create(5);
//     if(epollfd==-1)
//     {
//         std::cout << "epoll create failure" << std::endl;
//         exit(EXIT_FAILURE);
//     }
//     // 监听socket listenfd上是不能注册EPOLLONESHOT事件，否则应用程序只能处理一个客户连接，因为后续的客户连接请求将不再触发listenfd上的EPOLLIN事件
//     addfd(epollfd, listenfd, false);

//     while(1)
//     {
//         int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
//         if(ret < 0)
//         {  
//             std::cout << "epoll failure with [ERROR]:" << errno << " error is: " << strerror(errno) << std::endl;
//             break;
//         }

//         for(int i = 0; i < ret; i++)
//         {
//             int sockfd = events[i].data.fd;
//             if(sockfd == listenfd)
//             {
//                 struct sockaddr_in client_address;
//                 socklen_t client_addrlength = sizeof(client_address);
//                 int connfd  = accept(sockfd, (struct sockaddr*)&client_address, &client_addrlength);
//                 // 对每个非监听文件描述符都注册EPOLLONESHOT事件
//                 addfd(epollfd, connfd, true);
//             }
//             else if(events[i].events & EPOLLIN)
//             {
//                 pthread_t thread;
//                 fds fds_for_new_worker;
//                 fds_for_new_worker.epollfd = epollfd;
//                 fds_for_new_worker.sockfd = sockfd;
//                 pthread_create(&thread, NULL, worker, (void *)&fds_for_new_worker);
//             }
//             else
//             {
//                 std::cout << "something else happened" << std::endl;
//             }
//         }
//     }

//     close(listenfd);
//     return 0;

// }