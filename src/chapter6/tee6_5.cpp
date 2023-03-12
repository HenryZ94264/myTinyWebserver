#include "../../inc/include_set.h"
#include "../../inc/mySocket.h"

// tee函数在两个管道文件描述符之间复制数据(区别于splice的移动数据，这次双方都有副本)

// 这是一个测试splice和tee函数特性的demo
// splice函数能将数据从一个fd**移动**到另一个fd，且至少有一个fd是管道。splice的in端数据是不会保存的
// tee函数能将数据从一个fd**复制**到另一个fd，要求两个fd都是管道。tee的in端数据是继续保留的
// 下面的demo目的是将test_res->tee_in且test_res->STDOUT
// 按顺序执行1.2.3.4.，是没有问题的，因为                fd(test_res)->(空)splice(有)->(有)tee(有)->STDOUT
//                                                                         (空)->(空)splice(有)->teein
// 若1.4.2.3.，再tee则会阻塞，一直等待数据输入。         fd(test_res)->(空)splice(空)->(空)splice(有)->teein
//                若设置为非阻塞，则会报错                                        ->（空）tee... 

int main()
{
    // 将test_res读入，写入到tee_in以及标准输出
    const char* filename("resource/test_res");
    const char* teein("resource/tee_in");
    int filefd = open(filename, O_RDONLY); 
    struct stat res_stat;
    fstat(filefd, &res_stat);
    int filefd_teein = open(teein, O_CREAT | O_WRONLY | O_TRUNC, 0666); // 最后一位表示文件权限为所有用户可读可写

    int pipefd_splice_outfile[2];
    if(pipe(pipefd_splice_outfile) == -1) 
    {LOG("pipefd_splice_outfile failed");
        exit(EXIT_FAILURE);
    }
    int pipefd_stdout[2];
    if(pipe(pipefd_stdout) == -1)
    { LOG("pipefd_stdout failed");
        exit(EXIT_FAILURE);
    }

    // 1. test_res内容读入
    int ret = splice(filefd, NULL, pipefd_splice_outfile[1], NULL, res_stat.st_size, SPLICE_F_MOVE);
    if(ret == -1)
    {   
        LOG("read file failed");
        exit(EXIT_FAILURE);
    }

    // // 4. 将管道splice_outfile的输出输入到tee_in中
    // ret = splice(pipefd_splice_outfile[0], NULL, filefd_teein, NULL, res_stat.st_size, SPLICE_F_MOVE);
    // if(ret == -1) 
    // {   
    //     LOG("write in tee_in failed");
    //     exit(EXIT_FAILURE);
    // }
    
    // 2. 将管道splice outfile的输出输入到管道stdout
    ret = tee(pipefd_splice_outfile[0], pipefd_stdout[1], res_stat.st_size, SPLICE_F_NONBLOCK);   // 若4.在前，设置nonblock，否则会一直阻塞等待输入
    if(ret == -1)
    { 
        std::cout << "[errno]: " << errno << " error is: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // 3. 管道stdout 输出到 stdout
    ret = splice(pipefd_stdout[0], NULL, STDOUT_FILENO, NULL, res_stat.st_size, SPLICE_F_MOVE );
    if(ret == -1) 
    {
        LOG("stdout failed");
        exit(EXIT_FAILURE);

    }
    
    // // 4. 将管道splice_outfile的输出输入到tee_in中
    // ret = splice(pipefd_splice_outfile[0], NULL, filefd_teein, NULL, res_stat.st_size, SPLICE_F_MOVE);
    // if(ret == -1) 
    // {   
    //     LOG("write in tee_in failed");
    //     exit(EXIT_FAILURE);
    // }

    for(int i = 0; i < 2; i++)
    {
        close(pipefd_splice_outfile[i]);
        close(pipefd_stdout[0]);
    }
    close(filefd);
    close(filefd_teein);
    return 0;
}