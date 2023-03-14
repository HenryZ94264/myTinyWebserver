// #include <sys/types.h>
// #include <unistd.h>
// #include <iostream>
// #include <cstdio>
// #include <assert.h>


// int main()
// {
//     uid_t uid, euid, suid;
//     getresuid(&uid, &euid, &suid);
    
//     std::cout << "before set" << std::endl;
//     std::cout << "uid: " << uid << ", euid: " << euid << ", suid: " << suid << std::endl;
    
//     // setuid(0);
//     // getresuid(&uid, &euid, &suid);
//     // std::cout << "after set" << std::endl;
//     // std::cout << "uid: " << uid << ", euid: " << euid << ", suid: " << suid << std::endl;

//     assert(euid == 0);  // 保证具有root权限
//     // 此处是使用root在目录下创建的test目录
//     std::cout << remove("/home/test/test1") << std::endl;

//     setuid(getuid());   // 降权，回到普通用户权限，这一步应该需要加锁保证成功？
    
//     getresuid(&uid, &euid, &suid);
//     std::cout << "uid: " << uid << ", euid: " << euid << ", suid: " << suid << std::endl;

//     return 0;
// }