# 视频分析：P66《4_8-3 UDP 编程示例》

这节视频约 **16 分 18 秒**。它和上一节 TCP 是非常好的对照：老师基本是在把 TCP 程序“删掉连接相关步骤”，然后改成 UDP 的 `sendto()/recvfrom()`。

这节最核心的一句话是：

> <span style="color:#FF0000; background:#00FF80;">**UDP 不需要先建立连接。发送一个数据报时直接告诉内核“这次数据发给哪个 IP:Port”；接收一个数据报时，内核同时告诉你“这包数据是谁发来的”。**</span>

------

## 1. 视频类型判断

**主要类型：** 教程类、编程实战类
**次要类型：** 网络编程讲解类、TCP/UDP 对比类

判断依据：

1. 从上一节 TCP Server/Client 代码修改出 UDP 版本。
2. 使用 `SOCK_DGRAM` 创建 UDP socket。
3. Server 使用 `bind()` + `recvfrom()`。
4. Client 主要使用 `sendto()`。
5. 后面实际编译运行验证 UDP 通信。
6. 最后用板书总结 UDP 中 `connect()` **可用，也可以不用**。

------

## 2. 一句话总结

TCP 是：

```text
先建立关系
       ↓
再通信

Client:
socket
↓
connect
↓
send/recv

Server:
socket
↓
bind
↓
listen
↓
accept
↓
send/recv
```

而 UDP 更像：

```text
不用先建立连接

Server:
socket
↓
bind
↓
recvfrom / sendto


Client:
socket
↓
sendto / recvfrom
```

所以 UDP 最核心的变化就是：

```text
TCP：

“你先告诉内核，我以后要跟这个人通信。”
              ↓
            connect()


UDP：

“这一包我要发给这个人。”
              ↓
sendto(..., 对方IP+Port)
```

------

# 3. 时间段拆解

| 时间段      | 内容概括                     | 画面/代码重点                                  | 作用                            |
| ----------- | ---------------------------- | ---------------------------------------------- | ------------------------------- |
| 00:00-01:10 | 从资料和上一节 TCP 引出 UDP  | UDP 编程资料、板书                             | 建立 TCP/UDP 对照               |
| 01:10-03:20 | 画 UDP Server/Client 流程    | `socket()`、`bind()`、`recvfrom()`、`sendto()` | 先搭整体模型                    |
| 03:20-05:40 | 开始修改 Server 程序         | 原 TCP Server 代码修改                         | 从 TCP 迁移到 UDP               |
| 05:40-07:40 | 创建 UDP socket 和服务器地址 | `SOCK_DGRAM`、`AF_INET`、`INADDR_ANY`、8888    | 创建并绑定 UDP Server           |
| 07:40-09:50 | Server 使用 `recvfrom()`     | 接收数据并取得 Client 地址                     | UDP 接收核心                    |
| 09:50-11:20 | 编译运行 Server/Client       | 终端中出现客户端消息                           | 验证 UDP 通信                   |
| 11:20-12:40 | 查看 `sendto()` 手册         | `man sendto`，重点看 `dest_addr`               | 解释为什么 UDP 发送要带目标地址 |
| 12:40-14:10 | 修改 Client                  | `fgets()` + `sendto()`                         | 实现 UDP Client                 |
| 14:10-15:30 | 再次编译和本机测试           | `127.0.0.1` 等                                 | 验证程序                        |
| 15:30-16:18 | 板书总结 UDP 与 TCP          | `connect()` 标注“可用可不用”                   | 强化 TCP/UDP 区别               |

------

# 4. 第一处最重要变化：`SOCK_STREAM` → `SOCK_DGRAM`

上一节 TCP：

```c
iSocketServer = socket(AF_INET, SOCK_STREAM, 0);
```

这节 UDP：

```c
iSocketServer = socket(AF_INET, SOCK_DGRAM, 0);
```

其中：

```text
AF_INET
↓
IPv4
```

TCP：

```text
SOCK_STREAM
↓
字节流 socket
↓
TCP
```

UDP：

```text
SOCK_DGRAM
↓
Datagram socket
↓
UDP
```

所以：

```c
socket(AF_INET, SOCK_DGRAM, 0);
```

可以直接翻译：

> Linux，请给我创建一个 **IPv4 UDP socket**。

------

# 5. 从内核底层看，这一步也和 TCP 不一样

你前面已经学到了：

```text
fd
↓
struct file
↓
struct socket
↓
struct sock
```

TCP 时继续进入 TCP 协议实现。

UDP 则变成：

```text
socket(AF_INET, SOCK_DGRAM, 0)
             ↓
        系统调用
             ↓
       struct socket
             ↓
        struct sock
             ↓
          UDP协议
```

发送时大概：

```text
sendto()
 ↓
socket层
 ↓
udp_sendmsg()
 ↓
IP层
 ↓
net_device
 ↓
网卡驱动
 ↓
DMA / MAC / PHY
```

所以 TCP 和 UDP：

```text
上面 socket 接口很像
下面 IP / 网卡驱动也基本共用

真正最大的区别
          ↓
中间是 TCP 还是 UDP
```

可以画成：

```text
                    APP
                     │
                  socket
                     │
             ┌───────┴───────┐
             ▼               ▼
            TCP             UDP
             │               │
             └───────┬───────┘
                     ▼
                     IP
                     ▼
                网卡驱动
```

------

# 6. UDP Server 为什么还是需要 `bind()`？

视频里的 Server 仍然有：

```c
tSocketServerAddr.sin_family = AF_INET;

tSocketServerAddr.sin_port =
        htons(SERVER_PORT);

tSocketServerAddr.sin_addr.s_addr =
        INADDR_ANY;

bind(iSocketServer, ...);
```

这点和 TCP Server 一样。

为什么？

因为客户端必须知道：

```text
UDP数据到底送到
哪台机器的哪个程序？
```

所以 Server 必须固定：

```text
Server IP + Server Port
```

例如：

```text
192.168.1.123:8888
```

因此：

```text
bind()
```

仍然是在告诉 Linux：

> **凡是发给本机 UDP 8888 端口的数据，可以交给这个 socket。**

------

# 7. 但是 UDP 为什么没有 `listen()`？

这正是 UDP 和 TCP 最本质的差别之一。

TCP：

```text
Client
   │
   │ SYN
   ▼
Server

三次握手
↓
建立 TCP Connection
```

因此服务器需要：

```c
listen();
```

告诉 TCP：

> 我要开始接受连接。

UDP 根本不存在：

```text
TCP连接状态
SYN
SYN+ACK
ESTABLISHED
```

Client 可以第一包直接：

```text
UDP Datagram
──────────────→ Server
```

所以没有东西可 `listen()`。

------

# 8. UDP 为什么也没有 `accept()`？

还是因为：

> **UDP 没有“建立一个客户端连接”这个过程。**

TCP Server：

```text
listen_fd = 3

Client A connect
↓
accept()
↓
fd = 4


Client B connect
↓
accept()
↓
fd = 5
```

所以：

```text
Client A
↓
一个 connection socket

Client B
↓
另一个 connection socket
```

UDP 完全不是这样。

UDP Server 可以只有：

```text
一个 socket

fd = 3
```

然后：

```text
Client A ──Datagram──→
                         fd 3
Client B ──Datagram──→   UDP Server
                         fd 3
Client C ──Datagram──→
```

服务器并不会：

```text
每来一个Client
↓
accept出新的socket
```

而是：

> **一个 UDP socket 就可以接收很多不同客户端发来的数据报。**

------

# 9. 那怎么知道 UDP 包是谁发来的？

这就是视频里：

```c
recvfrom()
```

存在的意义。

大致形式：

```c
recvfrom(sockfd,
         buf,
         len,
         flags,
         (struct sockaddr *)&client_addr,
         &addrlen);
```

它一次返回两类信息：

```text
① 数据是什么？

buf
↓
"hello"
```

以及：

```text
② 谁发来的？

client_addr
↓
客户端 IP + Port
```

所以：

```text
UDP Datagram
     │
     ├── 数据："hello"
     │
     └── 来源：
         192.168.1.20:50001
```

`recvfrom()` 就会把这两个东西分别交给程序。

------

# 10. 这和 TCP 的 `recv()` 有一个非常大的区别

TCP：

```c
recv(client_fd, buf, ...);
```

为什么不需要问：

> 这是谁发来的？

因为：

```text
client_fd
```

本身就已经代表：

```text
Server
192.168.1.123:8888

        ↕

Client A
192.168.1.20:50001
```

这个具体连接。

所以内核已经知道：

> 这个 `client_fd` 对面就是 Client A。

------

UDP：

```text
一个 socket
↓
可能同时收到：

A的数据
B的数据
C的数据
```

所以每次：

```c
recvfrom()
```

都可能要告诉你：

> “这次是 A 发来的。”

下一次可能：

> “这次是 B 发来的。”

这就是两种设计的本质差别。

------

# 11. `sendto()` 为什么比 `send()` 多一个目标地址？

视频大约 **11:20-12:40** 专门打开了 `sendto()` 的手册。

你可以比较：

TCP：

```c
send(sockfd,
     buf,
     len,
     flags);
```

UDP：

```c
sendto(sockfd,
       buf,
       len,
       flags,
       dest_addr,
       addrlen);
```

多出来：

```text
dest_addr
```

也就是：

```text
“这一包发给谁？”
```

------

# 12. 为什么 TCP 的 `send()` 不需要 IP 和 Port？

因为 TCP 前面已经：

```c
connect(server IP:port);
```

例如：

```text
fd = 3

connect(fd,
        192.168.1.123:8888)
```

连接建立以后，内核里的 TCP socket 已经记住：

```text
Local:
192.168.1.20:50001

Remote:
192.168.1.123:8888
```

所以：

```c
send(fd, buf, len, 0);
```

内核自然知道：

> 发给 `192.168.1.123:8888`。

------

# 13. UDP 没有 `connect()` 时，就必须 `sendto()`

UDP 一般：

```text
socket()
↓
没有建立connection
↓
socket并没有固定一个“对方”
```

所以每一包都可以：

```c
sendto(fd, "AAA", ...,
       192.168.1.10:8888);

sendto(fd, "BBB", ...,
       192.168.1.20:9999);

sendto(fd, "CCC", ...,
       10.0.0.5:1234);
```

同一个 UDP socket：

```text
       ┌──→ Server A
       │
fd ────┼──→ Server B
       │
       └──→ Server C
```

这正是 UDP “无连接”的一个具体表现。

------

# 14. 这里有个特别重要的地方：UDP 也可以 `connect()`

视频最后特意在板书上写：

```text
connect()
可用可不用
```

这个非常重要。

很多人会学成：

> UDP 绝对不能 `connect()`。

这是错的。

UDP 完全可以：

```c
connect(udp_fd, &server_addr, ...);
```

但是：

> **UDP 的 `connect()` 不会像 TCP 一样进行三次握手。**

------

# 15. UDP `connect()` 到底做了什么？

它更接近告诉内核：

> **以后这个 UDP socket 默认只跟这个目标通信。**

比如：

```c
connect(fd, 192.168.1.123:8888);
```

之后 socket 里面记住：

```text
默认 Remote：

192.168.1.123:8888
```

于是可以直接：

```c
send(fd, buf, len, 0);
```

而不用每次：

```c
sendto(fd, buf, len, 0,
       &server_addr, ...);
```

所以：

```text
UDP 不 connect：

sendto()
↓
每一次指定目标
```

而：

```text
UDP connect：

先固定一个默认目标
↓
可以 send()
```

------

# 16. 但是这里千万不要混淆

TCP：

```c
connect()
```

意味着：

```text
发 SYN
↓
三次握手
↓
建立连接
↓
ESTABLISHED
```

UDP：

```c
connect()
```

没有：

```text
SYN
SYN+ACK
ACK
```

不会在网络上先建立 UDP 连接。

所以准确地说：

> **UDP `connect()` 更像给本地 UDP socket 设置默认通信对端，而不是建立 TCP 那种网络连接。**

------

# 17. UDP 最大的另一个特征：保留“消息边界”

这节视频虽然主要讲 API，但这个概念你最好现在就和 TCP 区分开。

假设客户端：

```c
sendto(fd, "ABC", 3, ...);

sendto(fd, "DEF", 3, ...);
```

对于 UDP，这实际上是：

```text
Datagram 1
ABC

Datagram 2
DEF
```

Server：

```c
recvfrom()
```

会按数据报读取。

UDP 是：

> **Datagram-oriented，面向数据报。**

------

TCP 则不同：

```c
send(fd, "ABC", 3, 0);
send(fd, "DEF", 3, 0);
```

TCP 只认为：

```text
ABCDEF
```

是一串连续字节。

可能：

```text
recv() → ABCDEF
```

也可能：

```text
recv() → AB
recv() → CDEF
```

所以：

```text
TCP
↓
字节流
不保留send边界


UDP
↓
数据报
保留一包一包的边界
```

这是非常重要的区别。

------

# 18. UDP 为什么比 TCP 简单？

因为 UDP 基本没有 TCP 那套复杂状态机。

TCP：

```text
三次握手
序列号
ACK
重传
乱序重组
滑动窗口
流量控制
拥塞控制
四次挥手
TIME_WAIT
```

UDP：

```text
没有这些 TCP 机制

应用把Datagram交给UDP
        ↓
加UDP Header
        ↓
交给IP
```

所以 UDP header 也非常简单，主要包含：

```text
Source Port
Destination Port
Length
Checksum
```

------

# 19. 从你刚才学的“内核缓冲区”理解 UDP

用户：

```c
sendto(fd, buf, len, ...);
```

大致：

```text
用户 buf
   ↓
sendto()
   ↓
════════════════
Linux内核
   ↓
UDP socket
   ↓
内核发送数据
   ↓
UDP Header
   ↓
IP Header
   ↓
Ethernet
   ↓
驱动 / DMA
```

但是和 TCP 有一个重要区别。

TCP 发送缓冲区需要长期保存某些未确认的数据，因为：

```text
可能需要重传
```

UDP 本身没有：

```text
ACK
重传
```

所以 UDP 不需要维护 TCP 那种：

```text
“已发送但未确认，
以后可能重传”
```

的复杂发送状态。

这也是 UDP 更简单的原因之一。

------

# 20. UDP 接收底层是怎样的？

结合我们前面讲的网卡：

```text
网线
 ↓
PHY
 ↓
MAC
 ↓
DMA
 ↓
RX Buffer
 ↓
IRQ / NAPI
 ↓
网卡 Driver
 ↓
skb
 ↓
IP
 ↓
UDP
```

UDP 根据：

```text
Destination Port
```

找到对应：

```text
UDP socket
```

例如：

```text
Destination Port = 8888
```

找到：

```text
bind(0.0.0.0:8888)
```

那个 socket。

然后：

```text
UDP socket receive queue
          ↓
      recvfrom()
          ↓
用户 buffer
```

因此你前面那个“Linux 内核缓冲区”的理解，在这里正好也能接上。

------

# 21. UDP Server 的完整模型

```text
Server APP
    │
    │ socket(AF_INET, SOCK_DGRAM)
    ▼
UDP socket fd
    │
    │ bind()
    ▼
0.0.0.0:8888
    │
    │
    ▼

while(1)
{
    recvfrom()
       │
       ├── 得到数据
       │
       └── 得到发送者IP:Port
}
```

假设：

```text
Client A
192.168.1.20:50001
          │
          │ "AAA"
          ▼

        Server
     0.0.0.0:8888

          ▲
          │ "BBB"
          │
Client B
192.168.1.30:50002
```

同一个：

```text
Server UDP socket
```

处理两个人。

没有：

```text
accept A
accept B
```

------

# 22. UDP Client 完整模型

视频后半段的客户端基本可以理解：

```text
socket()
 ↓
构造Server地址
 ↓
fgets()
 ↓
sendto(
   socket,
   data,
   Server IP:Port
 )
```

所以：

```text
键盘
 ↓
stdin
 ↓
fgets()
 ↓
ucSendBuf
 ↓
sendto()
 ↓
UDP
 ↓
IP
 ↓
Network
 ↓
Server
```

------

# 23. 这节与 TCP 最值得对比的一张表

| TCP                     | UDP                            |
| ----------------------- | ------------------------------ |
| `SOCK_STREAM`           | `SOCK_DGRAM`                   |
| 面向连接                | 无连接                         |
| 三次握手                | 无握手                         |
| Server `listen()`       | 不需要                         |
| Server `accept()`       | 不需要                         |
| `send()/recv()`         | 常用 `sendto()/recvfrom()`     |
| socket 通常固定一个连接 | 一个 socket 可向不同对端发数据 |
| 字节流                  | 数据报                         |
| 有序可靠                | UDP 本身不保证可靠、有序       |
| TCP 自己重传            | UDP 自己不重传                 |
| 有流量控制              | 无 TCP 式流量控制              |
| 有拥塞控制              | UDP 本身没有 TCP 式拥塞控制    |
| 四次挥手                | 无四次挥手                     |

------

# 24. 这里还有一个容易误解的点

说：

> UDP 不可靠。

并不是说：

```text
UDP一定会丢包
```

而是：

> **UDP 协议本身不承诺“丢了我一定重新传”。**

例如：

```text
sendto("ABC")
       ↓
网络丢包
       ↓

UDP：
“没有 ACK，也没有重传机制”
```

应用程序如果需要可靠性，可以自己实现：

```text
应用层序号
ACK
超时
重传
```

这样就相当于：

> 在 UDP 上自己设计一套可靠传输协议。

------

# 25. 重点片段

### 片段一：01:10-03:20

板书直接把 UDP Server/Client 与 TCP 做对比。

重点看：

```text
Server:
socket
bind
recvfrom
sendto
```

这里最重要的是：

> `listen/accept` 消失了。

------

### 片段二：05:40-09:50

真正修改 Server：

```c
socket(AF_INET, SOCK_DGRAM, 0);
```

然后：

```c
bind();
recvfrom();
```

这是这节最核心代码。

------

### 片段三：11:20-12:40

打开：

```text
man sendto
```

重点看：

```c
dest_addr
```

它正好解释 UDP 为什么不需要提前建立连接：

> **目标地址可以跟着每一包数据一起指定。**

------

### 片段四：15:30-16:18

最后板书：

```text
UDP connect()
可用可不用
```

这一小段特别值得记住。

------

# 26. 你现在最应该弄懂的 6 个问题

```text
① UDP为什么没有listen？
→ 因为没有建立连接这件事。


② 为什么没有accept？
→ 因为不会为每个客户端建立TCP式connection socket。


③ UDP Server为什么还是bind？
→ Client需要知道把UDP数据报发往哪个固定端口。


④ recvfrom为什么要返回地址？
→ 因为一个UDP socket可能收到很多不同客户端的数据。


⑤ sendto为什么需要dest_addr？
→ 因为UDP没有预先固定对端，
  每一包都需要知道发给谁。


⑥ UDP能不能connect？
→ 能。
  但不会三次握手，
  主要是给本地socket设置默认对端。
```

------

# 27. 记忆版总结

```text
类型：
UDP Socket 编程示例


UDP Server：

socket(AF_INET, SOCK_DGRAM)
        ↓
bind(IP + Port)
        ↓
recvfrom()
        ↓
得到：
数据 + Client地址
        ↓
sendto()


UDP Client：

socket(AF_INET, SOCK_DGRAM)
        ↓
sendto(
    数据,
    Server IP + Port
)


核心区别：

TCP：
先建立connection
再收发数据

UDP：
没有connection
每个Datagram自己携带通信目标信息


TCP Server：

socket
bind
listen
accept
recv/send


UDP Server：

socket
bind
recvfrom/sendto


TCP：
一个connection socket
通常对应一个客户端连接

UDP：
一个socket
可以接收很多不同客户端的数据报
```

### 一句话记忆

> **TCP 是“先认识你，再一直跟你聊天”；UDP 是“我这封信直接写上你的 IP 和端口就发出去”。所以 UDP 没有 `listen/accept`，主要通过 `sendto/recvfrom` 在每个数据报层面确定通信对象。**





# 问题1

我有个问题，在linux里面好像所有和外部设备打交道都需要通过内核中的一片缓冲区作为中间地带，然后app从通过系统调用的方式从这个缓冲区中读取数据。比如从磁盘、网卡、串口、输入输出设备等，都是有个内核动态申请的缓冲区好像，这里应该是Linux里对应设备的驱动程序申请的吧。



你这个理解**大方向很对，但“所有设备都一定经过一块由驱动动态申请的内核缓冲区”这句话要稍微修正**。

更准确地说：

> <span style="color:#FF0000; background:#00FF80;">**Linux 中，用户 APP 通常不能直接操作硬件。数据往往先进入内核管理的数据结构或缓冲区，再通过 `read/write/recv/send` 等系统调用在用户空间和内核之间传递。**</span>
>
> 但这个“缓冲区”不一定由驱动自己申请，也不一定总是存在一块固定连续的 buffer。

你可以先建立一个最常见模型：

```text
              用户空间

          Application
              │
        read()/write()
        recv()/send()
              │
══════════════╪══════════════
              │
              ▼
              内核

        内核缓冲/队列
              │
              ▼
          设备驱动
              │
              ▼
             DMA
              │
              ▼
             硬件
```

接收方向反过来：

```text
硬件
 ↓
DMA / 驱动
 ↓
内核缓冲/队列
 ↓
read()/recv()
 ↓
用户空间 buffer
```

这个模型对于你目前学习 Linux 驱动非常好用。

------

## 1. 为什么 Linux 喜欢在中间放一个内核缓冲区？

主要有几个原因。

### 第一：用户程序和硬件速度不一样

比如串口：

```text
串口：
不断有字符进来

APP：
可能此时正在睡眠
```

如果没有缓冲：

```text
字符来了
↓
APP没及时读取
↓
数据丢了
```

所以：

```text
串口硬件
 ↓
驱动/TTY缓冲区
 ↓
先存着
 ↓
APP以后read()
```

<span style="color:#FF0000; background:#00FF80;">这样 APP 不需要时时刻刻盯着硬件。</span>

------

### 第二：保护内核和硬件

用户程序：

```c
char buf[100];
read(fd, buf, 100);
```

<span style="color:#FF0000; background:#00FF80;">Linux 不会通常让硬件直接随便操作：</span>

```text
用户进程内存
```

<span style="color:#FF0000; background:#00FF80;">因为用户进程：</span>

```text
可能退出
可能换页
虚拟地址会变化
权限需要检查
```

所以通常先由内核掌控数据，再：

```text
copy_to_user()
```

复制给 APP。

------

### 第三：方便异步和 DMA

比如网卡：

```text
CPU现在去干别的
        ↓
网卡收到数据
        ↓
DMA直接写内存
```

这时候 APP 甚至根本没有运行。

所以需要有：

```text
提前准备好的内核/DMA buffer
```

让网卡能先把数据放进去。

------

## 2. 但是，“是谁申请缓冲区”不能一概说是驱动

这是你现在最需要修正的地方。

Linux 往往是分层的：

```text
用户APP
   ↓
Linux某个子系统
   ↓
设备驱动
   ↓
硬件
```

<span style="color:#FF0000; background:#00FF80;">于是缓冲区可能由：</span>

```text
驱动
```

<span style="color:#FF0000; background:#00FF80;">申请，也可能由：</span>

```text
内核通用子系统
```

负责。

------

## 3. 以网卡为例

这个你现在最熟悉。

接收：

```text
网卡
 ↓
DMA
 ↓
RX Buffer
 ↓
网卡驱动
 ↓
sk_buff
 ↓
TCP/IP
 ↓
Socket Receive Queue
 ↓
recv()
 ↓
用户buffer
```

这里其实已经有**多级缓冲**。

比如：

### 第一级

网卡驱动准备：

```text
RX Descriptor Ring
+
DMA Buffer
```

这部分通常确实跟驱动关系非常大。

------

### 第二级

<span style="color:#FF0000; background:#00FF80;">网络协议栈：</span>

```text
struct sk_buff
```

这是<span style="color:#FF0000; background:#00FF80;"> Linux 网络子系统管理的数据结构。</span>

------

### 第三级

TCP socket：

```text
TCP Receive Queue
```

由：

```text
struct sock / tcp_sock
```

管理。

所以：

> 网卡接收数据并不是“只有一个内核缓冲区”。

而可能是：

```text
硬件 RX Buffer
      ↓
skb
      ↓
TCP socket receive queue
      ↓
用户buffer
```

------

## 4. 串口也很典型

UART 接收到：

```text
A B C D E
```

硬件可能有：

```text
UART RX FIFO
```

先在硬件内部存几个字节。

然后：

```text
UART硬件 FIFO
      ↓
IRQ / DMA
      ↓
UART Driver
      ↓
TTY Layer
      ↓
TTY Buffer / queue
      ↓
read()
      ↓
用户buf
```

所以串口也不只是：

```text
Driver Buffer
```

Linux 中还有：

```text
TTY subsystem
```

这一层。

也就是说：

> <span style="color:#FF0000; background:#00FF80;">串口驱动负责和 UART 硬件打交道，而字符缓存、行规程等很多东西由 TTY 子系统参与管理。</span>

------

## 5. 磁盘又不一样

磁盘是一个特别好的反例，因为磁盘的“内核缓冲”很多时候不是驱动自己搞的一块 buffer。

例如：

```c
read(fd, buf, 4096);
```

普通文件读取经常涉及：

```text
APP
 │
 │ read()
 ▼
VFS
 │
 ▼
Page Cache
 │
 ▼
文件系统
 │
 ▼
Block Layer
 │
 ▼
磁盘驱动
 │
 ▼
硬盘
```

这里非常重要的内核缓存就是：

```text
Page Cache
页缓存
```

<span style="color:#FF0000;">它不是：</span>

> <span style="background:#00FF80;">“某个硬盘驱动自己 malloc 出来的缓存”。</span>

而是：

> <span style="color:#FF0000; background:#00FF80;">**Linux 内核文件系统/内存管理体系共同管理的 page cache。**</span>

例如第一次：

```text
磁盘
 ↓
DMA
 ↓
RAM中的page
 ↓
Page Cache
 ↓
用户APP
```

第二次再读同一块文件：

```text
APP
 ↓
Page Cache
```

甚至可能：

> 根本不需要再次访问磁盘。

所以：

```text
磁盘缓存
```

<span style="color:#FF0000; background:#00FF80;">更多是 Linux 通用内核子系统负责。</span>

------

## 6. 键盘、鼠标也是类似思想

比如键盘：

```text
键盘硬件
 ↓
USB / PS2控制器
 ↓
设备驱动
 ↓
Linux Input Subsystem
 ↓
event queue
 ↓
/dev/input/eventX
 ↓
read()
 ↓
APP
```

APP：

```c
read(fd, &event, sizeof(event));
```

读到的是：

```text
struct input_event
```

这些事件已经由：

```text
驱动 + input subsystem
```

在内核里处理好了。

所以 APP 并不是：

```text
直接读键盘寄存器
```

------

## 7. 这正好体现 <span style="background:#00FF80; color:#FF0000;">Linux 为什么要有“子系统”</span>

<span style="color:#FF0000; background:#00FF80;">如果每个驱动都自己完成：</span>

```text
缓冲
队列
阻塞
唤醒
select
poll
权限
用户复制
```

那<span style="color:#FF0000; background:#00FF80;">所有驱动都会重复造轮子。</span>

所以<span style="color:#FF0000; background:#00FF80;"> Linux 通常设计成：</span>

```text
                APP
                 │
                 ▼
        Linux 通用子系统
                 │
       ┌─────────┼─────────┐
       ▼         ▼         ▼
      TTY      Network    Block
       │         │          │
       ▼         ▼          ▼
   UART Driver NIC Driver Disk Driver
       │         │          │
       ▼         ▼          ▼
     UART       NIC        Disk
```

因此：

> **<span style="background:#00FF80; color:#FF0000;">驱动主要负责“硬件相关”的部分，而缓冲、队列、协议、调度等很多工作由上面的内核子系统负责</span>。**

<span style="color:#FF0000; background:#00FF80;">这句话特别重要。</span>

------

## 8. 所以“驱动申请内核缓冲区”什么时候成立？

有很多场景成立。

比如网卡：

```text
RX DMA buffers
TX DMA buffers
descriptor ring
```

驱动经常需要：

```c
dma_alloc_coherent(...)
```

或者映射内存供 DMA 使用。

UART 驱动也可能管理：

```text
DMA buffer
ring buffer
```

所以：

> <span style="color:#FF0000; background:#00FF80;">**与具体硬件 DMA 直接交互的 buffer，通常确实是驱动或者驱动所属子系统负责准备的。**</span>

------

## 9. 但是有一些设备甚至可以绕过传统“copy一次”的模式

这也是为什么不能说“所有设备一定都经过一个内核 buffer”。

例如：

```c
mmap()
```

你之前学 framebuffer 就碰到了。

APP：

```text
User virtual address
       │
       │ page table mapping
       ▼
Framebuffer physical memory
```

于是<span style="color:#FF0000; background:#00FF80;">应用可以直接访问映射后的内存。</span>

这种情况下不是：

```text
设备
↓
内核buffer
↓
copy_to_user
↓
APP
```

而是：

```text
同一块物理内存
       ▲
       │
 ┌─────┴─────┐
 │           │
Kernel      User
mapping     mapping
```

所以没有传统意义上的：

```text
read()复制一次
```

这和你之前 framebuffer 的问题正好能接起来。

------

## 10. 网络中也存在“零拷贝”

比如：

```text
sendfile()
splice()
mmap()
```

以及各种：

```text
zero-copy
```

技术。

目的就是减少：

```text
Kernel buffer
↓
User buffer
↓
Kernel buffer
```

这种反复复制。

所以你以后可以形成这样的认识：

> **“用户态与设备之间通常存在内核管理的数据通道”，这是对的；但是它不一定表现为一个单独的、中间的、连续内核 buffer。**

可能是：

```text
page cache
skb queue
ring buffer
DMA buffer
descriptor
socket buffer
TTY buffer
shared pages
mmap
```

等等。

------

## 11. 我给你把几个典型设备放一起对比

| 设备         | 常见中间内核机制                    | 谁主要管理                                                   |
| ------------ | ----------------------------------- | ------------------------------------------------------------ |
| 网卡         | RX/TX DMA buffer、skb、socket queue | <span style="color:#FF0000; background:#00FF80;">驱动 + 网络协议栈</span> |
| 磁盘         | Page Cache、bio/request、DMA pages  | <span style="color:#FF0000; background:#00FF80;">VFS/文件系统/Block层 + 驱动</span> |
| UART         | FIFO、TTY buffer、DMA/ring          | <span style="color:#FF0000; background:#00FF80;">UART驱动 + TTY子系统</span> |
| 键盘鼠标     | input event queue                   | <span style="color:#FF0000; background:#00FF80;">Input子系统 + 驱动</span> |
| 普通字符设备 | driver buffer / kfifo 等            | <span style="color:#FF0000; background:#00FF80;">驱动自行决定</span> |
| Framebuffer  | 显存/DDR buffer，可 mmap            | 驱<span style="color:#FF0000; background:#00FF80;">动/DRM/Framebuffer子系统</span> |
| 摄像头       | DMA/video buffer queue              | <span style="color:#FF0000; background:#00FF80;">V4L2 + 驱动</span> |

你看这里有一个规律：

> <span style="background:#00FF80; color:#FF0000;">**设备越复杂，通常越不是“驱动自己一个 buffer 解决所有问题”，而是“内核子系统 + 驱动 + DMA/硬件”共同完成。**</span>

------

## 12. 最后把你的原理解改成准确版本

你原来的想法：

> Linux 和外设打交道都有一个内核动态申请缓冲区，驱动负责申请，APP 再通过系统调用读取。

建议升级成：

> <span style="color:#FF0000; background:#00FF80;">**Linux 中，用户 APP 通常不会直接操作硬件，而是通过系统调用进入相应的内核子系统。硬件数据往往先进入由驱动或内核子系统管理的 DMA buffer、队列、页缓存、socket buffer 等内核数据结构，再由 `read()/recv()` 等接口交给用户空间；发送方向则相反。具体缓冲区由谁申请和管理取决于设备类型和 Linux 子系统，并不一定全部由驱动单独负责。**</span>

然后脑子里固定：

```text
                 用户APP
                    │
             read/write/ioctl
             send/recv/mmap
                    │
════════════════════╪════════════════
                    │
              Linux内核子系统
                    │
        ┌───────────┼───────────┐
        │           │           │
     缓冲区       队列        协议/缓存
        │           │           │
        └───────────┼───────────┘
                    │
                 Driver
                    │
              DMA / IRQ
                    │
                 Hardware
```

<span style="background:#00FF80; color:#FF0000;">这个模型比“所有设备都有驱动 malloc 的一个内核 buffer”更准确，也基本可以贯穿你后面学 **字符设备、块设备、网络设备、DMA、mmap、V4L2、DRM**。</span>

