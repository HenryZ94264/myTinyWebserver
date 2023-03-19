// #include "../../inc/include_set.h"
// #include <sys/epoll.h>
// #include <pthread.h>
// #include <assert.h>

// #define MAX_EVENT_NUMBER 1024
// #define BUFFER_SIZE 10

// // 将文件描述符设置为非阻塞的
// int setnonblocking(int fd)
// {
//     int old_option = fcntl(fd, F_GETFL);
//     int new_option = old_option | O_NONBLOCK;
//     fcntl(fd, F_SETFL, new_option);
//     return old_option;
// }

// // 将文件描述符fd上的 EPOLLIN 注册到 epollfd 指示的 epoll 内核事件表中，参数enable_et指定是否对fd启用ET模式
// void addfd(int epollfd, int fd, bool enable_et)
// {
//     epoll_event event;
//     event.data.fd = fd;
//     event.events = EPOLLIN;
//     if(enable_et)
//     {
//         event.events |= EPOLLET;
//     }
//     epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
//     setnonblocking(fd);
// }

// void lt(epoll_event* events, int number, int epollfd, int listenfd)
// {
//     char buf[BUFFER_SIZE];
//     for(int i = 0; i < number; i++)
//     {
//         int sockfd = events[i].data.fd;
//         if(sockfd == listenfd)
//         {
//             struct sockaddr_in client_address;
//             socklen_t client_addrlength = sizeof(client_address);
//             int connfd = accept(sockfd, (struct sockaddr*)&client_address, &client_addrlength);
//             addfd(epollfd, connfd, false);  // 对connfd禁用ET模式
//         }
//         else if(events[i].events & EPOLLIN)
//         {       
//         // 只要socket读缓存中还有未读出的数据，这段代码就触发
//         std::cout << "event trigger once" << std::endl;
//         std::memset(buf, '\0', BUFFER_SIZE);
//         int ret = recv(sockfd, buf, BUFFER_SIZE-1, 0);
//         if(ret <= 0)
//         {
//             close(sockfd);
//             continue;;
//         }
//         std::cout << "Get " << ret << " bytes of content: " << buf << std::endl;
//         }
//         else
//         {
//             std::cout << "something else happened (except read and connect)" << std::endl;
//         }
//     }
    
// }

// void et(epoll_event* events, int number, int epollfd, int listenfd)
// {
//     char buf[BUFFER_SIZE];
//     for(int i = 0; i < number; i++)
//     {
//         int sockfd = events[i].data.fd;
//         if(sockfd == listenfd)
//         {
//             struct sockaddr_in client_address;
//             socklen_t client_addrlength = sizeof(client_address);
//             int connfd = accept(sockfd, (struct sockaddr*)&client_address, &client_addrlength);
//             addfd(epollfd, connfd, true);
//         }
//         else if(events[i].events & EPOLLIN)
//         {
//             // 这段代码不会被重复触发，因此循环读取数据，确保把socket读缓存中的所有数据读出
//             std::cout << "event trigger once" << std::endl;
//             while(1)
//             {
//                 std::memset(buf, '\0', BUFFER_SIZE);
//                 int ret = recv(sockfd, buf, BUFFER_SIZE-1, 0);
//                 if(ret < 0)
//                 {
//                     // 对于非阻塞IO，以下条件成立表示数据已经全部读取完毕。此后，epoll就能再次触发sockfd上的EPOLLIN事件，以驱动下一次操作
//                     if((errno == EAGAIN) || (errno == EWOULDBLOCK))
//                     {
//                         std::cout << "read later" << std::endl;
//                         break;
//                     }
//                     close(sockfd);
//                     break;
//                 }
//                 else if(ret == 0)
//                 {
//                     close(sockfd);
//                 }
//                 else
//                 {
//                     std::cout << "Get " << ret << " bytes of content: " << buf << std::endl;
//                 }
//             }
//         }
//         else
//         {
//             std::cout << "something else happened " << std::endl;
//         }
//     }
// }

// int main(int argc, char* argv[])
// {
//     if(argc <= 2) 
//     {
//         std::cout << "usage: " << basename(argv[0]) << " ip_address port_number" << std::endl;
//         return 1;
//     }

//     const char* ip = argv[1];
//     int port = atoi(argv[2]);

//     struct sockaddr_in address;
//     std::memset(&address, 0, sizeof(address));
//     address.sin_family = AF_INET;
//     inet_pton(AF_INET, ip, &address.sin_addr);
//     address.sin_port = htons(port);

//     int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
//     assert(listen_fd >= 0);

//     int ret = bind(listen_fd, (struct sockaddr*)&address, sizeof(address));
//     assert(ret != -1);

//     ret = listen(listen_fd, 5);
//     assert(ret != -1);

//     epoll_event events[MAX_EVENT_NUMBER];
//     int epollfd = epoll_create(5);
//     assert(epollfd!=-1);
//     addfd(epollfd, listen_fd, true);

//     while(1)
//     {
//         ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
//         if(ret < 0)
//         {
//             std::cout << "epoll failure" << std::endl;
//             break;
//         }
//         lt(events, ret, epollfd, listen_fd);    // 会显示多次trigger once
//         // et(events, ret, epollfd, listen_fd);     // 每次只会显示一次trigger once
//     }
//     close(listen_fd);
//     return 0;

// }