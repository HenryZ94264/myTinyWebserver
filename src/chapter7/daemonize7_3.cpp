// #include "../../inc/include_set.h"

// bool daemonize()
// {
//     // 创建子进程，关闭父进程，这样可以使程序在后台运行
//     // 在父进程中，fork()函数返回子进程的pid，可以通过打印该值获取子进程的pid。
//     // 在子进程中，fork()函数返回0，可以通过打印该值获取子进程自己的pid。
//     pid_t pid = fork();

//     if (pid < 0)
//     {
//         std::cout << "fork failed" << std::endl;
//         return false;
//     }

//     else if(pid > 0)
//     {
//         std::cout << "In parent process, child pid=" << pid << ", parent pid = " << getpid() << std::endl;
//         std::cout << "parent exit" << std::endl;
//         exit(0);
//     }
    
//     std::cout << "Here is the child process, pid = " << getpid() << std::endl;
//     // 设置文件权限掩码
//     umask(0);

//     // 创建新的会话
//     pid_t sid = setsid();
//     if(sid < 0)
//     {
//         return false;
//     }

//     if((chdir("/")) < 0)
//     {
//         return false;
//     }

//     // 关闭标准输入设备等
//     close(STDIN_FILENO);
//     close(STDOUT_FILENO);
//     close(STDERR_FILENO);

//     // 关闭其他已经打开的文件描述符，代码省略


//     // 将标准输入、输出和标准错误输出定向到/dev/null
//     open("/dev/null", O_RDONLY);
//     open("/dev/null", O_WRONLY);
//     open("/dev/null", O_RDWR);
//     return true;
// }


// int main()
// {
//     daemonize();
// }