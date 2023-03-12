// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <netinet/in.h>
// #include <unistd.h>
// #include <errno.h>
// #include <cstring>
// #include <string>
// #include <cstdlib>
// #include <iostream>
// #include <libgen.h>
// #include <csignal>

// // get socket information


// int main(int argc, char* argv[])
// {

    
//     const char* ip = argv[1];
//     int port = atoi(argv[2]);

//     struct sockaddr_in address;
//     std::memset(&address, 0, sizeof(address));
//     address.sin_family = AF_INET;
//     inet_pton(AF_INET, ip, &address.sin_addr);
//     std::cout << port << std::endl;
//     address.sin_port = htons(port);   

//     int socket_fd;
//     if((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
//     {
//         std::cout << "socket failed" << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     if(bind(socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
//     {
//         std::cout << "bind failed" << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     if(listen(socket_fd, 2) < 0)
//     {
//         std::cout << "listen failed" << std::endl;
//         exit(EXIT_FAILURE);
//     }


//     // 获取local 
//     struct sockaddr_in localAddr;
//     socklen_t len = sizeof(localAddr);
//     if(getsockname(socket_fd, (struct sockaddr*)&localAddr, &len) == -1)
//     {
//         std::cout << "getsockname error" << std::endl;
//         exit(1);
//     }
//     char local_ip[INET_ADDRSTRLEN];
//     inet_ntop(AF_INET, &localAddr.sin_addr, local_ip, sizeof(local_ip));
//     int local_port = ntohs(localAddr.sin_port);
//     std::cout << "local address: " << local_ip << ":" << local_port << std::endl;


//     sleep(20);  // 等待remote telnet
//     int connfd;
//     socklen_t addrLength = sizeof(address);
//     if((connfd = accept(socket_fd, (struct sockaddr*)&address, &addrLength)) < 0)
//     {
//         "connection failed\n";
//     }
//     else
//     {
//         std::cout << "accepted!" << std::endl;
//         // 获取remote
//         struct sockaddr_in remoteAddr;
//         socklen_t len1 = sizeof(remoteAddr);
//         if(getpeername(connfd, (struct sockaddr*)&remoteAddr, &len1) == -1)
//         {
//             std::cout << "getpeername error" << std::endl;
//             exit(1);
//         }
//         char remote_ip[INET_ADDRSTRLEN];
//         inet_ntop(AF_INET, &remoteAddr.sin_addr, remote_ip, sizeof(remote_ip));
//         int remote_port = ntohs(remoteAddr.sin_port);
//         std::cout << "remote address: " << remote_ip << ":" << remote_port << std::endl;
//         close(connfd);
//     }


//     close(socket_fd);

//     return 0;
// }   