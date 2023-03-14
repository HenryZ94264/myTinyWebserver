# Linux 服务器程序规范

## 1. UID, EUID和SUID
UID: 当前系统登录的用户id

EUID: 执行当前进程的用户id（effective UID）

SUID: 保存的设置用户ID（saved set-user-ID），EUID的一个副本

在运行一个进程时，该进程的有效用户ID在一般情况下是实际用户的ID，但是如果该可执行文件具有SUID的权限，那么他的有效用户ID就是这个可执行程序的拥有者。

使用一个以下的例子来简单验证一下

myTest.cpp 文件中

```
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>


int main()
{
    uid_t uid, euid, suid;
    getresuid(&uid, &euid, &suid);
    
    std::cout << "before set" << std::endl;
    std::cout << "uid: " << uid << ", euid: " << euid << ", suid: " << suid << std::endl;
    
    // 尝试将uid设置为root
    setuid(0);
    getresuid(&uid, &euid, &suid);
    std::cout << "after set" << std::endl;
    std::cout << "uid: " << uid << ", euid: " << euid << ", suid: " << suid << std::endl;


    // 此处是使用root在目录下创建的test目录
    std::cout << remove("/home/test/") << std::endl;

    setuid(getuid());
    
    getresuid(&uid, &euid, &suid);
    std::cout << "uid: " << uid << ", euid: " << euid << ", suid: " << suid << std::endl;

    return 0;
}
```

- 测试1. 
  
    root将myTinyWebserver移动到普通用户zh的文件夹下，使用命令`chmod 755 myTinyWebserver`将生成的myTinyWebserver可执行文件的权限改为

    -rwxr-xr-x 1 root root 50872 Mar 14 11:09 myTinyWebserver

    同时，`/home/test`文件夹和`/home/test/test1`文件的权限改为711

    drwx--x--x 2 root   root   4096 Mar 14 11:29 test

    -rw-r--r-- 1 root root 0 Mar 14 11:55 test1
    
    用户zh执行`./myTinyWebserver`

    输出值为:

    ```
    before set
    uid: 1001, euid: 1001, suid: 1001
    after set
    uid: 1001, euid: 1001, suid: 1001
    -1
    uid: 1001, euid: 1001, suid: 1001
    ```
    用户zh尝试将euid改为0，尝试删除root用户创建的test1文件夹，都失败了。

- 测试2. 
  
    root将myTinyWebserver的权限改为具有root的SUID权限，使用以下命令`sudo chmod +s myTinyWebserver`。 此时的状态

    -rwsr-sr-x 1 root root 50872 Mar 14 11:54 myTinyWebserver

    用户zh执行`./myTinyWebserver`，输出为：

    ```
    before set
    uid: 1001, euid: 0, suid: 0
    after set
    uid: 0, euid: 0, suid: 0
    0
    uid: 0, euid: 0, suid: 0
    ```

    可以看到，setuid一改，当前由用户zh执行的进程具有root用户的权限，可以删除test1。为了安全起见，当某个用户进程需要root权限时，应该通过seteuid()，将euid修改，执行完操作以后，再修改回原来的euid，才能保证安全性。应该改为以下代码：

```
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <assert.h>


int main()
{
    uid_t uid, euid, suid;
    getresuid(&uid, &euid, &suid);
    
    std::cout << "before set" << std::endl;
    std::cout << "uid: " << uid << ", euid: " << euid << ", suid: " << suid << std::endl;
    
    // setuid(0);
    // getresuid(&uid, &euid, &suid);
    // std::cout << "after set" << std::endl;
    // std::cout << "uid: " << uid << ", euid: " << euid << ", suid: " << suid << std::endl;

    assert(euid == 0);  // 保证具有root权限
    // 此处是使用root在目录下创建的test目录
    std::cout << remove("/home/test/test1") << std::endl;

    setuid(getuid());   // 降权，回到普通用户权限，这一步应该需要加锁保证成功？
    
    getresuid(&uid, &euid, &suid);
    std::cout << "uid: " << uid << ", euid: " << euid << ", suid: " << suid << std::endl;

    return 0;
}
```

再次使用普通用户zh执行`./myTinyWebserver`输出如下:
```
before set
uid: 1001, euid: 0, suid: 0
0
uid: 1001, euid: 1001, suid: 1001
```
此时由于myTinyWebserver具有root的SUID权限，zh执行以后，euid和suid都是root，因此可以成功删除test1。然后通过setuid(getuid())，将进程降权，后续代码则无法使用root的特权。除了root以外的其他用户都无法修改代码，因此是安全的。