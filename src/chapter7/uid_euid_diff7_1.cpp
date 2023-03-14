// #include <unistd.h>
// #include <iostream>

// // 测试uid和euid的区别
// // 编译文件后，将生成的可执行文件(myTinyWebserver)的所有者设置为root，并设置该文件的set-user-id标志，然后运行该程序查看UID和EUID
// // 输入sudo chown root:root myTinyWebserver  //将文件所有者设置为root
// //    sudo chmod +s myTinyWebserver      // chmod +s 使得其他用户在执行时，也具有所有者（root）的权限。
// // 然后切换到其他用户，我这里是zh
// // ./myTinyWebserver 执行以后，能够得到输出如 user id is: 1001, effective user id is:0
// // 感觉这种做法很危险，因为如果这个文件中写了setuid(0)，而其他恶意用户也具有root权限，那么恶意用户就获得了uid为root的权限？
// // 通过设置该文件的可写权限来限制其他用户无法修改，因此应该是安全的

// int main()
// {
//     uid_t uid = getuid();
//     uid_t euid = geteuid();

//     std::cout << "user id is: " << uid << ", effective user id is:" << euid << std::endl;
//     return 0;
// }
