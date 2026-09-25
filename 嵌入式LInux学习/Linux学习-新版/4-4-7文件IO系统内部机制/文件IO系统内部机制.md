# 视频分析：P46 `4_4-7 文件 I/O 系统调用内部机制`

这节视频大约 **34 分 56 秒**。它非常重要，因为前几节你只是站在 APP 这一侧使用：

```c
open()
read()
write()
lseek()
```

而这一节第一次真正往下面钻：

> <span style="background:#00FF80;">**APP 调用 `open/read` 以后，CPU 是怎么进入 Linux 内核的？内核怎么根据系统调用号找到对应函数？`open()` 返回的 fd 又到底是怎么和 `struct file`、当前进程的 fd 表建立关系的？**</span>

而且它和我们最近讨论的：

```text
用户空间 / 内核空间
task_struct
files_struct
fdtable
struct file
f_pos
```

几乎完全接上了。

------

## 1. 视频类型判断

**主要类型：讲解类 / Linux 内核机制分析**

**次要类型：源码分析类 / 实验验证类**

判断依据：

1. 从 APP → glibc → 系统调用 → Linux 内核逐层分析。
2. 对比 ARM old ABI、EABI、ARM64 的系统调用进入方式。
3. 查看 Linux 内核源码中的 `SYSCALL_DEFINE3(open, ...)`、`do_sys_open()`。
4. 实际运行程序，通过 `/proc/PID/fd` 验证文件描述符。
5. 最后深入 `task_struct → files_struct → fdtable → struct file`。

------

## 2. 一句话总结

这节视频可以压缩成：

> <span style="background:#00FF80;">**用户程序调用 `open/read`，通常先经过 glibc 的封装，然后通过 `SVC` 等系统调用机制从用户态进入内核态；内核根据系统调用号找到 `sys_open/sys_read` 一类内核实现。`open()` 最终获得一个 `struct file`，再从当前进程的文件描述符表找一个空闲 fd，把 `fd → struct file \*` 的关系记录下来并把 fd 返回给 APP。**</span>

最核心链条：

```text
APP
 ↓
open()
 ↓
glibc
 ↓
系统调用号 + SVC
 ↓
CPU 用户态 → 内核态
 ↓
sys_call_table
 ↓
sys_open / do_sys_open
 ↓
获得 struct file
 ↓
当前进程 files_struct
 ↓
fdtable[fd] = struct file *
 ↓
返回 fd
```

------

## 3. 时间段拆解

| 时间段      | 主要内容                                                     | 这一段的作用                          |
| ----------- | ------------------------------------------------------------ | ------------------------------------- |
| 00:00-04:30 | 区分 APP、API/glibc、Linux 内核，介绍系统调用                | 建立三层结构                          |
| 04:30-08:30 | 展示 `open/read` 从 glibc 进入内核的路径                     | 解释用户态→内核态                     |
| 08:30-13:30 | 分析 old ABI / EABI / ARM64 的系统调用号和 `SVC`             | 解释 CPU 如何知道“要调用哪个系统调用” |
| 13:30-18:30 | 查看系统调用表及内核函数实现，准备实验程序                   | 从理论落到源码                        |
| 18:30-23:30 | 运行程序、使用 `ps`、`/proc/PID/fd` 查看 fd                  | 实际验证 fd 0/1/2/3                   |
| 23:30-27:30 | 深入 `do_sys_open()`：找空闲 fd、打开文件、`fd_install()`    | 解释 fd 怎么产生                      |
| 27:30-32:30 | 查看 `task_struct`、`files_struct`、`fdtable`、`struct file` | 建立内核数据结构关系                  |
| 32:30-34:56 | 总结 `open → fd → file → f_pos → read`                       | 把前面几节 IO 知识串起来              |

时间段按照画面中的源码、板书和实验阶段划分，可能有几十秒误差。

------

## 4. 整节课最重要的一张图：APP、glibc、Kernel

视频一开始就在强调：

```text
用户空间
─────────────────────

APP
 │
 │ open()
 ▼
glibc
 │
 │ 系统调用入口
 ▼

════════════════════
     用户/内核边界
════════════════════

Linux Kernel
```

这里<span style="background:#00FF80; color:#FF0000;">特别容易产生一个误区：</span>

```text
APP调用open()
=
直接跳到Linux里的open函数
```

不是这么简单。

一般是：

```text
APP
↓
调用 libc 暴露的 open()
↓
libc 准备系统调用参数
↓
准备 syscall number
↓
执行系统调用指令
↓
CPU进入内核
↓
Linux寻找对应系统调用实现
```

------

## 5. <span style="background:#00FF80; color:#FF0000;">为什么还要经过 glibc？</span>

比如你写：

```c
fd = open("1.txt", O_RDWR);
```

<span style="background:#00FF80;">源程序看到的是：</span>

```text
open()
```

但是：

> <span style="color:#FF0000; background:#00FF80;">`open()` 这个用户态接口通常由 C 库提供。</span>

也就是：

```text
Application
↓
glibc / libc
↓
Linux System Call
```

所以<span style="color:#FF0000; background:#00FF80;"> glibc 在中间承担了：</span>

```text
C函数接口
        ↓
转化
        ↓
Linux系统调用约定
```

<span style="color:#FF0000; background:#00FF80;">这个角色。</span>

------

## 6. 视频里的 `weak_alias` 是什么？

画面中有类似：

```text
weak_alias(__libc_open, open)
weak_alias(__libc_read, read)
```

你现在不用去死记宏的语法。

只要理解：

> <span style="background:#00FF80;">glibc 内部真正可能实现的是诸如 `__libc_open` 的内部符号，然后让用户熟悉的 `open` 这个名字指向对应实现。</span>

概念上：

```text
APP调用：

open()
  │
  ▼
glibc中的内部open实现
```

<span style="color:#FF0000; background:#00FF80;">所以你写：</span>

```c
open(...)
```

<span style="background:#00FF80; color:#FF0000;">并不代表 glibc 源码里一定恰好存在一个最简单的：</span>

```c
int open(...)
{
}
```

实现形式。

<span style="background:#00FF80;">实际 glibc 不同版本还可能有不同封装路径，所以**视频这里重点是理解“libc wrapper → syscall”，不要死记某个版本源码函数名**。</span>

------

## 7. 那系统调用到底怎么“进入内核”？

这是视频前半部分最重要的内容。

对于 ARM：

```text
用户态代码
↓
准备系统调用号
↓
准备参数
↓
执行 SVC
↓
CPU触发异常
↓
进入内核异常入口
```

这里你前面已经问过：

> <span style="color:#FF0000;">“进入内核是不是 CPU 从用户态切到内核态？”</span>

是的。

更准确：

> <span style="background:#00FF80;">**`SVC` 是 CPU 提供的异常/系统调用入口机制。**</span>

不是普通：

```c
foo();
```

函数调用。

------

## 8. 系统调用号是什么？

假设有：

```text
open
read
write
close
```

<span style="color:#FF0000;">Linux 需要知道：</span>

> <span style="color:#FF0000; background:#00FF80;">这次 APP 到底想让内核执行哪一个服务？</span>

于是<span style="color:#FF0000; background:#00FF80;">每一种系统调用都有编号。</span>

概念：

```text
read   → 一个编号
write  → 一个编号
open   → 一个编号
close  → 一个编号
```

<span style="background:#00FF80;">视频里面写成：</span>

```text
__NR_open
__NR_read
```

<span style="color:#FF0000; background:#00FF80;">这里：</span>

```text
NR
≈ Number
```

所以：

```text
__NR_open
```

可以理解成：

> <span style="color:#FF0000; background:#00FF80;">**open 对应的 system call number。**</span>

------

## 9. 为什么 old ABI、EABI、ARM64 看起来不一样？

视频专门画了三条路径。

因为：

> <span style="background:#00FF80;">**系统调用接口不仅是 Linux 规定的，还涉及 CPU 架构和 ABI。**</span>

不同 ARM 时代，约定不同。

视频大致展示：

```text
ARM old ABI
ARM EABI
ARM64
```

系统调用号放在哪里、用什么指令进入内核，可能不同。

------

## 10. 你当前 i.MX6ULL 最应该关注哪一条？

你用的是：

```text
i.MX6ULL
↓
ARM Cortex-A7
↓
ARMv7-A
↓
32-bit ARM Linux
```

所以<span style="color:#FF0000; background:#00FF80;">你学习时重点看：</span>

> <span style="background:#00FF80;">**ARM EABI**</span>

这一条就行。

<span style="color:#FF0000; background:#00FF80;">典型思想：</span>

```text
系统调用号
↓
放到约定寄存器（如 r7）

参数
↓
放到 r0、r1、r2...等约定寄存器

执行 svc
↓
进入Linux内核
```

你<span style="background:#00FF80;">没必要现在把：</span>

```text
old ABI
ARM EABI
ARM64
```

<span style="background:#00FF80;">三个都背下来。</span>

<span style="color:#FF0000; background:#00FF80;">视频把它们放在一起主要是告诉你：</span>

> <span style="background:#00FF80;">**“系统调用”这个概念一样，但不同 ABI 的具体寄存器约定可能不同。**</span>

------

## 11. `SVC` 为什么能让 CPU 进入内核？

比如：

```text
CPU当前：

User Mode
↓
执行 svc
↓
发生 Supervisor Call exception
↓
CPU保存必要现场
↓
跳转到内核异常入口
↓
进入特权执行环境
```

这不是：

```text
open函数
↓
直接call kernel函数
```

而是经过：

> **受 CPU 硬件保护的异常入口。**

这也是用户程序不能随便：

```text
jmp kernel_function
```

来执行内核代码的原因之一。

------

## 12. 进入内核后，怎么知道调用 `open` 还是 `read`？

这就用到了：

```text
sys_call_table
```

你<span style="color:#FF0000; background:#00FF80;">可以把它想象成一个：</span>

> <span style="background:#00FF80; color:#FF0000;">**系统调用函数指针表。**</span>（<span style="color:#FF0000; background:#00FF80;">注意：这个系统调用函数指针表 可以仿照着看成中断向量表，机制类似，只不过中断向量表是由外部触犯的，这个是由内部触发的</span>）

概念：

```text
sys_call_table

[0] → syscall A
[1] → syscall B
[2] → syscall C
...
[__NR_read] → read实现
[__NR_open] → open实现
```

于是：

```text
系统调用号 = __NR_open
↓
sys_call_table[__NR_open]
↓
open对应内核实现
```

视频里正是在强调：

```text
调用 sys_call_table[__NR_open]
调用 sys_call_table[__NR_read]
```

------

## 13. 这和我们之前学的“函数指针”其实是一回事

你可以把它简化成类似：

```c
void (*table[])(void);
```

然后：

```c
table[number]();
```

<span style="color:#FF0000; background:#00FF80;">Linux 实际实现当然复杂很多，但思想非常类似：</span>

```text
编号
↓
数组下标
↓
找到函数
```

所以：

> <span style="background:#00FF80; color:#FF0000;">系统调用号解决的是“到底想调用哪个内核服务”的问题。</span>

------

## 14. 视频里的 `SYSCALL_DEFINE3(open, ...)`

画面中能看到类似：

```c
SYSCALL_DEFINE3(open,
                const char __user *, filename,
                int, flags,
                umode_t, mode)
```

<span style="color:#FF0000;">这里：</span>

```text
DEFINE3
```

可以先理解：

> <span style="background:#00FF80;">这个 syscall 有 3 个参数。</span>

<span style="color:#FF0000; background:#00FF80;">正好对应：</span>

```c
open(filename, flags, mode)
```

<span style="color:#FF0000; background:#00FF80;">三个参数。</span>

<span style="background:#00FF80;">所以：</span>

```text
SYSCALL_DEFINE3(open, ...)
```

<span style="color:#FF0000; background:#00FF80;">经过宏展开以后，会形成相应的 Linux 内核系统调用实现。</span>

------

## 15. `__user` 又是什么意思？

源码中：

```c
const char __user *filename
```

这里：

```text
__user
```

是在提醒内核开发者：

> <span style="background:#00FF80;">这个指针指向的是**用户地址空间**。</span>

比如 APP：

```c
open("/home/book/1.txt", ...);
```

字符串：

```text
"/home/book/1.txt"
```

原本位于：

```text
Process User Space
```

而内核函数现在拿到的是：

```text
用户空间地址
```

<span style="background:#00FF80; color:#FF0000;">所以 Linux 不能像普通内核指针一样随便解引用，而需要进行安全的用户地址访问处理。</span>

这和你最近问的：

```text
用户空间buf
↓
write
↓
内核
```

完全是同一个问题。

------

## 16. 视频后半段最重要的源码：`do_sys_open()`

能看到核心代码类似：

```c
long do_sys_open(int dfd,
                 const char __user *filename,
                 int flags,
                 umode_t mode)
{
    ...
    fd = get_unused_fd_flags(flags);

    if (fd >= 0)
    {
        struct file *f = do_filp_open(...);

        if (IS_ERR(f))
        {
            ...
        }
        else
        {
            fsnotify_open(f);
            fd_install(fd, f);
        }
    }

    return fd;
}
```

视频特别标了三个步骤。

这三个步骤你最好直接记住。

------

## 17. 第一步：找一个没用过的 fd

```c
fd = get_unused_fd_flags(flags);
```

可以理解：

> 到当前进程的 fd 表里找一个空位置。

比如：

```text
fdtable

0 → stdin
1 → stdout
2 → stderr
3 → 空
4 → 空
...
```

于是：

```text
get_unused_fd...
↓
返回3
```

所以 APP 最后经常拿到：

```text
fd = 3
```

------

## 18. 第二步：真正打开文件，得到 `struct file`

```c
struct file *f = do_filp_open(...);
```

经过：

```text
路径查找
VFS
文件系统
权限检查
...
```

最终得到：

```text
struct file *
```

也就是我们之前说的：

> **“这一次打开”的内核对象。**

概念：

```text
struct file
├── flags
├── f_pos
├── path
├── file operations
└── ...
```

------

## 19. 第三步：`fd_install(fd, f)`

这一步是整节课最重要的地方之一。

```c
fd_install(fd, f);
```

可以理解成：

```text
fdtable[fd] = f;
```

例如：

```text
fd = 3
f  = 0x某个struct file地址
```

执行后：

```text
当前进程 fdtable：

0 → stdin对应file
1 → stdout对应file
2 → stderr对应file

3 ────────────────┐
                  ▼
              struct file
                  │
                  ├── f_pos
                  ├── flags
                  └── path
```

于是：

> **整数 fd 3 和内核中的 `struct file` 建立关联。**

------

## 20. 所以 `open()` 返回的 3 本身并不是文件

这个你现在一定要特别清楚。

```c
fd = open(...);
```

得到：

```text
fd = 3
```

这个：

```text
3
```

不是：

```text
文件地址
inode号
磁盘块号
struct file地址
```

它只是：

> **当前进程 fdtable 的下标。**

所以：

```text
fd = 3
↓
fdtable[3]
↓
struct file *
```

------

## 21. 视频为什么要使用 `/proc/PID/fd`？

这是用用户空间的方法观察内核 fd 关系。

例如程序保持运行，然后：

```bash
ps
```

找到：

```text
PID = 27753
```

进入：

```bash
/proc/27753/fd
```

可能看到：

```text
0 -> /dev/pts/0
1 -> /dev/pts/0
2 -> /dev/pts/0
3 -> /home/book/.../xxx.c
```

它告诉你：

```text
fd 0
→ 当前标准输入

fd 1
→ 当前标准输出

fd 2
→ 当前标准错误

fd 3
→ 程序刚刚open的文件
```

这正好验证：

```text
open()
↓
返回3
```

不是老师随便编出来的。

------

## 22. 为什么另一个进程也能有 fd=3？

因为：

> <span style="background:#00FF80;">**每个进程有自己的 fd 表。**</span>

例如：

```text
Process A fdtable：

0
1
2
3 → a.txt
```

同时：

```text
Process B fdtable：

0
1
2
3 → b.txt
```

完全没冲突。

所以：

```text
fd
```

必须始终和：

> **“当前进程”**

一起理解。

------

## 23. 那当前进程的 fdtable 到底藏在哪里？

视频后面开始看：

```c
task_struct
```

里面有类似：

```text
files
```

也就是：

```text
current
↓
task_struct
↓
files
↓
files_struct
```

所以：

```text
当前线程/task
       │
       ▼
  task_struct
       │
       └── files
              │
              ▼
         files_struct
```

------

## 24. `files_struct` 又连接 `fdtable`

视频源码里可以看到：

```c
struct files_struct
{
    ...
    struct fdtable __rcu *fdt;
    struct fdtable fdtab;
    ...
};
```

简化：

```text
files_struct
      │
      ▼
   fdtable
      │
      └── fd[]
```

而：

```c
struct fdtable
{
    unsigned int max_fds;
    struct file __rcu **fd;
    ...
};
```

最关键就是：

```c
struct file **fd;
```

所以你前面问：

> “文件描述符表的一项到底放什么？”

答案就在这里：

> **放的是 `struct file \*`。**

------

## 25. 整条数据结构链终于可以写完整了

```text
current
  │
  ▼
task_struct
  │
  ▼
files_struct
  │
  ▼
fdtable
  │
  ├── fd[0]
  ├── fd[1]
  ├── fd[2]
  └── fd[3]
         │
         ▼
     struct file
         │
         ├── f_flags
         ├── f_pos
         ├── f_path
         └── ...
```

这张图是整个 P46 最值得记住的图。

------

## 26. 然后为什么 `read(fd,...)` 能知道从哪里读？

这就回到我们前面学过的：

```text
f_pos
```

比如：

```c
read(3, buf, 100);
```

内核：

```text
当前进程
↓
fdtable[3]
↓
struct file
↓
f_pos = 0
↓
读100字节
↓
f_pos = 100
```

下一次：

```c
read(3, buf, 100);
```

还是：

```text
fdtable[3]
↓
同一个 struct file
↓
f_pos = 100
```

于是：

```text
从100继续读
```

------

## 27. `open()` 和 `read()` 终于可以真正串起来

以前你看到：

```c
fd = open("1.txt", O_RDONLY);

read(fd, buf, 100);
```

可能只是知道：

> `open` 给 fd，`read` 用 fd。

现在可以展开：

```text
open()

① 找空闲fd：
   fd = 3

② 创建/获得：
   struct file

③ 安装：
   fdtable[3] → struct file

④ 返回：
   APP得到3
```

然后：

```text
read(3,...)

① 找当前进程
② 找files_struct
③ 找fdtable
④ 取fdtable[3]
⑤ 得到struct file
⑥ 读取f_pos
⑦ 执行文件读取
⑧ 更新f_pos
```

这就是真正的内部关系。

------

## 28. 这个视频和“内核动态内存”也能接起来

你刚刚还在问：

> `task_struct`、`files_struct`、`struct file` 放在内核空间哪里？

这节视频提供了真实例子。

这些：

```text
task_struct
files_struct
fdtable
struct file
```

都属于：

> **内核管理的数据结构。**

尤其：

```text
struct file
```

是在运行过程中：

```text
open文件
↓
动态得到
```

不是：

```text
kernel .data
```

里提前准备一百万个。

所以刚才我们讲：

```text
Kernel Image静态段
≠
Kernel Runtime Objects
```

现在你应该更能感受到为什么了。

------

## 29. 视频中 `struct file` 里的 `f_pos`

能看到源码类似：

```c
struct file
{
    ...
    loff_t f_pos;
    ...
};
```

这就是：

> 当前文件偏移。

和我们 P43/P44 里说的：

```text
write()
↓
f_pos推进

read()
↓
f_pos推进

lseek()
↓
修改f_pos
```

完全对应。

------

## 30. `struct file` 为什么还有 operations？

文件打开以后：

```text
普通文件
设备文件
socket
管道
```

最终执行的操作可能不一样。

所以：

```text
struct file
```

会关联：

```text
file_operations
```

概念上：

```text
struct file
      │
      ▼
file_operations
      │
      ├── read
      ├── write
      ├── open
      ├── release
      └── ...
```

这也是你以后学：

> **Linux 驱动**

时最关键的桥梁之一。

比如：

```c
read(fd,...)
```

如果 fd 对应：

```text
/dev/xxx
```

最终可能落到驱动程序提供的：

```text
.read
```

回调里。

------

## 31. 人物分析

这个视频是屏幕录制式技术教学，没有以人物情绪、关系或人格表现为重点。

因此不适合做：

```text
情绪
心理
人格
人品
```

分析。

主要分析对象应该是：

> **技术逻辑、源码关系和数据结构。**

------

## 32. 观点分析

这不是评论型视频，没有价值立场。

明确技术观点主要有：

| 技术观点                                       | 明确/可能 | 依据                   |
| ---------------------------------------------- | --------- | ---------------------- |
| 用户 API 不等于直接调用内核函数                | 明确      | APP/glibc/kernel分层图 |
| 系统调用通过异常机制进入内核                   | 明确      | `SVC` 路径             |
| syscall number 用于选择内核服务                | 明确      | `__NR_open/__NR_read`  |
| 内核通过 syscall table 找系统调用实现          | 明确      | 系统调用表图           |
| `open()` 先获取空闲 fd，再获得 `struct file`   | 明确      | `do_sys_open()`        |
| `fd_install()` 建立 fd 和 `struct file` 的关系 | 明确      | 源码标注               |
| fd 属于当前进程                                | 明确      | `/proc/PID/fd` 实验    |
| 当前文件位置在 `struct file` 中                | 明确      | `f_pos` 源码           |

------

## 33. Who / Whom / Whose / When / Where / What / Why / How

| 维度  | 视频中的答案                                                 |
| ----- | ------------------------------------------------------------ |
| Who   | 用户进程、glibc、Linux内核                                   |
| Whom  | APP向Linux请求文件IO服务                                     |
| Whose | fd表属于当前进程/共享文件表对象；`f_pos` 属于对应打开文件对象 |
| When  | APP调用 `open/read` 系统调用时                               |
| Where | 用户空间 → CPU系统调用入口 → 内核空间                        |
| What  | 将用户API转换为内核系统调用，并建立fd与文件对象关系          |
| Why   | 用户态不能直接操作内核和硬件资源                             |
| How   | libc wrapper → syscall number → SVC → syscall table → 内核实现 |

------

## 34. 最值得精读的 5 个片段

### 片段一：约 04:30-08:30

系统调用总图：

```text
APP
↓
glibc
↓
SVC
↓
kernel
```

这是整节课的总框架。

------

### 片段二：约 08:30-13:30

重点：

```text
old ABI
EABI
ARM64
```

你只要特别抓住：

```text
i.MX6ULL
→ ARM32 EABI
→ syscall number + 寄存器 + SVC
```

------

### 片段三：约 18:30-23:30

实际：

```bash
/proc/PID/fd
```

看到：

```text
0
1
2
3
```

这个实验把“fd表”从抽象概念变成了能看到的东西。

------

### 片段四：约 24:00-27:00

源码：

```c
get_unused_fd_flags()
do_filp_open()
fd_install()
```

这是全视频最重要的一段。

可以直接翻译：

```text
找编号
↓
打开文件得到file对象
↓
把编号和file绑起来
```

------

### 片段五：约 29:00-32:30

源码结构：

```text
task_struct
↓
files_struct
↓
fdtable
↓
struct file *
```

这是你最近一直在问的：

> “fdtable 和 struct file 到底怎么关联？”

最直接的答案。

------

## 35. 我建议你把这一节和前面四节这样连起来

你目前已经学：

```text
P41/P42：
open()
↓
怎么打开文件


P43：
write()
↓
使用 f_pos 写


P44：
read()
↓
使用 f_pos 读


P45：
综合处理CSV


P46：
这些函数进入内核后
到底发生什么
```

所以现在整个知识链已经是：

```text
用户APP
│
│ open/read/write/lseek
▼
glibc
│
│ syscall
▼
CPU系统调用机制
│
│ SVC
▼
Linux Kernel
│
├── sys_call_table
│
├── sys_open/sys_read...
│
▼
current
│
▼
task_struct
│
▼
files_struct
│
▼
fdtable
│
▼
struct file
│
├── f_pos
├── flags
└── operations
│
▼
VFS
│
▼
文件系统 / 驱动
│
▼
Page Cache / 存储设备
```

------

## 36. 这里再帮你纠正一个特别容易出现的理解

不要认为：

```text
open()
↓
创建fd
↓
fd自己保存了文件信息
```

不对。

应该是：

```text
fd只是一个小整数
```

真正信息在：

```text
struct file
```

所以：

```text
fd
=
索引

fdtable
=
索引表

struct file
=
真正的打开文件对象
```

这是非常重要的三层。

------

## 37. 再加上 inode 就完整了

```text
fd
↓
struct file
↓
dentry
↓
inode
```

区别：

```text
fd
→ 当前进程里的编号

struct file
→ “这一次打开”

inode
→ “这个文件本身”
```

比如：

```c
fd1 = open("a.txt", O_RDONLY);
fd2 = open("a.txt", O_RDONLY);
```

通常：

```text
fd1
↓
struct file A
     \
      → 同一个inode
     /
struct file B
↑
fd2
```

所以两个独立 open：

```text
可以有两个独立 f_pos
```

但是指向同一个文件本体。

------

## 38. 记忆版总结

```text
类型：
Linux系统调用内部机制

核心问题：
open/read到底怎样进入内核？

用户态：
APP
↓
glibc
↓
系统调用号
↓
SVC

内核态：
异常入口
↓
sys_call_table
↓
sys_open / sys_read
↓
内核文件操作


open内部：

get_unused_fd_flags()
↓
得到fd

do_filp_open()
↓
得到struct file *

fd_install(fd, file)
↓
建立：

fdtable[fd] → struct file


数据结构：

current
↓
task_struct
↓
files_struct
↓
fdtable
↓
struct file
↓
f_pos


read/write：

通过fd找到struct file
↓
使用f_pos
↓
执行IO
↓
更新f_pos
```

## 一句话记忆

> **`open()` 真正做的核心事情之一，就是在 Linux 内核里建立“当前进程的 fd 数字 → `struct file` 对象”的映射；随后 `read/write/lseek` 都拿这个 fd 重新找到同一个 `struct file`，再使用里面的 `f_pos` 等状态完成操作。**

而你当前使用的 i.MX6ULL，可以优先记住这一条：

```text
ARM32 APP
↓
glibc open/read
↓
EABI约定准备寄存器
↓
SVC
↓
Linux Kernel
↓
syscall table
↓
具体系统调用
```

这一节其实正好把你最近问的 **CPU 特权级、用户/内核空间、系统调用、task_struct、files_struct、fdtable、struct file、f_pos** 全部汇到了一条线上。



# 问题0

current

↓

task_struct

↓

files_struct

↓

fdtable

↓

struct file

↓

f_pos

这里数据结构分别表示什么呢？



可以，这一串其实就是 <span style="background:#00FF80;">**“当前线程怎么一路找到某个 fd 对应的打开文件对象，以及当前文件偏移”**。</span>

你可以先记住一句话：

> ```
> current → task_struct → files_struct → fdtable → struct file → f_pos
> ```
>
> 就是在说：**<span style="color:#FF0000; background:#00FF80;">当前线程 → 它的任务描述 → 它使用的文件表 → fd 索引表 → 某次打开文件的对象 → 这个打开实例当前读写到哪里。</span>**

我给你一个个拆。

| 数据结构/字段  | 表示什么                           | 你可以把它理解成                 |
| -------------- | ---------------------------------- | -------------------------------- |
| `current`      | 当前 CPU 正在执行的 task           | “现在是谁在运行”                 |
| `task_struct`  | 当前线程/task 的核心内核描述结构   | “这个线程的档案”                 |
| `files_struct` | 这个 task 使用的打开文件描述符集合 | “这个进程/线程组正在使用哪些 fd” |
| `fdtable`      | fd 到 `struct file *` 的映射表     | “fd 号码索引表”                  |
| `struct file`  | 一次 `open()` 对应的打开文件实例   | “这一次打开文件的状态”           |
| `f_pos`        | 当前文件偏移                       | “现在读/写到文件第几个字节”      |

比如：

```c
int fd = open("a.txt", O_RDONLY);
read(fd, buf, 100);
```

假设 `open()` 返回：

```text
fd = 3
```

内核里概念上就是：

```text
current
  ↓
task_struct
  ↓
files_struct
  ↓
fdtable
  ↓
fdtable[3]
  ↓
struct file
  ↓
f_pos
```

------

### `current`

`current` 不是一个普通进程对象本身，它更像：

> **Linux 内核快速获得“当前正在 CPU 上执行的 task”的入口。**

比如此刻 CPU0 正在执行 Thread A：

```text
CPU0
↓
current
↓
Thread A 的 task_struct
```

所以内核执行：

```c
read(3, ...)
```

时首先要知道：

> “这个 `3` 是谁的 fd=3？”

因为 fd 是**进程/文件表局部的编号**，不是全系统唯一。

------

### `task_struct`

这是 Linux 最核心的数据结构之一。

每个线程/task 通常都有自己的：

```c
struct task_struct
```

里面记录很多东西，比如概念上：

```text
task_struct
├─ PID / TID
├─ 调度状态
├─ 优先级
├─ CPU相关状态
├─ mm        → 地址空间
├─ files     → 文件表
├─ 信号相关
└─ ...
```

所以它相当于：

> **Linux 对一个可调度任务的总档案。**

你以前说 PCB/TCB，在 Linux 里最接近的核心对象就是：

```text
task_struct
```

------

### `files_struct`

<span style="color:#FF0000; background:#00FF80;">`task_struct` 里面有一个类似：</span>

```c
struct files_struct *files;
```

<span style="color:#FF0000; background:#00FF80;">它指向：</span>

```text
files_struct
```

这个结构主要管理：

> <span style="color:#FF0000; background:#00FF80;">**这一组任务当前打开的文件描述符。**</span>

比如：

```text
0 → stdin
1 → stdout
2 → stderr
3 → a.txt
4 → socket
5 → /dev/led
```

这些不是直接全塞在 `task_struct` 里，而是通过：

```text
task_struct
↓
files_struct
```

管理。

还有一个很重要的细节：

> 同一进程里的多个线程通常共享 `files_struct`。

所以：

```text
Thread A task_struct ─┐
                      ├→ same files_struct
Thread B task_struct ─┘
```

这也是为什么同一进程里的线程通常能看到彼此打开的 fd。

------

### `fdtable`

<span style="color:#FF0000; background:#00FF80;">`files_struct` 里面又会管理：</span>

```text
fdtable
```

它才真正负责：

> **fd 数字 → `struct file *` 的映射。**

可以想成：

```text
fdtable

index     value
-----------------------
0       → struct file *
1       → struct file *
2       → struct file *
3       → struct file *
4       → NULL
5       → struct file *
```

如果：

```c
fd = 3;
```

内核就相当于：

```text
fdtable[3]
```

取出里面那个：

```text
struct file *
```

所以 fd 本质上就是：

> **这个表的下标。**

------

### `struct file`

这个最重要。

```c
struct file
```

不是“磁盘上的文件本体”，而是：

> <span style="color:#FF0000; background:#00FF80;">**一次打开操作对应的内核打开文件对象。**</span>

比如：

```c
fd1 = open("a.txt", O_RDONLY);
fd2 = open("a.txt", O_RDONLY);
```

虽然打开的是同一个 `a.txt`，但通常会得到两个不同的：

```text
struct file A
struct file B
```

它们可以共享同一个 inode，但各自有自己的打开状态。

概念上：

```text
fd1
 ↓
struct file A
 ├─ f_pos = 0
 ├─ flags
 └─ path
      \
       → inode(a.txt)

fd2
 ↓
struct file B
 ├─ f_pos = 500
 ├─ flags
 └─ path
      /
       → same inode
```

所以：

> `inode` 更像“文件本身”，
> `struct file` 更像“这一次打开”。

------

### `f_pos`

`f_pos` 就是：

> **当前文件位置偏移。**

比如文件：

```text
ABCDEFGHIJ
```

刚打开：

```text
f_pos = 0
```

执行：

```c
read(fd, buf, 3);
```

读：

```text
ABC
```

然后：

```text
f_pos = 3
```

再：

```c
read(fd, buf, 2);
```

就从第 3 个偏移继续：

```text
DE
```

然后：

```text
f_pos = 5
```

如果：

```c
lseek(fd, 8, SEEK_SET);
```

就是修改：

```text
f_pos = 8
```

所以我们前面说：

```text
read
write
lseek
```

为什么都跟同一个 fd 有关系？

因为它们最终都能找到：

```text
fd
↓
struct file
↓
f_pos
```

------

把这一整条翻译成人话就是：

```text
current
↓
现在是哪个线程？

task_struct
↓
找到这个线程的内核档案

files_struct
↓
找到它使用的文件描述符集合

fdtable
↓
根据 fd 数字查表

struct file
↓
找到“这一次打开文件”的对象

f_pos
↓
知道现在读写到文件哪里
```

比如：

```c
read(3, buf, 100);
```

内核可以抽象理解成：

```text
current
↓
current->task_struct
↓
current->files
↓
fdtable
↓
fdtable[3]
↓
struct file
↓
f_pos = 200
↓
从偏移200开始读100字节
↓
f_pos变成300
```

最值得你记的是这三个层次：

```text
fd
= 一个整数下标

struct file
= 一次open产生的打开实例

inode
= 文件本体的内核表示
```

而 `f_pos` 属于：

```text
struct file
```

所以两个独立 `open()` 同一个文件时，通常可以各自有不同的 `f_pos`。







# 问题1

你可以把它简化成类似：

```
void (*table[])(void);
```

然后：

```
table[number]();
```

Linux 实际实现当然复杂很多，但思想非常类似：

```
编号
↓
数组下标
↓
找到函数
```

所以：

> 系统调用号解决的是“到底想调用哪个内核服务”的问题。

这里的这个函数指针要怎么理解，我有点看不太懂这里。





可以。<span style="background:#00FF80;">你卡住的其实不是 Linux，而是 **C 语言“函数指针 + 函数指针数组”这个语法本身比较反人类**。把它拆成三步，你就会一下子看懂：</span>

```c
void (*table[])(void);
```

先别整句看，我们从普通函数开始。

------

## 1. 普通函数是什么？

例如：

```c
void hello(void)
{
    printf("hello\n");
}
```

这里：

```text
hello
```

是一个函数名。

这个函数：

```text
没有参数
返回值是 void
```

所以它的类型可以理解成：

```c
void (void)
```

也就是：

```text
返回 void
参数 void
```

调用：

```c
hello();
```

------

## 2. <span style="background:#00FF80;">函数本身在内存里也有地址</span>

程序编译后：

```c
void hello(void)
{
    ...
}
```

<span style="color:#FF0000; background:#00FF80;">最终变成 `.text` 里的机器指令。</span>

假设：

```text
hello函数机器指令
位于地址：

0x1000
```

那么：

```text
hello
```

<span style="color:#FF0000; background:#00FF80;">在很多表达式里可以近似理解成：</span>

> <span style="background:#00FF80; color:#FF0000;">`hello` 函数的入口地址。</span>

类似于：

```c
int a;
```

可以：

```c
int *p = &a;
```

变量指针：

```text
p
↓
指向变量 a
```

<span style="background:#00FF80;">函数也可以：</span>

```c
void (*p)(void) = hello;
```

函数指针：

```text
p
↓
指向函数 hello
```

------

## 3. `void (*p)(void)` 到底怎么看？

这是最关键的。

```c
void (*p)(void);
```

<span style="color:#FF0000; background:#00FF80;">不要从左往右硬读。</span>

先找到：

```text
p
```

然后看：

```text
*p
```

说明：

> `p` 是一个指针。

<span style="color:#FF0000; background:#00FF80;">再看：</span>

```c
(*p)(void)
```

说明：

> <span style="color:#FF0000; background:#00FF80;">这个指针指向的是一个函数，这个函数没有参数。</span>

<span style="color:#FF0000; background:#00FF80;">最后看最左边：</span>

```c
void
```

说明：

> <span style="background:#00FF80;">这个函数返回 `void`。</span>

所以：

```c
void (*p)(void);
```

翻译成人话：

> <span style="color:#FF0000; background:#00FF80;">**p 是一个函数指针，它指向“无参数、返回 void”的函数。**</span>

------

## 4. 举个完整例子

```c
#include <stdio.h>

void hello(void)
{
    printf("hello\n");
}

int main(void)
{
    void (*p)(void);

    p = hello;

    p();

    return 0;
}
```

这里：

```c
p = hello;
```

就是：

```text
p
↓
保存 hello 函数的地址
```

假设：

```text
hello地址 = 0x1000
```

那么概念上：

```text
p = 0x1000
```

<span style="color:#FF0000; background:#00FF80;">之后：</span>

```c
p();
```

<span style="color:#FF0000; background:#00FF80;">CPU就会：</span>

```text
从 p 中取出地址
↓
得到 0x1000
↓
跳到 0x1000 执行
↓
执行 hello()
```

<span style="color:#FF0000; background:#00FF80;">所以：</span>

```c
hello();
```

<span style="color:#FF0000; background:#00FF80;">和：</span>

```c
p();
```

<span style="color:#FF0000; background:#00FF80;">效果可以一样。</span>

------

## 5. 那<span style="background:#00FF80;">为什么要用函数指针？</span>

比如你有：

```c
void add(void);
void del(void);
void show(void);
```

以前调用：

```c
add();
```

或者：

```c
del();
```

<span style="color:#FF0000; background:#00FF80;">都是把函数名写死。</span>

<span style="color:#FF0000; background:#00FF80;">如果现在想：</span>

> <span style="color:#FF0000; background:#00FF80;">根据一个数字，运行不同函数。</span>

你<span style="color:#FF0000; background:#00FF80;">就可以用函数指针。</span>

------

## 6. 先创建三个函数

```c
void func0(void)
{
    printf("func0\n");
}

void func1(void)
{
    printf("func1\n");
}

void func2(void)
{
    printf("func2\n");
}
```

然后：

```c
void (*p)(void);
```

可以：

```c
p = func0;
p();
```

输出：

```text
func0
```

再：

```c
p = func2;
p();
```

输出：

```text
func2
```

<span style="color:#FF0000; background:#00FF80;">所以：</span>

```text
一个函数指针 p
↓
可以在不同时刻指向不同函数
```

------

## 7. 那 `table[]` 又是什么？

普通数组：

```c
int table[3];
```

表示：

```text
table

[0] → int
[1] → int
[2] → int
```

也就是说：

> 数组里的每一个元素都是 `int`。

<span style="color:#FF0000; background:#00FF80;">而：</span>

```c
void (*table[3])(void);
```

表示：

> <span style="color:#FF0000; background:#00FF80;">数组里的每一个元素，都是一个函数指针。</span>

所以：

```text
table

[0] → 函数指针
[1] → 函数指针
[2] → 函数指针
```

<span style="color:#FF0000; background:#00FF80;">每个函数指针都要求指向：</span>

```text
无参数
返回void
```

<span style="color:#FF0000; background:#00FF80;">的函数。</span>

------

## 8. 把三个函数放进去

```c
void (*table[3])(void);

table[0] = func0;
table[1] = func1;
table[2] = func2;
```

内存关系可以想成：

```text
table

┌───────────────┐
│ table[0]      │ ─────→ func0()
│ 存func0地址    │
├───────────────┤
│ table[1]      │ ─────→ func1()
│ 存func1地址    │
├───────────────┤
│ table[2]      │ ─────→ func2()
│ 存func2地址    │
└───────────────┘
```

假设：

```text
func0 = 0x1000
func1 = 0x2000
func2 = 0x3000
```

那么数组里其实可以理解成：

```text
table[0] = 0x1000
table[1] = 0x2000
table[2] = 0x3000
```

<span style="color:#FF0000; background:#00FF80;">存的都是：</span>

> 函数入口地址。

------

## 9. 那 `table[1]()` 是什么意思？

比如：

```c
table[1]();
```

分两步：

```text
table[1]
↓
取出第1号元素
↓
得到 func1 的函数地址
```

<span style="color:#FF0000; background:#00FF80;">然后：</span>

```text
()
↓
调用这个地址对应的函数
```

所以：

```c
table[1]();
```

<span style="color:#FF0000; background:#00FF80;">等价于：</span>

```c
func1();
```

------

## 10. `table[number]()` 就很好理解了

假设：

```c
int number = 2;
```

执行：

```c
table[number]();
```

就是：

```text
number = 2
↓
table[2]
↓
得到func2地址
↓
调用func2()
```

如果：

```text
number = 0
```

则：

```text
table[0]
↓
func0()
```

所以：

> <span style="color:#FF0000; background:#00FF80;">**一个数字就可以决定调用哪个函数。**</span>

------

## 11. 这就是<span style="background:#00FF80; color:#FF0000;">系统调用表的核心思想</span>

<span style="color:#FF0000; background:#00FF80;">假设为了讲解，Linux 有：</span>

```text
0 → read
1 → write
2 → open
3 → close
```

那么<span style="color:#FF0000; background:#00FF80;">可以想象：</span>

```text
sys_call_table

[0] ─────→ sys_read()
[1] ─────→ sys_write()
[2] ─────→ sys_open()
[3] ─────→ sys_close()
```

<span style="color:#FF0000; background:#00FF80;">APP想执行：</span>

```c
open(...)
```

<span style="color:#FF0000; background:#00FF80;">libc准备：</span>

```text
system call number = 2
```

进入内核后：

```text
number = 2
↓
sys_call_table[2]
↓
得到 sys_open 的地址
↓
调用对应系统调用实现
```

所以我之前才说：

```text
编号
↓
数组下标
↓
函数地址
↓
执行函数
```

------

## 12. 你可以把它看成一个“菜单”

例如：

```text
菜单：

0：查询余额
1：存款
2：取款
3：退出
```

用户输入：

```text
2
```

<span style="color:#FF0000; background:#00FF80;">普通写法可能：</span>

```c
if (number == 0)
    check();
else if (number == 1)
    save();
else if (number == 2)
    withdraw();
else if (number == 3)
    quit();
```

<span style="color:#FF0000; background:#00FF80;">但是函数指针表可以：</span>

```c
table[number]();
```

<span style="color:#FF0000; background:#00FF80;">直接实现：</span>

```text
2
↓
table[2]
↓
withdraw()
```

<span style="color:#FF0000; background:#00FF80;">所以它相当于：</span>

> **“数字 → 函数”的查表机制。**

------

## 13. 给你一个能直接运行的完整例子

```c
#include <stdio.h>

void add(void)
{
    printf("执行 add\n");
}

void del(void)
{
    printf("执行 del\n");
}

void show(void)
{
    printf("执行 show\n");
}

int main(void)
{
    void (*table[3])(void);

    table[0] = add;
    table[1] = del;
    table[2] = show;

    int number = 2;

    table[number]();

    return 0;
}
```

运行：

```text
number = 2
↓
table[2]
↓
show函数地址
↓
show()
```

输出：

```text
执行 show
```

如果：

```c
number = 0;
```

则调用：

```c
add();
```

------

## 14. 可以直接在定义时初始化

前面的：

```c
void (*table[3])(void);

table[0] = add;
table[1] = del;
table[2] = show;
```

<span style="color:#FF0000; background:#00FF80;">也可以写成：</span>

```c
void (*table[3])(void) =
{
    add,
    del,
    show
};
```

对应：

```text
table[0] → add
table[1] → del
table[2] → show
```

于是：

```c
table[1]();
```

就是：

```c
del();
```

------

## 15. <span style="background:#00FF80; color:#FF0000;">函数有参数怎么办？</span>

比如：

```c
int add(int a, int b)
{
    return a + b;
}
```

那么函数类型：

```text
参数：
int, int

返回：
int
```

<span style="color:#FF0000; background:#00FF80;">对应函数指针：</span>

```c
int (*p)(int, int);
```

可以：

```c
p = add;
```

然后：

```c
int result = p(10, 20);
```

结果：

```text
30
```

所以函数指针类型必须和函数匹配。

------

## 16. <span style="color:#FF0000; background:#00FF80;">函数指针数组也可以带参数</span>

例如：

```c
int add(int a, int b);
int sub(int a, int b);
int mul(int a, int b);
```

定义：

```c
int (*table[3])(int, int) =
{
    add,
    sub,
    mul
};
```

然后：

```c
int number = 1;

int result = table[number](10, 3);
```

过程：

```text
number = 1
↓
table[1]
↓
sub函数地址
↓
sub(10, 3)
↓
7
```

这<span style="color:#FF0000; background:#00FF80;">就更接近 Linux syscall 的思想了，因为系统调用当然是有参数的。</span>

------

## 17. 为什么我之前用了：

```c
void (*table[])(void);
```

只是为了：

> **把“函数指针数组”的核心思想讲简单。**

它并不是在说 Linux 真正的：

```text
sys_call_table
```

就严格声明成这一模一样的 C 类型。

Linux 的真实实现会涉及：

```text
不同架构
寄存器现场
系统调用包装宏
参数类型
内核版本
安全检查
```

复杂得多。

所以你只应该从这个简化例子里带走：

> <span style="background:#00FF80; color:#FF0000;">**表里存的是函数入口/处理入口，系统调用号帮助内核选择对应的系统调用处理逻辑。**</span>

------

## 18. `void (*table[])(void)` 为什么括号这么重要？

这个稍<span style="color:#FF0000; background:#00FF80;">微进阶，但特别容易考。</span>

```c
void (*p)(void);
```

是：

> <span style="background:#00FF80;">函数指针。</span>

<span style="color:#FF0000; background:#00FF80;">但是：</span>

```c
void *p(void);
```

<span style="color:#FF0000; background:#00FF80;">完全不是一个东西。</span>

它表示：

> 一<span style="color:#FF0000; background:#00FF80;">个函数，返回 `void *` 指针。</span>

对比：

```text
void (*p)(void)

p
↓
是指针
↓
指向函数
```

而：

```text
void *p(void)

p
↓
是函数
↓
函数返回void*
```

所以：

```text
(*p)
```

<span style="color:#FF0000; background:#00FF80;">外面的括号很重要。</span>

------

## 19. 怎么读这种复杂声明？

教你一个很好用的方法：

> **从变量名开始，向外一层一层读。**

比如：

```c
void (*table[10])(void);
```

从：

```text
table
```

开始。

先看到：

```text
table[10]
```

说明：

> table 是数组，有10个元素。

再看到：

```text
*table[10]
```

说明：

> 每个元素是指针。

再看到：

```text
(*table[10])(void)
```

说明：

> 每个指针指向一个无参数函数。

最后：

```text
void
```

说明：

> 这些函数返回 void。

最终：

> <span style="background:#00FF80; color:#FF0000;">**table 是一个有 10 个元素的数组，每个元素都是一个指向“无参数、返回 void 的函数”的指针。**</span>

------

## 20. 最后和 Linux 再完整连一次

你可以把系统调用想成：

```text
用户空间：

open(...)
   ↓
libc
   ↓
准备：
system call number
参数
   ↓
SVC
```

进入内核：

```text
Kernel

拿到：
syscall number

例如：
number = open对应编号

      ↓

系统调用分派逻辑

      ↓

找到对应处理函数

      ↓

open内核实现
```

用最简化的 C 模型表达就是：

```c
table[number]();
```

所以真正要理解的不是这行奇怪语法，而是：

```text
table
=
一张“函数地址表”

number
=
我要第几个服务

table[number]
=
找到对应函数地址

table[number]()
=
执行这个函数
```

一句话记：

> <span style="color:#FF0000; background:#00FF80;">**函数指针就是“保存函数地址的指针”；函数指针数组就是“保存很多函数地址的一张表”；系统调用号就像这张表的索引，让内核知道应该选择哪一种系统调用处理逻辑。**</span>



# 问题2

**table 是一个有 10 个元素的数组，每个元素都是一个指向“无参数、返回 void 的函数”的指针。**

这里我有一个问题，就是不同的系统调用对应的参数个数和参数类型不是不一样吗，不应该定义的函数指针也应该不一样吗，都用void (*p)(void)应该不行吧，这样应该有不同参数个数参数类型的系统调用表吧，我怎么知道系统调用号要放到哪个表呢。



对，你这个质疑完全正确。前面我写的：

```c
void (*table[10])(void);
```

<span style="background:#00FF80;">只是为了说明“**编号 → 查表 → 找到函数地址**”这个思想，**绝对不能把它当成 Linux 真正系统调用表的函数类型**。</span>

因为你已经发现关键问题了：

```c
read(fd, buf, count)        // 3个参数
close(fd)                   // 1个参数
lseek(fd, offset, whence)   // 3个参数
mmap(...)                   // 更多参数
```

<span style="color:#FF0000;">参数个数、参数类型都不同，如果真的全部写成：</span>

```c
void (*p)(void)
```

<span style="background:#00FF80; color:#FF0000;">然后像普通 C 函数那样直接调用，当然是不行的。</span>

真正解决办法不是“为 1 参数、2 参数、3 参数各建一张表”，而是：

> <span style="background:#00FF80; color:#FF0000;">**先利用系统调用 ABI，把所有参数统一放进规定的 CPU 寄存器；进入内核后，再由系统调用号决定这些寄存器里的值应该按照什么参数类型解释。**</span>

这一下就能把问题解开。

------

## 1. 先看 i.MX6ULL 的 ARM32 EABI

你现在的平台是：

```text
i.MX6ULL
↓
Cortex-A7
↓
ARMv7-A
↓
32位 ARM Linux
```

<span style="color:#FF0000; background:#00FF80;">典型系统调用约定可以先这样理解：</span>

```text
r7
→ 系统调用号

r0
→ 第1个参数

r1
→ 第2个参数

r2
→ 第3个参数

r3
→ 第4个参数

r4
→ 第5个参数

r5
→ 第6个参数
```

<span style="color:#FF0000;">所以参数本身并不是一开始就以</span>：

```c
int
char *
size_t
```

这种“C语言类型”存进系统调用表。

<span style="color:#FF0000; background:#00FF80;">进入内核时，它们首先只是：</span>

> **CPU 寄存器中的一串机器字。**

------

## 2. 举<span style="background:#00FF80;">两个参数数量不同的系统调用</span>

比如：

```c
close(fd);
```

<span style="color:#FF0000; background:#00FF80;">只有一个参数。</span>

假设：

```text
fd = 3
```

<span style="color:#FF0000; background:#00FF80;">那么进入内核前可能类似：</span>

```text
r7 = __NR_close

r0 = 3
```

剩下：

```text
r1
r2
r3
...
```

<span style="color:#FF0000; background:#00FF80;">不需要使用。</span>

------

而：

```c
read(fd, buf, 100);
```

<span style="color:#FF0000; background:#00FF80;">有三个参数：</span>

```text
r7 = __NR_read

r0 = fd
r1 = buf地址
r2 = 100
```

所以<span style="color:#FF0000; background:#00FF80;">虽然两个系统调用的参数完全不同：</span>

```text
close：
1个参数

read：
3个参数
```

<span style="color:#FF0000; background:#00FF80;">进入系统调用入口时仍然可以统一表示成：</span>

```text
系统调用号
+
一组寄存器
```

------

## 3. 所以<span style="background:#00FF80;">系统调用入口看到的是这种东西</span>

可以<span style="color:#FF0000; background:#00FF80;">先想象 CPU 保存了这样一份现场：</span>

```text
Register Frame

r0 = ?
r1 = ?
r2 = ?
r3 = ?
r4 = ?
r5 = ?
r6 = ?
r7 = syscall number

PC
SP
CPSR
...
```

<span style="color:#FF0000; background:#00FF80;">进入 Linux 后，首先看：</span>

```text
r7
```

例如：

```text
r7 = __NR_read
```

于是：

> “哦，这次是 read。”

<span style="color:#FF0000; background:#00FF80;">那 Linux 就知道：</span>

```text
r0
→ 应该解释成 int fd

r1
→ 应该解释成 void *buf

r2
→ 应该解释成 size_t count
```

<span style="color:#FF0000; background:#00FF80;">如果：</span>

```text
r7 = __NR_close
```

<span style="color:#FF0000; background:#00FF80;">那就知道：</span>

```text
r0
→ int fd
```

其他参数不需要。

------

## 4. 所以<span style="color:#FF0000; background:#00FF80;">“参数类型”其实由系统调用号隐含决定了</span>

<span style="color:#FF0000;">这一点特别重要。</span>

<span style="color:#FF0000; background:#00FF80;">假设：</span>

```text
syscall number = READ
```

<span style="color:#FF0000; background:#00FF80;">它天然对应一个约定：</span>

```c
read(int fd,
     void *buf,
     size_t count)
```

<span style="color:#FF0000; background:#00FF80;">于是：</span>

```text
r0 = int fd
r1 = 用户地址 buf
r2 = size_t count
```

<span style="color:#FF0000; background:#00FF80;">而：</span>

```text
syscall number = CLOSE
```

<span style="color:#FF0000; background:#00FF80;">天然对应：</span>

```c
close(int fd)
```

<span style="color:#FF0000; background:#00FF80;">于是：</span>

```text
r0 = int fd
```

所以：

> <span style="background:#00FF80; color:#FF0000;">**系统调用号不只是在告诉内核“调用哪个函数”，它也间接决定了“寄存器里的参数应该按什么形式解释”。**</span>

------

## 5. 那<span style="color:#FF0000;">系统调用表是不是需要很多不同函数指针类型？</span>

<span style="color:#FF0000; background:#00FF80;">如果严格站在普通 C 语言函数指针类型的角度，你的判断没错：</span>

```c
long (*p1)(int);

long (*p3)(int, void *, size_t);

long (*p6)(...);
```

本来不是同一种类型。

所以 Linux 不能简单照搬：

```c
void (*table[])(void);
```

这种普通教学写法，然后直接：

```c
table[nr]();
```

来处理所有 syscall。

现实内核会通过：

> **低级汇编入口 + 统一包装函数(wrapper) + 宏生成代码**

把不同参数形式统一起来。

具体实现会随：

```text
CPU架构
Linux版本
ABI
```

不同而变化。

------

## 6. 最容易理解的一种实现思路：统一成“寄存器现场指针”

你可以想象 Linux 把所有系统调用表项统一成：

```c
long (*table[])(struct pt_regs *regs);
```

注意这只是帮助你理解的模型，不表示所有架构源码都一模一样。

这样不管：

```text
read
close
write
mmap
```

<span style="color:#FF0000; background:#00FF80;">表里面的函数类型都统一：</span>

```text
接收：
struct pt_regs *

返回：
long
```

于是：

```c
table[nr](regs);
```

就合法了。

------

## 7. `pt_regs` 是什么？

你可以把它理解成：

> <span style="background:#00FF80; color:#FF0000;">**CPU进入内核时保存下来的一份寄存器现场。**</span>

概念：

```c
struct pt_regs {
    unsigned long r0;
    unsigned long r1;
    unsigned long r2;
    unsigned long r3;
    unsigned long r4;
    unsigned long r5;
    unsigned long r6;
    unsigned long r7;
    ...
};
```

例如用户调用：

```c
read(3, buf, 100);
```

进入内核后：

```text
regs

r0 = 3
r1 = buf的用户虚拟地址
r2 = 100
r7 = __NR_read
```

统一入口就可以：

```text
nr = regs->r7
↓
找到 read 对应 handler
```

------

## 8. 然后<span style="color:#FF0000; background:#00FF80;"> read 的 wrapper 自己拆参数</span>

概念上可以想成：

```c
long read_wrapper(struct pt_regs *regs)
{
    int fd;
    void *buf;
    size_t count;

    fd    = regs->r0;
    buf   = (void *)regs->r1;
    count = regs->r2;

    return real_read(fd, buf, count);
}
```

而 close：

```c
long close_wrapper(struct pt_regs *regs)
{
    int fd;

    fd = regs->r0;

    return real_close(fd);
}
```

于是表里：

```text
sys_call_table

[READ]  → read_wrapper
[CLOSE] → close_wrapper
[WRITE] → write_wrapper
...
```

每个表项统一接：

```text
regs
```

但是<span style="color:#FF0000; background:#00FF80;"> wrapper 内部知道：</span>

> <span style="background:#00FF80;">自己到底该取几个寄存器、把它们解释成什么类型。</span>

------

## 9. 这就把你问的矛盾彻底解决了

表面上：

```text
read：
3个参数

close：
1个参数

mmap：
更多参数
```

不统一。

但是系统调用入口把它们统一成：

```text
一个 syscall number
+
一个完整寄存器现场
```

所以调度层可以统一：

```text
number
↓
table[number]
↓
wrapper(regs)
```

然后：

```text
wrapper
↓
根据自己的系统调用定义
↓
从regs中取正确参数
↓
调用真正的内核实现
```

------

## 10. `SYSCALL_DEFINE3()` 就和这个有关

你上一节视频看到：

```c
SYSCALL_DEFINE3(read,
                unsigned int, fd,
                char __user *, buf,
                size_t, count)
```

这里：

```text
3
```

就是：

> 三个参数。

而类似：

```c
SYSCALL_DEFINE1(close,
                unsigned int, fd)
```

就是：

> 一个参数。

<span style="color:#FF0000; background:#00FF80;">这些宏不只是为了让源码好看。</span>

<span style="color:#FF0000; background:#00FF80;">它们会帮助内核生成：</span>

```text
系统调用入口函数
参数转换
类型处理
wrapper
真正实现之间的连接
```

所以你不需要手工写：

```c
read_wrapper(...)
close_wrapper(...)
```

大量重复代码。

宏帮内核做了很多这类事情。

------

## 11. 那为什么不建“1参数表、2参数表、3参数表”？

比如你设想：

```text
table_1arg
table_2arg
table_3arg
...
```

理论上当然可以设计一个这样的系统。

但有一个问题：

用户进入内核时只有一个：

```text
syscall number
```

如果还要先判断：

> “这个号对应几参数？”

那你还是需要另外一份信息。

而 syscall number 本身已经可以唯一确定：

```text
是哪一个系统调用
↓
自然就知道参数个数
↓
自然就知道参数类型
```

所以没有必要按照参数个数再分表。

更直接：

```text
一个 syscall number
↓
一个入口
↓
这个入口自己知道如何解析参数
```

就够了。

------

## 12. 系统调用号到底“放到哪个表”？

这里你的措辞可以改一下：

不是：

> “系统调用号放到哪个表里？”

而是：

> **系统调用号作为索引，用来查当前 ABI 对应的系统调用分派表。**

例如概念上：

```text
nr = __NR_read
↓
sys_call_table[nr]
↓
read对应入口
```

所以系统调用号自己不是：

```text
存进table里
```

而是：

```text
拿来当table下标
```

------

## 13. 什么时候才可能有“多张 syscall table”？

这和**参数个数没关系**，更多和：

```text
CPU架构 / ABI
```

有关。

比如一个 64 位内核可能还支持：

```text
64-bit native ABI

+

32-bit compatibility ABI
```

那么：

```text
同一个系统调用号数字
```

在不同 ABI 下可能需要不同处理。

所以可能存在不同：

```text
native syscall dispatch
compat syscall dispatch
```

或者不同入口路径。

ARM 历史上也有：

```text
OABI
EABI
```

这样的差异。

但这是：

> **“ABI不同所以分派机制不同”**

不是：

> “1参数一张表，3参数另一张表”。

------

## <span style="color:#FF0000; background:#00FF80;">14. 举一个完整例子：`read`</span>

用户代码：

```c
read(3, buf, 100);
```

### 用户空间 libc

准备：

```text
r0 = 3
r1 = buf
r2 = 100
r7 = __NR_read
```

执行：

```asm
svc 0
```

------

### CPU

<span style="color:#FF0000; background:#00FF80;">发生异常：</span>

```text
用户态
↓
内核态
```

<span style="color:#FF0000; background:#00FF80;">保存寄存器现场。</span>

------

### Linux syscall entry

得到：

```text
nr = __NR_read
```

查：

```text
syscall table
↓
read handler
```

------

### read wrapper

<span style="color:#FF0000; background:#00FF80;">从寄存器取：</span>

```text
fd    = r0
buf   = r1
count = r2
```

<span style="color:#FF0000; background:#00FF80;">然后调用：</span>

```text
真正的read内核逻辑
```

------

## 15. 再看 `close`

用户：

```c
close(3);
```

寄存器：

```text
r0 = 3
r7 = __NR_close
```

仍然走：

```text
同一个SVC入口
↓
同一种系统调用分派逻辑
```

但：

```text
nr = CLOSE
```

于是找到：

```text
close handler
```

它只取：

```text
r0
```

根本不关心：

```text
r1
r2
r3
...
```

------

## 16. 你可以把它想成“快递柜”

这个类比挺合适。

<span style="color:#FF0000; background:#00FF80;">每个 syscall 都有一个编号：</span>

```text
read  → 63号柜
write → 64号柜
close → 57号柜
```

<span style="color:#FF0000; background:#00FF80;">寄存器就像统一携带的一组格子：</span>

```text
参数槽0
参数槽1
参数槽2
参数槽3
...
```

read 说：

```text
我要使用槽0、1、2
```

close 说：

```text
我只使用槽0
```

所以完全没必要：

```text
建一个1参数快递柜区
再建一个3参数快递柜区
```

每个编号自己已经知道：

> “我应该读取几个参数槽。”

------

## 17. 再回头看 `void (*table[])(void)` 就知道它为什么只是“思想模型”

前面那个：

```c
void (*table[])(void);
```

只为了表达：

```text
table里放函数入口
↓
number选某一项
↓
跳到某个处理函数
```

真实 syscall 表更接近：

```text
统一低级调用约定
+
wrapper
+
寄存器参数
```

而不是普通 C 语言意义上的：

```c
void func(void)
```

数组。

所以以后你看到我写：

```c
table[number]();
```

应该脑补成：

```text
根据系统调用号
↓
找到对应的系统调用入口
↓
把保存好的寄存器参数交给它
↓
wrapper再按照本系统调用的真实签名解释参数
```

------

## 最后用一张图彻底定型

```text
用户程序

read(fd, buf, count)
close(fd)
open(path, flags, mode)
      │
      ▼

按照ABI把参数放寄存器

r0
r1
r2
r3
...
+
r7 = syscall number
      │
      ▼
     SVC
      │
════════════════════
      ▼
Linux内核
      │
      ▼
得到寄存器现场
      │
      ▼
读取 syscall number
      │
      ▼
系统调用分派表
      │
 ┌────┼─────────┐
 ▼    ▼         ▼
read  close    open
入口   入口      入口
 │      │        │
 │      │        │
取3参   取1参    取3参
 │      │        │
 ▼      ▼        ▼
真正内核实现
```

所以你的问题最终答案就是：

> <span style="color:#FF0000; background:#00FF80;">**不同 syscall 的确有不同参数个数和类型，因此不能真的全部当成 `void (\*)(void)` 普通函数来调用。Linux 利用 ABI 把参数统一放入寄存器，在内核入口保存成统一的寄存器现场，再由系统调用号选择对应 wrapper；每个 wrapper 自己知道应该从哪些寄存器中取几个参数、按什么类型解释。通常不需要按参数个数建立多张系统调用表。**</span>

你能问到这里，其实已经从“会用 syscall”进入到 **ABI + CPU寄存器 + 内核分派机制** 这一层了。







# 问题3

既然执行系统调用的时候会借助寄存器来传递参数值，那么我们在一个程序中调用系统调用的api，例如read()，write()等的时候是不是先要在自己这个程序，即这个线程用户栈中先将CPU现场保存起来呢，执行完系统调用完之后再恢复现场，这个现场保存应该发生在用户空间吧，保存在用户空间的线程栈，恢复现场也是发生在用户空间的吧，因为要把用户线程栈空间中的现场给恢复，但是这里保存和恢复现场执行的是哪里的代码呢，是Linux内核的.TXET对应的代码吗。



<span style="background:#00FF80;">对，你现在问到的其实是 **“系统调用发生时，CPU 现场到底保存在哪里、谁来保存、谁来恢复”**。这里最容易混的是：**用户函数调用时的保存现场** 和 **用户态→内核态时的保存现场** 其实是两套不同机制。</span>

最重要的结论先给你：

> <span style="color:#FF0000; background:#00FF80;">调用 `read()/write()` 时，用户程序自己的普通函数调用现场，可能会按照 C/ABI 规则保存在用户栈中；但真正因为 `SVC` 从用户态进入 Linux 内核时，Linux 为了以后返回用户态而保存的 CPU 用户态现场，主要保存在 **当前线程的内核栈** 中，而不是用户栈中。</span>

而<span style="color:#FF0000; background:#00FF80;">保存/恢复这部分真正的系统调用入口、退出代码，确实属于：</span>

```text
Linux Kernel .text
```

更准确地说，是内核里的底层汇编入口/异常处理代码。

------

## 1. 先<span style="background:#00FF80; color:#FF0000;">把“现场保存”分成两种</span>

你现在脑子里可能想的是：

```text
read()
↓
把CPU现场保存到User Stack
↓
进入内核
↓
完成read
↓
从User Stack恢复
```

这不完全对。

<span style="color:#FF0000; background:#00FF80;">实际上要分：</span>

```text
① 普通函数调用现场
② 系统调用/异常入口现场
```

------

## ① <span style="background:#00FF80;">普通函数调用现场</span>

例如：

```c
void foo(void)
{
    int a = 10;

    read(fd, buf, 100);

    a++;
}
```

<span style="color:#FF0000; background:#00FF80;">从 C 语言角度：</span>

```text
foo()
↓
调用 libc 的 read()
```

这<span style="color:#FF0000; background:#00FF80;">只是一次普通用户态函数调用。</span>

<span style="color:#FF0000; background:#00FF80;">编译器可能按照 ARM EABI 保存：</span>

```text
返回地址
某些需要保存的寄存器
局部变量
```

这些东西可能放到：

```text
User Stack
```

所以：

```text
用户态普通函数调用
→ 可能使用User Stack
```

这个理解没问题。

------

## 2. 但是<span style="color:#FF0000; background:#00FF80;">这不是系统调用真正的“用户态现场保存”</span>

<span style="color:#FF0000;">真正发生：</span>

```asm
svc 0
```

<span style="color:#FF0000; background:#00FF80;">以后，就不是普通函数调用了。</span>

假设当前：

```text
Thread A
正在用户态运行
```

<span style="color:#FF0000; background:#00FF80;">CPU大概有：</span>

```text
r0
r1
r2
...
r7
sp
lr
pc
CPSR
```

<span style="color:#FF0000; background:#00FF80;">这些状态。</span>

<span style="color:#FF0000; background:#00FF80;">执行：</span>

```asm
svc 0
```

<span style="color:#FF0000; background:#00FF80;">以后：</span>

```text
User Mode
↓
SVC Exception
↓
进入特权模式
↓
进入Linux内核
```

这时候 <span style="background:#00FF80; color:#FF0000;">Linux 要保证：</span>

> <span style="color:#FF0000; background:#00FF80;">“以后处理完 read，我还能让这个线程从原来的用户代码位置继续运行。”</span>

所以<span style="color:#FF0000; background:#00FF80;">必须把用户态寄存器现场保存下来。</span>

------

## 3. 这个现场主要保存在哪里？

不是放用户栈。

而是：

> <span style="background:#00FF80; color:#FF0000;">**当前线程对应的 Kernel Stack。**</span>

也就是：

```text
Thread A

User Space:
    User Stack

Kernel Space:
    Kernel Stack
```

<span style="color:#FF0000; background:#00FF80;">系统调用进入后：</span>

```text
Thread A User Mode
       │
       │ SVC
       ▼
Thread A Kernel Mode
       │
       ▼
Kernel Stack A
```

<span style="color:#FF0000; background:#00FF80;">内核入口代码会把必要的用户寄存器状态保存到这里。</span>

<span style="background:#00FF80;">Linux/ARM 中常常形成一个类似：</span>

```c
struct pt_regs
```

<span style="color:#FF0000; background:#00FF80;">这样的寄存器现场结构。</span>

概念上：

```text
Kernel Stack A

高地址
┌────────────────────┐
│ 原用户态 r0        │
│ 原用户态 r1        │
│ 原用户态 r2        │
│ ...                │
│ 原用户态 sp        │
│ 原用户态 lr        │
│ 原用户态 pc        │
│ 原用户态状态寄存器 │
└────────────────────┘
↓
内核函数调用现场
```

具体字段和保存顺序依架构/内核版本而不同，但思想就是这样。

------

## 4. 为什么不能把这些东西存在 User Stack？

这里特别关键。

因为用户栈属于：

```text
用户空间
```

而用户程序本身可以修改它。

如果内核把自己重要的异常返回现场完全相信并长期放在：

```text
User Stack
```

用户程序就可能：

```text
篡改返回PC
篡改权限状态
篡改寄存器
```

这会非常危险。

所以 Linux 要把受内核控制的重要入口/返回现场放到：

> **Kernel Stack**

用户态没有权限直接修改。

------

## 5. 那 User Stack 在 SVC 发生时是不是完全不用？

也不能这么说。

<span style="color:#FF0000;">假设：</span>

```c
foo()
{
    int a;
    read(...);
}
```

<span style="color:#FF0000; background:#00FF80;">在进入系统调用之前，User Stack 上本来可能已经有：</span>

```text
foo的局部变量
foo的返回地址
libc read wrapper的调用现场
```

这些东西当然还在那里。

所以：

```text
User Stack
```

并没有消失。

只是<span style="background:#00FF80;">系统调用进入内核后：</span>

> <span style="color:#FF0000; background:#00FF80;">Linux 不再把自己的内核函数调用压到 User Stack 上。</span>

而是改用：

```text
Kernel Stack
```

------

## 6. 所以<span style="color:#FF0000; background:#00FF80;">一个线程在系统调用时可能同时存在“两套现场”</span>

例如：

```text
Thread A

User Stack
┌─────────────────────┐
│ foo()局部变量        │
│ 返回地址             │
│ libc调用现场         │
└─────────────────────┘


Kernel Stack
┌─────────────────────┐
│ 用户态寄存器pt_regs  │
│ syscall入口现场      │
│ VFS调用现场          │
│ 文件系统调用现场      │
└─────────────────────┘
```

这两套不是重复。

它们分别描述：

```text
User Stack
→ 用户程序函数调用世界

Kernel Stack
→ 内核执行系统调用的函数调用世界
```

------

## 7. 那谁负责把用户寄存器保存到 Kernel Stack？

这里有两部分：

```text
CPU硬件
+
Linux kernel entry code
```

共同完成。

不是全靠 C 代码。

------

## 8. ARM Cortex-A7 执行 `SVC` 时，CPU硬件会先做什么？

你现在用 i.MX6ULL：

```text
Cortex-A7
ARMv7-A
32-bit ARM
```

执行：

```asm
svc 0
```

以后，CPU硬件首先完成异常进入的基础工作。

概念上：

```text
当前用户态状态
↓
SVC异常
↓
CPU切换到SVC/特权异常环境
↓
保存必要的返回状态
↓
跳到异常向量入口
```

例如 ARMv7-A 会涉及：

```text
LR_svc
SPSR_svc
```

等异常模式相关寄存器。

<span style="color:#FF0000; background:#00FF80;">可以粗略理解：</span>

```text
CPU硬件先记住：

“以后从哪里回来？”
“原来是什么CPU状态？”
```

------

## 9. 然后 Linux Kernel `.text` 中的汇编入口代码接管

<span style="color:#FF0000; background:#00FF80;">CPU跳到 Linux 设置的异常入口后：</span>

```text
CPU
↓
执行Linux kernel .text
```

这时内核低级汇编代码会进一步保存用户态寄存器。

例如概念：

```text
r0
r1
r2
r3
...
用户sp
用户lr
pc
状态
```

放进当前线程：

```text
Kernel Stack
```

形成类似：

```text
pt_regs
```

的现场。

所以你问：

> <span style="color:#FF0000;">“保存现场的代码是不是 Linux 内核 `.text`？”</span>

<span style="background:#00FF80;">对于**系统调用异常入口的完整现场保存**来说：</span>

<span style="background:#00FF80;">**是的，核心保存逻辑属于内核的异常/系统调用入口代码，位于内核 `.text` 中。**</span>

但<span style="color:#FF0000; background:#00FF80;">还有一部分初始状态保存是 CPU 硬件自动完成的。</span>

------

## 10. 然后才真正执行 `sys_read`

完整过程：

```text
Thread A用户态

foo()
↓
libc read()
↓
准备：

r0 = fd
r1 = buf
r2 = count
r7 = syscall number
↓
svc
```

然后：

```text
CPU硬件
↓
异常进入
↓
切到内核态
```

然后：

```text
Linux kernel .text
↓
系统调用入口汇编
↓
把用户寄存器保存到
Thread A Kernel Stack
↓
找到 syscall number
↓
调用 read 对应的内核处理
```

再往下：

```text
VFS
↓
struct file
↓
Page Cache
↓
...
```

------

## 11. 那 `read()` 执行完后怎么恢复？

反过来。

假设：

```text
sys_read
↓
完成
↓
返回值 = 100
```

内核会把返回值按 ABI 放到对应寄存器。

ARM32 一般：

```text
r0
```

用于返回值。

于是：

```text
r0 = 100
```

然后 Linux 系统调用退出代码：

```text
Kernel Stack
↓
取出之前保存的用户态寄存器现场
↓
恢复必要寄存器
↓
恢复用户PC/状态
↓
异常返回
```

然后 CPU：

```text
Kernel Mode
↓
User Mode
```

用户程序继续：

```c
len = read(fd, buf, 100);
```

下一条语句。

------

## 12. 注意：恢复的也不是 User Stack 里的“内核现场”

你刚才说：

> “恢复现场应该发生在用户空间，因为从用户线程栈恢复。”

这个就是需要修正的地方。

系统调用退出时：

> <span style="color:#FF0000; background:#00FF80;">**内核先在内核态，从当前线程的 Kernel Stack 中恢复保存好的用户寄存器状态。**</span>

<span style="color:#FF0000;">然后执行异常返回。</span>

CPU才切回：

```text
User Mode
```

回到用户空间。

所以顺序是：

```text
Kernel Mode
↓
Kernel .text代码恢复现场
↓
从Kernel Stack取用户寄存器状态
↓
执行异常返回
↓
CPU切到User Mode
↓
继续用户程序
```

而不是：

```text
先回用户空间
↓
再由用户代码恢复CPU现场
```

------

## 13. 那 User Stack 上的普通函数现场什么时候恢复？

这是另一层。

假设：

```text
foo()
↓
调用 libc read()
```

系统调用回来之后：

```text
CPU已经恢复到用户态
↓
继续执行libc read wrapper
```

libc wrapper完成以后：

```text
ret
↓
返回foo()
```

这时候才按照普通函数调用规则：

```text
User Stack
↓
恢复函数调用相关现场
```

比如：

```text
返回地址
callee-saved registers
局部调用帧
```

这部分是：

> **用户态 ABI + 编译器/libc代码**

管理。

------

## 14. 所以你一定要区分这两次“恢复”

第一种：

```text
Kernel Stack
↓
恢复用户CPU寄存器现场
↓
内核态 → 用户态
```

负责人：

```text
CPU硬件 + Kernel .text入口/退出代码
```

------

第二种：

```text
User Stack
↓
恢复普通C函数调用现场
```

负责人：

```text
用户态编译器生成代码 / libc代码
```

它们完全不是一回事。

------

## 15. 举一个完整例子

代码：

```c
int foo(void)
{
    int x = 10;

    int ret = read(fd, buf, 100);

    x++;

    return ret;
}
```

执行：

```text
foo()
```

首先 User Stack：

```text
User Stack

foo返回地址
x = 10
需要保存的寄存器
...
```

然后调用：

```text
libc read()
```

仍然用户态。

------

接着 libc：

```text
r0 = fd
r1 = buf
r2 = 100
r7 = __NR_read
```

然后：

```text
svc
```

------

CPU进入内核：

```text
Kernel Stack

┌─────────────────┐
│ 用户r0          │
│ 用户r1          │
│ 用户r2          │
│ ...             │
│ 用户SP          │
│ 用户LR          │
│ 用户PC          │
│ 用户状态        │
├─────────────────┤
│ syscall函数现场 │
│ VFS调用现场     │
│ 文件系统现场     │
└─────────────────┘
```

------

read完成：

```text
r0 = 实际读到的字节数
```

内核：

```text
从Kernel Stack恢复
↓
异常返回
```

CPU回：

```text
User Mode
```

然后 libc wrapper：

```text
返回给foo()
```

普通函数返回再利用：

```text
User Stack
```

继续：

```c
x++;
```

------

## 16. 那 `SP` 本身怎么处理？

这个也很有意思。

系统调用之前：

```text
SP_user
```

指向：

```text
User Stack
```

进入内核以后：

```text
SP_kernel
```

会指向：

```text
当前线程Kernel Stack
```

所以可以概念上理解：

```text
用户态：

SP
↓
Thread A User Stack


SVC
↓


内核态：

SP
↓
Thread A Kernel Stack
```

这就是为什么：

> 内核函数不会继续往用户栈里压自己的调用现场。

------

## 17. 和 Cortex-M 的 MSP/PSP 很像，但不要完全画等号

你前面学过 FreeRTOS：

```text
Task运行
→ PSP

中断
→ MSP
```

所以你可能觉得 Linux 也类似。

思想上确实有点像：

> 不同执行环境使用不同栈。

但 ARMv7-A Linux 的 CPU mode、异常栈、Linux kernel stack 管理方式和 Cortex-M 的 MSP/PSP 不是同一套硬件机制。

所以只能类比：

```text
用户态有用户栈
内核态有内核栈
```

不要直接写成：

```text
Linux User Stack = PSP
Linux Kernel Stack = MSP
```

这是不对的。

------

## 18. <span style="background:#00FF80; color:#FF0000;">如果系统调用中途被阻塞怎么办？</span>

这个<span style="color:#FF0000; background:#00FF80;">更能说明 kernel stack 为什么必须每线程独立。</span>

比如：

```c
read(pipe_fd, buf, 100);
```

但<span style="color:#FF0000; background:#00FF80;">管道暂时没数据。</span>

Thread A：

```text
进入内核
↓
执行read
↓
发现没数据
↓
阻塞
```

它的<span style="color:#FF0000; background:#00FF80;"> Kernel Stack 此时可能保留：</span>

```text
系统调用现场
内核函数调用链
用户寄存器pt_regs
```

<span style="color:#FF0000; background:#00FF80;">然后调度器：</span>

```text
切换到Thread B
```

<span style="color:#FF0000; background:#00FF80;">Thread B 使用：</span>

```text
自己的Kernel Stack B
```

<span style="color:#FF0000; background:#00FF80;">以后数据来了：</span>

```text
Thread A重新被调度
↓
继续使用Kernel Stack A
↓
read继续执行
↓
最终返回用户态
```

所以：

> <span style="background:#00FF80; color:#FF0000;">**每线程一个 kernel stack 是 Linux 能让不同线程独立进入/阻塞/恢复内核执行的重要基础。**</span>

------

## 19. <span style="color:#FF0000; background:#00FF80;">那调度切换时是不是还会多保存一套 CPU context？</span>

是的。

这<span style="color:#FF0000; background:#00FF80;">是第三层“现场”。</span>

你<span style="color:#FF0000; background:#00FF80;">现在已经可以分成三种了：</span>

```text
① 用户普通函数调用现场
→ User Stack


② syscall/exception用户寄存器现场
→ Kernel Stack中的pt_regs等


③ scheduler线程切换现场
→ task/thread相关内核结构 + kernel stack等
```

第三种是：

```text
Thread A
↓
切Thread B
```

调度器需要保存足够的 CPU kernel execution context。

这个属于：

```text
Linux scheduling/context switch
```

比单纯 syscall 再深一层。

------

## 20. 最后用一张总图把它彻底定型

```text
                Thread A

┌────────────────────────────────────┐
│          USER SPACE                │
│                                    │
│  foo()                             │
│   │                                │
│   └── read() libc wrapper          │
│                                    │
│  User Stack                        │
│  ├─ foo局部变量                    │
│  ├─ 普通函数返回地址               │
│  └─ ABI保存寄存器                  │
└────────────────┬───────────────────┘
                 │
                 │ SVC
                 ▼
════════ 用户态 / 内核态 ════════════
                 ▼
┌────────────────────────────────────┐
│          KERNEL SPACE              │
│                                    │
│ Kernel .text                       │
│  ├─ syscall entry assembly         │
│  ├─ syscall dispatcher             │
│  ├─ sys_read / VFS                 │
│  └─ syscall exit assembly          │
│                                    │
│ Thread A Kernel Stack              │
│  ├─ 用户态pt_regs现场              │
│  ├─ syscall调用现场                │
│  ├─ VFS调用现场                    │
│  └─ 内核局部变量                   │
└────────────────┬───────────────────┘
                 │
                 │ 异常返回
                 ▼
          回到User Mode
                 │
                 ▼
          libc read()返回
                 │
                 ▼
             foo()继续
```

你可以直接记这几句话：

> **用户态调用 `read()` 前后的普通函数现场，属于 User Stack。**

> **`SVC` 进入内核后，为了以后恢复用户态而保存的寄存器现场，主要进入当前线程的 Kernel Stack。**

> **系统调用入口和退出的保存/恢复逻辑属于 Linux Kernel `.text` 中的低级汇编/异常处理代码，同时 CPU 硬件也会自动完成一部分异常状态保存。**

> **内核是在内核态先恢复好用户现场，然后执行异常返回，CPU 才重新回到用户态；不是先回到用户空间再由用户程序恢复。**

这四句话把系统调用的“现场保存”问题基本就完全理顺了。





# 问题4（了解一下）

你上一节视频看到：

```
SYSCALL_DEFINE3(read,
                unsigned int, fd,
                char __user *, buf,
                size_t, count)
```

这里：

```
3
```

就是：

> 三个参数。

而类似：

```
SYSCALL_DEFINE1(close,
                unsigned int, fd)
```

就是：

> 一个参数。

这些宏不只是为了让源码好看。



它们会帮助内核生成：

```
系统调用入口函数
参数转换
类型处理
wrapper
真正实现之间的连接
```

所以你不需要手工写：

```
read_wrapper(...)
close_wrapper(...)
```

大量重复代码。



宏帮内核做了很多这类事情。

这里我不是太理解，这里宏内核是个什么东西呢，好像和我们平时见到的宏感觉有点不太一样呢。



对，这里你感觉“它不像平时的宏”很正常，因为 Linux 内核把 C 宏用到了比较高级的程度。

先把最关键的一点纠正清楚：

> **`SYSCALL_DEFINE3` 没有什么特殊的“内核宏机制”。它<span style="color:#FF0000; background:#00FF80;">本质上仍然只是普通的 C 预处理器宏 `#define`。</span>**
>
> <span style="background:#00FF80;">只不过我们平时见到的宏可能只是替换一个数字、一个表达式；Linux 内核里的宏经常拿来**批量生成函数、变量名、参数声明甚至辅助代码**，所以看起来像一种新语法。</span>

而且我上一条说“它生成系统调用入口函数”有点说得太宽了。更准确地说：

> `SYSCALL_DEFINE3` 主要负责定义这个系统调用的 C 层实现及相关包装代码；真正的 `SVC → 异常入口 → syscall 分派` 那套低级入口，是架构相关的另外一部分内核代码。

------

## 1. 先从你熟悉的普通宏开始

最简单：

```c
#define PI 3.14
```

你写：

```c
float a = PI;
```

预处理以后：

```c
float a = 3.14;
```

所以宏最本质的事情只有：

> **在正式编译 C 代码之前，做文本/标记级展开。**

------

## 2. 再高级一点：带参数的宏

比如：

```c
#define ADD(a, b) ((a) + (b))
```

写：

```c
int c = ADD(10, 20);
```

<span style="color:#FF0000; background:#00FF80;">预处理器展开：</span>

```c
int c = ((10) + (20));
```

这个你应该比较熟悉。

------

## 3. 宏甚至可以“生成函数”

比如我自己定义：

```c
#define DEFINE_FUNC(name) \
    void name(void)       \
    {                     \
        printf(#name "\n"); \
    }
```

然后写：

```c
DEFINE_FUNC(hello)
```

<span style="color:#FF0000; background:#00FF80;">经过预处理以后，相当于生成：</span>

```c
void hello(void)
{
    printf("hello\n");
}
```

你会发现：

> <span style="background:#00FF80;">宏已经不只是替换一个数字了，它可以直接帮我们**生成一整段 C 源代码**。</span>

Linux 内核大量使用的就是这种思想。

------

## 4.`SYSCALL_DEFINE3` 本质也是这个东西

你看到：

```c
SYSCALL_DEFINE3(read,
                unsigned int, fd,
                char __user *, buf,
                size_t, count)
```

注意它的参数排列非常规律：

```text
read
↓
系统调用名字


unsigned int, fd
↓
第1个参数：
类型 + 参数名


char __user *, buf
↓
第2个参数：
类型 + 参数名


size_t, count
↓
第3个参数：
类型 + 参数名
```

所以：

```text
SYSCALL_DEFINE3
               ↑
               3
```

表示：

> <span style="color:#FF0000; background:#00FF80;">后面有 3 组“类型 + 参数名”。</span>

------

## 5. 你可以先把它想象成一个非常简化的宏

假设 Linux 只是这样定义：

```c
#define SYSCALL_DEFINE3(name, t1, a1, t2, a2, t3, a3) \
long sys_##name(t1 a1, t2 a2, t3 a3)
```

那么：

```c
SYSCALL_DEFINE3(read,
                unsigned int, fd,
                char __user *, buf,
                size_t, count)
{
    /* read 的实现 */
}
```

预处理以后，大概就变成：

```c
long sys_read(unsigned int fd,
              char __user *buf,
              size_t count)
{
    /* read 的实现 */
}
```

你看，突然就很普通了。

------

## 6. `##` 又是什么？

刚才：

```c
sys_##name
```

这里：

```text
##
```

叫：

> **token pasting，记号拼接。**

比如：

```text
name = read
```

那么：

```c
sys_##name
```

展开成：

```c
sys_read
```

如果：

```text
name = close
```

就是：

```c
sys_close
```

所以一个宏：

```c
SYSCALL_DEFINE...
```

就能自动根据名字生成不同函数名。

------

## 7. 这就是为什么 Linux 特别喜欢宏

假设 Linux 有几百个系统调用。

如果每一个都手工写：

```c
sys_read(...)
sys_write(...)
sys_close(...)
sys_open(...)
```

还要重复写：

```text
参数转换
类型检查
wrapper
trace相关信息
符号命名
```

会有大量重复代码。

于是内核开发者就说：

> “这些代码结构都差不多，那我写一个宏模板，让预处理器自动生成。”

所以：

```c
SYSCALL_DEFINE3(...)
```

本质很像：

> **代码生成模板。**

------

## 8. 但真实 Linux 比刚才那个简单宏复杂得多

现代 Linux 的通用 syscall 宏展开后，经常会形成几层函数。

你可以先用这个**概念模型**理解：

```text
SYSCALL_DEFINE3(read, ...)
        │
        │ 预处理器展开
        ▼

系统调用包装层
        │
        │ 参数转换 / 类型处理
        ▼

真正的 read C 实现
```

现代内核里你可能看到类似名字：

```text
__se_sys_read
__do_sys_read
```

其中可以粗略理解：

```text
__se_sys_read
↓
wrapper
负责系统调用参数表示/转换等


__do_sys_read
↓
真正按C参数类型处理业务逻辑
```

具体符号和层次会随架构、内核版本和配置有所区别，所以不要死背名字。

------

## 9. 为什么还需要 wrapper？

这正好接上你上一问：

> 每个系统调用参数类型都不一样，系统调用入口拿到的却首先是一组寄存器值，那怎么办？

比如用户调用：

```c
read(3, buf, 100);
```

ARM32 EABI 概念上：

```text
r0 = 3
r1 = buf地址
r2 = 100
r7 = __NR_read
```

CPU寄存器本身并不知道：

```text
r0 是 unsigned int

r1 是 char __user *

r2 是 size_t
```

寄存器里本质都是二进制值。

所以系统调用包装层需要把：

```text
机器级参数表示
↓
转换/解释
↓
C函数真正需要的参数类型
```

然后再交给真正实现。

------

## 10. 举个特别简化的 wrapper

假设底层统一拿到：

```text
arg0
arg1
arg2
```

那么可以想象：

```c
long read_wrapper(long arg0,
                  long arg1,
                  long arg2)
{
    unsigned int fd;
    char __user *buf;
    size_t count;

    fd    = (unsigned int)arg0;
    buf   = (char __user *)arg1;
    count = (size_t)arg2;

    return real_read(fd, buf, count);
}
```

于是：

```text
寄存器/机器参数
      ↓
read_wrapper
      ↓
类型转换
      ↓
real_read(fd, buf, count)
```

Linux 的宏就是在帮忙自动产生大量类似的套路代码。

------

## 11. 所以 `SYSCALL_DEFINE3` 不是系统调用本身的“SVC入口”

一定要把这里分开。

完整链条应该是：

```text
用户空间

read()
↓
libc wrapper
↓
准备系统调用号和寄存器
↓
SVC


══════════════════════

ARM/Linux低级异常入口
↓
保存寄存器现场
↓
获取系统调用号
↓
syscall分派
↓
找到read对应入口
↓
SYSCALL_DEFINE3产生的C层实现/包装
↓
真正read逻辑
```

所以：

```text
SVC入口
```

和：

```text
SYSCALL_DEFINE3
```

不是一回事。

一个偏：

> CPU / 汇编 / 架构入口。

一个偏：

> C 层系统调用定义。

------

## 12. `SYSCALL_DEFINE1、2、3、4...` 为什么分这么多？

就是为了不同参数数量。

例如：

```c
SYSCALL_DEFINE1(close,
                unsigned int, fd)
```

宏知道：

```text
1个参数
```

于是模板按照：

```text
类型1 参数1
```

展开。

------

而：

```c
SYSCALL_DEFINE3(read,
                unsigned int, fd,
                char __user *, buf,
                size_t, count)
```

知道：

```text
3个参数
```

模板就处理：

```text
类型1 参数1
类型2 参数2
类型3 参数3
```

所以：

```text
SYSCALL_DEFINE1
SYSCALL_DEFINE2
SYSCALL_DEFINE3
...
```

本质上就是：

> **不同参数数量的代码生成模板。**

------

## 13. 但它们背后通常还会汇总到一个更通用的宏

内核不会真的把所有代码复制写：

```text
DEFINE1一整套
DEFINE2又一整套
DEFINE3又一整套
```

通常还会继续套娃。

概念类似：

```c
#define SYSCALL_DEFINE1(...) \
    SYSCALL_DEFINEx(1, ...)

#define SYSCALL_DEFINE2(...) \
    SYSCALL_DEFINEx(2, ...)

#define SYSCALL_DEFINE3(...) \
    SYSCALL_DEFINEx(3, ...)
```

然后真正大量通用逻辑放：

```text
SYSCALL_DEFINEx
```

里面。

所以你会发现 Linux 内核宏经常：

```text
宏
↓
展开成另一个宏
↓
再展开
↓
再生成C代码
```

这也是为什么第一次看内核源码会觉得：

> “这根本不像 C 语言。”

其实最终经过预处理以后，还是正常 C/C++ 编译器能理解的代码。

------

## 14. 编译流程在这里特别重要

假设内核源码：

```c
SYSCALL_DEFINE3(read, ...)
{
    ...
}
```

编译不是直接：

```text
源码
↓
编译器理解SYSCALL_DEFINE3
```

而是：

```text
Linux C源码
↓
C预处理器 cpp
↓
展开所有 #define / 宏
↓
得到普通C代码
↓
C编译器
↓
汇编
↓
机器码
```

所以：

> CPU 运行的时候根本不知道世界上有 `SYSCALL_DEFINE3` 这个宏。

它早在编译阶段就没了。

CPU最终看到的只有：

```text
函数机器指令
```

------

## 15. 这和你之前学 Makefile 里的变量不一样

这个也可以顺手区分。

C 宏：

```c
#define ABC 123
```

由：

```text
C预处理器
```

处理。

Makefile：

```make
CC = gcc
```

由：

```text
make
```

处理。

Shell：

```bash
VAR=123
```

由：

```text
shell
```

处理。

它们看起来都有“替换”的感觉，但不是同一种机制。

------

## 16. `__user` 也不是普通 C 类型的一部分

比如：

```c
char __user *buf
```

你可能觉得：

```text
char是类型

__user是什么新类型？
```

其实 `__user` 主要是一种内核源码中的地址空间标注，配合静态分析工具（例如 Sparse）帮助发现：

> “你是不是把用户空间指针当成内核指针乱用了？”

所以：

```text
char __user *buf
```

强调：

> `buf` 指向用户空间。

并不是 CPU 多了一个叫 `__user` 的硬件类型。

------

## 17. 为什么 Linux 内核宏看起来特别吓人？

因为它会同时使用 C 预处理器很多能力：

```text
带参数宏
多层宏展开
## token拼接
# 字符串化
可变参数
条件编译
typeof
编译器扩展
```

所以有时候一个：

```c
SYSCALL_DEFINE3(...)
```

展开后可能生成很多东西。

但你学习的时候不要一开始就追所有宏定义。

最好：

```text
第一层：
它想表达什么？

↓
定义一个3参数系统调用


第二层：
为什么用宏？

↓
自动生成重复模板代码


第三层：
宏展开后大概是什么？

↓
wrapper + 真正实现


最后再看：
Linux源码具体怎么写
```

这样不会掉进宏套宏的迷宫。

------

## 18. 你可以把 `SYSCALL_DEFINE3` 当成“高级函数声明模板”

例如普通 C：

```c
long read_impl(unsigned int fd,
               char *buf,
               size_t count)
{
    ...
}
```

Linux 写：

```c
SYSCALL_DEFINE3(read,
                unsigned int, fd,
                char __user *, buf,
                size_t, count)
{
    ...
}
```

不是为了让它变成某种“新语言”。

而是：

> **让预处理器在你写这一份真正逻辑的同时，自动补齐系统调用规范需要的外围代码。**

------

## 最后用一条链记住

```text
你看到源码：

SYSCALL_DEFINE3(read, ...)
        │
        ▼
C预处理器展开
        │
        ▼
生成若干普通C函数/包装代码
        │
        ▼
C编译器编译
        │
        ▼
Kernel .text机器指令
        │
        ▼
运行时系统调用分派后执行
```

所以一句话：

> **Linux 内核里的 `SYSCALL_DEFINE3` 本质还是普通 `#define` 宏，只是它不像 `#define PI 3.14` 那么简单，而是把宏当作“编译前代码生成器”，自动生成系统调用实现所需的函数声明、包装、参数转换和相关辅助代码。**

而真正的：

```text
SVC
↓
保存现场
↓
查系统调用号
```

属于另一套 **ARM 架构相关的内核 `.text` 入口代码**。两层接起来，才是完整的系统调用路径。



# 问题5（一个重要思考）

我有一个思考，就是我们好像可以将每一个函数看成可以一个操作逻辑，这个操作可能需要用到局部变量，CPU内部寄存器，其他操作逻辑传递过来的参数，故我们每次切换到另外一个操作逻辑的时候需要在切换前在本操作逻辑内保存现场，即CPU寄存器等内容。另外如果是发生了中断的情况下，则是在中断发生的那个操作逻辑（即被切换的那个操作逻辑）中来保存所切换的那个操作逻辑的现场以及恢复现场。另外如果利用svc进入内核态，即系统调用这种情况，这里其实牵涉到两套保存现场，一部分由操作逻辑保存自己现场，另一部分由svc之后由被切换到的操作的逻辑保存所切换逻辑的现场。这个思考对吗。



你的这个思考**大方向是对的，而且已经抓到“现场保存”的统一思想了**：<span style="color:#FF0000; background:#00FF80;">只要一段执行逻辑要被暂时离开、以后还要从原处继续，就必须保留足够的信息。</span>

不过有两个地方需要修正：<span style="background:#00FF80;">**不是每次函数切换都保存“完整 CPU 现场”**；还有**中断发生时，不是被中断的代码自己去保存现场，而是 CPU 硬件 + 中断入口代码替它保存**。</span>

可以把你这个想法整理成下面这套模型。

### 1. 普通函数调用：<span style="background:#00FF80;">保存的是“函数调用现场”，不是完整 CPU 现场</span>

比如：

```c
foo();
bar();
```

CPU 从 `foo()` 调到 `bar()`，确实相当于切换到另一段操作逻辑，但<span style="background:#00FF80;">不需要把所有寄存器全部保存。</span>

通常按照 ABI 约定，<span style="color:#FF0000; background:#00FF80;">只保存“以后恢复 `foo()` 必需的那些东西”</span>：

```text
foo()
│
├─ 局部变量
├─ 返回地址
├─ 必要寄存器
│
└─ 调用 bar()
      ↓
     bar()
      ↓
     return
      ↓
foo()继续
```

这里<span style="color:#FF0000;">保存工作由：</span>

> <span style="color:#FF0000;">编译器生成的用户态指令 + ABI 约定</span>

完成，<span style="background:#00FF80;">通常使用 **用户栈**。</span>

而且 CPU 寄存器还分成类似：

```text
caller-saved
callee-saved
```

不是所有寄存器每次函数调用都压栈。

所以更准确：

> <span style="background:#00FF80;">**普通函数切换保存“函数调用所需现场”，通常不是完整 CPU 上下文。**</span>

------

### 2. 中断：被中断的逻辑来不及自己保存

这个是你原思考里最需要改的一点。

假设：

```text
Task A / 用户程序
正在执行
      ↓
突然来了中断
```

<span style="color:#FF0000; background:#00FF80;">中断是异步发生的。</span>

<span style="color:#FF0000;">被中断的程序不会先执行一句：</span>

```text
“等一下，我先把自己保存好。”
```

因为<span style="background:#00FF80; color:#FF0000;">它根本不知道中断什么时候来。</span>

<span style="color:#FF0000; background:#00FF80;">实际是：</span>

```text
正在运行的逻辑
      ↓
中断突然发生
      ↓
CPU硬件自动保存一部分必要状态
      ↓
跳到中断入口
      ↓
中断入口代码继续保存剩余需要的状态
      ↓
ISR运行
```

所以这里更准确地说：

> <span style="background:#00FF80; color:#FF0000;">**被中断逻辑的现场，是由 CPU 硬件和中断处理入口替它保存的。**</span>

恢复也是一样：

```text
ISR结束
↓
中断退出代码
↓
恢复之前保存的寄存器
↓
CPU异常返回
↓
原程序从被打断的位置继续
```

------

### 3. SVC / 系统调用：你的“两套现场”理解非常接近

这里确实可以理解成两层。

假设：

```c
foo()
{
    read(fd, buf, 100);
}
```

第一层：

```text
foo()
↓
调用 libc read()
```

这还是普通函数调用。

所以可能产生：

```text
User Stack

foo的局部变量
返回地址
ABI要求保存的寄存器
libc调用帧
```

这是：

> **普通用户态函数调用现场。**

然后 libc 准备：

```text
r0 = fd
r1 = buf
r2 = 100
r7 = syscall number
```

执行：

```asm
svc 0
```

这时进入第二层：

```text
SVC
↓
CPU硬件保存异常返回所必需的信息
↓
进入Kernel
↓
Kernel入口代码
↓
把用户寄存器现场保存到Kernel Stack
```

所以你说系统调用涉及“两套保存”，是可以的：

```text
第一套：
普通函数调用现场
↓
User Stack
↓
为了 read() 返回后继续 foo()


第二套：
用户态 → 内核态 的异常现场
↓
Kernel Stack
↓
为了系统调用结束后重新回到用户态
```

它们目的不一样。

------

### 4. 为什么需要两套？因为跨了两个不同的“边界”

可以这样看：

```text
foo()
 │
 │ 普通函数调用
 ▼
libc read()
 │
 │ SVC异常
 ▼
Linux Kernel
```

第一个边界：

```text
foo → libc read
```

属于：

> **函数调用边界**

按照 ABI 处理。

第二个边界：

```text
User Mode → Kernel Mode
```

属于：

> **CPU特权级 / 异常边界**

<span style="background:#00FF80;">按照 CPU 异常机制 + Linux 内核入口处理。</span>

所以<span style="color:#FF0000; background:#00FF80;">确实可能同时存在：</span>

```text
User Stack
+
Kernel Stack
```

<span style="color:#FF0000; background:#00FF80;">两套不同意义的现场。</span>

------

### 5. 还有<span style="background:#00FF80;">第三种：线程切换</span>

比如：

```text
Thread A运行
↓
调度器决定切换
↓
Thread B运行
```

这时候<span style="color:#FF0000;">也需要保存：</span>

> <span style="color:#FF0000; background:#00FF80;">Thread A 以后重新运行需要的 CPU 执行上下文。</span>

于是：

```text
Thread A
↓
保存调度上下文
↓
选择Thread B
↓
恢复Thread B上下文
↓
CPU继续执行Thread B
```

<span style="color:#FF0000; background:#00FF80;">这里又和普通函数调用不同，因为：</span>

> <span style="background:#00FF80;">函数调用通常还是同一个线程；</span>
> <span style="background:#00FF80;">线程切换则是整个执行流换掉了。</span>

所以<span style="color:#FF0000; background:#00FF80;">你可以把“现场保存”分成三个层次：</span>

| 场景           | 保存什么                             | 谁保存                                                       | 常见位置                                                     |
| -------------- | ------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 普通函数调用   | 函数调用需要的部分寄存器、返回地址等 | 编译器生成代码 / ABI                                         | <span style="color:#FF0000;">User Stack 或 Kernel Stack</span>，<span style="background:#00FF80;">取决于当前运行态</span> |
| 中断 / SVC     | 被打断执行流的异常现场               | CPU硬件 + 异常入口代码                                       | 通常内核受控的栈/结构                                        |
| 线程上下文切换 | 足够恢复该线程的调度上下文           | <span style="color:#FF0000; background:#00FF80;">OS调度器</span> | <span style="color:#FF0000; background:#00FF80;">Kernel Stack + task相关结构</span> |

------

### 6. 你的“操作逻辑”这个模型可以保留，但要加一个条件

你说：

> 每一个函数都可以看成一个操作逻辑。

这个作为理解模型很好。

但不要进一步认为：

> 每切换一个函数，就等于发生一次线程上下文切换。

不是。

可以这样分：

```text
函数
→ 小粒度执行逻辑

线程
→ 一条完整执行流

进程
→ 资源容器

中断/异常
→ 暂时打断当前执行流的特殊控制转移
```

例如：

```text
Thread A

main()
↓
foo()
↓
bar()
↓
read()
```

这些函数虽然不断切换，但仍然是：

> **同一个 Thread A 在执行。**

只有：

```text
Thread A
↓
Scheduler
↓
Thread B
```

才是线程执行上下文真正换了。

------

### 7. 再把你原来的表述改成一个更准确的版本

你原来的想法可以升级成：

> <span style="background:#00FF80; color:#FF0000;">**一段执行逻辑如果要被暂时离开而以后继续，就必须保留足够的执行状态。但不同控制转移保存的现场不同：普通函数调用由 ABI 和编译器只保存函数调用所需的部分状态；中断和 SVC 属于异常控制转移，被打断的代码不会主动保存自己，而是 CPU 硬件和异常入口代码保存它的状态；系统调用因为通常先经过 libc 普通函数调用、再通过 SVC 进入内核，因此可能同时存在用户栈上的普通函数调用现场和内核栈上的异常/系统调用现场。线程上下文切换则由调度器保存足够的线程执行上下文，以便以后继续运行。**</span>

最值得记的图就是：

```text
用户线程执行

foo()
 │
 │ 普通call
 ▼
libc read()
 │
 │
 │ User Stack
 │ 保存普通函数调用现场
 │
 ▼
SVC
 │
 │ CPU硬件 + Kernel entry
 ▼
Kernel Stack
保存用户态异常现场
 │
 ▼
sys_read / VFS
 │
 ▼
Kernel exit
恢复Kernel Stack中的用户现场
 │
 ▼
回到libc read()
 │
 │ 普通return
 ▼
恢复User Stack中的函数调用现场
 │
 ▼
foo()继续
```

所以你这个思考的核心是对的，只需要把“**谁保存**”和“**保存的是哪一种现场**”再分清楚，就非常完整了。







# 问题6

你能够再给我解释下系统调用中途被阻塞时候的保存现场是什么样的吗，这里我还是有点不是太懂。



可以，这里确实值得单独再拆一次。你真正卡住的点应该是：

> <span style="background:#00FF80;">**系统调用已经进入内核了，执行到一半因为没有数据而阻塞，那么 CPU 现在要去运行别的线程。此时原线程到底要保存什么？保存在哪里？等它醒来以后又怎么从 `read()` 中间继续？**</span>

这个过程里其实有 **两层现场同时存在**，而且最关键的一点是：

> <span style="background:#00FF80;">**线程阻塞时，并不会把整个 Kernel Stack 再复制一份到别处。它原来的 Kernel Stack 就原封不动地留在内存里。调度器只需要再保存少量<span style="color:#FF0000;">“内核执行上下文”</span>，然后把 CPU 的 SP 切到另一个线程的 Kernel Stack。**</span>

我们用一个具体例子看最清楚：

```c
read(pipefd, buf, 100);
```

<span style="color:#FF0000; background:#00FF80;">假设这个管道现在一个字节都没有。</span>

------

## 先看阻塞发生之前

Thread A 在用户态：

```text
Thread A
User Space

foo()
  ↓
read(pipefd, buf, 100)
```

用户栈里可能已经有：

```text
Thread A User Stack

┌─────────────────────┐
│ foo() 局部变量       │
│ 普通函数返回地址     │
│ libc read调用现场    │
└─────────────────────┘
```

然后 libc 准备参数，执行：

```asm
svc 0
```

进入内核。

<span style="color:#FF0000; background:#00FF80;">这时候第一套现场已经产生：</span>

```text
Thread A Kernel Stack

┌─────────────────────┐
│ 用户态寄存器现场     │
│ pt_regs             │
│                     │
│ 用户 r0/r1/r2...    │
│ 用户 SP             │
│ 用户 PC             │
│ 用户状态寄存器       │
└─────────────────────┘
```

它的作用是：

> <span style="background:#00FF80; color:#FF0000;">将来 `read()` 完成以后，能够从内核重新回到用户程序。</span>

<span style="color:#FF0000;">然后内核继续执行：</span>

```text
sys_read
↓
VFS
↓
pipe read
```

<span style="background:#00FF80; color:#FF0000;">因此 Kernel Stack 上还会继续出现内核函数的调用现场：</span>

```text
Thread A Kernel Stack

┌────────────────────────┐
│ 用户态 pt_regs          │
├────────────────────────┤
│ syscall/read 调用现场   │
├────────────────────────┤
│ VFS 调用现场            │
├────────────────────────┤
│ pipe read 调用现场      │
└────────────────────────┘
```

注意，<span style="background:#00FF80;">这些都是 **Thread A 自己的 Kernel Stack**。</span>

------

## 关键来了：发现管道没数据

内核执行到某个地方发现：

```text
Pipe
↓
没有数据
```

<span style="color:#FF0000;">如果这是一个阻塞式 `read()`，Linux不会一直：</span>

```text
while (没数据) {
    一直检查;
}
```

这样会白白占着 CPU。

<span style="color:#FF0000; background:#00FF80;">而是大致：</span>（<span style="color:#FF0000; background:#00FF80;">注意：进程或者线程的调度是通过时钟中断产生的，发生调度肯定会牵涉到中断现场的保存和恢复，这一部分也是在内核栈中实现的，这里的内核栈不是属于某个线程的内核栈，而是这个内核的主栈</span>）

```text
Thread A
↓
“现在条件不满足，我先睡。”
↓
加入等待机制
↓
把自己的状态设为 sleeping
↓
调用 schedule()
```

比如概念上：

```text
TASK_RUNNING
↓
TASK_INTERRUPTIBLE
```

或者某些情况：

```text
TASK_UNINTERRUPTIBLE
```

然后进入调度器。

------

## 这时候为什么还需要“第二次保存现场”？

因为现在 Thread A 已经在：

```text
Kernel Mode
```

里面执行了。

CPU当前寄存器不是原来用户态的：

```text
用户 r0
用户 r1
用户 PC
```

而是内核正在执行：

```text
pipe_read()
schedule()
```

所使用的寄存器。

比如此刻概念上：

```text
CPU

r4 = 某个内核变量
r5 = 某个指针
SP = Thread A Kernel Stack
PC = schedule附近的内核代码
...
```

现在 CPU 要去执行 Thread B。

那么以后 Thread A 被唤醒时，必须能够：

> **从 `schedule()` 后面的那个位置继续执行。**

所以还需要保存一套：

> **Kernel Context，内核执行上下文。**

------

## 但是这里不会重新保存“整个用户态现场”

因为用户态现场：

```text
pt_regs
```

早就在：

```text
Thread A Kernel Stack
```

里面了。

所以阻塞时没有必要再来一次：

```text
再保存用户r0
再保存用户r1
再保存用户PC
```

它们根本没丢。

真正新增保存的是：

> **Thread A 当前在内核里执行到什么地方，以及恢复内核执行所需要的寄存器。**

------

## 所以阻塞时有两套不同的现场

可以直接这样记：

```text
Thread A Kernel Stack

┌──────────────────────────┐
│ 用户态现场 pt_regs        │
│                          │
│ 作用：                    │
│ syscall最后回用户空间     │
├──────────────────────────┤
│ read/VFS/pipe函数调用帧    │
│                          │
│ 作用：                    │
│ 保存内核函数调用过程       │
└──────────────────────────┘

另外：
Kernel Scheduling Context

SP
必要的callee-saved寄存器
内核返回位置等

作用：
以后让Thread A继续执行内核代码
```

第二部分的具体保存位置和形式是架构相关的，可能涉及：

```text
task_struct
thread_struct
Kernel Stack
```

等。

不要把它理解成固定一个：

```c
struct saved_context
```

所有架构都完全一样。

------

## 调度器真正做了什么？

可以非常粗略地想成：

```text
Thread A 正在运行

CPU:
SP ─────→ Thread A Kernel Stack
PC ─────→ Linux Kernel .text 中的 schedule相关代码
```

<span style="color:#FF0000; background:#00FF80;">调用调度器：</span>

```text
保存 Thread A 必要的 kernel context
↓
选择 Thread B
↓
恢复 Thread B 的 kernel context
```

最有意思的是：

> **Kernel Stack 本身不用搬。**

只要改变：

```text
CPU的 SP
```

就可以换一个线程的栈。

比如原来：

```text
SP
 ↓
Thread A Kernel Stack
```

切换后：

```text
                    Thread A Kernel Stack
                    ┌──────────────┐
                    │现场还在这里  │
                    │没人动它      │
                    └──────────────┘


CPU SP
 ↓
Thread B Kernel Stack
┌──────────────┐
│B的内核现场   │
└──────────────┘
```

这一下非常重要。

------

## Thread A 的内核栈不会消失

假设：

```text
Thread A Kernel Stack
```

里面已经是：

```text
pt_regs
read调用帧
VFS调用帧
pipe调用帧
schedule相关现场
```

Thread A 被切走以后：

```text
这些数据原封不动
```

依然留在 RAM 中。

CPU只是：

> 不再使用这个 Stack 了。

转而使用：

```text
Thread B Kernel Stack
```

所以可以把 Kernel Stack 想成：

> **Thread A 暂停时被冻结的一摞函数调用现场。**

------

## 举一个特别形象的例子

假设 Thread A 的内核调用链：

```text
sys_read()
  ↓
vfs_read()
  ↓
pipe_read()
  ↓
schedule()
```

现在没有数据。

Thread A Kernel Stack 可以粗略理解成：

```text
┌──────────────────────┐
│ 用户态 pt_regs        │
├──────────────────────┤
│ sys_read frame       │
├──────────────────────┤
│ vfs_read frame       │
├──────────────────────┤
│ pipe_read frame      │
├──────────────────────┤
│ schedule相关现场      │
└──────────────────────┘
```

然后：

```text
CPU离开Thread A
```

但是这整摞东西：

```text
一张都不扔
```

就放在那里。

------

## 然后 CPU 去执行 Thread B

比如：

```text
Thread B
```

自己也有：

```text
User Stack B
Kernel Stack B
task_struct B
```

CPU切换后：

```text
SP = Thread B Kernel Stack
```

然后：

```text
PC = Thread B之前被切走的位置
```

继续执行 B。

所以真正切换：

```text
Thread A Kernel Stack
                ↓
             暂时不用

CPU
↓
Thread B Kernel Stack
```

------

## 那 Thread A 怎么被唤醒？

假设后来 Thread C：

```c
write(pipefd, "hello", 5);
```

给管道写了数据。

内核发现：

```text
管道现在有数据了
```

于是：

```text
唤醒等待这个pipe的Thread A
```

Thread A 状态：

```text
Sleeping
↓
Runnable
```

注意：

> **唤醒不等于立刻运行。**

只是：

```text
“你现在具备运行条件了。”
```

然后进入 CPU 的 runnable 调度体系。

等 Scheduler 真正选择 Thread A：

```text
restore Thread A kernel context
↓
SP重新指向Thread A Kernel Stack
↓
恢复需要的内核寄存器
↓
继续执行
```

------

## 最神奇的一点：`schedule()` 会像“正常返回”一样继续

假设 Thread A 原来：

```c
pipe_read()
{
    ...
    schedule();
    /* 这里 */
    ...
}
```

它被切出去。

很久以后重新被调度回来：

```text
恢复内核上下文
↓
schedule() 返回
↓
继续执行 schedule() 后面的代码
```

对于 `pipe_read()` 来说，就好像：

```text
schedule()
```

只是“花了特别长时间才返回”。

这就是上下文切换非常漂亮的地方。

------

## 然后 read 再检查一次条件

Thread A醒来：

```text
schedule返回
↓
继续pipe_read
↓
重新检查pipe
```

现在：

```text
pipe中有"hello"
```

于是：

```text
读出5字节
↓
copy到用户buf
↓
向上返回
```

调用栈开始一层一层退出：

```text
pipe_read()
↑
vfs_read()
↑
sys_read()
↑
syscall exit
```

Kernel Stack 上这些调用帧也逐渐被释放。

------

## 最后还剩第一套现场：pt_regs

到系统调用准备退出：

```text
Kernel Stack

┌──────────────────┐
│ 用户态 pt_regs    │
└──────────────────┘
```

Linux 从这里恢复：

```text
用户PC
用户SP
用户寄存器
用户状态
```

然后异常返回：

```text
Kernel Mode
↓
User Mode
```

CPU重新使用：

```text
Thread A User Stack
```

继续：

```c
ret = read(pipefd, buf, 100);

/* 从这里继续 */
```

假设读到 5 字节：

```text
ret = 5
buf = "hello"
```

------

## 所以完整过程其实是这样

```text
Thread A用户态
│
│ read()
▼
SVC
│
▼
保存用户态现场
→ Thread A Kernel Stack中的pt_regs
│
▼
sys_read
│
▼
vfs_read
│
▼
pipe_read
│
│ 没数据
▼
加入等待
│
设置Sleeping
│
▼
schedule()
│
│
│ 保存Thread A kernel context
│
▼
==============================
        Context Switch
==============================
│
▼
Thread B运行


          一段时间以后


Thread C写pipe
│
▼
唤醒Thread A
│
▼
Thread A变Runnable
│
▼
Scheduler选中A
│
▼
恢复A的kernel context
│
▼
SP → A Kernel Stack
│
▼
schedule()返回
│
▼
pipe_read继续
│
│ 现在有数据
▼
读数据
│
▼
vfs_read返回
│
▼
sys_read返回
│
▼
从pt_regs恢复用户现场
│
▼
异常返回
│
▼
Thread A用户态继续
```

------

## 这里最容易混淆的是“到底保存了几次”

你可以认为有三层：

| 时机                    | 保存什么         | 主要在哪里                  | 为了什么               |
| ----------------------- | ---------------- | --------------------------- | ---------------------- |
| 用户函数调用 `read()`   | 普通函数调用现场 | User Stack                  | libc返回后继续原函数   |
| `SVC` 进入内核          | 用户态寄存器现场 | Kernel Stack / `pt_regs`    | syscall结束后回用户态  |
| syscall阻塞发生线程切换 | 内核执行上下文   | Kernel Stack + task相关结构 | 醒来后继续执行 syscall |

所以：

> **阻塞不是把前面的现场推翻重新保存，而是在已经存在的 syscall 现场上，再增加一层“调度上下文保存”。**

这句话特别关键。

------

## 还有一个非常重要的区别

不要认为 Thread A 阻塞以后：

```text
整个Kernel Stack
↓
复制到task_struct
```

不是。

正确的是：

```text
Kernel Stack A
依然原地存在
```

调度器只保存：

> **以后重新启用这份 Kernel Stack 所需要的关键 CPU 上下文。**

例如可以粗略理解：

```text
SP
必要的寄存器
恢复执行的位置
```

然后切：

```text
SP_A
↓
SP_B
```

这就是为什么每个线程必须拥有自己的 Kernel Stack。

------

## 最后给你一个“书签”类比

这个特别适合现在这个问题。

Thread A执行：

```text
read
→ VFS
→ pipe_read
→ schedule
```

就像一本书看到这里：

```text
第100页第15行
```

Thread A Kernel Stack 就像：

> **整本书当前已经展开的章节关系仍然放在桌子上。**

调度上下文只需要额外记录：

> **书签：我以后从哪里继续。**

然后 CPU拿起 Thread B 的书。

等 Thread A醒来：

```text
重新拿起A的书
↓
找到书签
↓
继续阅读
```

最后：

```text
pipe_read
↓
vfs_read
↓
sys_read
```

一层层返回，

直到：

```text
pt_regs
```

告诉 CPU：

> “系统调用之前，用户程序读到这里。”

然后再回用户态。

所以一句话记住：

> **系统调用阻塞时，用户态现场早已保存在 Thread 的 Kernel Stack 中；内核调用链本身也冻结在这个 Kernel Stack 上。调度器只需要再保存足够的内核调度上下文并切换到另一线程的 Kernel Stack。线程被唤醒并重新调度后，恢复这套内核上下文，从原来 `schedule()` 附近继续执行，最终系统调用完成后再利用早先的 `pt_regs` 返回用户态。**



# 问题7（某个线程被中断时不是发生在这个线程内核栈里面的，而是发生在per cpu的ISR栈里面，但保存的这个线程的现场确是在这个线程的内核栈里面的）

 注意：进程或者线程的调度是通过时钟中断产生的，发生调度肯定会牵涉到中断现场的保存和恢复，这一部分也是在内核栈中实现的，这里的内核栈不是属于某个线程的内核栈，而是这个内核的主栈。我的理解对吗。



你的理解里有一半是对的，但有两个关键点需要修正：

> <span style="background:#00FF80;">**调度确实经常和时钟中断有关，中断现场也确实必须保存；但 Linux 并没有一个“整个内核大家共用的主栈”来承担所有中断和调度。**</span>

更准确地说，<span style="color:#FF0000; background:#00FF80;">Linux 里要区分：</span>

```text
① 当前线程自己的 Kernel Stack
② 某些架构/配置下每个 CPU 自己的 IRQ Stack
③ 线程调度时保存的线程上下文
```

而且，**调度也不一定由时钟中断触发**。

------

## 1. 先纠正第一个点：<span style="background:#00FF80;">调度不只是时钟中断产生的</span>

<span style="color:#FF0000;">时钟中断确实是最经典的一种情况：</span>

```text
Thread A运行
    ↓
定时器中断
    ↓
Linux更新运行时间
    ↓
发现A该让出CPU了
    ↓
设置 need_resched
    ↓
适当时机调度
```

但<span style="color:#FF0000; background:#00FF80;">线程还可能因为：</span>

```text
read()没数据 → 主动睡眠
mutex拿不到 → 阻塞
等待I/O → 阻塞
主动 yield
更高优先级任务被唤醒
```

<span style="color:#FF0000; background:#00FF80;">而发生调度。</span>

比如我们刚刚说的：

```c
read(pipefd, buf, 100);
```

没有数据：

```text
Thread A
↓
进入内核
↓
发现条件不满足
↓
自己进入睡眠状态
↓
schedule()
↓
切换Thread B
```

这里甚至：

> **不需要等下一次时钟中断。**

所以应该记：

> **时钟中断是触发调度的重要来源之一，但不是唯一来源。**

------

## 2. 如果确实是时钟中断触发，现场保存在哪里？

假设 CPU 当前正在运行：

```text
Thread A
```

而且 A 此刻在用户态：

```text
Thread A User Mode
```

突然：

```text
Timer IRQ
```

发生。

逻辑上会经历：

```text
Thread A用户态
      ↓
   Timer IRQ
      ↓
CPU进入异常/IRQ处理
      ↓
保存Thread A被打断时的用户现场
      ↓
进入Linux中断处理
```

<span style="color:#FF0000; background:#00FF80;">这里被打断的是：</span>

> **Thread A。**

<span style="color:#FF0000; background:#00FF80;">所以这份“以后恢复 Thread A 所需的现场”必须和 Thread A 联系起来。</span>

<span style="color:#FF0000; background:#00FF80;">通常会落到：</span>

```text
Thread A Kernel Stack
```

上的 `pt_regs` 一类现场中，具体实现依架构和内核版本而异。

所以并不是：

```text
所有线程
   ↓
共同使用一个Kernel Main Stack
```

------

## 3. 为什么不能所有线程共用一个“内核主栈”？

假设真有：

```text
唯一Kernel Stack
```

然后：

```text
Thread A进入内核
↓
正在read()
↓
阻塞
```

Kernel Stack 里已经有：

```text
A的pt_regs
read()
VFS
pipe_read()
schedule()
```

这时候切到 Thread B。

如果 B 再：

```text
write()
```

也往同一个 Kernel Stack 里压：

```text
B的pt_regs
write()
VFS
...
```

那 A 原来的现场很容易被覆盖或混在一起。

所以 Linux 的基本思想是：

```text
Thread A
→ Kernel Stack A

Thread B
→ Kernel Stack B

Thread C
→ Kernel Stack C
```

这样：

```text
A阻塞
↓
Kernel Stack A冻结

CPU去运行B
↓
使用Kernel Stack B
```

以后回到 A：

```text
重新使用Kernel Stack A
```

它原来的调用链还在那里。

------

## 4. 那中断不是“属于CPU”吗？为什么还跟线程栈有关？

这个问题很关键。

中断本身确实是：

> **CPU 接收到的硬件事件。**

例如：

```text
Timer
↓
IRQ
↓
CPU0
```

<span style="color:#FF0000; background:#00FF80;">但是中断发生的一瞬间，CPU0 一定正在执行某个东西。</span>

比如：

```text
CPU0
正在执行 Thread A
```

因此<span style="color:#FF0000; background:#00FF80;">首先需要保存的是：</span>

> **“Thread A 被中断时是什么状态”。**

所以这个现场自然和：

```text
current task
```

联系起来。

例如：

```text
CPU0
│
└── current = Thread A
        │
        └── Kernel Stack A
```

IRQ发生后：

```text
Thread A的被中断现场
↓
与A关联保存
```

------

## 5. 但是你说的“专门的中断栈”也不是完全错

这里就出现一个非常重要的细节。

<span style="color:#FF0000; background:#00FF80;">现代 Linux 在一些架构和配置下会使用：</span>

> **per-CPU IRQ Stack**

<span style="color:#FF0000; background:#00FF80;">也就是：</span>

```text
CPU0 → IRQ Stack 0
CPU1 → IRQ Stack 1
CPU2 → IRQ Stack 2
...
```

<span style="color:#FF0000; background:#00FF80;">注意它是：</span>

```text
每CPU一个
```

不是：

```text
整个Linux只有一个主栈
```

<span style="color:#FF0000; background:#00FF80;">例如：</span>

```text
CPU0
正在执行Thread A
↓
IRQ进入
↓
先保存必要的A现场
↓
可能切换到CPU0自己的IRQ Stack
↓
执行中断处理程序
```

<span style="color:#FF0000; background:#00FF80;">于是可能形成：</span>

```text
Thread A Kernel Stack
┌────────────────────┐
│ A的异常/返回现场    │
└────────────────────┘

CPU0 IRQ Stack
┌────────────────────┐
│ Timer IRQ handler  │
│ 中断处理调用现场    │
└────────────────────┘
```

这样做的一个好处就是：

> 避免很深的中断处理继续消耗线程有限的 Kernel Stack。

不过：

**是否使用独立 IRQ stack、具体在哪一步切换，取决于 CPU 架构、Linux 版本和配置。**

所以不要死记成所有 Linux 都完全一样。

------

## 6. 所以你脑中的“内核主栈”最好换成“per-CPU IRQ Stack”

你原来的模型：

```text
Linux Kernel
↓
一个主栈
↓
所有中断都用它
```

不太准确。

<span style="color:#FF0000; background:#00FF80;">更好的模型：</span>

```text
Linux Kernel

Thread A
├── User Stack A
└── Kernel Stack A

Thread B
├── User Stack B
└── Kernel Stack B


CPU0
└── 可能有 IRQ Stack 0

CPU1
└── 可能有 IRQ Stack 1
```

这里：

```text
Kernel Stack
→ 每个task/thread一份

IRQ Stack
→ 某些实现下每CPU一份
```

两者不是一回事。

------

## 7. 那“时钟中断触发调度”时到底在哪张栈上发生？

假设：

```text
CPU0
正在运行 Thread A 用户态
```

<span style="color:#FF0000; background:#00FF80;">时钟中断：</span>

```text
Timer IRQ
↓
CPU0进入Kernel
```

<span style="color:#FF0000; background:#00FF80;">大致可以理解成：</span>

```text
① 保存Thread A的被中断现场
   ↓
   与A的Kernel Stack关联

② 执行Timer IRQ handler
   ↓
   可能在A的Kernel Stack
   或CPU0的IRQ Stack
   取决于架构实现

③ 更新调度信息
   ↓
   发现A应该被抢占

④ 设置 need_resched

⑤ 中断处理结束

⑥ 在安全的调度点
   ↓
   schedule()

⑦ 保存A的线程调度上下文

⑧ 恢复B的调度上下文

⑨ SP切换：
   Kernel Stack A
          ↓
   Kernel Stack B
```

注意：

> <span style="background:#00FF80;">**中断处理和线程上下文切换虽然连着发生，但不是同一个“保存现场动作”。**</span>

------

## 8. 这里其实有两层现场

和我们之前系统调用阻塞非常类似。

### <span style="background:#00FF80;">第一层：IRQ / Exception 现场</span>

目的：

> <span style="background:#00FF80;">“以后怎么回到被中断的 Thread A？”</span>

例如：

```text
用户PC
用户SP
状态寄存器
通用寄存器
...
```

对应：

```text
pt_regs等
```

------

### 第二层：Scheduler Context

目的：

> <span style="color:#FF0000; background:#00FF80;">“Thread A 现在在 Linux 内核执行到哪里，以后怎么继续？”</span>

例如：

```text
Kernel SP
必要的callee-saved寄存器
内核执行恢复位置
```

对应调度上下文。

所以：

```text
时钟中断现场
≠
线程上下文切换现场
```

虽然一次抢占过程中可能两个都出现。

------

## 9. 举一个完整例子

假设：

```text
Thread A
正在用户态计算
```

CPU：

```text
PC = A用户程序地址
SP = A User Stack
```

突然：

```text
Timer IRQ
```

首先：

```text
CPU + Kernel IRQ entry
↓
保存A的用户态现场
```

于是：

```text
Kernel Stack A

┌─────────────────────┐
│ A的用户PC           │
│ A的用户SP           │
│ A的用户寄存器       │
│ 状态寄存器          │
└─────────────────────┘
```

然后：

```text
timer interrupt handler
```

发现：

```text
A时间片该结束
```

<span style="color:#FF0000; background:#00FF80;">Linux可能：</span>

```text
need_resched = 1
```

<span style="color:#FF0000; background:#00FF80;">然后中断返回路径发现：</span>

```text
需要调度
```

<span style="color:#FF0000; background:#00FF80;">于是进入：</span>

```text
schedule()
```

------

## 10. 真正的 context switch

<span style="color:#FF0000; background:#00FF80;">此时 Thread A 已经在：</span>

```text
Kernel Mode
```

了。

CPU当前：

```text
SP → Kernel Stack A
```

Scheduler 决定：

```text
next = Thread B
```

<span style="color:#FF0000; background:#00FF80;">于是：</span>

```text
保存A必要的kernel execution context
↓
恢复B之前保存的kernel execution context
```

最核心的直觉就是：

```text
CPU SP

原来：
↓
Kernel Stack A

切换后：
↓
Kernel Stack B
```

Thread A 的：

```text
Kernel Stack A
```

还在 RAM 中原封不动。

------

## 11. Thread B 接下来可能有两种情况

如果 B 以前也是在内核里被切走的：

```text
恢复Kernel Stack B
↓
从内核中的某个位置继续执行
```

然后以后：

```text
返回用户态
```

如果 B 是刚创建等情况，启动路径会有所不同。

但核心仍然：

> Scheduler 恢复的是 B 自己的执行上下文，而不是把所有线程塞到同一张内核栈里。

------

## 12. 这和你以前学 FreeRTOS 的“主栈”容易混

你之前学 Cortex-M：

```text
Task
→ PSP

Interrupt/Handler
→ MSP
```

所以很容易自然地映射成：

```text
Linux线程
→ Thread Kernel Stack

Linux中断
→ Kernel Main Stack
```

但这个映射不能直接成立。

ARM Cortex-M：

```text
MSP / PSP
```

是 Cortex-M 特定的硬件机制。

而你现在：

```text
i.MX6ULL
↓
Cortex-A7
↓
ARMv7-A
```

异常模式和 Linux 栈管理机制完全不同。

Linux更应该理解：

```text
每个task：
Kernel Stack

每个CPU：
可能还有IRQ Stack

而不是：
全系统唯一Kernel Main Stack
```

------

## 13. 那 Cortex-A7 本身不是还有 IRQ mode / SVC mode 的 banked SP 吗？

有，这里再往底层一点。

ARMv7-A 有：

```text
User mode
SVC mode
IRQ mode
...
```

某些异常模式拥有 banked registers，例如：

```text
SP_irq
LR_irq

SP_svc
LR_svc
```

但这里千万不要直接得出：

```text
SP_irq
=
Linux所有IRQ共用的“内核主栈”
```

因为：

> **CPU 硬件提供的是异常模式寄存器机制；Linux 软件会在此基础上建立自己的 task kernel stack、per-CPU stack、异常入口和上下文管理。**

这两个层次要分开：

```text
ARM硬件：
异常模式 + banked registers

Linux软件：
task kernel stack
IRQ stack
pt_regs
scheduler context
```

------

## 14. 最重要的一张总图

你可以把一次“时钟中断 → 抢占 Thread A → 运行 Thread B”理解成：

```text
                   CPU0

Thread A User Mode
        │
        │ Timer IRQ
        ▼
════════════════════════
       Kernel
        │
        ▼
保存A被中断的用户现场
        │
        ▼
Kernel Stack A
┌────────────────────┐
│ pt_regs            │
└────────────────────┘
        │
        ▼
Timer IRQ Handler
        │
        │
        └─ 可能使用CPU0 IRQ Stack
        │
        ▼
need_resched = 1
        │
        ▼
中断退出/调度检查
        │
        ▼
schedule()
        │
        ▼
保存A的kernel context
        │
        ▼
Context Switch
        │
        ▼
恢复B的kernel context
        │
        ▼
CPU SP → Kernel Stack B
        │
        ▼
Thread B继续执行
```

所以最终可以把你的原话改成：

> <span style="color:#FF0000; background:#00FF80;">**进程/线程调度可以由时钟中断触发，但也可以由阻塞、唤醒、主动让出 CPU 等原因触发。发生时钟中断时，被打断线程的异常现场通常会与该线程自己的 Kernel Stack 关联保存；某些 Linux 架构还会切换到每 CPU 独立的 IRQ Stack 执行中断处理。真正发生线程上下文切换时，调度器则保存当前线程的内核执行上下文，并切换到下一个线程自己的 Kernel Stack。Linux 并不存在一个所有线程共同使用的全局“内核主栈”。**</span>

其中最值得你改掉的就是：

```text
“内核主栈”
```

这个词。

以后看到 Linux，最好想：

```text
每线程：
User Stack
Kernel Stack

每CPU：
可能有IRQ Stack

整个内核：
没有一张所有任务共同使用的主栈
```

这样你后面学 Linux 中断、调度、驱动时就不会和 Cortex-M 的 MSP 混到一起。



# 问题8（调度时发生的保存线程A的一个是用户态的现场，一个是内核态的现场，二者不是同一个现场）

                   CPU0

```
Thread A User Mode
        │
        │ Timer IRQ
        ▼
════════════════════════
       Kernel
        │
        ▼
保存A被中断的用户现场
        │
        ▼
Kernel Stack A
┌────────────────────┐
│ pt_regs            │
└────────────────────┘
        │
        ▼
Timer IRQ Handler
        │
        │
        └─ 可能使用CPU0 IRQ Stack
        │
        ▼
need_resched = 1
        │
        ▼
中断退出/调度检查
        │
        ▼
schedule()
        │
        ▼
保存A的kernel context
        │
        ▼
Context Switch
        │
        ▼
恢复B的kernel context
        │
        ▼
CPU SP → Kernel Stack B
        │
        ▼
Thread B继续执行
```

这里中断退出前应该要恢复线程A的现场吧，不然下面发生调度的时候就又保存了一次A的现场，如果不恢复的话不就保存了两遍A的现场了吗。



对，你这个问题抓得很准。**我上一张图把“中断处理函数结束”和“真正从异常返回、恢复 A 的用户态现场”画得太像同一件事了。**

实际上 Linux 这里要分成两个“退出”：

> **IRQ Handler 执行结束 ≠ 已经恢复 Thread A 并返回 User Mode。**

<span style="color:#FF0000; background:#00FF80;">如果发现 `need_resched`，Linux 可以在**真正恢复 A 的用户态现场之前**先调度出去。因此不会出现“先完整恢复 A，再马上又完整保存 A”这种重复操作。</span>

更准确的过程是下面这样。

------

## 1. 时钟中断发生时，先保存的是 A 的“被中断现场”

假设：

```text
CPU0 正在执行 Thread A 用户态
```

此时：

```text
PC → A用户代码
SP → A User Stack
```

突然 Timer IRQ：

```text
Thread A User Mode
        │
        │ Timer IRQ
        ▼
CPU/Kernel异常入口
        │
        ▼
保存A的用户态现场
        │
        ▼
Kernel Stack A

┌──────────────────────┐
│ pt_regs              │
│                      │
│ A的用户寄存器        │
│ A的User SP           │
│ A的User PC           │
│ 状态寄存器           │
└──────────────────────┘
```

这份现场的目的只有一个：

> **以后真正要返回 Thread A 用户态时使用。**

注意现在 A 的现场已经安全地待在：

```text
Kernel Stack A
```

里了。

------

## 2. 然后执行 Timer IRQ Handler

<span style="color:#FF0000; background:#00FF80;">CPU现在已经不是在执行 A 的用户代码了，而是在：</span>

```text
Kernel .text
```

<span style="color:#FF0000; background:#00FF80;">执行时钟中断处理程序。</span>

<span style="color:#FF0000; background:#00FF80;">可能：</span>

```text
A Kernel Stack
```

<span style="color:#FF0000; background:#00FF80;">或者某些架构使用：</span>

```text
CPU0 IRQ Stack
```

<span style="color:#FF0000; background:#00FF80;">执行 IRQ handler。</span>

比如：

```text
timer handler
↓
更新时间
↓
调度器记账
↓
发现A应该让出CPU
↓
need_resched = 1
```

然后中断处理函数本身结束。

------

## 3. 关键：IRQ Handler 结束以后，并没有马上恢复 A 的用户现场

这里就是你问的核心。

<span style="color:#FF0000; background:#00FF80;">如果没有调度需求：</span>

```text
IRQ Handler结束
↓
取出A的pt_regs
↓
恢复A用户寄存器
↓
异常返回
↓
A继续运行
```

这个你想的是对的。

<span style="color:#FF0000; background:#00FF80;">但如果：</span>

```text
need_resched = 1
```

<span style="color:#FF0000; background:#00FF80;">那么可以变成：</span>

```text
IRQ Handler结束
↓
准备从中断返回
↓
发现 need_resched
↓
“先别回A用户态”
↓
进入调度器
```

也就是说：

> **A 的 `pt_regs` 继续留在 Kernel Stack A 上，一动不动。**

没有恢复。

------

## 4. 所以这时候调度器保存的不是同一份“用户现场”

这是你担心“保存两遍”的地方。

<span style="color:#FF0000; background:#00FF80;">第一次已经保存的是：</span>

```text
A User Context
```

比如：

```text
用户r0-r12
用户SP
用户PC
用户状态
```

它存在：

```text
Kernel Stack A → pt_regs
```

<span style="color:#FF0000; background:#00FF80;">而现在进入 `schedule()` 时，CPU正在：</span>

```text
Kernel Mode
```

运行内核代码。

CPU当前寄存器已经是类似：

```text
SP → Kernel Stack A
PC → 内核的调度/中断退出代码
r4/r5... → 内核正在使用的数据
```

<span style="color:#FF0000; background:#00FF80;">因此调度器需要保存的是：</span>

> **A 当前的 Kernel Execution Context**

<span style="color:#FF0000; background:#00FF80;">而不是再保存一次 A 用户态 `pt_regs`。</span>

<span style="color:#FF0000; background:#00FF80;">两者是不同东西：</span>

```text
第一份：
A User Context
→ 将来回A用户态用


第二份：
A Kernel Context
→ 将来让A重新进入当前这条内核返回路径用
```

<span style="color:#FF0000; background:#00FF80;">所以不是重复。</span>

------

## 5. 可以把 A 此刻想成有“两层书签”

此时 Thread A：

```text
Kernel Stack A

┌──────────────────────────┐
│ A 用户态 pt_regs         │ ← 第一层书签
│                          │
│ “以后回用户程序的位置”   │
├──────────────────────────┤
│ 中断返回/内核调用现场     │
│                          │
│ “内核现在执行到哪里”     │
└──────────────────────────┘
```

<span style="color:#FF0000; background:#00FF80;">然后调度器再保存必要的：</span>

```text
Kernel SP
必要的callee-saved registers
内核恢复位置
```

<span style="color:#FF0000; background:#00FF80;">相当于第二层书签。</span>

所以：

```text
书签1：
以后回用户态哪里继续

书签2：
以后回内核哪里继续
```

完全不同。

------

## 6. 然后切换到 B

此时：

```text
Thread A Kernel Stack
```

<span style="color:#FF0000; background:#00FF80;">就被冻结在 RAM 里：</span>

```text
┌──────────────────────┐
│ A的用户pt_regs       │
├──────────────────────┤
│ A的内核返回现场      │
└──────────────────────┘
```

<span style="color:#FF0000; background:#00FF80;">CPU切到：</span>

```text
Thread B
```

<span style="color:#FF0000; background:#00FF80;">本质上：</span>

```text
CPU SP

原来：
    ↓
Kernel Stack A

切换后：
    ↓
Kernel Stack B
```

然后恢复 B 的 kernel context。

------

## 7. A 什么时候恢复？

<span style="color:#FF0000; background:#00FF80;">以后调度器再次选中 A：</span>

```text
Scheduler
↓
restore A kernel context
↓
SP → Kernel Stack A
↓
恢复A的内核执行位置
```

<span style="color:#FF0000; background:#00FF80;">A不是直接突然回用户程序。</span>

<span style="color:#FF0000; background:#00FF80;">而是先回到之前：</span>

```text
中断退出 / 调度返回路径
```

附近。

<span style="color:#FF0000; background:#00FF80;">概念上：</span>

```text
schedule()
↓
过了很久
↓
A再次被选中
↓
schedule()“返回”
↓
继续执行中断返回路径
```

<span style="color:#FF0000; background:#00FF80;">然后才：</span>

```text
从Kernel Stack A取pt_regs
↓
恢复A用户寄存器
↓
恢复User PC / User SP
↓
异常返回
↓
Thread A User Mode
```

这时才真正把第一份现场消费掉。

------

## 8. 所以<span style="color:#FF0000; background:#00FF80;">完整顺序应该改成这样</span>

之前那张图最好改成：

```text
                   CPU0

Thread A User Mode
        │
        │ Timer IRQ
        ▼
════════════════════════════
        Kernel
        │
        ▼
保存A的用户态异常现场
        │
        ▼
Kernel Stack A
┌────────────────────┐
│ pt_regs            │
└────────────────────┘
        │
        ▼
Timer IRQ Handler
        │
        ▼
need_resched = 1
        │
        ▼
IRQ Handler结束
        │
        │ 注意：
        │ 还没有恢复A用户现场！
        ▼
中断返回路径检查 need_resched
        │
        ▼
进入 schedule()
        │
        ▼
保存A的 Kernel Context
        │
        │ A的pt_regs仍在Kernel Stack A
        ▼
Context Switch
        │
        ▼
恢复B的 Kernel Context
        │
        ▼
CPU SP → Kernel Stack B
        │
        ▼
Thread B运行
```

过一段时间：

```text
Scheduler重新选A
        │
        ▼
恢复A的Kernel Context
        │
        ▼
CPU SP → Kernel Stack A
        │
        ▼
继续中断返回路径
        │
        ▼
现在才从pt_regs恢复A用户现场
        │
        ▼
异常返回
        │
        ▼
Thread A User Mode继续
```

这才是准确的核心逻辑。

------

## 9. “中断退出”这个词为什么容易误导？

因为它其实可以指两件不同的事。

### 第一层：中断处理函数结束

比如：

```text
timer handler
```

执行完了。

这时候可以说：

> IRQ handler 完成了。

但 CPU 仍然可能处于：

```text
Kernel Mode
```

并在执行异常返回路径。

------

### 第二层：真正 Exception Return

这才是：

```text
恢复被中断现场
↓
CPU退出内核/异常环境
↓
重新执行被打断代码
```

你刚才说的：

> “中断退出前应该恢复 A。”

如果这里的“退出”是指第二种——**真正异常返回**——那你完全正确。

但是：

> Linux可以在执行真正异常返回之前发现需要调度，于是先把 A 切出去。

所以：

```text
IRQ handler结束
```

与：

```text
exception return完成
```

中间还可以插：

```text
scheduler
```

------

## 10. 为什么不能先恢复 A 再 schedule？

理论上可以设计成：

```text
恢复A
↓
回用户态
↓
马上再次进入内核
↓
schedule
```

但这显然多绕了一圈。

已经知道：

```text
A现在不该继续运行
```

为什么还：

```text
恢复全部用户寄存器
→ 切到用户态
→ 再进入内核
→ 又保存
```

浪费。

所以更合理：

```text
A用户现场先保持冻结
↓
直接在内核返回路径调度
↓
运行B
```

以后真轮到 A 时：

```text
再恢复
```

------

## 11. 这也正好解释<span style="color:#FF0000; background:#00FF80;">为什么 Kernel Stack 必须跟线程绑定</span>

因为 A 被切走以后：

```text
Kernel Stack A
```

里面还躺着：

```text
pt_regs
中断返回相关现场
```

这些不能消失。

B有自己的：

```text
Kernel Stack B
```

于是：

```text
A stack冻结

CPU
↓
B stack运行
```

以后：

```text
CPU
↓
A stack恢复
```

非常自然。

------

## 12. 如果使用 per-CPU IRQ Stack 呢？

这个也需要补一下。

可能是：

```text
A User Mode
↓
IRQ
↓
先建立与A有关的异常现场
↓
切到 CPU0 IRQ Stack
↓
执行Timer IRQ Handler
```

handler结束以后：

```text
CPU0 IRQ Stack
↓
退出hardirq处理
↓
回到Thread A自己的Kernel Stack返回路径
```

然后发现：

```text
need_resched
```

再进行线程调度。

也就是说：

> <span style="color:#FF0000; background:#00FF80;">**IRQ Stack 主要承担中断处理函数自己的调用栈；Thread A 的可恢复用户现场仍然要与 A 自己的 task/kernel stack 联系在一起。**</span>

这也说明：

```text
per-CPU IRQ stack
≠
线程上下文保存仓库
```

------

## 最后把你这个问题压缩成一句最关键的话

你说：

> “如果不恢复A，不就保存了两遍A现场了吗？”

答案是：

> <span style="color:#FF0000; background:#00FF80;">**不会，因为两次保存的根本不是同一种现场。**</span>

第一次 Timer IRQ：

```text
保存：
A的User Context
↓
pt_regs
↓
为了最终回A用户态
```

第二次 schedule：

```text
保存：
A当前的Kernel Context
↓
为了以后继续内核的中断返回路径
```

所以关系是：

```text
A User Context
        │
        │ 被“包”在Kernel Stack里
        ▼
A Kernel Context
        │
        │ Scheduler切走
        ▼
Thread B
```

等 A 重新运行：

```text
先恢复 A Kernel Context
↓
回到中断返回路径
↓
再恢复 A User Context
↓
真正回用户态
```

你这个追问其实非常关键，因为它把 **“异常现场”** 和 **“线程调度上下文”** 两种经常被统称为“保存现场”的东西真正区分开了。







# 问题9 （这里先不看，也就是RTOS那里的调用者保存寄存器和被调用者保存寄存器那一套）

**普通函数切换保存“函数调用所需现场”，通常不是完整 CPU 上下文。**  这里如果只是保存函数调用现场的话我怎么知道哪些寄存器该保存哪些寄存器不该保存呢，这里我有点搞不太懂。



对，这里真正决定“哪些寄存器该保存、哪些不用保存”的，不是程序员临时猜，而是 **ABI / Calling Convention（函数调用约定）提前规定好的**。

你可以把它理解成：编译器和所有函数之间事先签了一份“寄存器使用协议”。

最核心就是把寄存器分成两大类：

```text
Caller-saved
调用者保存寄存器

Callee-saved
被调用者保存寄存器
```

这两个名字一旦理解，整个问题就通了。

------

## 先看一个最简单的场景

```c
int foo(void)
{
    int x = 10;

    bar();

    return x + 1;
}
```

执行关系：

```text
foo()
  │
  │ call
  ▼
bar()
  │
  │ return
  ▼
foo()继续
```

问题来了：

假设 `foo()` 在调用 `bar()` 之前，有些重要数据暂时放在 CPU 寄存器里。

例如：

```text
某个寄存器 = x = 10
```

但是 `bar()` 运行的时候也需要寄存器。

那么到底：

> bar 能不能直接把这个寄存器改掉？

这就必须事先约定，否则大家会互相踩数据。

------

## 1. Caller-saved 是什么意思？

意思是：

> **这个寄存器，callee 可以随便修改。Caller 如果调用完以后还想保留原值，就必须在调用前自己保存。**

比如假设：

```text
r0
r1
r2
r3
```

属于 caller-saved。

那么：

```text
foo
↓
r0 = 100
↓
准备调用 bar()
```

如果 `foo()` 调用完 `bar()` 以后还需要这个：

```text
r0 = 100
```

那 `foo()` 就不能相信：

```text
bar执行完以后r0还是100
```

因为 `bar()` 有权把 r0 改成：

```text
3
500
0
任意值
```

所以 caller 要么：

```text
调用bar之前
↓
把r0的旧值压栈
```

要么更聪明一点：

```text
把重要值转移到callee-saved寄存器
```

或者编译器干脆重新计算。

------

## 2. Callee-saved 又是什么意思？

正好反过来：

> **Caller 可以假设这些寄存器调用前后不变。如果 callee 想使用它，就必须先保存旧值，用完之后恢复。**

比如假设：

```text
r4
```

是 callee-saved。

调用前：

```text
foo:

r4 = 100
```

然后：

```text
foo
↓
bar()
```

如果 `bar()` 根本不用 r4：

```text
什么都不用做
```

如果 `bar()` 想用 r4：

```text
bar进入：

先保存：
old r4 = 100

然后：
r4 = 自己的数据

...

bar返回前：

恢复：
r4 = 100
```

于是回到 `foo()`：

```text
r4仍然 = 100
```

所以：

> callee-saved 并不是“不能修改”。

而是：

> **可以修改，但返回前必须恢复成调用之前的样子。**

------

## 3. 所以并不是“所有寄存器都保存”

这就是你刚才问题的答案。

编译器会根据：

```text
这个函数用了哪些寄存器？
哪些值需要跨函数调用继续存在？
ABI规定这些寄存器属于哪一类？
```

决定到底保存什么。

假设：

```text
r0 r1 r2 r3
→ caller-saved

r4 r5 r6 r7
→ callee-saved
```

`bar()`只使用：

```text
r0
r1
```

那么它完全没必要：

```text
保存r4
保存r5
保存r6
保存r7
```

因为它根本没碰。

同样，即使 `bar()` 用了：

```text
r4
```

但没用：

```text
r5
r6
```

那么它通常只需要保存：

```text
r4
```

而不是全保存。

------

## 4. 这就是“保存最小必要现场”

假设 CPU 有：

```text
r0 ~ r15
```

普通函数调用绝对不是：

```text
每调用一次函数
↓
r0-r15全部压栈
↓
执行函数
↓
r0-r15全部恢复
```

这样太浪费了。

而是：

```text
根据ABI
+
根据当前函数真实需求
↓
只保存必要寄存器
```

这就是为什么普通函数调用比完整线程上下文切换轻很多。

------

# 5. 放到你现在的 i.MX6ULL / ARM32 上看

你现在学习的是：

```text
i.MX6ULL
↓
Cortex-A7
↓
ARMv7-A
↓
AAPCS / ARM EABI
```

可以先用这个简化表理解：

| ARM寄存器 | 常见作用               | 保存约定                   |
| --------- | ---------------------- | -------------------------- |
| `r0-r3`   | 参数、返回值、临时计算 | caller-saved               |
| `r4-r8`   | 通用寄存器             | callee-saved               |
| `r9`      | 平台相关               | 具体ABI决定                |
| `r10-r11` | 通用/帧相关            | 通常 callee-saved          |
| `r12`     | 临时 scratch/IP        | caller-saved               |
| `r13`     | SP 栈指针              | 必须正确维护               |
| `r14`     | LR 返回地址            | 调用其他函数时经常需要处理 |
| `r15`     | PC                     | 当前执行位置               |

你现在不用死背全部，先牢牢记：

```text
r0-r3
→ 参数/返回值
→ 调用别的函数后不能指望它们保持原值

r4-r11一部分
→ callee-saved
→ callee用了就必须恢复
```

------

# 6. 举个真正有感觉的例子

假设：

```c
int add5(int x)
{
    return x + 5;
}
```

调用：

```c
result = add5(10);
```

根据 ARM 调用约定：

```text
调用前：

r0 = 10
```

进入 `add5()`：

```text
r0 = r0 + 5
```

得到：

```text
r0 = 15
```

返回以后：

```text
r0
```

同时又是返回值。

所以：

```text
参数从r0进去
返回值从r0出来
```

根本不需要保存旧 r0。

因为调用者本来就知道：

> r0 是 caller-saved，调用函数以后值可能被覆盖。

------

# 7. 再来一个“为什么需要 callee-saved”的例子

```c
int foo(int a)
{
    int x = a + 10;

    bar();

    return x + 1;
}
```

注意：

```text
x
```

在：

```text
调用bar之前
```

算出来，

但是：

```text
bar返回以后
```

还需要继续使用。

所以 `x` 是一个：

> **live across call**
>
> “跨函数调用仍然存活的值”。

编译器有几个选择。

一种：

```text
把x放到User Stack
```

调用以后再拿出来。

另一种更常见的优化思路：

```text
把x放到callee-saved寄存器
例如r4
```

于是：

```text
foo:

r4 = x

bar()

回来以后：

r4仍然 = x
```

为什么能相信？

因为：

```text
AAPCS规定r4是callee-saved
```

如果 `bar()` 修改 r4：

```text
bar自己负责保存和恢复
```

------

# 8. 于是可能出现这种汇编

概念上：

```asm
foo:
    push {r4, lr}

    add  r4, r0, #10

    bl   bar

    add  r0, r4, #1

    pop  {r4, lr}
    bx   lr
```

这段特别值得你看懂。

进入 `foo()`：

```asm
push {r4, lr}
```

为什么保存：

```text
r4
lr
```

而不是：

```text
r0-r15全部保存？
```

因为 `foo()`：

```text
要使用r4
```

而 r4 是 callee-saved，所以 `foo()` 必须保护调用它的上一层函数原来的 r4。

同时：

```text
foo里面还要调用bar
```

执行：

```asm
bl bar
```

会改写：

```text
LR
```

所以 `foo()` 也需要保存自己原来的返回地址。

------

# 9. 这里非常有意思：一个函数同时是 Caller 和 Callee

比如：

```text
main
 ↓
foo
 ↓
bar
```

站在：

```text
main → foo
```

这个关系上：

```text
main = caller
foo  = callee
```

但是站在：

```text
foo → bar
```

这个关系上：

```text
foo = caller
bar = callee
```

所以 `foo()` 同时承担两种责任：

```text
作为main的callee：
必须保护callee-saved寄存器


作为bar的caller：
如果caller-saved寄存器里的值
在bar之后还需要，
foo自己必须保护
```

这就是为什么叫：

```text
Calling Convention
```

而不是简单说：

```text
某些寄存器永远由谁保存
```

它是相对于一次：

```text
Caller → Callee
```

关系定义的。

------

# 10. `LR` 为什么特别？

ARM里面：

```text
r14 = LR
```

也就是 Link Register。

执行：

```asm
bl bar
```

CPU会把：

```text
foo调用bar之后应该返回的位置
```

放到 LR。

于是 bar：

```asm
bx lr
```

就能回到 foo。

但是假设：

```text
main
↓
foo
↓
bar
```

main 调用 foo 时：

```text
LR = foo以后回main的位置
```

然后 foo 又执行：

```asm
bl bar
```

LR会被重新写成：

```text
bar以后回foo的位置
```

这样：

```text
foo原来回main的地址
```

不就丢了吗？

所以 foo 往往在调用 bar 前，把 LR 保存到栈里：

```asm
push {lr}
```

之后：

```asm
pop {lr}
```

或者直接：

```asm
pop {pc}
```

返回。

这就是普通函数“现场保存”的一个非常具体例子。

------

# 11. 那局部变量是不是都压到栈？

也不是。

例如：

```c
int foo(void)
{
    int a = 10;
    int b = 20;

    return a + b;
}
```

优化后甚至可能直接变成：

```text
返回30
```

根本没有：

```text
a
b
```

真正的内存对象。

或者：

```text
a存在r4
b存在r5
```

根本不上栈。

只有：

```text
寄存器不够
需要取地址
跨调用需要保存
数组/大对象
编译器判断合适
```

等情况下，局部变量才更可能出现在栈上。

所以：

> **C源码里的“变量”不等于一定有一个栈内存位置。**

这个非常重要。

------

# 12. 谁决定到底用哪个寄存器？

绝大多数时候：

> **编译器。**

你写：

```c
int foo(int a)
{
    int x = a + 10;
    bar();
    return x;
}
```

你没有写：

```text
把x放r4
把a放r0
```

而是：

```text
ABI规定：
第一个参数通常放r0

编译器分析：
x需要跨bar保存

↓
编译器可能选择r4
```

然后编译器生成相应：

```asm
push
mov
bl
pop
```

机器指令。

所以 C 程序员通常不用手工管理。

------

# 13. 那编译器怎么知道哪些寄存器“bar会破坏”？

它甚至不需要知道 `bar()` 内部代码。

只要知道 ABI：

```text
r0-r3
= caller-saved
```

那么编译器就必须保守地认为：

```text
调用bar以后
r0-r3可能全部被改掉
```

即使实际 `bar()` 根本没碰 r2。

而：

```text
r4-r11
= callee-saved
```

编译器就可以相信：

```text
bar返回以后
这些寄存器仍保持原值
```

因为如果 bar 要修改，它自己有责任恢复。

这就是 ABI 的巨大作用：

> **不同 C 文件、不同库、甚至不同编译器生成的代码，都可以互相调用，而不需要知道对方内部实现。**

------

# 14. 所以 Calling Convention 本质就是“大家签协议”

你可以这么理解：

```text
Caller：

“r0-r3我不要求你帮我保护。
如果里面有我还要的数据，
我自己先存。”

Callee：

“r4-r11如果我要改，
我保证先保存，
返回前恢复。”
```

于是：

```text
foo()
```

和：

```text
bar()
```

即使是两个完全不同的人写的，

甚至：

```text
foo在你的程序里

bar在glibc.so里
```

也能正常工作。

因为双方都遵守同一个 ABI。

------

# 15. 然后就能和 `read()` 接起来了

例如：

```c
ret = read(fd, buf, 100);
```

用户态普通函数层面：

```text
你的函数
↓
libc read wrapper
```

按照 AAPCS：

```text
r0 = fd
r1 = buf
r2 = 100
```

这些本来就是：

```text
caller-saved
```

所以非常适合拿来传参数。

然后 libc 最终执行：

```asm
svc
```

这才进入我们上一问说的：

```text
另一套异常现场保存机制
```

也就是说：

```text
普通call：
遵循Calling Convention
只保存必要寄存器


SVC：
异常/系统调用入口
CPU + Kernel保存更完整的用户执行状态
```

这两层不要混在一起。

------

# 16. 为什么系统调用要保存得更多，而函数调用不用？

因为普通函数调用是：

```text
双方都遵守ABI
```

caller 本来就知道：

```text
r0-r3可能被破坏
```

所以不需要把整个 CPU 都保护起来。

但是 SVC / 中断：

```text
用户程序正在任意位置运行
↓
突然进入内核
```

内核以后必须尽量让用户程序感觉：

> “系统调用回来后，我还能按照 ABI 规定继续正常执行。”

所以异常入口需要保存足够完整的用户 CPU 状态。

------

# 17. 还有一个很关键的优化：Leaf Function

如果一个函数内部不调用其他函数：

```c
int add(int a, int b)
{
    return a + b;
}
```

叫：

> **leaf function，叶子函数。**

因为它不再：

```text
call别人
```

这种函数可能连 LR 都不用压栈。

例如概念：

```asm
add:
    add r0, r0, r1
    bx  lr
```

就结束了。

完全：

```text
没有push
没有pop
```

这进一步说明：

> “调用函数就必须压栈保存一大堆寄存器”是不对的。

真正情况取决于函数做了什么。

------

# 18. 一个非常好用的判断原则

以后看到一个普通函数时，你可以这样问：

```text
这个函数有没有调用其他函数？

有没有使用callee-saved寄存器？

哪些值需要跨函数调用继续存在？

有没有需要保留返回地址LR？

局部数据是否必须放栈？
```

编译器根据这些信息，再结合 ABI，决定：

```text
push哪些
pop哪些
```

而不是固定模板。

------

## 最后把这件事压缩成一个模型

```text
ARM Calling Convention

Caller-saved
例如 r0-r3, r12
│
├─ callee可以随便改
│
└─ caller如果还要旧值
   → caller自己先保存


Callee-saved
例如 r4-r8,r10,r11 等
│
├─ caller可以假设调用前后不变
│
└─ callee如果要修改
   → callee先保存
   → 返回前恢复
```

所以你问：

> “我怎么知道哪些寄存器该保存？”

答案就是：

> **先由 ABI 规定哪些是 caller-saved、哪些是 callee-saved；再由编译器根据当前函数真正用了哪些寄存器、哪些值需要跨调用保留，生成最少必要的保存/恢复代码。**

因此普通函数调用保存的不是“CPU完整现场”，而是：

> **为了满足 ABI，并保证函数返回后程序逻辑正确所必需的最小现场。**

这也是为什么你以后反汇编经常会看到：

```asm
push {r4, r5, lr}
```

而不是：

```asm
push {r0-r15}
```

因为前者就是编译器算出来的“这次真正需要保护的东西”。

