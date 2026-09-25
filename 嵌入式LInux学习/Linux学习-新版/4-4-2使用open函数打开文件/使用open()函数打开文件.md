# 视频分析：P41 4_4-2 使用 `open` 函数打开文件

这节课约 **17 分 45 秒**。它和你刚才问的“`open()` 进入内核后到底发生什么”衔接得非常好。

这节视频更偏**应用层实验**：上一节告诉你 `open()` 属于系统调用 I/O，这一节真正写程序，然后通过 **`errno`、`perror`、`/proc/<pid>/fd`** 把 `open()` 的结果“看得见”。

------

## 1. 视频类型判断

**主要类型：教程类 / 讲解类**
**次要类型：Linux 系统调用实验、代码实操**

判断依据：

1. 从 `man` 手册查询 `open()` 函数原型开始。
2. 现场编写 `open.c`，通过命令行参数指定要打开的文件。
3. 实际制造“文件不存在”“权限不足”等错误。
4. 最后进入 `/proc/<PID>/fd`，直接观察进程的文件描述符。

------

## 2. 一句话总结

这节视频的主线就是：

> **程序通过 `open(文件名, flags)` 请求 Linux 打开文件；成功时内核返回一个非负的文件描述符 `fd`，失败时返回 `-1` 并设置 `errno`；我们还能通过 `/proc/<pid>/fd` 看到这个 fd 当前到底指向哪个文件。**

如果结合上一节，你可以理解成：

```text
open("1.txt", O_RDWR)
        ↓
系统调用
        ↓
Linux内核找到文件、检查权限
        ↓
建立打开文件相关内核对象
        ↓
放入当前进程的 fd 表
        ↓
例如分配 fd = 3
        ↓
返回 APP
```

------

## 3. 时间段拆解

| 时间段      | 主要内容                                       | 这一段真正要理解什么                           |
| ----------- | ---------------------------------------------- | ---------------------------------------------- |
| 00:00-01:45 | 查看教材中的 `open()` 原型、进入 `man` 手册    | `open()` 是系统调用接口，学会自己查手册        |
| 01:45-03:30 | 创建 `open.c`，准备头文件和 `main(argc, argv)` | 用命令行参数把文件名传给程序                   |
| 03:30-06:10 | 编写参数检查和 `open(argv[1], O_RDWR)`         | `argv[1]` 就是要打开的文件路径                 |
| 06:10-09:50 | 研究 `errno`、`strerror()`、`perror()`         | 系统调用失败以后怎样知道“为什么失败”           |
| 09:50-12:30 | 完善错误处理代码                               | `fd < 0` 时输出具体错误                        |
| 12:30-14:00 | 编译运行，测试文件不存在等情况                 | `open()` 失败，返回值和错误信息如何体现        |
| 14:00-16:30 | 用 `ps`、`/proc/<pid>/fd` 查看进程             | 实际验证 fd 0、1、2、3                         |
| 16:30-17:45 | 修改文件权限并再次测试                         | 权限检查失败时 `errno=13`，`Permission denied` |

时间点为按视频实际操作节奏划分，个别切换点可能有十几秒左右误差。

------

## 4. 开头首先讲：`open()` 到底长什么样

视频里展示的函数原型大致是：

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int open(const char *pathname, int flags);

int open(const char *pathname, int flags, mode_t mode);
```

现在先看最基本的两个参数：

```text
open(pathname, flags)
      │         │
      │         └── 我要以什么方式打开
      │
      └── 我要打开哪个文件
```

例如：

```c
open("1.txt", O_RDWR);
```

意思就是：

```text
文件：
1.txt

打开方式：
O_RDWR
= Read + Write
= 可读可写
```

------

## 5. 视频中的 `O_RDWR` 到底是什么

课件里还能看到很多 `flags`：

```text
O_RDONLY    只读
O_WRONLY    只写
O_RDWR      可读可写

O_APPEND    追加
O_CREAT     不存在就创建
O_EXCL      配合 O_CREAT 使用
O_TRUNC     截断原文件
O_NONBLOCK  非阻塞
...
```

这节实验重点使用：

```c
O_RDWR
```

也就是说它提出的请求是：

> “Linux，我希望以**读写权限**打开这个文件。”

这件事会直接影响后面内核的权限检查。

例如：

```text
文件权限：
-r--r--r--

APP：
open("1.txt", O_RDWR)
```

你的请求里包含“我要写”。

但文件没有写权限。

那么内核可能判断：

```text
不允许
↓
open失败
↓
返回 -1
↓
errno = EACCES
```

这也是视频最后测试权限错误的意义。

------

## 6. `argc` 和 `argv` 这一段也很重要

视频没有把文件名写死：

```c
open("1.txt", O_RDWR);
```

而是设计成：

```c
open(argv[1], O_RDWR);
```

这样程序可以：

```bash
./open 1.txt
```

也可以：

```bash
./open open.c
```

甚至：

```bash
./open /etc/passwd
```

所以：

```text
./open 1.txt
```

进入 `main()` 后：

```text
argc = 2

argv[0] = "./open"

argv[1] = "1.txt"
```

这就是视频代码注释里专门写的：

```c
/*
 * ./open 1.txt
 *
 * argc = 2
 * argv[0] = "./open"
 * argv[1] = "1.txt"
 */
```

------

## 7. 为什么先判断 `argc != 2`

代码类似：

```c
if (argc != 2)
{
    printf("Usage: %s <file>\n", argv[0]);
    return -1;
}
```

意思是：

> <span style="background:#00FF80; color:#FF0000;">这个程序规定：你必须给我一个文件名。</span>

正常：

```bash
./open 1.txt
```

有：

```text
程序名 + 1个参数
```

所以：

```text
argc = 2
```

如果你只输入：

```bash
./open
```

那么：

```text
argc = 1
```

<span style="color:#FF0000;">此时根本不存在合法的：</span>

```c
argv[1]
```

<span style="color:#FF0000;">所以不能继续：</span>

```c
open(argv[1], ...)
```

于是先提示：

```text
Usage: ./open <file>
```

这是非常典型的 Linux C 程序写法。

------

## 8. 核心代码：`fd = open(argv[1], O_RDWR)`

视频最后形成的核心逻辑大概就是：

```c
int fd;

fd = open(argv[1], O_RDWR);

if (fd < 0)
{
    ...
}
```

这个：

```c
fd
```

就是：

> **file descriptor，文件描述符。**

### 成功

可能：

```text
fd = 3
```

### 失败

```text
fd = -1
```

所以：

```c
if (fd < 0)
```

其实就是在判断：

> `open()` 有没有成功。

------

## 9. 为什么视频里经常得到 `fd = 3`

这是整节课我认为最值得你注意的地方之一。

一个普通 Linux 进程启动时，通常已经有：

```text
fd 0 → 标准输入 stdin
fd 1 → 标准输出 stdout
fd 2 → 标准错误 stderr
```

所以：

```text
0、1、2
```

<span style="color:#FF0000; background:#00FF80;">通常已经被占用了。</span>

然后你：

```c
fd = open("open.c", O_RDWR);
```

Linux 通常寻找：

> <span style="background:#00FF80;">**当前进程文件描述符表中最小的未使用编号。**</span>

于是：

```text
0   已使用
1   已使用
2   已使用
3   空闲
```

所以：

```text
fd = 3
```

------

## 10. 这里要和我们刚才讲的内核过程连起来

你现在看到：

```c
fd = open("open.c", O_RDWR);
```

表面只是一行代码。

底下其实可以展开成：

```text
APP

open("open.c", O_RDWR)
        │
        ▼
libc / 系统调用接口
        │
        ▼
CPU进入内核态
        │
        ▼
Linux VFS
        │
        ▼
找到 open.c
        │
        ▼
检查：
文件存在吗？
权限允许 O_RDWR 吗？
        │
        ▼
建立 struct file
        │
        ▼
当前进程的 fd table

0 → stdin
1 → stdout
2 → stderr
3 → 新的 struct file
        │
        ▼
返回 3
        │
        ▼
APP

fd = 3
```

所以 `fd=3` **不是 Linux 随便给的数字**。

它背后真的有一张属于这个进程的表。

------

## 11. 视频后半段直接把这张表“看出来了”

大约 **14:00 之后**，讲师先让程序保持运行，然后：

```bash
ps
```

<span style="color:#FF0000; background:#00FF80;">找到程序的 PID。</span>

例如视频中出现过类似：

```text
PID    CMD

4438   open
```

然后：

```bash
cd /proc/4438/fd
```

再：

```bash
ls -l
```

你会看到类似：

```text
0 -> /dev/pts/0
1 -> /dev/pts/0
2 -> /dev/pts/0
3 -> /home/book/fileio/01_open/open.c
```

这个实验特别漂亮。

因为<span style="color:#FF0000; background:#00FF80;">它直接证明：</span>

```text
程序里的 fd = 3

并不是抽象理论。

Linux真的能告诉你：

这个进程的 3
当前对应的是 open.c
```

------

## 12.`/proc/<pid>/fd` 到底是什么

这里我给你展开一下。

<span style="color:#FF0000; background:#00FF80;">Linux 有一个特殊的：</span>

```text
/proc
```

它并不是普通意义上存在于 eMMC 里的目录文件。

它<span style="color:#FF0000;">主要是内核把自己的运行信息，以“文件”的形式暴露给用户空间。</span>

例如：

```text
/proc/4557/
```

表示：

> <span style="background:#00FF80;">PID 为 4557 的进程相关信息。</span>

里面的：

```text
/proc/4557/fd/
```

专门显示：

> <span style="color:#FF0000; background:#00FF80;">这个进程当前打开了哪些文件描述符。</span>

所以：

```bash
ls -l /proc/4557/fd
```

相当于在问 Linux：

> <span style="color:#FF0000; background:#00FF80;">“把进程 4557 当前 fd 表的大致情况给我看看。”</span>

------

## 13. 那为什么看到的是符号链接？

视频中类似：

```text
3 -> /home/book/fileio/01_open/open.c
```

这里 `/proc/<pid>/fd/3`<span style="color:#FF0000; background:#00FF80;"> 表现为一个符号链接。</span>

它方便人类看到：

```text
fd 3
↓
到底关联了什么
```

但是不要误以为内核真正的 fd table 就是：

```text
3 → "/home/.../open.c" 字符串
```

内核里面更接近：

```text
进程
 │
 ▼
files_struct
 │
 ▼
fd table
 │
 ├─0 → struct file *
 ├─1 → struct file *
 ├─2 → struct file *
 └─3 → struct file *
              │
              ▼
         dentry/inode
              │
              ▼
           open.c
```

而 `/proc/<pid>/fd` 是 Linux 给你提供的一个**观察窗口**。

------

## 14. 为什么程序里加了 `while(1)`？

视频代码后面可以看到类似：

```c
while (1)
{
    sleep(10);
}
```

这不是 `open()` 必须这么写。

讲师这么写是为了：

> <span style="background:#00FF80;">**让进程别结束。**</span>

因为如果：

```c
open(...)
return 0;
```

程序马上结束。

进程一结束：

```text
Linux回收进程
↓
关闭它的fd
↓
/proc/<PID> 消失
```

那么讲师根本来不及去：

```bash
/proc/PID/fd
```

观察它。

所以：

```text
while(1)
```

在这里是**教学观察手段**。

------

## 15. 这一节为什么花很多时间讲 `errno`

这是第二个核心知识点。

例如：

```c
fd = open("abc.txt", O_RDWR);
```

如果 `abc.txt` 不存在：

```text
open失败
```

你只看：

```c
fd == -1
```

只能知道：

> “失败了。”

但是为什么失败？

可能有很多原因：

```text
文件不存在
没有权限
路径错误
路径中的某级不是目录
打开文件过多
只读文件系统
...
```

所以 Linux / libc 使用：

```c
errno
```

告诉程序：

> **最近一次失败对应的具体错误编号是什么。**

------

## 16. `errno` 是错误编号

例如视频最后演示：

```text
errno = 13
```

然后：

```text
Permission denied
```

意思就是：

```text
13
↓
EACCES
↓
Permission denied
```

所以：

```text
errno
```

主要适合程序判断：

```c
if (errno == EACCES)
{
    ...
}
```

但：

```text
13
```

对人不是很友好。

于是又有下面两个函数。

------

## 17.`strerror(errno)` 是干什么的

视频专门查了：

```bash
man 3 strerror
```

它可以：

```c
strerror(errno)
```

把：

```text
13
```

转换为类似：

```text
"Permission denied"
```

因此：

```c
printf("errno = %d\n", errno);
printf("err: %s\n", strerror(errno));
```

可能输出：

```text
errno = 13
err: Permission denied
```

关系就是：

```text
errno
  │
  │ 13
  ▼
strerror(13)
  │
  ▼
"Permission denied"
```

------

## 18.`perror("open")` 又是什么

视频还专门查了：

```bash
man 3 perror
```

它更方便。

直接：

```c
perror("open");
```

可能得到：

```text
open: Permission denied
```

它实际上会参考当前的：

```text
errno
```

并给你打印对应的错误信息。

所以：

```c
perror("open");
```

你可以简单理解为：

```text
打印：
"open"
+
": "
+
errno对应的人类可读错误
```

------

## 19. 三种输出方式的区别

视频实际上把三种方法都演示了：

```c
printf("errno = %d\n", errno);

printf("err: %s\n", strerror(errno));

perror("open");
```

假设：

```text
errno = 13
```

可能分别得到：

```text
errno = 13

err: Permission denied

open: Permission denied
```

所以：

| 方法              | 得到什么                      |
| ----------------- | ----------------------------- |
| `errno`           | 数字错误码                    |
| `strerror(errno)` | 根据错误码得到字符串          |
| `perror("open")`  | 直接打印“上下文 + 错误字符串” |

------

## 20. 一个非常容易误解的地方：`errno` 不是 `open()` 的返回值

注意：

```c
fd = open(...);
```

失败时：

```text
fd = -1
```

与此同时：

```text
errno = 某个错误码
```

它们分别回答两个问题：

```text
fd < 0
↓
“成功还是失败？”


errno
↓
“为什么失败？”
```

例如：

```text
fd = -1
errno = 2
```

可能表示：

```text
失败
+
No such file or directory
```

而：

```text
fd = -1
errno = 13
```

表示：

```text
失败
+
Permission denied
```

------

## 21. 最后那个 `chmod -w 1.txt` 实验特别值得理解

视频最后做了类似：

```bash
echo 100ask > 1.txt

chmod -w 1.txt
```

把文件的写权限去掉。

然后程序却是：

```c
open(argv[1], O_RDWR);
```

注意：

```text
O_RDWR
```

代表：

> 我要“读 + 写”。

但文件：

```text
没有写权限
```

于是：

```text
APP
↓
open("1.txt", O_RDWR)
↓
进入内核
↓
Linux检查文件权限
↓
发现你的请求包含写
↓
不允许
↓
open失败
↓
fd = -1
↓
errno = 13
```

最终视频看到：

```text
can not open file 1.txt

errno = 13

err: Permission denied

open: Permission denied
```

这正好验证了我们上一轮讨论的：

> <span style="background:#00FF80; color:#FF0000;">**`open()` 进入内核后会检查“你的打开请求”和“文件权限”是否匹配。**</span>

------

## 22. 这跟硬件有没有关系？

这一节基本没有深入到硬件。

这一次 `open()` 最值得关注的层次是：

```text
APP
↓
系统调用
↓
Linux Kernel
↓
VFS
↓
权限
↓
fd
```

而不是：

```text
CPU
↓
eMMC
```

因为对于 `open()`：

> 大部分工作是“找到文件并建立内核管理关系”。

即使涉及文件系统元数据，很多情况下也可能已经缓存在 DDR 中，不需要真的访问 eMMC。

等后面：

```c
read(fd, ...)
```

真正读取文件内容时，才更容易看到：

```text
VFS
↓
文件系统
↓
Page Cache
↓
块设备
↓
驱动
↓
eMMC/SD
```

这一条硬件链。

------

## 23. 这节视频和我们刚才讲的 `struct file` 可以完全对应起来

视频自己主要让你看到：

```text
fd = 3
```

但你现在已经可以比视频再往下一层理解：

```text
APP：

int fd = open("open.c", O_RDWR);

                │
                ▼

Linux：

当前进程
 │
 ▼
文件描述符表
 │
 ├── fd 0
 ├── fd 1
 ├── fd 2
 └── fd 3
       │
       ▼
   struct file
       │
       ├── 打开flags
       ├── 当前文件位置
       ├── 操作函数
       │
       ▼
     inode
       │
       ▼
    open.c
```

因此：

```text
fd
```

只是用户程序手里的一个“小号码”。

真正复杂的文件对象：

> **全在内核里面。**

------

## 24. <span style="background:#00FF80;">为什么 APP 只需要一个小整数就够了？</span>

这<span style="color:#FF0000;">其实是操作系统设计得很漂亮的地方。</span>

假如 Linux 把：

```text
struct file *
inode *
磁盘块位置
驱动结构
```

<span style="background:#00FF80; color:#FF0000;">全暴露给 APP：</span>

<span style="background:#00FF80; color:#FF0000;">会非常危险，也非常复杂。</span>

所以 Linux 只给 APP：

```text
3
```

<span style="color:#FF0000; background:#00FF80;">APP以后：</span>

```c
read(3, ...)
write(3, ...)
close(3)
```

<span style="color:#FF0000; background:#00FF80;">然后 Linux 自己：</span>

```text
3
↓
查fd table
↓
struct file
↓
找到真正对象
↓
完成操作
```

所以：

> <span style="background:#00FF80;">**fd 本质上是一种“句柄/索引”，把用户程序和内核内部复杂结构隔离开。**</span>

------

## 25. 视频结构

整体表达路径可以总结成：

```text
先认识 open()
        ↓
学会查 man 手册
        ↓
自己写 open.c
        ↓
用 argc/argv 接收文件名
        ↓
open(argv[1], O_RDWR)
        ↓
判断 fd 是否失败
        ↓
引出 errno / strerror / perror
        ↓
真正运行程序
        ↓
用 /proc/PID/fd 验证 fd
        ↓
修改权限制造错误
        ↓
再次验证 open 的错误机制
```

这一节实际上是一个非常完整的小实验。

------

## 26. 观点分析

这是纯技术教学，没有需要分析的争议性观点。

讲师明确想传达的技术结论主要有：

| 技术判断                                      | 明确/可能 | 依据                |
| --------------------------------------------- | --------- | ------------------- |
| `open()` 成功返回文件描述符                   | 明确      | 代码及实验直接验证  |
| `open()` 失败返回负值并设置 `errno`           | 明确      | 错误测试            |
| `strerror()` 和 `perror()` 可以解释错误       | 明确      | 查手册并运行        |
| 一个进程的 fd 可以在 `/proc/<pid>/fd` 观察    | 明确      | 现场 `ls -l` 验证   |
| 0、1、2 通常已被标准 I/O 占用，新文件常得到 3 | 明确      | `/proc/PID/fd` 实验 |

------

## 27. 人物分析

本视频主要是屏幕录制、代码和终端演示，没有足够的人物视觉信息，因此不适合进行情绪、性格、人格或人品分析。

能明确看到的是讲师采用了：

```text
查手册
→ 写代码
→ 制造错误
→ 查看内核暴露的信息
→ 再制造权限错误
```

这种“边写边验证”的教学方式。

------

## 28. Who / Whom / Whose / When / Where / What / Why / How

| 维度  | 视频中的答案                                        |
| ----- | --------------------------------------------------- |
| Who   | Linux 用户态应用程序                                |
| Whom  | 向 Linux 内核请求打开指定文件                       |
| Whose | `fd` 属于当前进程的文件描述符空间                   |
| When  | 程序执行 `open()` 时                                |
| Where | 用户空间 → Linux 内核/VFS                           |
| What  | 打开一个已有文件并取得 fd                           |
| Why   | 后续 `read/write/close` 都需要通过 fd 指定对象      |
| How   | `open(path, flags)` → 内核检查 → 分配 fd → 返回 APP |

------

## 29. 最重要的 4 个片段

### 片段一：约 03:00-06:10

代码从：

```c
main(argc, argv)
```

发展到：

```c
fd = open(argv[1], O_RDWR);
```

**重要性：**

这是把“命令行参数”和“系统调用”第一次真正接起来。

------

### 片段二：约 06:10-10:00

讲：

```text
errno
strerror
perror
```

**重要性：**

以后不只是 `open()`，几乎所有系统编程都会经常遇到这种错误处理模式。

------

### 片段三：约 14:00-16:30

进入：

```bash
/proc/<pid>/fd
```

看到：

```text
0
1
2
3
```

以及：

```text
3 -> open.c
```

**这是整节课最值得看的实验。**

它把你脑子里的：

```text
fd = 3
```

变成了 Linux 里真的可以观察到的东西。

------

### 片段四：约 16:30-17:45

```bash
chmod -w 1.txt
```

然后：

```text
errno = 13
Permission denied
```

它证明：

> `open()` 不只是“找文件”，内核还要检查你的访问模式是否合法。

------

## 30. 这节课你应该重点掌握什么

不用把所有 `O_XXX` 一次性全背下来。

目前优先掌握这 **7 件事**：

```text
1. open() 是干什么的？

2. pathname 和 flags 分别是什么？

3. O_RDONLY / O_WRONLY / O_RDWR 有什么区别？

4. open成功为什么返回 fd？

5. 为什么第一个新打开的文件经常 fd=3？

6. open失败时 -1、errno、perror 分别是什么关系？

7. /proc/<pid>/fd 为什么能够看到进程打开的文件？
```

如果这 7 个问题你都能自己解释，这一节就是真的懂了。

------

## 31. 最终记忆版

```text
调用：

fd = open("1.txt", O_RDWR);


成功：

open
 ↓
进入Linux内核
 ↓
找到文件
 ↓
检查权限
 ↓
创建/关联打开文件对象
 ↓
放入当前进程fd表
 ↓
例如：
3 → struct file → 1.txt
 ↓
返回 fd = 3


失败：

open
 ↓
内核发现文件不存在/权限不够等
 ↓
返回 -1
 ↓
设置 errno

errno = 错误编号
strerror(errno) = 错误文字
perror("open") = 直接打印错误


查看：

/proc/<PID>/fd

可以观察：

0 → stdin
1 → stdout
2 → stderr
3 → 新打开的文件
```

## 一句话记忆

> <span style="color:#FF0000; background:#00FF80;">**`open()` 并不是把文件内容“拿到 APP 里”，而是让 Linux 内核为这个进程建立一次“打开文件”的关系，然后把这个关系用一个简单整数 `fd` 交给 APP；之后 `read/write/close` 都拿这个 fd 去找同一个内核文件对象。**</span>

这节视频和你刚才学的内部机制正好可以合成一条非常关键的链：

```text
open("1.txt")
      ↓
系统调用
      ↓
Linux VFS
      ↓
inode
      ↓
struct file
      ↓
fd table
      ↓
fd = 3
      ↓
APP
      ↓
read(3, ...)
write(3, ...)
close(3)
```

这条链建议你彻底记住，后面学文件 I/O 和字符设备驱动都会反复用到。







# 问题1

每个进程是有一个打开文件表吗，这里对应的刚好就是这个open()系统调用操作，这个打开文件表的内容是什么呢，这个打开文件表是存在内核栈还是进程栈里面呢，还有如何通过这个打开文件表的指向找到磁盘中的文件呢。



对，你这个问题正好到了 Linux 文件 I/O 里一个特别关键的结构：**fd 表、`struct file`、inode 到底是什么关系**。

先纠正一个很容易混淆的叫法：

> <span style="background:#00FF80;">**每个进程确实有自己的“文件描述符表 fd table”，但并不是简单地说“每个进程有一张独立的打开文件表”。**</span>

Linux 更准确的关系是：

```text
进程
 │
 ▼
文件描述符表 fd table
 │
 ├── fd 0 ──→ struct file
 ├── fd 1 ──→ struct file
 ├── fd 2 ──→ struct file
 └── fd 3 ──→ struct file
                    │
                    ▼
                 dentry
                    │
                    ▼
                  inode
                    │
                    ▼
               具体文件系统
                    │
                    ▼
              磁盘上的数据
```

而且<span style="background:#00FF80;">这些结构**都不在用户栈，也不在内核栈里**，主要是内核动态分配和维护的数据结构。</span>

下面细拆。

------

## 一、每个进程到底有什么？

<span style="color:#FF0000; background:#00FF80;">每个进程在 Linux 内核里都有一个核心结构：</span>

```c
struct task_struct
```

它可以理解成：

> Linux 内核描述“这个进程是谁”的总档案。

里面包含很多东西，例如：

```text
task_struct
│
├── PID
├── 进程状态
├── 调度信息
├── 内存信息
├── 信号信息
├── 用户身份
└── files ──→ 文件相关信息
```

<span style="color:#FF0000; background:#00FF80;">其中和我们现在最相关的是：</span>

```c
struct files_struct *files;
```

它里面管理：

> **当前进程的文件描述符表。**

所以可以理解成：

```text
task_struct
    │
    ▼
files_struct
    │
    ▼
fd table
```

------

## 二、fd table 到底是什么？

假设<span style="color:#FF0000; background:#00FF80;">程序刚启动：</span>

```text
fd table

0 → stdin
1 → stdout
2 → stderr
3 → 空
4 → 空
5 → 空
...
```

然后：

```c
int fd = open("hello.txt", O_RDWR);
```

Linux 找到最小的空闲编号：

```text
0 已占用
1 已占用
2 已占用

3 空闲
```

于是建立：

```text
fd table

0 → struct file A
1 → struct file B
2 → struct file C
3 → struct file D
```

然后：

```c
fd = 3;
```

返回给 APP。

所以：

> `fd=3` 本质上就是这张表的索引。

------

## 三、那“打开文件表”是什么？

很多教材为了方便，会画成三层：

```text
进程文件描述符表
        ↓
系统打开文件表
        ↓
inode表
```

这个模型是对的，但 Linux 内核实际实现里你可以把“打开文件表中的一项”主要对应理解为：

```c
struct file
```

也就是：

> **一次“打开文件”的内核对象。**

例如：

```c
fd = open("hello.txt", O_RDWR);
```

Linux 不只是找到 `hello.txt`。

还要新建一个：

```text
struct file
```

表示：

> <span style="color:#FF0000; background:#00FF80;">“某个进程现在以某种方式打开了这个文件。”</span>

------

## 四、`struct file` 里面有什么？

你现在不用背源码字段，但最好理解它保存哪类信息。

可以粗略想成：

```text
struct file
│
├── 当前读写位置 f_pos
│
├── 打开方式 flags
│      O_RDONLY
│      O_WRONLY
│      O_RDWR
│      O_APPEND
│      ...
│
├── 指向文件路径
│      dentry
│      mount
│
├── 指向 inode
│
├── 文件操作函数
│      read
│      write
│      mmap
│      ...
│
└── 文件系统/驱动私有数据
```

最重要的几个信息是：

```text
当前读写位置
打开状态
到底打开的是哪个文件
以后用什么函数操作这个文件
```

------

## 五、为什么还需要 `struct file`？直接 fd → inode 不行吗？

因为：

> **“文件本身”跟“这一次打开文件的状态”不是一回事。**

举个特别关键的例子。

```c
int fd1 = open("hello.txt", O_RDONLY);
int fd2 = open("hello.txt", O_RDONLY);
```

磁盘上：

```text
只有一个 hello.txt
```

所以大致只有一个对应的 inode。

但是你打开了两次：

```text
第一次 open
↓
struct file A

第二次 open
↓
struct file B
```

关系：

```text
fd 3 → struct file A ─┐
                     ├──→ inode → hello.txt
fd 4 → struct file B ─┘
```

<span style="color:#FF0000; background:#00FF80;">为什么需要 A 和 B 两个？</span>

因为它们的：

```text
当前文件位置
打开 flags
状态
```

<span style="color:#FF0000; background:#00FF80;">可以不同。</span>

例如：

```c
read(fd1, buf, 100);
```

之后：

```text
struct file A:
f_pos = 100
```

但：

```text
struct file B:
f_pos = 0
```

它们<span style="color:#FF0000;">虽然指向同一个文件，但：</span>

> <span style="background:#00FF80; color:#FF0000;">**两次打开行为是独立的。**</span>

------

## 六、所以你可以区分三层

这是最值得记住的一张图：

```text
第一层：当前进程

fd table

fd 3
 │
 ▼


第二层：这一次“打开”

struct file

- 当前偏移
- flags
- 操作方式
 │
 ▼


第三层：文件本身

inode

- 文件类型
- 权限
- UID/GID
- 大小
- 时间
- 文件系统信息
 │
 ▼

磁盘上的实际内容
```

可以记成：

> <span style="background:#00FF80;">**fd 描述“你用哪个编号访问”**</span>
> <span style="background:#00FF80;">**struct file 描述“这一次怎么打开”**</span>
> <span style="background:#00FF80;">**inode 描述“这个文件本身是什么”**</span>

------

## 七、这些东西存在哪里？

你问得特别好：

> fd 表是在进程栈还是内核栈？

答案：

### 都不是。

它们主要存在于：

> <span style="background:#00FF80;">**内核空间中由内核动态分配的内存。**</span>

例如：

```text
task_struct
files_struct
fdtable
struct file
dentry
inode
```

这些都属于：

```text
Linux 内核管理的数据结构
```

一般是通过内核的内存分配机制，在：

```text
kernel heap / slab / SLUB
```

之类的区域动态获得。

------

## 八、那内核栈是什么？

这里特别容易混。

一个进程执行：

```c
read(fd, buf, 100);
```

进入内核之后，内核执行函数时需要：

```text
局部变量
函数调用返回地址
保存现场
```

这些东西会临时使用：

> **内核栈**

所以：

```text
内核栈
```

更像：

> <span style="background:#00FF80;">CPU 当前执行内核代码时的临时工作区。</span>

它不是长期保存 fd table 的地方。

例如：

```text
进程运行几秒、几分钟、几小时
```

<span style="color:#FF0000; background:#00FF80;">fd 表都可以一直存在。</span>

但一次系统调用：

```text
open()
↓
进入内核
↓
使用内核栈
↓
处理完成
↓
返回用户态
```

这一次调用结束以后，栈上的临时数据就没意义了。

所以<span style="color:#FF0000; background:#00FF80;">不可能把长期存在的：</span>

```text
struct file
fd table
inode
```

<span style="color:#FF0000; background:#00FF80;">全部长期放在内核栈里。</span>

------

## 九、用户栈又是什么？

你的程序：

```c
int main()
{
    int fd;
    char buf[100];

    fd = open(...);
}
```

这里：

```text
fd
buf
```

这些局部变量可能存在：

> 用户空间的用户栈。

所以：

```text
用户空间：

int fd = 3
char buf[100]
```

APP 手里只有：

```text
3
```

这个整数。

真正的：

```text
fd table
struct file
inode
```

全部在：

> Linux 内核空间。

这正是用户态和内核态隔离的体现。

------

## 十、完整关系其实是这样

```text
                  用户空间
────────────────────────────────

APP

int fd = 3;

只有一个整数 3


════════ 系统调用边界 ════════


                  内核空间
────────────────────────────────

task_struct
     │
     ▼
files_struct
     │
     ▼
fd table
     │
     ├── 0
     ├── 1
     ├── 2
     └── 3
          │
          ▼
      struct file
          │
          ├── f_pos
          ├── flags
          ├── file operations
          │
          ▼
        dentry
          │
          ▼
         inode
          │
          ▼
     具体文件系统
          │
          ▼
      磁盘/Flash
```

------

## 十一、那么最关键的问题来了：怎么通过 fd 最终找到磁盘文件？

假设：

```c
read(3, buf, 100);
```

你可以顺着这条链走。

第一步：

```text
fd = 3
```

Linux 知道：

> 当前是哪个进程在调用。

所以：

```text
current
↓
当前进程 task_struct
```

------

第二步：

找到当前进程的：

```text
files_struct
```

然后：

```text
fd table[3]
```

得到：

```text
struct file *
```

所以：

```text
3
↓
struct file
```

------

第三步：

`struct file` 告诉内核：

```text
这是哪个文件
当前读到哪里
使用什么文件操作
```

例如：

```text
f_pos = 100
```

说明：

> 这次从文件偏移 100 附近继续读。

------

## 十二、`struct file` 怎么找到 inode？

其中<span style="color:#FF0000; background:#00FF80;">保存了路径相关信息：</span>

```text
struct file
↓
f_path
↓
dentry
↓
inode
```

可以粗略画：

```text
struct file
    │
    ▼
  dentry
    │
    ▼
   inode
```

------

## 十三、dentry 是解决“名字”的

比如：

```text
/home/book/hello.txt
```

路径里的：

```text
home
book
hello.txt
```

都是名字。

<span style="color:#FF0000; background:#00FF80;">VFS 路径解析时会使用：</span>

```text
dentry
```

你可以把它理解成：

> 文件名/目录名与 inode 之间的桥梁。

所以：

```text
"hello.txt"
     ↓
   dentry
     ↓
   inode
```

------

## 十四、inode 是描述“文件本体”的

inode 里面保存：

```text
文件类型
权限
UID
GID
大小
时间
文件系统相关操作
数据位置相关信息
```

但有一个特别重要的点：

> inode 里面并不是简单保存“磁盘扇区号 = 12345”。

因为不同文件系统组织磁盘数据的方法不同。

例如：

```text
ext4
FAT
UBIFS
NFS
```

完全不同。

所以 inode 会连接到：

> 具体文件系统实现。

------

## 十五、如果是 ext4 文件系统呢？

假设：

```text
hello.txt
```

在 ext4 分区中。

那么：

```text
inode
↓
ext4 inode信息
↓
extents / block mapping
```

ext4 可以根据：

```text
文件偏移
```

计算：

> <span style="color:#FF0000; background:#00FF80;">对应的数据在哪些逻辑块。</span>

例如概念上：

```text
hello.txt

文件偏移：
0 ~ 4095
        ↓
文件系统逻辑块 800

4096 ~ 8191
        ↓
文件系统逻辑块 801
```

于是：

```text
文件第0~4095字节
↓
块 800
```

------

## 十六、然后逻辑块如何到磁盘硬件？

继续：

```text
inode
↓
ext4
↓
逻辑块
↓
块设备层
↓
MMC/块设备驱动
↓
USDHC
↓
eMMC / SD
```

如果是你的 i.MX6ULL：

```text
read(fd, ...)
     ↓
VFS
     ↓
ext4
     ↓
Page Cache / block I/O
     ↓
Linux block layer
     ↓
MMC subsystem
     ↓
i.MX6ULL USDHC driver
     ↓
USDHC硬件控制器
     ↓
SD / eMMC
```

这才真正到硬件。

------

## 十七、但是注意：多数 `read()` 不一定真的去磁盘

这一点一定要补。

Linux 有：

> **Page Cache**

所以可能：

```text
read(fd, ...)
↓
fd table
↓
struct file
↓
inode
↓
Page Cache
↓
数据已经在DDR
↓
直接返回
```

根本不用碰 eMMC。

只有数据：

```text
Page Cache 没有
```

才可能：

```text
文件系统
↓
块设备
↓
驱动
↓
硬件
```

所以：

> **找到文件 ≠ 每次都找到磁盘扇区并访问硬盘。**

Linux 会大量使用内存缓存。

------

## 十八、`open()` 本身需要找到磁盘数据块吗？

通常不是主要任务。

`open()` 主要完成：

```text
路径解析
↓
找到 dentry
↓
找到 inode
↓
权限检查
↓
创建 struct file
↓
放进 fd table
```

也就是说：

```text
open()
```

主要是建立：

> “当前进程 ↔ 打开的文件”

这层关系。

真正：

```text
文件偏移
↓
数据块
↓
磁盘
```

通常是后面的：

```c
read()
write()
```

更关心的事情。

------

## 十九、可以把 `open()` 和 `read()` 分成两条链

## `open()`

```text
路径字符串
"/home/book/hello.txt"
      ↓
VFS路径解析
      ↓
dentry
      ↓
inode
      ↓
权限检查
      ↓
struct file
      ↓
fd table
      ↓
fd = 3
```

重点：

> <span style="color:#FF0000; background:#00FF80;">找到“这个文件是谁”。</span>

------

## `read()`

```text
fd = 3
↓
fd table
↓
struct file
↓
当前偏移 f_pos
↓
inode
↓
Page Cache
↓
如果缓存没有
↓
文件系统
↓
存储块
↓
驱动
↓
硬件
```

重点：

> 找到“我要的数据在哪里”。

这两个区分开，你脑子会清晰很多。

------

## 二十、还有一个很重要的细节：`fork()` 以后怎么办？

你如果以后学进程，会碰到一个有意思的情况。

例如：

```text
父进程

fd 3
 │
 ▼
struct file
```

执行：

```c
fork();
```

子进程可能继承文件描述符。

于是：

```text
父进程 fd 3 ─┐
             ├──→ 同一个 struct file
子进程 fd 3 ─┘
```

这样它们可能共享：

```text
文件偏移 f_pos
```

这进一步证明：

> **fd table 是进程层面的，而 `struct file` 并不必然只属于一个进程。**

同一个 `struct file` 可以被多个 fd 引用。

------

## 二十一、`dup()` 也会出现类似情况

例如：

```c
int fd1 = open("hello.txt", O_RDONLY);
int fd2 = dup(fd1);
```

可能：

```text
fd 3 ─┐
      ├──→ struct file A
fd 4 ─┘
```

所以：

```text
fd不同
```

并不代表：

```text
struct file一定不同
```

这一点就是为什么“每个进程拥有一张打开文件表”这种说法容易不够准确。

更准确：

> **每个进程拥有自己的文件描述符表；文件描述符表中的项指向内核的打开文件对象 `struct file`。**

------

## 二十二、你现在可以把整个结构记成四层

这是最推荐你记的版本：

```text
【第一层】
进程

task_struct
↓


【第二层】
文件描述符表

files_struct / fdtable

0
1
2
3
↓


【第三层】
一次打开

struct file

f_pos
flags
operations
↓


【第四层】
文件本体

dentry
↓
inode
↓
filesystem
↓
storage
```

对应一句口诀：

> **进程找 fd，fd 找 file，file 找 inode，inode 找文件系统，文件系统最终找数据。**

------

## 二十三、从软件和硬件结合起来看

最终可以画成：

```text
APP
│
│ fd = 3
│
▼
read(3, buf, 100)

════════ 用户态/内核态 ════════

current
│
▼
task_struct
│
▼
files_struct
│
▼
fdtable[3]
│
▼
struct file
│
├── f_pos
├── flags
└── f_path
      │
      ▼
    dentry
      │
      ▼
     inode
      │
      ▼
具体文件系统
 ext4 / FAT / UBIFS
      │
      ▼
   Page Cache
      │
      │ 未命中
      ▼
Linux块设备层
      │
      ▼
MMC/设备驱动
      │
      ▼
USDHC控制器
      │
      ▼
  SD / eMMC
```

------

## 最后纠正你最开始那句话

你可以把：

> “每个进程有一个打开文件表”

改成更准确的：

> <span style="color:#FF0000; background:#00FF80;">**每个进程拥有自己的文件描述符表 `fd table`；其中每个有效 fd 都指向一个内核中的打开文件对象 `struct file`。`struct file` 再通过 dentry/inode 关联真正的文件。**</span>

这些结构：

> **不在用户栈，也不长期放在内核栈，而是由 Linux 内核在内核空间动态维护。**

而从 `fd=3` 找到磁盘数据，大致是：

```text
fd
↓
fd table
↓
struct file
↓
dentry / inode
↓
具体文件系统
↓
Page Cache
↓
必要时块设备层
↓
驱动
↓
存储控制器
↓
eMMC / SD
```

其中最值得你现在牢牢记住的是：

**`fd → struct file → inode`**

这三个东西是三个完全不同的层次。



