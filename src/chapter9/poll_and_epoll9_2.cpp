// #include <sys/epoll.h>
// #include <sys/poll.h>

// int ret = poll(fds, MAX_EVENT_NUMBER, -1);
// // 遍历所有已注册文件描述符并找到其中的就绪者
// for(int i = 0; i < MAX_EVENT_NUMBER; i++)
// {
//     if(fds[i].revents & POLLIN)
//     {
//         int sockfd = fds[i].fd;
//         // 处理sockfd
//     }
// }


// int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
// for(int i = 0; i < ret; i++)
// {
//     int sockfd = events[i].data.fd;
//     // sockfd 肯定就绪，直接处理
// }