// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <csignal>
// #include <cstdlib>
// #include <libgen.h>
// #include <unistd.h>
// #include <assert.h>
// #include <iostream>
// #include <cstring>
// #include <stdio.h>

// static bool stop = false;

// // socket 监听程序，远端socket可以通过telnet来连接服务器上端口，服务器上使用 netstat -nt | grep 端口号 来获取连接信息

// // SIGTERM 信号的处理函数，触发时结束主程序中的循环
// static void handle_term(int sig)
// {
//     stop = true;
// }

// int main(int argc, char* argv[])
// {
//     signal(SIGTERM, handle_term);

//     if (argc <= 3)
//     {
//         std::cout << "usage: " << basename(argv[0]) << " ip_address port_number backlog\n";
//         return 1;
//     }

//     const char* ip = argv[1];
//     int port = std::atoi(argv[2]);
//     int backlog = std::atoi(argv[3]);   // 完整连接为backlog+1
//     int socket_fd;

//     // 创建socket， IPv4, 流服务，默认协议
//     if((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0 )
//     {
//         perror("socket failed");
//         exit(EXIT_FAILURE);
//     }

//     // 创建一个IPv4 socket地址
//     struct sockaddr_in address;
//     std::memset(&address, 0, sizeof(address));   // 尝试用memset代替bzero
//     address.sin_family = AF_INET;
//     inet_pton(AF_INET, ip, &address.sin_addr);  // 字符串转二进制
//     address.sin_port = htons(port);     // 将主机字节序转换为网络字节序

//     // 命名socket，将address 分配给 socket_fd，最后一个参数指出socket地址的长度。将socket与ip地址和端口号绑定
//     if(bind(socket_fd, (struct sockaddr*) &address, sizeof(address)) < 0)
//     {
//         perror("bind failed");
//         exit(EXIT_FAILURE);
//     }

//     // 将socket_fd设置为监听状态
//     if(listen(socket_fd, backlog) < 0)
//     {
//         perror("listen");
//         exit(EXIT_FAILURE);
//     }

//     // 循环等待连接，直到由sigterm信号将其中断
//     while( !stop )
//     {
//         sleep(1);
//     }

//     close(socket_fd);
//     return 0;

// }
