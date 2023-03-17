#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include "../../inc/mySocket.h"

// 对http请求进行解析，并没有进行响应
// 直接从浏览器中输入地址:端口即可

#define BUFFER_SIZE 4096    // 读缓冲区大小

// 主状态机的两种可能状态：正在分析请求行、正在分析头部字段
enum CHECK_STATE {CHECK_STATE_REQUESTLINE=0, CHECK_STATE_HEADER};

// 从状态机的三种可能状态（行的读取状态）:读取到一个完整的行，行出错，行数据尚未完整
enum LINE_STATUS {LINE_OK = 0, LINE_BAD, LINE_OPEN};

// 服务器处理http请求的结果:
// NO_REQUEST:请求不完整
// GET_REQUEST: 获得了一个完整的客户端请求
// BAD_REQUEST: 客户请求有语法错误
// FORBIDDEN_REQUEST: 客户对资源没有访问权限
// INTERNAL_ERROR: 服务器内部错误
// CLOSED_CONNECTION: 客户端已关闭连接
enum HTTP_CODE {NO_REQUEST, GET_REQUEST, BAD_REQUEST, FORBIDDEN_REQUEST, INTERNAL_ERROR, CLOSED_CONNECTION};

static const char* szret[] = {"Correct Request\n", "Something wrong\n"};

// 从状态机解析出一行内容
// checked_index 指向 buffer中正在分析的字节，read_index指向buffer客户数据中的尾部的下一字节
LINE_STATUS parse_line(char* buffer, int& checked_index, int& read_index)
{
    char temp;
    for(; checked_index < read_index; checked_index++)
    {
        // 获取当前要分析的字节
        temp = buffer[checked_index];
        // 如果当前的字节是"\r“，即回车符号，说明可能读到一个完整的行
        if(temp == '\r')
        {
            // 当前字符是buffer中读到的最后一个字符，说明没有读到一个完整的行，返回LINE_OPEN表示需要进一步读取客户数据
            if((checked_index+1) == read_index)
            {
                return LINE_OPEN;
            }
            // 如果下一个字符是'\n'。则成功读取到一个完整的行
            else if(buffer[checked_index + 1] == '\n')
            {
                buffer[checked_index++] = '\0';
                buffer[checked_index++] = '\0';
                return LINE_OK;
            }
            // 否则，客户端的http请求存在语法问题
            return LINE_BAD;
        }

        else if(temp == '\n')
        {
            if((checked_index > 1) && buffer[checked_index - 1] == '\r')
            {
                buffer[checked_index - 1] = '\0';
                buffer[checked_index++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }
    // 所有内容都分析完毕，没有遇到'\r‘，返回LINE_OPEN
    return LINE_OPEN;
}


// 分析请求行
HTTP_CODE parse_requestline(char* temp, CHECK_STATE& checkstate)
{
    char* url = strpbrk(temp, " \t");   // 该函数返回 str1 中第一个匹配字符串 str2 中字符的字符数，如果未找到字符则返回 NULL。

    if( !url )
    {
        return BAD_REQUEST;
    }
    *url++ = '\0';

    char* method = temp;
    if(strcasecmp(method, "GET") == 0)  // strcasecmp用忽略大小写比较字符串
    {
        std::cout << "The request method is GET\n" << std::endl;
    }
    else
    {
        return BAD_REQUEST;
    }

    url += strspn(url, "\t");   // 检索字符串 str1 中第一个不在字符串 str2 中出现的字符下标。
    char* version = strpbrk(url, " \t");
    if(!version)
    {
        return BAD_REQUEST;
    }
    *version++ = '\0';
    version += strspn(version, "\t");
    // 仅支持HTTP/1.1
    if(strcasecmp(version, "HTTP/1.1") != 0)
    {
        return BAD_REQUEST;
    }
    // 检查url是否合法
    if(strncasecmp(url, "http://", 7) == 0)
    {
        url += 7;
        url = strchr(url, '/');
    }

    if(!url || url[0] != '/')
    {
        return BAD_REQUEST;
    }
    std::cout << "The request URL is: " << url << std::endl;
    // 请求行处理完毕，状态转移至头部字段的分析
    checkstate = CHECK_STATE_HEADER;
    return NO_REQUEST;
}

// 分析头部字段
HTTP_CODE parse_headers(char* temp)
{
    // 空行，http请求正确
    if(temp[0] == '\0')
    {
        return GET_REQUEST;
    }
    else if(strncasecmp(temp, "Host:", 5) == 0)
    {
        temp += 5;
        temp += strspn(temp, " \t");
        std::cout << "The request host is: " << temp << std::endl;
    }
    else
    {
        std::cout << "I can't handle this header\n" << std::endl;
    }
    return NO_REQUEST;
}

// 分析HTTP请求的入口函数
HTTP_CODE parse_content(char* buffer, int& checked_index, CHECK_STATE& checkstate, int& read_index, int& start_line)
{
    LINE_STATUS linestatus = LINE_OK;
    HTTP_CODE retcode = NO_REQUEST;

    // 主状态机,从buffer中取出完整的行
    while((linestatus = parse_line(buffer, checked_index, read_index)) == LINE_OK)
    {
        char* temp = buffer + start_line;    // start_line是行在buffer中的起始位置
        start_line = checked_index;     // 记录下一行的起始位置
        // checkstate 记录主状态机当前的状态
        switch (checkstate)
        {
        case CHECK_STATE_REQUESTLINE:   // 分析请求行
        {   
            retcode = parse_requestline(temp, checkstate);
            if(retcode == BAD_REQUEST)
            {
                return BAD_REQUEST;
            }
            break;
        }
        case CHECK_STATE_HEADER:    // 分析头部字段
        {
            retcode = parse_headers(temp);
            if(retcode == BAD_REQUEST)
            {
                return BAD_REQUEST;
            }
            else if(retcode == GET_REQUEST)
            {
                return GET_REQUEST;
            }
            break;
        }
        default:
        {
            return INTERNAL_ERROR;
        }
        }
    }

    // 若没有读到一个完整的行，则还需要继续读取
    if(linestatus == LINE_OPEN)
    {
        return NO_REQUEST;
    }
    else
    {
        return BAD_REQUEST;
    }
}

int main(int argc, char* argv[])
{
    if(argc <= 2)
    {
        std::cout << "usage: " << basename(argv[0]) << "ip_address port_number" << std::endl;
        return 1;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);

    int sockfd = getSocket(ip, port);

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int fd = accept(sockfd, (struct sockaddr*)&client, &client_addrlength);

    if(fd < 0)
    {
        std::cout << "errno is: " << errno << std::endl;
    }
    else
    {
        char buffer[BUFFER_SIZE];
        std::memset(buffer, '\0', BUFFER_SIZE);
        int data_read = 0;
        int read_index = 0;     // 当前已经读取了多少字节的客户数据
        int checked_index = 0;  // 当前已经分析完了多少字节的客户数据
        int start_line = 0;     // 行在buffer中的起始位置
        // 设置主状态机的初始状态
        CHECK_STATE checkstate = CHECK_STATE_REQUESTLINE;
        while (1)   // 循环读取客户数据bingfenxi
        {
            data_read = recv(fd, buffer+read_index, BUFFER_SIZE - read_index, 0);
            if(data_read == -1)
            {
                std::cout << "reading failed" << std::endl;
                break;
            }
            else if(data_read == 0)
            {
                std::cout << "remote client has closed the connection" << std::endl;
                break;
            }
            read_index += data_read;
            // 分析目前已经获得的所有客户数据
            HTTP_CODE result = parse_content(buffer, checked_index, checkstate, read_index, start_line);
            if(result == NO_REQUEST)    // 尚未获得一个完整的http请求
            {
                continue;
            }
            else if(result == GET_REQUEST)  // 得到一个完整正确的http请求
            {
                send(fd, szret[0], strlen(szret[0]), 0);
                break;
            }
            else    // 其他情况表示发生错误
            {
                send(fd, szret[1], strlen(szret[1]), 0);
                break;
            }
        }
        close(fd);
        
    }
    close(sockfd);
    return 0;

}
