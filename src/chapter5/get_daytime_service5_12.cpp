// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <netdb.h>
// #include <iostream>
// #include <cstdlib>
// #include <errno.h>
// #include <assert.h>
// #include <unistd.h>

// 客户端程序，由于我使用的是win10客户端，该程序下很多函数都被定义为不安全，因此先跳过这部分，等到后面有需要再回头看看对应部分。

// int main(int argc, char* argv[])
// {
//     // if(argc != 2) return -1;
//     // char* host = argv[1];
//     char* host = "";
//     // 获取目标主机地址信息
//     struct hostent* hostinfo = gethostbyaddr(host);
//     assert(hostinfo);
//     // 获取daytime服务信息
//     struct servent* servinfo = getservbyname("daytime", "tcp");
//     assert(servinfo);
//     std::cout << "daytime port is " << ntohs(servinfo->s_port) << std::endl;

//     struct sockaddr_in address;
//     address.sin_family = AF_INET;
//     address.sin_port = servinfo->s_port;
//     address.sin_addr = *(struct in_addr*)*hostinfo->h_addr_list;

//     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     int result = connect(sockfd, (struct sockaddr*)&address, sizeof(address));
//     assert(result != -1);

//     char buffer[128];
//     result = read(sockfd, buffer, sizeof(buffer));
//     assert(result > 0);
//     std::cout <<buffer << std::endl;
//     close(sockfd);
//     return 0;

// }