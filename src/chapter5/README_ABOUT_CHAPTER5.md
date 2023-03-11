chapter 5 中，主要讲述socket的大量接口，对于一些细节（踩过的坑），这里需要说明一下。

## 文件结构:
- byteorder5_1.cpp，判断主机字节序

- testlisten5_3.cpp, 使用socket进行监听的demo程序，流程包括
    1. 接受main传进来的ip, port以及backlog参数。backlog规定了内核监听队列的最大长度。这里的ip需要设置成**网卡ip**，不然无法监听到公网的telnet请求
    2. `socket()`函数创建socket
    3. 建立IPV4地址，使用`bind()`函数绑定socket和该ip
    4. `listen()`函数开始监听
    5. 设置一个循环使程序持续进行

- testaccept5_5.cpp, 使用`accept()`函数。`accept()`只是从监听队列中取出连接，而不关心其状态(无论是ESTABLISHED还是CLOSE_WAIT)
    1. 前四个流程与testlisten5_3.cpp一致
    2. `accept()`接受客户端请求，并建立连接，accept会返回一个**新的fd**,用于服务器与客户端之间的通信(该通信的唯一标识符？)

- oobsend5_6.cpp和oobrecv5_7.cpp，模拟客户端与服务器之间的TCP带外数据读写：
    1. 客户端：只需要实现`socket()`函数，然后通过`connect()`连接到服务器端。连接成功后通过`send()`函数向服务器发送数据
    2. 服务器端：`socket()`->`bind()`->`listen()`->等待客户端connect->`accept()`，**最后使用`accept()`产生的fd来`recv()`**
    3. ！！！这里请特别注意`connect()`与`accept()`的区别，connect是客户端主动连接服务器的函数，而accept是服务器接受客户端的请求。accept会产生新的fd，而connect还是使用原来与服务器连接的fd。
    4. `connect()`和`accept()`与tcp三次握手建立连接有非常紧密的联系。客户端调用`connect()`时，向服务器发送 SYN 包(first)，服务器调用`accept()`后会在等待连接时阻塞，当客户端发送 SYN 包时，服务器收到后会回复一个 SYN+ACK 包(second)。客户端收到后会再回复一个 ACK 包，此时服务器会接触阻塞状态，accept返回一个与客户端相连的 socket fd，从而建立连接。

- try_get_socket.cpp, 我想尝试使用`getsockname()`和`getpeername()`来获取本端socket地址以及远端socket地址写的demo，发现了一些细节，包括上面提到的`connect()`与`accept()`的注意事项。`getpeername()`必须accept后才有效。感觉这个函数没什么用，毕竟accept就能跟获取到远端socket地址了。

## 常用命令
`netstat -nt | grep 端口号`