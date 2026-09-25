# 视频分析：P43 `4_4-4 使用 write 函数写文件`

这节视频约 **10 分 59 秒**。它和我们前面刚聊的 `fd → fdtable → struct file → f_pos` 接得特别好，因为这一节实际上第一次把 **`struct file` 里的“当前文件位置 f_pos”真正用起来了**。

这节最核心的不是单纯记住 `write()` 怎么写，而是理解：

> **`write()` 会从当前文件位置开始写数据，写完以后当前文件位置自动向后移动；`lseek()` 则可以主动修改这个当前位置。**

------

## 1. 视频类型判断

**主要类型：教程类 / Linux 文件 I/O 实操**
**次要类型：讲解类 / 实验验证类**

判断依据：

1. 先讲 `write()` 的函数原型。
2. 用板书解释多次 `write()` 与文件位置的变化。
3. 编写程序，把命令行字符串写入文件。
4. 检查 `write()` 返回值。
5. 最后通过 `lseek()` 修改文件位置，再写入数据，验证“覆盖而不是插入”。

------

## 2. 一句话总结

这节视频主要讲：

> **`write(fd, buf, count)` 从 fd 对应的当前文件偏移处写入最多 `count` 个字节，并返回实际写入的字节数；写成功后文件偏移会向后移动，因此连续调用 `write()` 会连续向后写，而 `lseek()` 可以人为改变下一次写入的位置。**

------

## 3. 时间段拆解

| 时间段      | 内容                                               | 作用                            |
| ----------- | -------------------------------------------------- | ------------------------------- |
| 00:00-00:40 | 查看 `write()` 函数原型                            | 明确参数和返回值                |
| 00:40-01:50 | 板书解释连续两次 `write()`                         | 引出“当前文件位置”              |
| 01:50-04:40 | 修改程序，接收多个字符串并循环调用 `write()`       | 实际实现写文件                  |
| 04:40-06:20 | 编译运行、查看文件内容                             | 验证连续写入效果                |
| 06:20-07:50 | 增加 `write()` 返回值检查，继续测试                | 说明实际写入长度的重要性        |
| 07:50-08:30 | 回到教材查看 `write/lseek` 原型                    | 为修改文件偏移做准备            |
| 08:30-10:00 | 编写 `write_in_pos.c`，用 `lseek(fd, 3, SEEK_SET)` | 主动修改 f_pos                  |
| 10:00-10:59 | 在偏移 3 写入 `"\r\n"` 并 `cat` 验证               | 证明 write 从当前位置覆盖原数据 |

时间段是根据视频实际画面和操作节奏划分，可能有几秒误差。

------

## 4. `write()` 的函数原型

视频中的核心函数：

```c
ssize_t write(int fd, const void *buf, size_t count);
```

三个参数可以直接理解成：

```text
write(fd, buf, count)
       │    │      │
       │    │      └── 最多写多少字节
       │    │
       │    └────────── 数据从哪里来
       │
       └─────────────── 写到哪个已打开文件
```

例如：

```c
write(fd, "100ask", 6);
```

就是：

> 向 `fd` 对应的文件，从**当前文件位置**开始写入 `"100ask"` 的 6 个字节。

------

## 5. `buf` 在哪里？

例如：

```c
char buf[] = "100ask";

write(fd, buf, 6);
```

这里：

```text
buf
↓
用户空间中的内存
```

可能位于：

```text
栈
.data
.rodata对应的数据
heap
```

具体取决于程序怎么定义它。

执行：

```c
write(fd, buf, 6);
```

以后：

```text
用户空间

buf
"100ask"
   │
   │ write系统调用
   ▼

Linux内核
   │
   ▼
fd → struct file
   │
   ▼
文件系统
```

也就是说 <span style="color:#FF0000; background:#00FF80;">APP 把：</span>

> <span style="background:#00FF80;">“数据在哪 + 写多少 + 写哪个文件”</span>

<span style="color:#FF0000; background:#00FF80;">告诉内核。</span>

------

## 6. 视频前面板书真正想表达什么？

视频大约一分钟附近画的是：

```c
write(fd, buf, N);
```

开始时：

```text
pos = 0
```

写完 N 字节：

```text
0                  N
↓                  ↓
┌──────────────────┐
│     N字节数据     │
└──────────────────┘

当前pos = N
```

接着再：

```c
write(fd, buf2, M);
```

不是重新从 0 开始。

而是：

```text
0                  N                 N+M
│                  │                  │
├──── N字节 ───────┼──── M字节 ──────┤
                   ↑                  ↑
                写之前pos          写完pos
```

所以：

> <span style="background:#00FF80;">**连续两次 `write()` 默认是接着写。**</span>

------

## 7. 这个 `pos` 到底存在哪里？

这正好对应我们前面聊的：

```text
fd
↓
文件描述符表
↓
struct file
↓
f_pos
```

也就是说：

```c
write(fd, ...)
```

进入内核后：

```text
fd = 3
↓
当前进程 fdtable[3]
↓
struct file
↓
f_pos
```

<span style="background:#00FF80;">内核通过 `f_pos` 知道：</span>

> <span style="background:#00FF80; color:#FF0000;">“下一次应该从文件哪个位置开始操作。”</span>

比如刚打开：

```text
f_pos = 0
```

写 10 字节：

```text
f_pos = 10
```

再写 20 字节：

```text
f_pos = 30
```

这节视频实际上把前面抽象的 `struct file->f_pos` 变成了可观察行为。

------

## 8. 视频程序为什么用 `argv[]`？

程序允许类似：

```bash
./write 1.txt www.100ask.net weidongshan
```

所以：

```text
argv[0] = "./write"

argv[1] = "1.txt"

argv[2] = "www.100ask.net"

argv[3] = "weidongshan"
```

其中：

```text
argv[1]
```

<span style="color:#FF0000; background:#00FF80;">用来：</span>

```c
open(argv[1], ...)
```

而：

```text
argv[2]
argv[3]
...
```

<span style="color:#FF0000; background:#00FF80;">作为写入文件的数据。</span>

------

## 9. 核心循环是什么？

视频里的代码大致是：

```c
for (i = 2; i < argc; i++)
{
    len = write(fd, argv[i], strlen(argv[i]));

    if (len != strlen(argv[i]))
    {
        perror("write");
        break;
    }
}
```

例如：

```bash
./write 1.txt www.100ask.net weidongshan
```

第一次：

```c
write(fd,
      "www.100ask.net",
      strlen("www.100ask.net"));
```

假设写了 14 字节：

```text
f_pos:
0 → 14
```

第二次：

```c
write(fd,
      "weidongshan",
      strlen("weidongshan"));
```

从：

```text
pos = 14
```

继续写。

<span style="color:#FF0000; background:#00FF80;">所以如果不额外写空格或者换行，结果就会连起来：</span>

```text
www.100ask.netweidongshan
```

这也是视频前半段实验看到的现象。

------

## 10. 为什么后来字符串之间出现换行？

<span style="color:#FF0000; background:#00FF80;">视频后面程序进一步修改，在每个参数之间加入了换行一类的数据。</span>

于是：

```text
www.100ask.net
weidongshan
100ask.taobao.com
```

本质上还是连续的：

```text
write("www.100ask.net")
↓
write("\n")
↓
write("weidongshan")
↓
write("\n")
↓
...
```

文件里面不存在什么特殊的“行”。

所谓：

```text
第一行
第二行
```

<span style="color:#FF0000; background:#00FF80;">本质只是字符数据里存在：</span>

```text
\n
```

<span style="color:#FF0000; background:#00FF80;">终端 `cat` 看见换行字符以后换行显示。</span>

------

## 11. `write()` 的返回值特别重要

函数：

```c
ssize_t write(...);
```

返回的不是：

```text
成功 = 0
失败 = -1
```

这么简单。

<span style="color:#FF0000;">成功时返回：</span>

> <span style="background:#00FF80;">**实际写进去多少字节。**</span>

例如：

```c
len = write(fd, buf, 100);
```

<span style="color:#FF0000; background:#00FF80;">理想情况：</span>

```text
len = 100
```

因此视频检查：

```c
if (len != strlen(argv[i]))
{
    perror("write");
}
```

就是在判断：

> <span style="color:#FF0000; background:#00FF80;">“我原本要求写这么多字节，实际上是否全部写完？”</span>

------

## 12. 为什么 `write()` 可能没有一次写完整？

普通文件中很多时候会完整写入，但系统编程中不能永远假设：

```text
请求100字节
=
一定返回100
```

尤其以后操作：

```text
pipe
socket
终端
非阻塞设备
驱动设备
```

<span style="color:#FF0000; background:#00FF80;">可能出现：</span>

```text
要求写100
↓
实际只写40
↓
write返回40
```

这叫：

> **partial write，部分写入。**

所以<span style="color:#FF0000; background:#00FF80;">更健壮的程序有时需要循环：</span>

```text
没写完
↓
继续write剩余数据
```

<span style="background:#00FF80;">视频当前是入门阶段，所以主要让你先认识：</span>

> <span style="color:#FF0000; background:#00FF80;">`write()` 的返回值 = 实际写入字节数。</span>

------

## 13. 为什么打开文件用了 `O_TRUNC`？

前半段程序可以看到类似：

```c
fd = open(argv[1],
          O_RDWR | O_CREAT | O_TRUNC,
          0644);
```

这里：

```text
O_CREAT
↓
不存在就创建
```

而：

```text
O_TRUNC
↓
文件已存在时把长度清零
```

所以<span style="color:#FF0000; background:#00FF80;">每次测试：</span>

```bash
./write 1.txt ...
```

<span style="color:#FF0000; background:#00FF80;">都会从一个空文件开始。</span>

<span style="color:#FF0000; background:#00FF80;">否则上一次文件内容可能残留，容易影响实验结果。</span>

------

## 14. 后半段最重要：`lseek()`

视频后面查看了：

```c
off_t lseek(int fd, off_t offset, int whence);
```

然后写：

```c
lseek(fd, 3, SEEK_SET);
```

这个是什么意思？

```text
SEEK_SET
↓
以文件开头为基准

offset = 3
↓
移动到距离文件头3字节的位置
```

于是：

```text
原来：

f_pos = 某处

执行：
lseek(fd, 3, SEEK_SET)

变成：

f_pos = 3
```

这再次说明：

> <span style="background:#00FF80; color:#FF0000;">**文件当前位置不是一个虚无的概念，它真的是内核为这次打开维护的状态。**</span>

------

## 15. 这和 `struct file` 完全对应

你现在甚至可以把：

```c
lseek(fd, 3, SEEK_SET);
```

理解成概念上的：

```text
fd
↓
fdtable[fd]
↓
struct file
↓
修改 f_pos
↓
f_pos = 3
```

当然实际 Linux 内核实现比这一行复杂，但思想就是这样。

然后下一次：

```c
write(fd, ...);
```

自然就从：

```text
f_pos = 3
```

开始。

------

## 16. 视频最后那个实验特别值得理解

文件原来类似：

```text
www.100ask.net
weidongshan
100ask.taobao.com
```

程序执行：

```c
lseek(fd, 3, SEEK_SET);
write(fd, "\r\n", 2);
```

先数：

```text
w  w  w  .  1  0  0  a  s  k ...
0  1  2  3  4  5  6  7  8  9
```

执行：

```c
lseek(fd, 3, SEEK_SET);
```

当前位置来到：

```text
位置3
 ↓
 .
```

然后写两个字节：

```text
\r
\n
```

它们会覆盖位置：

```text
3、4
```

也就是原来的：

```text
'.'
'1'
```

------

## 17. <span style="background:#00FF80;">`write()` 是覆盖，不是“插进去”</span>

这个是本节最后实验最重要的隐含知识。

原来：

```text
www.100ask.net
```

<span style="color:#FF0000; background:#00FF80;">假设在位置 3 写两个字符：</span>

```text
XX
```

<span style="color:#FF0000;">结果不是：</span>

```text
wwwXX.100ask.net
```

<span style="color:#FF0000; background:#00FF80;">也就是不会自动把后面内容整体往后推。</span>

而是：

```text
wwwXX00ask.net
```

因为：

```text
位置3原来的 '.'
位置4原来的 '1'
```

被覆盖。

所以：

> **普通 `write()` 在当前位置写数据，本质是覆盖对应位置已有的数据；只有超过原文件结尾时，才会扩展文件。**

------

## 18. 视频里写的是 `"\r\n"`

所以原来：

```text
www.100ask.net
```

在偏移 3 写：

```text
\r\n
```

以后大致变成：

```text
www\r\n00ask.net
```

终端显示时：

```text
www
00ask.net
```

于是看起来文件被“从 www 后面断开了”。

但实际上并不是：

> 插入了换行，让后面整体后移。

而是：

> 原来偏移 3、4 的两个字节被 `\r\n` 替换了。

这个区别非常重要。

------

## 19. 为什么 `lseek()` 修改的是“本次打开”的位置？

假设：

```c
fd1 = open("1.txt", O_RDWR);
fd2 = open("1.txt", O_RDWR);
```

通常：

```text
fd1
↓
struct file A
↓
f_pos = 0


fd2
↓
struct file B
↓
f_pos = 0
```

执行：

```c
lseek(fd1, 100, SEEK_SET);
```

可能：

```text
struct file A
f_pos = 100
```

但：

```text
struct file B
f_pos = 0
```

没变。

因为<span style="color:#FF0000; background:#00FF80;">文件偏移属于：</span>

> <span style="color:#FF0000; background:#00FF80;">**打开文件对象 `struct file`**</span>

而不是 inode。

这一点正好和我们前面讲的三层对应：

```text
fd
↓
struct file
    ├── flags
    ├── f_pos    ← 本节重点
    └── ...
↓
inode
```

------

## 20.<span style="background:#00FF80; color:#FF0000;"> inode 为什么不能保存这个位置？</span>

因为<span style="color:#FF0000; background:#00FF80;">同一个文件：</span>

```text
1.txt
```

<span style="color:#FF0000; background:#00FF80;">可以同时被多个程序打开。</span>

比如：

```text
Process A
读到第100字节

Process B
读到第500字节
```

<span style="color:#FF0000;">如果位置放在 inode：</span>

```text
inode
f_pos = ???
```

<span style="color:#FF0000; background:#00FF80;">那两个进程就打架了。</span>

所以：

```text
inode
↓
描述“文件本身”
```

而：

```text
struct file
↓
描述“这一次打开”
```

当前位置：

```text
f_pos
```

自然应该属于 `struct file`。

------

## 21. 从硬件软件结合角度看 `write()`

假设：

```c
write(fd, buf, 100);
```

完整链大致：

```text
用户空间

buf
│
│ 100字节
▼

write(fd, buf, 100)
│
▼
系统调用
│
▼
CPU用户态 → 内核态
│
▼
当前进程fdtable
│
▼
fd → struct file
│
├── 找到f_pos
│
└── 找到对应inode
│
▼
VFS
│
▼
具体文件系统
│
▼
Page Cache
│
▼
标记页面为dirty
│
│
│ 后续适当时机
▼
块设备层
│
▼
设备驱动
│
▼
eMMC / SD / SSD
```

特别注意：

> `write()` 返回时，数据不一定已经真正写进 eMMC。

很可能只是：

```text
用户数据
↓
进入内核/Page Cache
↓
标记为脏页
```

以后内核再进行真正的存储设备写入。

<span style="color:#FF0000; background:#00FF80;">这也是后面：</span>

```c
fsync(fd);
```

<span style="background:#00FF80; color:#FF0000;">存在的意义之一。</span>

------

## 22. 所以 `write()` 返回成功 ≠ Flash/eMMC 已经完成物理写入

这是你现在学习 Linux 特别值得提前建立的概念：

```text
write成功
```

主要意味着：

> Linux 已经接受了这次写请求，对文件内容的逻辑修改已经完成到系统要求的程度。

不一定意味着：

```text
电子已经写入 NAND/eMMC
```

因为中间还有：

```text
Page Cache
文件系统
块设备缓存
设备本身缓存
```

等层次。

后面学：

```text
fsync
sync
```

时就会继续接上。

------

## 23. 视频结构

整体表达路径：

```text
认识 write()
     ↓
理解当前文件位置
     ↓
连续 write 会继续向后写
     ↓
编程把多个参数写入文件
     ↓
检查 write 返回长度
     ↓
引出 lseek
     ↓
主动修改 f_pos
     ↓
再次 write
     ↓
证明指定位置写入属于覆盖
```

------

## 24. 观点分析

这是纯技术教学，没有价值判断型观点。

主要明确技术结论：

| 技术结论                          | 明确/可能 | 依据                       |
| --------------------------------- | --------- | -------------------------- |
| `write()` 返回实际写入字节数      | 明确      | 函数原型与代码             |
| 写入以后当前文件位置向后移动      | 明确      | 板书连续写入               |
| 多次 `write()` 默认连续写         | 明确      | 文件实验                   |
| `lseek()` 可以修改当前文件偏移    | 明确      | 后半段代码实验             |
| 指定位置 `write()` 会覆盖已有字节 | 明确      | 最终 `cat` 验证            |
| 文件偏移属于打开状态              | 明确      | `lseek + write` 行为可验证 |

------

## 25. Who / Whom / Whose / When / Where / What / Why / How

| 维度  | 视频中的答案                                   |
| ----- | ---------------------------------------------- |
| Who   | Linux 用户态应用程序                           |
| Whom  | 向 fd 对应的已打开文件写数据                   |
| Whose | `f_pos` 属于这次打开对应的 `struct file`       |
| When  | 调用 `write()` 时                              |
| Where | 用户空间 → Linux 内核 → VFS/文件系统           |
| What  | 把用户缓冲区的数据写入文件                     |
| Why   | APP 需要持久化或修改文件内容                   |
| How   | `fd → struct file → f_pos → write → 更新f_pos` |

------

## 26. 这节视频最重要的三个片段

### 片段一：约 00:40-01:50

板书：

```text
write N字节
↓
pos: 0 → N

再write M字节
↓
pos: N → N+M
```

这是整节课的理论核心。

------

### 片段二：约 02:00-06:30

程序循环：

```c
write(fd, argv[i], strlen(argv[i]));
```

实际把多个字符串连续写进文件。

重点理解：

> **每次 `write()` 都接着上一次的 `f_pos`。**

------

### 片段三：约 08:30-10:59

```c
lseek(fd, 3, SEEK_SET);
write(fd, "\r\n", 2);
```

这是全视频最值得看的实验。

它把：

```text
fd
↓
struct file
↓
f_pos
```

从抽象数据结构变成了真实文件内容变化。

------

## 27. 你学完这节最应该会回答的问题

```text
1. write(fd, buf, count) 三个参数分别是什么？

2. write成功为什么返回的不是0，而是字节数？

3. 为什么连续两次write会接着写？

4. 当前文件位置到底存在哪里？

5. lseek(fd, 3, SEEK_SET) 修改了什么？

6. 为什么在文件中间write是覆盖，而不是插入？

7. write返回成功为什么不代表数据已经真正写入eMMC？
```

其中 **第 3、4、5 个问题**和你前面正在研究的 `struct file` 关系最大。

------

## 28. 记忆版总结

```text
write：

ssize_t write(fd, buf, count)

fd
→ 指定哪个打开文件

buf
→ 用户空间数据地址

count
→ 请求写入多少字节

返回值
→ 实际写入多少字节


核心机制：

fd
↓
fdtable
↓
struct file
↓
f_pos
↓
从当前位置写入
↓
写N字节
↓
f_pos += N


lseek：

lseek(fd, 3, SEEK_SET)

↓

把当前文件位置设为3


再write：

从偏移3开始覆盖已有内容
而不是插入内容
```

## 一句话记忆

> **`open()` 建立“fd → struct file”的关系，`write()` 根据这个 `struct file` 中的当前偏移 `f_pos` 写数据并推进偏移，而 `lseek()` 则专门负责改变这个 `f_pos`。**

所以你最近几节课其实已经可以连成一条非常漂亮的主线：

```text
open()
↓
得到fd
↓
fdtable[fd]
↓
struct file
├── flags
└── f_pos
      │
      ├── write() → 从这里写，并向后移动
      │
      └── lseek() → 主动改变这里
↓
inode
↓
文件系统
↓
Page Cache
↓
存储设备
```

这张图基本就是后面继续学习 `read()`、`lseek()`、`close()`，甚至文件系统和驱动的基础。





# 问题1

O_RDWR | O_CREAT | O_TRUNC 这3个参数的英文名称是什么呢，该如何从英文字面上的意思来理解呢，前面的O_又代表什么呢，还有RDWR代表什么呢，还有其他的一些这种宏吗。



可以，<span style="color:#FF0000;">这几个宏其实只要把英文拆开，基本就不需要死背了。</span>

你看到的：

```c
O_RDWR | O_CREAT | O_TRUNC
```

可以直接读成：

> **Open Read/Write + Create + Truncate**

也就是：

> <span style="background:#00FF80;">**以可读可写方式打开；不存在就创建；如果已经存在，就把原内容清空。**</span>

------

## 1. `O_` 前缀是什么意思？

`O_` 是 `open()` <span style="color:#FF0000;">这一类文件打开选项宏的统一前缀。</span>

你可以<span style="color:#FF0000; background:#00FF80;">为了记忆把它理解成：</span>

```text
O_ ≈ Open
```

所以：

```text
O_RDWR
```

<span style="color:#FF0000; background:#00FF80;">可以记成：</span>

```text
Open + Read/Write
O_CREAT
```

记成：

```text
Open + Create
O_TRUNC
```

记成：

```text
Open + Truncate
```

严格来说，`O_` 是 POSIX/Linux 对这一组 `open` flags 的命名约定，不需要把它理解成一个单独传给 `open()` 的参数。

------

## 2. `O_RDWR`

英文可以拆成：

```text
O_   RD   WR
│    │    │
│    │    └── Write
│    └─────── Read
└──────────── Open
```

所以：

```text
RDWR
=
Read + Write
=
读 + 写
```

因此：

```c
open("1.txt", O_RDWR);
```

就是：

> 以**既能读又能写**的方式打开 `1.txt`。

对应的另外两个特别常见：

| 宏         | 英文拆解   | 含义     |
| ---------- | ---------- | -------- |
| `O_RDONLY` | Read Only  | 只读     |
| `O_WRONLY` | Write Only | 只写     |
| `O_RDWR`   | Read Write | 可读可写 |

这里也很好拆：

```text
RDONLY
= Read Only

WRONLY
= Write Only

RDWR
= Read Write
```

所以你以后根本不用背。

------

## 3. `O_CREAT`

这个来自：

```text
CREATE
```

但宏写成：

```text
CREAT
```

少了一个 `E`。

所以：

```text
O_CREAT
≈ Open + Create
```

意思：

> **如果文件不存在，就创建它。**

例如：

```c
open("1.txt", O_RDWR | O_CREAT, 0644);
```

如果：

```text
1.txt 已存在
→ 打开

1.txt 不存在
→ 创建后再打开
```

这里第三个：

```c
0644
```

就是我们上一节说的：

> 创建新文件时希望赋予的 `mode` 权限。

------

## 4. `O_TRUNC`

这个来自：

```text
truncate
```

英文意思就是：

> **截短、截断。**

所以：

```text
TRUNC
≈ TRUNCate
```

例如原来的：

```text
1.txt

hello world
```

文件长度是 11。

<span style="background:#00FF80; color:#FF0000;">执行：</span>

```c
open("1.txt", O_RDWR | O_TRUNC);
```

<span style="color:#FF0000; background:#00FF80;">成功以后：</span>

```text
1.txt
```

<span style="color:#FF0000; background:#00FF80;">文件还在，但是：</span>

```text
长度 = 0
```

所以你可以非常形象地把：

```text
O_TRUNC
```

记成：

> <span style="background:#00FF80;">**打开时顺手把原文件“截成 0 长度”。**</span>

不是删除文件。

<span style="color:#FF0000; background:#00FF80;">inode/文件本身还存在，只是普通文件内容被清空了。</span>

------

## 5. 所以这三个组合起来到底什么意思？

```c
O_RDWR | O_CREAT | O_TRUNC
```

逐字翻译：

```text
O_RDWR
→ Read + Write
→ 我要读写


O_CREAT
→ Create
→ 不存在就创建


O_TRUNC
→ Truncate
→ 已存在就清空原内容
```

于是：

```c
open("1.txt",
     O_RDWR | O_CREAT | O_TRUNC,
     0644);
```

完整人话就是：

> “Linux，请以可读可写方式打开 `1.txt`。如果它不存在，就创建一个权限模式为 `0644` 的新文件；如果它已经存在，就把原来的文件内容截断为 0，然后打开。”

这个解释基本就是这一行代码的完整含义。

------

## 6. 为什么它们之间使用 `|`？

这里：

```c
O_RDWR | O_CREAT | O_TRUNC
```

的：

```text
|
```

<span style="color:#FF0000; background:#00FF80;">不是逻辑或：</span>

```text
||
```

而是：

> **按位或 Bitwise OR**

<span style="color:#FF0000; background:#00FF80;">因为这些宏内部实际上都是整数位标志。</span>

概念上比如：

```text
O_CREAT = 0000 0100
O_TRUNC = 0000 1000
```

按位或：

```text
0000 0100
|
0000 1000
─────────
0000 1100
```

<span style="color:#FF0000; background:#00FF80;">于是一个整数里面可以同时告诉 Linux：</span>

```text
我要这个功能
+
还要这个功能
+
还要这个功能
```

<span style="color:#FF0000; background:#00FF80;">所以 flags 本质上像一个“功能开关集合”。</span>

不过：

```text
O_RDONLY
O_WRONLY
O_RDWR
```

属于访问模式，三者通常是**三选一**，不能理解成把 `O_RDONLY | O_WRONLY` 拼起来就等于 `O_RDWR`。

------

## 7. 还有哪些常见的 `O_XXX` 宏？

这一组你后面会经常遇到：

| 宏            | 英文          | 字面意思   | 实际作用              |
| ------------- | ------------- | ---------- | --------------------- |
| `O_RDONLY`    | Read Only     | 只读       | 只能读                |
| `O_WRONLY`    | Write Only    | 只写       | 只能写                |
| `O_RDWR`      | Read Write    | 读写       | 可读可写              |
| `O_CREAT`     | Create        | 创建       | 不存在就创建          |
| `O_TRUNC`     | Truncate      | 截断       | 已存在则清空内容      |
| `O_APPEND`    | Append        | 追加       | 每次写到文件末尾      |
| `O_EXCL`      | Exclusive     | 排他的     | 常与 `O_CREAT` 配合   |
| `O_NONBLOCK`  | Non-blocking  | 非阻塞     | 操作不要一直等待      |
| `O_SYNC`      | Synchronous   | 同步       | 强化写入同步要求      |
| `O_CLOEXEC`   | Close on Exec | exec时关闭 | 执行新程序时关闭该 fd |
| `O_DIRECTORY` | Directory     | 目录       | 要求打开对象是目录    |
| `O_NOFOLLOW`  | No Follow     | 不跟随     | 不跟随最终符号链接    |

目前你最需要掌握前 7 个就够了。

------

## 8. `O_APPEND` 特别容易和 `O_TRUNC` 搞混

这两个可以放一起记。

### `O_TRUNC`

```text
Truncate
↓
清空旧内容
↓
从头开始
```

比如：

```text
原文件：

ABCDEF
```

打开：

```c
open(..., O_WRONLY | O_TRUNC);
```

之后：

```text
空文件
```

再写：

```text
123
```

结果：

```text
123
```

------

### `O_APPEND`

```text
Append
↓
追加
↓
每次写到末尾
```

原来：

```text
ABCDEF
```

再写：

```text
123
```

结果：

```text
ABCDEF123
```

所以：

```text
TRUNC = 先清空

APPEND = 后面追加
```

非常好记。

------

## 9. `O_EXCL` 又怎么从英文理解？

```text
EXCL
≈ Exclusive
```

中文：

> 排他的、独占的。

它经常与：

```c
O_CREAT
```

组合：

```c
O_CREAT | O_EXCL
```

意思可以理解成：

> <span style="background:#00FF80; color:#FF0000;">“我要创建这个文件，而且要求这个文件原来必须不存在。”</span>

如果文件已经存在：

```text
open失败
```

<span style="color:#FF0000; background:#00FF80;">这非常适合避免：</span>

> 不小心覆盖已有文件。

所以：

```text
CREATE + EXCLUSIVE
```

可以理解成：

> **只允许我完成一次真正的“新建”。**

------

## 10. `O_NONBLOCK`

拆：

```text
NON
+
BLOCK

=
Non-blocking
```

就是：

> **非阻塞。**

<span style="color:#FF0000; background:#00FF80;">比如以后操作：</span>

```text
串口
管道
socket
设备文件
```

<span style="color:#FF0000; background:#00FF80;">如果暂时没有数据，普通阻塞模式可能：</span>

```text
read()
↓
没有数据
↓
进程等待
```

而 `O_NONBLOCK`：

```text
read()
↓
现在没数据
↓
直接返回
```

这在 Linux 驱动里非常重要。

------

## 11. 你还<span style="background:#00FF80; color:#FF0000;">可以发现 Linux 宏特别喜欢缩写</span>

例如：

```text
RD     = Read
WR     = Write
RDWR   = Read/Write

ONLY   = Only

CREAT  = Create
TRUNC  = Truncate
APPEND = Append
EXCL   = Exclusive
NONBLOCK = Non-blocking
SYNC   = Synchronous
```

所以以后看到：

```c
O_WRONLY | O_CREAT | O_APPEND
```

不用查资料都可以直接读：

```text
Open Write Only
+
Create
+
Append
```

人话：

> **以只写方式打开，不存在就创建，每次写都追加到末尾。**

<span style="color:#FF0000; background:#00FF80;">这就是 Linux 宏名设计得很有意思的地方——英文能拆开的话，大多数功能基本就猜得出来。</span>

------

## 最后给你一个最实用的记忆方式

把 `open()` 想成回答三个问题：

```text
open(path, flags, mode)

① 打开谁？
path


② 怎么打开？
O_RDONLY
O_WRONLY
O_RDWR


③ 还要附加什么行为？
O_CREAT
O_TRUNC
O_APPEND
O_EXCL
O_NONBLOCK
...


④ 如果要新建，新文件权限是什么？
mode
0644 / 0666 / ...
```

所以：

```c
open("1.txt",
     O_RDWR | O_CREAT | O_TRUNC,
     0644);
```

你以后看到它，可以直接在脑子里翻译成：

> **“打开 `1.txt`，读写模式；没有就创建，有就清空；新建时希望权限为 `0644`。”**

这样就不需要把 `O_RDWR/O_CREAT/O_TRUNC` 当成三个莫名其妙的符号去死记了。



# 问题2

off_t lseek(int fd, off_t offset, int whence); 这个函数的作用是什么呢，你能够给我举个例子吗。



可以。你现在已经学到 `open → fd → struct file → f_pos → write` 这条链了，所以 `lseek()` 其实特别好理解：

> <span style="background:#00FF80;">**`lseek()` 的作用就是：修改一个已打开文件的“当前文件位置 `f_pos`”。**</span>
>
> <span style="color:#FF0000; background:#00FF80;">它自己既不读取数据，也不写入数据，只是把“下一次从哪里读/写”的位置改掉。</span>

函数原型：

```c
off_t lseek(int fd, off_t offset, int whence);
```

可以直接理解成：

```text
lseek(
    操作哪个文件,
    移动多少,
    从哪里作为基准开始移动
)
```

------

## 1. 先看一个最简单的例子

假设文件 `1.txt` 内容是：

```text
ABCDEFGHIJ
```

给每个字符标上偏移：

```text
字符：   A B C D E F G H I J
偏移：   0 1 2 3 4 5 6 7 8 9
```

程序：

```c
int fd = open("1.txt", O_RDWR);
```

<span style="color:#FF0000; background:#00FF80;">刚打开时，通常：</span>

```text
f_pos = 0
```

也就是：

```text
ABCDEFGHIJ
↑
当前位置
```

如果直接：

```c
write(fd, "12", 2);
```

会从位置 0 开始覆盖：

```text
原来：
ABCDEFGHIJ

写入：
12

结果：
12CDEFGHIJ
```

<span style="color:#FF0000; background:#00FF80;">并且写完后：</span>

```text
f_pos = 2
```

------

## 2. 如果先使用 `lseek()`

例如：

```c
lseek(fd, 4, SEEK_SET);
```

意思：

> 从文件开头开始，移动到偏移 4。

于是：

```text
A B C D E F G H I J
0 1 2 3 4 5 6 7 8 9
        ↑
      f_pos = 4
```

接着：

```c
write(fd, "12", 2);
```

就从偏移 4 开始写。

原来：

```text
ABCDEFGHIJ
```

变成：

```text
ABCD12GHIJ
```

因为：

```text
E 被 1 覆盖
F 被 2 覆盖
```

然后：

```text
f_pos = 6
```

所以这两句：

```c
lseek(fd, 4, SEEK_SET);
write(fd, "12", 2);
```

连起来就是：

> **先把文件当前位置移动到第 4 个字节，再从那里开始写两个字节。**

------

## 3. 三个参数分别是什么

函数：

```c
off_t lseek(int fd, off_t offset, int whence);
```

### 第一个：`fd`

就是：

```text
file descriptor
文件描述符
```

例如：

```c
int fd = open("1.txt", O_RDWR);
```

得到：

```text
fd = 3
```

那么：

```c
lseek(3, 4, SEEK_SET);
```

就是修改：

```text
fd 3
 ↓
当前进程 fdtable[3]
 ↓
struct file
 ↓
f_pos
```

所以 `lseek()` 真正修改的是：

> **这个 `struct file` 对应的当前文件偏移。**

------

## 4. 第二个参数 `offset`

`offset`：

> **偏移量。**

例如：

```c
lseek(fd, 100, SEEK_SET);
```

这里：

```text
offset = 100
```

表示偏移 100 个字节。

它的类型：

```c
off_t
```

可以理解成：

> Linux 专门用来表示文件位置、文件偏移的一种整数类型。

为什么不用普通 `int`？

因为文件可能非常大，所以文件偏移需要能够表示很大的数。

------

## 5. 第三个参数 `whence` 最重要

`whence` 可以理解成：

> **“从哪个基准位置开始算 offset？”**

常见有三个：

```text
SEEK_SET
SEEK_CUR
SEEK_END
```

<span style="color:#FF0000; background:#00FF80;">这三个一定要掌握。</span>

------

### `SEEK_SET`

英文：

```text
SET
```

可以理解：

> 直接设置。

基准：

> <span style="background:#00FF80; color:#FF0000;">**文件开头。**</span>

例如：

```c
lseek(fd, 5, SEEK_SET);
```

意思：

```text
从文件开头
+
5字节
=
位置5
```

所以：

```text
文件：

A B C D E F G
0 1 2 3 4 5 6
          ↑
        f_pos
```

公式：

```text
新位置 = 0 + offset
```

------

## 6. `SEEK_CUR`

`CUR`：

```text
CURrent
=
当前
```

所以：

> <span style="color:#FF0000; background:#00FF80;">以当前文件位置为基准。</span>

比如现在：

```text
f_pos = 10
```

执行：

```c
lseek(fd, 5, SEEK_CUR);
```

得到：

```text
新位置
=
10 + 5
=
15
```

也就是：

```text
当前位置
   ↓

----------10----------
           |
           | +5
           ↓
----------15----------
```

如果：

```c
lseek(fd, -3, SEEK_CUR);
```

则：

```text
10 - 3 = 7
```

也就是说：

> 可以往前退。

所以 `offset` 可以是：

```text
正数 → 向后
负数 → 向前
```

只要最终位置合法。

------

## 7. `SEEK_END`

`END`：

> 文件末尾。

所以：

```c
lseek(fd, 0, SEEK_END);
```

意思：

> <span style="color:#FF0000; background:#00FF80;">把当前位置移动到文件末尾。</span>

假设文件长度：

```text
100 bytes
```

那么：

```text
f_pos = 100
```

这招特别常见。

例如：

```c
lseek(fd, 0, SEEK_END);
write(fd, "hello", 5);
```

就是：

```text
原文件
ABCDE
     ↑
   文件末尾
     ↓
写hello
```

结果：

```text
ABCDEhello
```

------

## 8. 三个 `whence` 可以直接这样记

| 参数       | 英文    | 基准         |
| ---------- | ------- | ------------ |
| `SEEK_SET` | Set     | 文件开头     |
| `SEEK_CUR` | Current | 当前文件位置 |
| `SEEK_END` | End     | 文件末尾     |

对应公式：

```text
SEEK_SET：
new_pos = offset

SEEK_CUR：
new_pos = current_pos + offset

SEEK_END：
new_pos = file_size + offset
```

------

## 9. 举一个完整程序

假设文件原来：

```text
0123456789
```

代码：

```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd;

    fd = open("1.txt", O_RDWR);

    if (fd < 0)
    {
        perror("open");
        return -1;
    }

    lseek(fd, 3, SEEK_SET);

    write(fd, "ABC", 3);

    close(fd);

    return 0;
}
```

执行过程：

```text
原文件：

0 1 2 3 4 5 6 7 8 9
      ↑
```

执行：

```c
lseek(fd, 3, SEEK_SET);
```

于是：

```text
f_pos = 3
```

然后：

```c
write(fd, "ABC", 3);
```

覆盖：

```text
3 → A
4 → B
5 → C
```

最终：

```text
012ABC6789
```

注意不是：

```text
012ABC3456789
```

因为：

> `write()` 是覆盖，不是插入。

------

## 10. `lseek()` 的返回值是什么？

它的返回类型：

```c
off_t
```

成功时：

> <span style="color:#FF0000; background:#00FF80;">**返回修改后的新文件位置。**</span>

例如：

```c
off_t pos;

pos = lseek(fd, 100, SEEK_SET);
```

<span style="color:#FF0000; background:#00FF80;">成功以后：</span>

```text
pos = 100
```

所以：

```c
printf("pos = %ld\n", (long)pos);
```

可能输出：

```text
pos = 100
```

失败：

```text
返回 -1
```

并设置：

```text
errno
```

所以常见写法：

```c
off_t pos;

pos = lseek(fd, 100, SEEK_SET);

if (pos == (off_t)-1)
{
    perror("lseek");
}
```

------

## 11. `lseek()` 还能用来获得当前文件位置

这个挺有意思。

如果：

```c
lseek(fd, 0, SEEK_CUR);
```

<span style="background:#00FF80; color:#FF0000;">什么意思？</span>

```text
以当前位置为基准
+
移动0
```

那当然：

```text
位置没变
```

<span style="color:#FF0000; background:#00FF80;">但是 `lseek()` 会把当前位置返回给你。</span>

所以：

```c
off_t pos;

pos = lseek(fd, 0, SEEK_CUR);
```

就可以理解成：

> “告诉我现在的 `f_pos` 是多少。”

例如：

```text
pos = 128
```

------

## 12. 还可以用来获得文件长度

一个很常见的小技巧：

```c
off_t size;

size = lseek(fd, 0, SEEK_END);
```

假设文件长度：

```text
1000 bytes
```

那么：

```text
文件末尾偏移 = 1000
```

于是：

```text
size = 1000
```

所以可以大致获得文件长度。

当然在实际程序中，查询文件属性通常也可以使用：

```c
stat()
fstat()
```

这往往更合适。

------

## 13. 甚至可以移动到文件末尾之后

比如原文件只有：

```text
10 bytes
```

你：

```c
lseek(fd, 1000, SEEK_SET);
```

然后：

```c
write(fd, "A", 1);
```

可能形成：

```text
原来数据
↓
0 ~ 9

10 ~ 999
↓
中间形成一个“空洞”

1000
↓
A
```

这种文件叫：

> **sparse file，稀疏文件。**

逻辑文件大小可能变成：

```text
1001 bytes
```

但中间那一大块“洞”不一定真的占用等量磁盘块。

这是 `lseek()` 后面一个很有意思的用途。

------

## 14. `lseek()` 不是所有 fd 都能用

比如普通磁盘文件：

```text
普通文件
→ 可以seek
```

但是：

```text
pipe
FIFO
某些socket
```

这种本质上是：

```text
数据流
```

没有：

```text
“第100个字节，我跳过去”
```

这种概念。

所以：

```c
lseek(pipe_fd, ...);
```

通常会失败。

比如错误：

```text
ESPIPE
Illegal seek
```

这也很好理解：

> 文件像一本书，可以翻到第 100 页；
> 管道更像水管，你没法说“我要跳到水管里昨天流过去的第 100 个字节”。

------

## 15. 结合你前面学的 `struct file`

现在你可以把这三个函数放到同一张图里：

```text
open()
  ↓
创建/获得 struct file
  ↓
f_pos = 初始位置
  ↓
返回 fd
```

之后：

```text
              fd
               │
               ▼
            fdtable
               │
               ▼
          struct file
               │
               ├── f_flags
               │
               └── f_pos
                    │
        ┌───────────┼───────────┐
        │                       │
      read()                  write()
        │                       │
 从f_pos开始读取          从f_pos开始写入
        │                       │
        └───────────┬───────────┘
                    ↓
                 推进f_pos


lseek()
   │
   └──────────────→ 主动修改f_pos
```

所以一句话：

> **`read/write` 是“使用并推进当前位置”，`lseek` 是“主动修改当前位置”。**

------

### 16. 还有一个容易混淆的特殊情况：`O_APPEND`

如果：

```c
fd = open("1.txt", O_WRONLY | O_APPEND);
```

`APPEND`：

> 追加模式。

这时候每次 `write()` 都会把写入位置放到文件末尾。

所以即使你：

```c
lseek(fd, 0, SEEK_SET);
```

然后：

```c
write(fd, "ABC", 3);
```

对于 `O_APPEND` 打开的普通文件，`write()` 仍会按追加语义写到文件末尾，而不是老老实实在开头覆盖。

所以：

```text
普通打开：
lseek决定下一次write位置

O_APPEND：
write时会按追加语义写到文件末尾
```

这个以后遇到日志文件时特别重要。

------

## 最后用一个生活类比记住

把文件想象成一本有编号的练习本：

```text
0 1 2 3 4 5 6 7 8 ...
```

`f_pos`：

> 当前笔尖停在哪。

`read()`：

> 从笔尖当前位置往后看，然后笔尖向后移动。

`write()`：

> 从笔尖当前位置开始写，然后笔尖向后移动。

`lseek()`：

> **直接把笔尖拿起来，放到你指定的位置。**

所以：

```c
lseek(fd, 10, SEEK_SET);
```

就是：

> “把笔尖直接放到从文件开头数第 10 个字节的位置。”

然后下一次 `read()` 或 `write()` 就从那里开始。

你把 `lseek()` 记成 **“修改 `struct file` 中的文件读写指针 `f_pos`”**，后面基本就不会忘了。





# 问题3

这是你现在学习 Linux 特别值得提前建立的概念：

```
write成功
```

主要意味着：

> Linux 已经接受了这次写请求，对文件内容的逻辑修改已经完成到系统要求的程度。

不一定意味着：

```
电子已经写入 NAND/eMMC
```

因为中间还有：

```
Page Cache
文件系统
块设备缓存
设备本身缓存
```

等层次。



后面学：

```
fsync
sync
```

时就会继续接上。

这里要怎么理解呢，我不是太懂，你能够给我解释下吗





可以，这里最关键的是把一句话真正吃透：

> **`write()` 成功，通常表示“数据已经交给 Linux 内核了”，但不一定表示“数据已经真正落到 eMMC/SSD/NAND 的非易失介质里了”。**

Linux 中间故意加了很多“缓存层”，目的就是提高性能。

先看最核心的一条链：

```text
你的 APP
   │
   │ write(fd, buf, 100)
   ▼
用户空间 buf
   │
   ▼
Linux 内核
   │
   ▼
Page Cache（RAM）
   │
   │ 标记为 dirty
   ▼
稍后进行 writeback
   │
   ▼
文件系统
   │
   ▼
块设备层
   │
   ▼
eMMC / SSD 控制器
   │
   ▼
设备内部缓存
   │
   ▼
真正的 Flash / NAND
```

也就是说：

> `write()` 返回成功的时候，很多时候数据才走到前半程。

------

## 1. 为什么 Linux 不让 `write()` 每次都真的写 Flash？

假设你连续执行：

```c
write(fd, "A", 1);
write(fd, "B", 1);
write(fd, "C", 1);
write(fd, "D", 1);
```

如果每次 `write()` 都必须：

```text
APP
↓
Linux
↓
eMMC控制器
↓
真正写Flash
↓
等Flash写完
↓
返回APP
```

那程序会非常慢。

因为 RAM 操作可能是纳秒级到更高量级，而真正存储设备写入通常慢很多。

所以<span style="color:#FF0000; background:#00FF80;"> Linux 的思路是：</span>

> <span style="background:#00FF80;">“你先把数据交给我，我先放 RAM 里，我以后找合适的时机批量写到磁盘。”</span>

于是：

```text
APP：

“Linux，帮我写100字节。”

Linux：

“好，我先记在内存里，
这次write算成功了，
你继续干别的吧。”
```

------

## 2. Page Cache 是什么？

你可以把：

```text
Page Cache
```

理解成：

> <span style="color:#FF0000; background:#00FF80;">**Linux 内核拿一部分 RAM，当作文件内容的缓存。**</span>

例如磁盘上的：

```text
hello.txt
```

内容：

```text
ABCDEFG
```

当程序读取它以后，这些内容可能被缓存到 RAM：

```text
物理存储
hello.txt
ABCDEFG
   │
   ▼
Page Cache
ABCDEFG
```

之后再次：

```c
read(fd, ...)
```

如果 Page Cache 已经有：

```text
ABCDEFG
```

就不用再读 eMMC。

直接：

```text
RAM → APP
```

所以 Page Cache 不只对写有用，对读也特别重要。

------

## 3. `write()` 的时候 Page Cache 怎么工作？

假设：

```c
char buf[] = "HELLO";

write(fd, buf, 5);
```

可以粗略理解成：

```text
用户空间

buf
"HELLO"
   │
   │ write系统调用
   ▼

内核空间

Page Cache
"HELLO"
```

Linux 把对应页面修改以后，会给它打一个标记：

```text
dirty
```

中文经常叫：

> **脏页**

------

## 4. “脏”是什么意思？

非常形象。

假设磁盘中原来：

```text
磁盘：
AAAAA
```

Page Cache 中被你修改：

```text
RAM：
HELLO
```

那么此时：

```text
RAM里的版本
≠
磁盘里的版本
```

于是 Linux 说：

> “这块缓存被改过了，但修改还没同步到底层设备。”

所以标记：

```text
dirty
```

也就是：

```text
Page Cache：

HELLO
状态：dirty
```

------

## 5. 所以 `write()` 返回时可能是什么状态？

例如：

```c
ret = write(fd, "HELLO", 5);
```

得到：

```text
ret = 5
```

这通常表示：

> Linux 已经接受并处理了这 5 字节写入请求。

但此时可能：

```text
用户空间：
已经认为write完成

        ↓

Page Cache：
HELLO
dirty

        ↓

eMMC：
还是旧内容 AAAAA
```

所以：

```text
write返回5
```

不能简单翻译成：

> “HELLO 已经烧进 Flash 芯片。”

------

## 6. 那什么时候才真正写到设备？

Linux 会在之后进行：

```text
writeback
```

可以理解为：

> **把脏的 Page Cache 页面回写到底层存储设备。**

例如：

```text
Page Cache

HELLO
dirty
   │
   │ writeback
   ▼
文件系统
   ▼
块设备层
   ▼
eMMC驱动
   ▼
eMMC控制器
   ▼
存储设备
```

完成以后：

```text
Page Cache：
HELLO
clean

磁盘：
HELLO
```

这时候两边重新一致。

------

## 7. Linux 为什么要延迟写？

这有很多好处。

<span style="color:#FF0000; background:#00FF80;">例如程序连续：</span>

```c
write(fd, "A", 1);
write(fd, "B", 1);
write(fd, "C", 1);
...
```

<span style="background:#00FF80;">Linux可以先在 RAM 里合并。</span>

<span style="color:#FF0000; background:#00FF80;">原本可能：</span>

```text
100次小write
```

<span style="color:#FF0000; background:#00FF80;">最终变成：</span>

```text
少量较大的设备I/O
```

效率高很多。

<span style="color:#FF0000; background:#00FF80;">还可以：</span>

```text
重新排序I/O
合并连续块
减少设备访问次数
```

所以这叫：

> **buffered I/O / write-back caching**

<span style="color:#FF0000;">是操作系统提升磁盘性能的重要手段。</span>

------

## 8. 如果突然断电会发生什么？

这就是问题所在。

假设：

```c
write(fd, "重要数据", ...);
```

已经返回成功。

现在状态：

```text
RAM Page Cache：
重要数据
dirty

eMMC：
还是旧数据
```

突然：

```text
断电
```

RAM 是易失的：

```text
Page Cache
↓
消失
```

于是刚刚的修改可能丢失。

所以：

> <span style="background:#00FF80;">**`write()` 成功主要保证写请求已经进入操作系统管理范围，并不等于已经具备掉电持久性。**</span>

------

## 9. `fsync()` 就是在解决这个问题

函数大致：

```c
int fsync(int fd);
```

你可以理解成：

> <span style="background:#00FF80; color:#FF0000;">**“Linux，请把这个 fd 对应文件尚未落盘的数据和必要元数据同步到底层存储，并等它完成。”**</span>

例如：

```c
write(fd, buf, 100);

fsync(fd);
```

过程：

```text
write()
↓
数据进入Page Cache
↓
dirty


fsync(fd)
↓
找到fd
↓
struct file
↓
inode / 文件系统
↓
把这个文件的dirty数据写回
↓
必要文件元数据也同步
↓
提交到底层设备
↓
等待完成
↓
fsync返回
```

所以：

```text
write
```

更像：

> “把修改交给 Linux。”

而：

```text
fsync
```

更像：

> “现在别再拖了，把这个文件的修改真正同步下去。”

------

## 10. 一个具体例子

假设程序：

```c
int fd = open("money.txt",
              O_WRONLY | O_CREAT | O_TRUNC,
              0644);

write(fd, "10000", 5);

close(fd);
```

你可能以为：

```text
write成功
+
close成功
=
10000肯定已经在eMMC
```

实际上不能完全这么保证。

更安全的关键数据场景可能：

```c
write(fd, "10000", 5);

fsync(fd);

close(fd);
```

这时候才主动要求：

```text
Page Cache中的修改
↓
尽快同步到底层稳定存储
```

------

## 11. `close()` 为什么不等于 `fsync()`？

这是一个特别容易误会的点。

```c
close(fd);
```

主要作用：

> <span style="color:#FF0000; background:#00FF80;">关闭这个文件描述符，释放这次打开相关的内核引用。</span>

它并不是：

> “强制把所有脏数据彻底同步到存储设备。”

所以：

```text
close()
≠
fsync()
```

不要因为文件关了就默认：

```text
所有数据都已经物理落盘
```

------

## 12. 那 `sync()` 又是什么？

你可以把：

```c
fsync(fd)
```

理解为：

> **同步一个指定文件。**

而：

```c
sync()
```

更像：

> **让系统把大量/所有待回写的脏数据同步下去。**

粗略：

```text
fsync(fd)
↓
这个文件


sync()
↓
整个系统范围的dirty数据
```

所以：

```text
fsync
```

比较精确。

```text
sync
```

范围更大。

------

## 13. `fdatasync()` 又是什么？

以后你可能还会看到：

```c
fdatasync(fd);
```

可以先理解成：

> 更偏重把“文件数据本身”和必要元数据同步下去。

而：

```c
fsync(fd);
```

通常要求：

> 文件数据 + 与其持久化相关的元数据。

你目前先掌握 `fsync()` 就够了。

------

## 14. 文件系统这一层又在干什么？

假设你写：

```c
write(fd, "HELLO", 5);
```

文件系统不能只想着：

```text
把HELLO放哪
```

还可能要维护：

```text
文件大小
时间戳
inode信息
数据块映射
目录项
日志
```

例如文件原来：

```text
size = 0
```

写 5 字节后：

```text
size = 5
```

这也是元数据变化。

所以真正持久化一个文件，不只是：

```text
写文件内容
```

还可能有：

```text
写文件元数据
```

这也是为什么 `fsync()` 不只是简单一句：

> 把 Page Cache 搬到磁盘。

中间还涉及具体文件系统。

------

## 15. journaling 文件系统又多一层

比如：

```text
ext4
```

可能启用了 journaling。

为了避免突然断电导致文件系统结构完全损坏，它可能先写：

```text
journal / 日志
```

然后再更新实际文件系统结构。

概念上：

```text
write
↓
Page Cache
↓
ext4
↓
journal相关处理
↓
实际数据/元数据
↓
块设备
```

所以 Linux 的文件写入链比：

```text
APP → 磁盘
```

复杂很多。

------

## 16. “块设备缓存”是什么？

再往下一层。

文件系统产生磁盘 I/O 后，会进入：

```text
Linux block layer
```

这里负责：

```text
块I/O请求
调度
合并
提交到设备驱动
```

概念：

```text
ext4
↓
block layer
↓
MMC/NVMe/SATA driver
↓
硬件控制器
```

所以 Page Cache 不是唯一的一层。

------

## 17. “设备本身缓存”又是什么？

即使 Linux 已经把数据交给：

```text
SSD
eMMC
硬盘
```

设备自己内部也可能还有：

```text
DRAM cache
SRAM buffer
控制器缓存
```

所以可能出现：

```text
Linux：
“我已经发给设备了。”

设备：
“收到，我先放我自己的缓存。”
```

然后设备内部再真正写：

```text
NAND Flash
磁盘盘片
```

因此要想真正做到：

> 掉电也绝不丢

底层设备还必须正确支持和执行：

```text
flush / cache flush
```

一类命令。

Linux 的 `fsync()` 最终会通过文件系统和设备栈去请求相应的持久化保证，但最终可靠性也依赖设备是否正确实现这些语义。

------

## 18. 所以完整的“写入”其实有好几个完成程度

你可以把它看成四级。

### 第一级：APP 调用了 `write`

```text
buf
↓
write()
```

------

### 第二级：Linux 接受数据

```text
Page Cache
dirty
```

此时：

```text
write可能已经返回成功
```

------

### 第三级：Linux 把数据提交给设备

```text
Page Cache
↓
文件系统
↓
块设备
↓
设备驱动
```

------

### 第四级：设备真正持久化

```text
设备缓存
↓
NAND / eMMC / 磁盘介质
```

最终：

> 掉电以后仍然存在。

------

## 19. 用“快递”来类比特别好理解

假设你要寄一份文件。

### `write()`

像：

> 你把快递交给小区快递站。

快递站说：

```text
“收到了。”
```

这相当于：

```text
write成功
```

但：

> 包裹还没有到收件人手里。

------

### Page Cache

就是：

```text
小区快递站仓库
```

暂时先屯着。

------

### writeback

相当于：

```text
快递车统一来拉货
```

批量运出去。

------

### 设备缓存

像：

```text
目的地配送中心
```

已经很接近终点，但还没有真正交到收件人。

------

### 真正写入 NAND/eMMC

才相当于：

```text
收件人已经签收
```

------

### `fsync()`

就像你跟快递公司说：

> **“这个包裹非常重要，不要再屯仓库了，现在给我送到最终目的地，送完再告诉我。”**

这个类比基本就能抓住本质。

------

## 20. 和前面学的 `fflush()` 也能串起来

假设你不是直接：

```c
write()
```

而是标准 IO：

```c
fprintf(fp, "hello");
```

还多了一层：

```text
APP
↓
stdio用户空间Buffer
↓
Page Cache
↓
存储设备
```

所以：

```c
fflush(fp);
```

主要做：

```text
stdio Buffer
↓
write系统调用
↓
Linux Page Cache
```

但是：

```text
fflush成功
```

仍然不一定意味着数据已经落盘。

如果特别需要持久化：

```c
fflush(fp);

fsync(fileno(fp));
```

概念上：

```text
用户空间stdio Buffer
   │
   │ fflush
   ▼
Linux Page Cache
   │
   │ fsync
   ▼
底层存储
```

这正好把你前面学的：

```text
标准IO
vs
系统调用IO
```

也串起来了。

------

## 21. `O_SYNC` 又是什么？

你前面刚问过 `O_XXX`。

还有：

```c
O_SYNC
```

它的思路就是：

> 让这个 fd 的写操作具有更强的同步语义。

例如：

```c
open("1.txt",
     O_WRONLY | O_CREAT | O_SYNC,
     0644);
```

这样每次：

```c
write()
```

通常会比普通 buffered write 更接近：

```text
写完并完成要求的同步
↓
再返回
```

代价就是：

> **慢很多。**

因为你失去了“先缓存、以后批量写”的性能优势。

------

## 22. 为什么数据库特别在乎这些东西？

比如银行系统：

```text
余额 = 10000
↓
扣款500
↓
余额 = 9500
```

如果程序：

```c
write(fd, "9500", 4);
```

返回以后立刻告诉用户：

```text
“交易成功”
```

但数据其实只在 RAM Page Cache。

这时断电：

```text
RAM消失
↓
磁盘可能还是10000
```

那就麻烦了。

所以数据库会非常认真地处理：

```text
日志
fsync
事务
WAL
journaling
```

等持久化机制。

------

## 23. 但不是说每次写文件都要 `fsync()`

也千万别走到另一个极端。

比如写一个：

```text
普通日志
临时缓存
下载中的临时文件
```

每写几个字节就：

```c
fsync(fd);
```

性能可能非常差。

所以：

```text
普通情况
→ 让Linux缓存优化

关键持久数据
→ 在适当事务边界fsync
```

这是工程中的权衡。

------

## 24. 最后把这几个函数一次区分清楚

| 操作        | 主要作用                                                     |
| ----------- | ------------------------------------------------------------ |
| `write()`   | 把用户数据交给内核文件系统                                   |
| `fflush()`  | <span style="color:#FF0000; background:#00FF80;">把 C 标准库用户空间 Buffer 往内核送</span> |
| `fsync(fd)` | <span style="color:#FF0000; background:#00FF80;">强制同步指定文件的数据及必要元数据</span> |
| `sync()`    | 推动整个系统范围的脏数据同步                                 |
| `close(fd)` | 关闭 fd，不等价于强制持久化                                  |
| `O_SYNC`    | 让写操作使用更强的同步写语义                                 |

可以记成：

```text
fprintf/fwrite
↓
【用户空间stdio Buffer】

fflush
↓
write
↓
【Linux Page Cache】

fsync
↓
【文件系统 / 块设备 / 设备】

最终
↓
【非易失存储介质】
```

## 一句话记忆

> **`write()` 更像“我已经把数据交给 Linux 了”；`fsync()` 更像“Linux，现在把这个文件的修改真正往持久存储同步，并等到该同步完成后再回来。”**

而所谓：

```text
write成功 ≠ 已经真正写进eMMC
```

本质原因就是：

> **Linux 为了性能，把“程序完成写请求”和“硬件最终完成持久化”这两个时间点故意分开了。**





# 问题4

这里好像牵涉到用户空间buffer往这个进程对应的内核空间送，然后这个内核空间中的数据往硬盘上送是吗？



对，你的方向是对的，但“**这个进程对应的内核空间**”这句话需要稍微修正一下。

更准确地说：

> <span style="background:#00FF80;">**用户空间里的数据，先通过系统调用进入 Linux 内核管理的缓存/文件系统；然后内核再在合适的时候把这些数据写到硬盘、eMMC、SSD 等持久化设备。**</span>

不是“每个进程都有一块独立的内核空间 buffer”。

可以画成：

```text
进程用户空间
┌─────────────────────┐
│ buf                  │
│ "hello"              │
└─────────┬───────────┘
          │
          │ write(fd, buf, 5)
          ▼
════════ 用户态 / 内核态 ════════
          ▼
Linux 内核
┌─────────────────────┐
│ Page Cache           │
│ dirty page           │
└─────────┬───────────┘
          │
          │ writeback
          ▼
文件系统 / 块设备层
          │
          ▼
设备驱动
          │
          ▼
eMMC / SSD / 硬盘
```

## 1. 第一段：用户空间 → 内核

比如：

```c
char buf[] = "hello";

write(fd, buf, 5);
```

此时：

```text
buf
```

是在这个进程自己的**用户虚拟地址空间**里的。

`write()` 发生后：

```text
用户空间 buf
      ↓
系统调用
      ↓
进入内核态
      ↓
Linux 根据 fd 找到文件
```

也就是：

```text
fd
↓
当前进程的文件描述符表
↓
struct file
↓
inode / 文件系统
```

<span style="color:#FF0000; background:#00FF80;">然后 Linux 会把你要写的数据交给内核的文件缓存体系。</span>

对于普通 buffered I/O，可以粗略理解为：

```text
用户 buf
↓
复制/处理
↓
Page Cache
```

------

## 2. Page Cache 属于谁？

这里特别重要。

<span style="color:#FF0000; background:#00FF80;">它不是：</span>

```text
Process A 的专属 kernel buffer
```

<span style="color:#FF0000; background:#00FF80;">也不是：</span>

```text
Process B 的专属 kernel buffer
```

而是：

> <span style="background:#00FF80; color:#FF0000;">**Linux 内核统一管理的文件缓存。**</span>

所以更准确的图是：

```text
Process A 用户空间
       │
       └──── write ────┐

Process B 用户空间     │
       │               ▼
       └──── write → Linux Kernel
                         │
                         ▼
                    Page Cache
```

<span style="color:#FF0000; background:#00FF80;">不同进程都可能通过内核访问同一个文件对应的 page cache。</span>

------

## 3. “当前进程”主要体现在哪里？

虽然 Page Cache 不是这个进程私有的，但 Linux 必须知道：

> 现在是谁在调用 `write()`？

所以会用当前进程的：

```text
task_struct
files_struct
fd table
credentials
kernel stack
```

这些信息。

例如：

```text
当前 Process A
↓
fd = 3
↓
Process A 的 fdtable[3]
↓
struct file
↓
hello.txt
```

所以：

> **“用哪个 fd 找哪个文件”是进程相关的。**

但：

> **文件缓存 Page Cache 本身是内核统一管理的，不是进程私有区。**

------

## 4. 第二段：内核 → 存储设备

`write()` 成功后，可能先变成：

```text
Page Cache
状态：dirty
```

例如：

```text
RAM中的Page Cache：
HELLO

磁盘：
旧内容
```

然后内核以后做：

```text
writeback
```

也就是：

```text
Page Cache
↓
文件系统
↓
块设备层
↓
设备驱动
↓
控制器
↓
eMMC / SSD / 硬盘
```

这一步才是真正往持久化设备方向走。

------

## 5. 所以可以理解成“两次跨越”

你可以这么记，很清楚：

### 第一次

```text
APP用户空间
↓
Linux内核
```

靠：

```text
write()
```

------

### 第二次

```text
Linux内核缓存
↓
存储设备
```

靠：

```text
writeback / fsync / 内核回写机制
```

所以：

```text
用户buf
   ↓
write
   ↓
内核Page Cache
   ↓
writeback
   ↓
磁盘/eMMC
```

------

## 6. 如果是标准 I/O，还多一层

如果不是：

```c
write()
```

而是：

```c
fwrite()
fprintf()
```

那更完整：

```text
APP
│
▼
C库 stdio Buffer
│
│ fflush / 缓冲满
▼
write()
│
▼
Linux Page Cache
│
│ fsync / writeback
▼
文件系统
│
▼
存储设备
```

所以这里其实有两种 buffer：

```text
用户空间：
stdio buffer

内核空间：
Page Cache
```

它们不是一个东西。

------

## 7. 一个具体例子

比如：

```c
FILE *fp = fopen("1.txt", "w");

fprintf(fp, "hello");
```

可能只是到了：

```text
用户空间 stdio Buffer
```

甚至还没进入内核。

然后：

```c
fflush(fp);
```

数据进一步：

```text
stdio Buffer
↓
write()
↓
Page Cache
```

此时：

```text
write已经发生
```

但仍可能没进 eMMC。

再：

```c
fsync(fileno(fp));
```

才进一步要求：

```text
Page Cache
↓
文件系统
↓
设备
```

进行持久化同步。

------

## 8. 最后给你一个最准确的版本

你刚才的理解：

> “用户空间 buffer 往这个进程对应的内核空间送，然后内核空间中的数据往硬盘送。”

建议改成：

> <span style="background:#00FF80; color:#FF0000;">**用户进程中的 buffer 通过 `write()` 系统调用把数据交给 Linux 内核；Linux 根据当前进程的 fd 表找到目标文件，并把数据放入内核统一管理的文件缓存（如 Page Cache）。之后内核再通过文件系统、块设备层和驱动，把脏数据回写到 eMMC、SSD 或硬盘。**</span>

最核心的链就是：

```text
用户空间 buf
↓
write()
↓
Linux Page Cache
↓
文件系统
↓
块设备层
↓
驱动
↓
存储设备
```

而不是：

```text
用户空间
↓
“这个进程专属的内核空间”
↓
硬盘
```

因为 Linux 的**内核空间是系统级的**，只是内核会维护很多“属于这个进程”的对象，比如 fd 表、`task_struct`、内核栈等。
