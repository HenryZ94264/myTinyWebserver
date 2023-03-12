// #include "../../inc/mySocket.h"
// #include <netinet/tcp.h>
// #include <iostream>
// #include <cstdlib>
// #include <cstring>
// #include <cstdio>
// #include <errno.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <sys/uio.h>    // iovec

// // 客户端telnet，服务器将固定文件中的内容发送给客户端

// #define BUFFER_SIZE 1024
// // 两种http状态码和状态信息
// static const char* status_line[2] = { "200 OK", "500 Internal server error"};

// int main(int argc, char* argv[])
// {
//     if(argc <= 2)
//     {
//         std::cout << "usage: " << basename(argv[0]) << " ip_address port_number" << std::endl;
//         return 1;
//     }

//     const char* ip = argv[1];
//     int port = atoi(argv[2]);

//     const char* file_name = "resource/test_cannot_read";    // TODO 修改资源文件

//     int sockfd = getSocket(ip, port);

//     struct sockaddr_in client;
//     socklen_t client_addrlength = sizeof(client);
//     int connfd = accept(sockfd, (struct sockaddr*)&client, &client_addrlength);
//     if(connfd < 0)
//     {
//         std::cout << "[errno]: " << errno << " error: " << strerror(errno) << std::endl;
//     }
//     else
//     {
//         // HTTP 应答的状态行
//         char header_buf[BUFFER_SIZE];
//         std::memset(header_buf, '\0', BUFFER_SIZE);

//         // 用于存放目标文件内容的应用程序缓存
//         char* file_buf;
        
//         // 用于获取目标文件的属性，是否是目录，文件大小等
//         struct stat file_stat;
//         // 记录文件是否有效
//         bool valid{true};
//         // 缓存区header_buf 目前已经使用了多少字节的空间
//         int len = 0;

//         if(stat(file_name, &file_stat)<0) // 目标文件不存在       stat系统调用，获取文件的元信息
//         {
//             valid = false;
//         }
//         else
//         {
//             if(S_ISDIR(file_stat.st_mode))  // 目标文件是个目录
//             {
//                 valid = false;
//             }
//             else if(file_stat.st_mode & S_IROTH)    // 当前用户有读取目标文件的权限
//             {
//                 // 动态分配缓存区 file_buf, 大小为目标文件大小+1，然后将目标文件读入file_buf中
//                 int fd = open(file_name, O_RDONLY);
//                 file_buf = new char[ file_stat.st_size + 1];
//                 std::memset(file_buf, '\0', file_stat.st_size + 1);
//                 if(read(fd, file_buf, file_stat.st_size) < 0)
//                 {
//                     valid = false;  // 读取错误
//                 }
//             }
//             else
//             {
//                 valid = false; // 用户没有权限读取
//             }
//         }
//         if(valid)   // 文件有效，且读取到file_buf中，发送HTTP应答
//         {
//             int ret = std::snprintf(header_buf, BUFFER_SIZE - 1, "%s %s\r\n", "HTTP/1.1", status_line[0]);
//             len += ret;
//             ret = std::snprintf(header_buf + len, BUFFER_SIZE - len - 1, "Content-Length: %d\r\n", file_stat.st_size);
//             len += ret;
//             ret = std::snprintf(header_buf + len, BUFFER_SIZE- len - 1, "%s", "\r\n");
//             // 用writerv将header buf 和file buf集中写
//             // sizeof 返回字节数，strlen返回字符数
//             struct iovec iv[2];
//             iv[0].iov_base = header_buf;
//             iv[0].iov_len = strlen(header_buf); // 将最后的'\0'去掉了
//             iv[1].iov_base = file_buf;
//             iv[1].iov_len = file_stat.st_size;

//             ret = writev(connfd, iv, 2);

//             delete[] file_buf;  // 删除file buf的操作应该写在这里，
//                                 // 因为如果valid=false, file_buf压根没有被使用new []分配，所以也不能使用delete []来删除，不然会出现段错误

//         }
//         else    // 目标文件无效
//         {  
//             int ret = std::snprintf(header_buf, BUFFER_SIZE - 1, "%s %s \r\n", "HTTP/1.1", status_line[1]);
//             len += ret;
//             ret = std::snprintf(header_buf + len, BUFFER_SIZE - len - 1, "%s" , "\r\n");
//             send(connfd, header_buf, strlen(header_buf), 0);
//         }
//         close(connfd);
//         // delete[] file_buf;
//     }


//     close(sockfd);
//     return 0;

    
// }