// #include <sys/types.h>
// #include <unistd.h>
// #include <iostream>

// // 该函数为将一个root启动的进程切换为一个普通用户运行的进程
// // 输入参数是目标用户，和目标组
// static bool switch_to_user(uid_t user_id, gid_t gp_id)
// {
//     // 如果目标函用户是root，则失败。假如一个目标用户想切换成root，会返回错误
//     if((user_id == 0) && (gp_id == 0))
//     {
//         return false;
//     }

//     gid_t gid = getgid();
//     uid_t uid = getuid();
//     // 确保当前用户合法：即root用户或目标用户
//     if( ((gid!=0) || (uid!=0)) && ((gid!=gp_id) || (uid != user_id)) )
//     {
//         return false;
//     }

//     if(uid != 0)
//     {
//         return true;
//     }

//     if( (setgid(gp_id) < 0 ) || (setuid(user_id) < 0))
//     {
//         return false;
//     }

//     return true;
// }

// // 在这里输出当前进程id
// // 然后通过调用ps -o user,euser,pid,cmd -p pid
// // 可以看到user被改成了zh
// int main()
// {
//     // bool swt = switch_to_user(1001, 1001);
//     // std::cout << swt << std::endl;

//     std::cout << getpid() << std::endl;


//     // 这一部分对比了seteuid和setuid的功能，setuid可以同时设置uid和euid，seteuid只能设置euid
//     // 如果当前进程为root进程，可以将uid和euid设置成其他
//     // 如果当前进程为普通用户进程，只有进程的uid和euid为0时，才能将uid和euid改为其他
//     std::cout << "current user id: " << getuid() << " current effective user id: " << geteuid() << std::endl;
//     // bool swt = switch_to_user(1000, 1000);
//     // std::cout << swt << std::endl;
//     std::cout << seteuid(1000) << std::endl;
//     std::cout << "after set euid, current user id: " << getuid() << " current effective user id: " << geteuid() << std::endl;
//     std::cout << setuid(0) << std::endl;
//     std::cout << "after set uid, current user id: " << getuid() << " current effective user id: " << geteuid() << std::endl;
//     std::cout << setuid(1001) << std::endl;
//     std::cout << "after set uid, current user id: " << getuid() << " current effective user id: " << geteuid() << std::endl;

//     // std::cout << setuid(1000) << std::endl;


//     std::cin.get();
//     return 0;
// }