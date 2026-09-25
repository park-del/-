# 视频分析

这段视频约 **9 分 22 秒**，核心是在建立后面 Linux 文件操作最重要的一张“地图”：**Linux 下操作文件，常见有两套 I/O 接口——标准 I/O 和系统调用 I/O；标准 I/O 最终仍然会落到系统调用。**

> 说明：下面时间点依据视频画面、课件和板书进行定位；对没有完全确认的口述内容不做逐字引用。

## 1. 视频类型判断

**主要类型：讲解类 / 科普类**
**次要类型：嵌入式 Linux 基础教学**

判断依据：

1. 先<span style="color:#FF0000;">把文件 I/O 函数分为两大类</span>。
2. <span style="color:#FF0000; background:#00FF80;">用层次图解释应用程序、C 库、系统调用、Linux 内核、文件系统和驱动之间的关系</span>。
3. 用 `read()` 连续读取与 `fread()` 缓冲读取的例子解释两类 I/O 的区别。

------

## 2. 一句话总结

这个视频主要讲的是：

> <span style="background:#00FF80;">**`fopen/fread/fwrite...` 属于标准 I/O，`open/read/write...` 属于系统调用 I/O；标准 I/O 在用户空间增加了一层缓冲，底层最终还是通过 `open/read/write` 等进入 Linux 内核。**</span>

这句话建议你先记住，后面的细节基本都围绕它展开。

------

## 3. 时间段拆解

| 时间段      | 内容概括                     | 画面/声音/人物表现                                           | 作用                                                         |
| ----------- | ---------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 00:00-00:55 | 文件 I/O 章节引入            | 展示《嵌入式 Linux 应用开发完全手册》以及 Linux/UNIX 相关参考书 | 进入文件 I/O 主题                                            |
| 00:55-01:35 | 提出两套文件 I/O 函数        | 课件列出标准 I/O 与系统调用 I/O，并展示完整层次图            | 建立本节核心分类                                             |
| 01:35-03:55 | 解释 POSIX、Linux、UNIX 关系 | 切换白板，画出 POSIX → Linux / Unix 的关系，并搜索 POSIX 定义 | 解释为什么不同 Unix-like 系统有类似 `open/read/write` 接口   |
| 03:55-05:45 | 系统调用 I/O 示例            | 板书 `open()`，循环调用 `read(fd, buf, 1)`，大约调用 100 次  | 引出<span style="color:#FF0000;">频繁系统调用的问题</span>   |
| 05:45-07:05 | 标准 I/O 缓冲机制            | 板书“第一次 fread 读较大块数据到 APP buffer，以后从 buffer 读取” | 解释<span style="color:#FF0000;">标准 I/O 为什么需要用户空间缓冲</span> |
| 07:05-08:35 | 两类 I/O 对照                | 回到总图，<span style="background:#00FF80;">并标记“标准 IO”“系统调用 IO”“用户 buffer”等</span> | 将前面的零散知识重新放回整体框架                             |
| 08:35-09:22 | 从用户空间进入内核           | 图中展示<span style="color:#FF0000; background:#00FF80;"> `svc`、`sys_open/sys_read`、文件系统和字符设备驱动</span> | 总结整个 I/O 调用链                                          |

------

## 4. 这张图才是整个视频真正的重点

你可以把视频中的图简化成这样：

```text
                    APP
                     │
        ┌────────────┴────────────┐
        │                         │
    标准 I/O                  系统调用 I/O
        │                         │
fopen/fread/fwrite          open/read/write
fseek/fflush/fclose         lseek/fsync/close
        │                         │
        │
   C库用户空间 Buffer
        │
        └────→ open/read/write...
                        │
                   系统调用
                        │
════════════════ 用户空间 / 内核空间 ════════════════
                        │
                  Linux 内核
                        │
                sys_open/sys_read...
                        │
              ┌─────────┴──────────┐
              │                    │
           文件系统             设备驱动
       ext4 / FAT ...        字符设备驱动
              │
           块设备驱动
              │
            硬件
```

理解了这张图，这节课基本就懂了。

------

## 5. 两类 I/O 到底有什么区别？

### 第一类：系统调用 I/O

例如：

```c
int fd;

fd = open("test.txt", ...);

read(fd, buf, 100);
write(fd, buf, 100);

close(fd);
```

典型函数：

```text
open
read
write
lseek
fsync
close
```

这里最明显的特点是：

```text
open()
  ↓
得到 fd
  ↓
read(fd, ...)
write(fd, ...)
```

这里的 `fd` 就是：

> **file descriptor，文件描述符。**

本质上是一个整数，例如：

```c
fd = 3;
```

应用程序以后通过这个整数告诉内核：

> “我要操作刚才打开的那个文件。”

------

## 6. 第二类：标准 I/O

标准 I/O 是 C 库提供的：

```c
FILE *fp;

fp = fopen("test.txt", "r");

fread(..., fp);
fwrite(..., fp);

fclose(fp);
```

常见函数：

```text
fopen
fread
fwrite
fseek
fflush
fclose
```

注意这里不再使用：

```text
int fd
```

而是：

```c
FILE *fp;
```

所以<span style="color:#FF0000;">你以后看到代码，甚至可以很快判断是哪套 I/O：</span>

```text
open() + fd
        ↓
系统调用 I/O


fopen() + FILE *
        ↓
标准 I/O
```

这是特别好用的识别方法。

------

## 7. 视频为什么一直强调 Buffer？

这是本视频最值得理解的部分。

假设<span style="color:#FF0000; background:#00FF80;">程序要连续读取 **100 个字节**。</span>

你可以这么写：

```c
for (i = 0; i < 100; i++)
{
    read(fd, buf, 1);
}
```

意思是：

```text
第1次 read → 读1字节
第2次 read → 读1字节
第3次 read → 读1字节
...
第100次 read → 读1字节
```

这样可能发生：

```text
用户程序
  ↓
进入内核
  ↓
回来

用户程序
  ↓
进入内核
  ↓
回来

……
```

反复发生很多次。

而**进入内核存在额外开销**。

------

### 标准 I/O 的思路就不一样了

假设你：

```c
fread(...);
```

表面上可能只要求一点数据。

<span style="color:#FF0000; background:#00FF80;">C 库可以在第一次真正需要数据的时候，底层一次：</span>

```text
read(fd, buffer, 较大的一块)
```

例如概念上：

```text
文件
 │
 │ 一次读取较大块数据
 ↓
┌─────────────────────┐
│ C库的用户空间 Buffer │
│ abcdefghijklmnop... │
└─────────────────────┘
       ↑
       │
 fread()
```

之后：

```text
第1次 fread ─┐
第2次 fread ─┤
第3次 fread ─┤──→ 直接从用户空间 Buffer 拿
......       │
第N次 fread ─┘
```

<span style="color:#FF0000;">不必每一次都进入内核。</span>

这就是视频大约 **05:45-07:05** 那段板书想表达的核心。

------

## 8. 所以标准 I/O 并不是另一套完全独立的文件系统

这一点特别容易误会。

你可能会觉得：

```text
fopen 是一套

open 又是另一套

是不是 Linux 内核里面有两套文件系统？
```

**不是。**

真实关系更接近：

```text
fopen()
  ↓
C库
  ↓
open()
  ↓
Linux 系统调用
  ↓
内核
```

以及：

```text
fread()
  ↓
C库 Buffer
  ↓
必要时调用 read()
  ↓
Linux 内核
```

所以：

> <span style="background:#00FF80;">**标准 I/O 是建立在更底层的文件 I/O / 系统调用接口之上的一层封装。**</span>

这也是为什么视频图里：

```text
fopen/fread/fwrite
       ↓
用户空间 buffer
       ↓
open/read/write
       ↓
内核
```

是一层套一层的。

------

## 9. 这里的 POSIX 又是什么？

视频 **01:35-03:55 左右**花了不少时间讲这个。

你可以先不要把 POSIX 想得太复杂。

把它理解成：

> **Unix-like 操作系统的一套接口规范。**

例如它规定很多类似这样的接口应该长什么样：

```c
open()
read()
write()
close()
```

于是：

```text
                POSIX规范
                   │
          ┌────────┴────────┐
          ↓                 ↓
        Linux             Unix
          │                 │
   open/read/write    open/read/write
```

所以程序员学会这一套以后，在很多 Unix-like 系统上都能看到相似接口。

### 一个很重要的区别

```text
POSIX
```

不是操作系统。

而是：

```text
规范 / 标准
```

Linux：

```text
操作系统内核
```

Unix：

```text
一大类操作系统体系
```

所以不要理解成：

```text
Linux → POSIX → Unix
```

而更像：

```text
          POSIX
        （规范）
         ↙   ↘
    Linux     Unix-like系统
    去实现这些规范
```

------

## 10. `svc 0` 是干什么的？

视频后面的图里还能看到：

```text
open/read/write
       ↓
     svc 0
       ↓
Linux内核
```

这和你正在学的 ARM 开发板关系很大。

你的 APP 正常运行在：

```text
用户空间
```

但真正访问内核资源时，要进入：

```text
内核空间
```

<span style="color:#FF0000; background:#00FF80;">在 ARM 架构里，可以通过类似 `svc` 的机制触发异常，从用户态进入内核态。</span>

可以先粗略理解：

```text
read()
  ↓
系统调用入口
  ↓
CPU 从用户态切换到内核态
  ↓
内核处理 read
  ↓
返回用户程序
```

所以视频画：

```text
svc
 ↓
sys_open / sys_read
```

就是在表达：

> <span style="background:#00FF80;">**应用层的系统调用最终要跨过用户态/内核态边界。**</span>

以后学 Linux 驱动时，这张图会越来越重要。

------

## 11. 还有一个非常容易混淆的地方：`fflush` 和 `fsync`

视频课件正好分别列出了：

标准 I/O：

```c
fflush()
```

系统调用 I/O：

```c
fsync()
```

它们不是一个意思。

可以先粗略理解为：

```text
APP
 │
 │ fwrite
 ↓
C库 Buffer
 │
 │ fflush
 ↓
Linux 内核中的缓存
 │
 │ fsync
 ↓
存储设备
```

所以：

### `fflush(fp)`

重点是：

> 把 **C 库用户空间缓冲区** 中的数据往下送。

而：

### `fsync(fd)`

重点是：

> 要求内核把这个文件的修改进一步同步到底层存储。

因此：

```text
fflush
```

和：

```text
fsync
```

虽然名字都有“flush/sync”的感觉，但工作层次不一样。

这个知识后面很容易考，也很容易写程序时搞混。

------

## 12. 内容结构

视频结构其实很清晰：

1. **开头**：引入文件 I/O。
2. **发展**：提出标准 I/O 和系统调用 I/O 两种函数。
3. **深入**：解释 POSIX、Linux、Unix 之间的关系。
4. **核心例子**：用连续 `read()` 和带 Buffer 的 `fread()` 对比效率。
5. **结尾**：重新回到完整架构图，把 APP → C 库 → 系统调用 → 内核 → 文件系统/驱动串起来。

整体表达路径：

> 从【两类文件 I/O 函数】开始，通过【POSIX + Buffer + read/fread 示例】解释差异，最后落到【Linux 完整文件 I/O 调用链】。

------

## 13. 观点分析

这个视频主要是技术讲解，**没有明显的价值观或争议性观点**，所以不需要强行做“作者立场分析”。

它真正希望观众接受的核心技术判断是：

| 内容                                                | 归属      | 明确/可能 | 依据                                  |
| --------------------------------------------------- | --------- | --------- | ------------------------------------- |
| Linux 文件操作常用接口可分为标准 I/O 和系统调用 I/O | 讲师/教材 | 明确      | 课件直接列出                          |
| 标准 I/O 存在用户空间缓冲                           | 讲师/教材 | 明确      | 架构图与板书重点解释                  |
| 标准 I/O 最终仍依赖底层系统调用                     | 讲师/教材 | 明确      | 图中 `fread → buffer → read → kernel` |
| Buffer 可以减少频繁进入内核的次数                   | 讲师      | 明确      | 通过 100 次读取例子解释               |

------

## 14. 人物分析

这个视频基本属于屏幕录制教学，讲师没有持续出镜，因此没有足够视觉信息去做情绪、性格、人格分析。

能明确看到的教学行为是：

```text
课件
 ↓
画架构图
 ↓
搜索 POSIX
 ↓
写代码例子
 ↓
用 Buffer 举例
 ↓
回到总图总结
```

说明这一节的教学重点不是让你记函数定义，而是建立**层次关系**。

至于情绪、人格、人品等，没有可靠证据，不适合判断。

------

## 15. Who / Whom / Whose / When / Where / What / Why / How

| 维度  | 视频中的答案                                                 |
| ----- | ------------------------------------------------------------ |
| Who   | Linux 应用程序以及 C 库、Linux 内核                          |
| Whom  | APP 对文件、设备进行读写                                     |
| Whose | `fopen/fread` 属于 C 标准库接口；`open/read/write` 属于更底层的系统调用 I/O 接口 |
| When  | APP 需要读取、写入文件或设备时                               |
| Where | 用户空间 → 内核空间 → 文件系统/驱动                          |
| What  | 两类文件 I/O 接口及其调用关系                                |
| Why   | 标准 I/O 引入缓冲，可以减少频繁系统调用                      |
| How   | `fread → 用户 Buffer → read → syscall → kernel → 文件系统/驱动` |

------

## 16. 重点片段精读

### 片段一：约 01:00-01:35

表面发生了什么：

课件第一次列出：

```text
标准 IO：
fopen/fread/fwrite/fseek/fflush/fclose

系统调用 IO：
open/read/write/lseek/fsync/close
```

更深层的意义：

**整个文件 I/O 章节以后基本就是在研究这两套接口以及它们的底层关系。**

------

### 片段二：约 03:55-05:45

板书类似：

```c
fd = open(...);

for (i = 0; i < 100; i++)
{
    read(fd, buf, 1);
}
```

重点不是语法。

重点是：

```text
100次 read
≈
可能发生很多次系统调用
```

是在为后面 Buffer 做铺垫。

------

### 片段三：约 05:45-07:05

这是我认为**本视频最重要的部分**。

讲师解释：

```text
fread
 ↓
第一次从底层读较大块数据
 ↓
APP / C库 Buffer
 ↓
后面的 fread 可以直接取
```

因此你一定要建立：

> <span style="background:#00FF80;">**标准 I/O = 系统调用 I/O + C 库封装 + 用户空间缓冲机制**</span>

这个认识。

------

### 片段四：约 07:05-09:22

讲师重新回到整张图：

```text
APP
 ↓
标准 IO
 ↓
Buffer
 ↓
系统调用 IO
 ↓
内核
 ↓
文件系统 / 驱动
```

这其实是在告诉你：

> 后面学文件、设备、驱动，看起来东西很多，但全部都可以放到这张图里找位置。

------

## 17. 我建议你这节课重点看什么？

这节课**不要优先背函数参数**。

你现在最重要的是把下面 5 个问题真正搞明白：

```text
1. fopen 和 open 为什么同时存在？

2. fread 和 read 到底是什么关系？

3. FILE * 和 fd 有什么区别？

4. Buffer 在什么位置？为什么需要它？

5. APP 调 read() 后，数据是怎么一路走到 Linux 内核和驱动的？
```

尤其是第 5 个。

因为它会直接连接你后面学习的：

```text
Linux应用开发
      ↓
系统调用
      ↓
Linux内核
      ↓
字符设备驱动
      ↓
硬件
```

这已经开始从“普通 C 编程”进入真正的 **Linux 应用与驱动之间的连接点**了。

------

## 18. 记忆版总结

```text
类型：Linux 文件 I/O 基础讲解

主线：
标准 I/O 与系统调用 I/O 的区别和联系

标准 I/O：
fopen / fread / fwrite / fclose
FILE *
带 C 库用户空间 Buffer

系统调用 I/O：
open / read / write / close
fd
更接近 Linux 内核

核心关系：
fread
 ↓
C库 Buffer
 ↓
read
 ↓
系统调用
 ↓
Linux 内核
 ↓
文件系统 / 驱动

Buffer 的意义：
减少频繁系统调用，提高效率

最重要时间段：
05:45-09:22

一句话记忆：
标准 I/O 不是绕开系统调用，而是在系统调用之上又封装了一层，并加入了用户空间缓冲。
```

你现在可以把这一节最核心的关系记成一个公式：

**`fopen/fread/fwrite = 更高层的 C 库接口`**
**`open/read/write = 更底层的 Linux I/O 接口`**

而两者最终操作的，仍然是**同一个 Linux 内核里的文件/设备体系**。





# 问题1

应用程序每次调用 open、read、write 的时候都会直接进入内核，都会直接访问内核这句话要怎么理解呢，我不是太懂，你能够给我详细解释下吗？



可以。你这里最容易被“**直接进入内核、直接访问内核**”这句话带偏。更准确地说应该是：

> <span style="background:#00FF80;">**应用程序调用 `open/read/write` 这类系统调用接口时，需要让 CPU 从用户态切换到内核态，由 Linux 内核替应用程序完成相应操作。**</span>

它不是说 APP 可以“随便进入内核、直接读写内核内存”。恰恰相反，<span style="background:#00FF80;">**APP 不能直接访问内核，只能通过系统调用这个受控入口请求内核帮忙。**</span>

我们一点一点拆。

------

## 一、先理解：程序正常运行时，其实不在“内核里面”

假设你运行：

```c
int main(void)
{
    int a = 10;
    int b = 20;
    int c = a + b;

    return 0;
}
```

这些普通计算：

```text
a = 10
b = 20
c = a + b
```

CPU 都是在执行你的应用程序代码。

此时属于：

```text
用户空间
User Space
```

CPU 处于：

```text
用户态
User Mode
```

可以简单画成：

```text
┌─────────────────────────────┐
│         用户空间             │
│                             │
│      你的应用程序 APP        │
│                             │
│   a = 10;                   │
│   b = 20;                   │
│   c = a + b;                │
│                             │
└─────────────────────────────┘

=========== 边界 =============

┌─────────────────────────────┐
│         内核空间             │
│                             │
│        Linux Kernel         │
│                             │
│ 文件系统 / 内存管理 / 驱动等 │
└─────────────────────────────┘
```

你的普通 C 代码并不会因为执行一个加法，就跑进 Linux 内核。

------

## 二、但是 APP 有些事情自己做不了

比如你的程序想读取：

```text
/test.txt
```

你可能写：

```c
read(fd, buf, 100);
```

问题来了：

<span style="color:#FF0000; background:#00FF80;">你的 APP 自己知道：</span>

> `/test.txt` 在硬盘哪个位置吗？

<span style="color:#FF0000; background:#00FF80;">不知道。</span>

<span style="color:#FF0000; background:#00FF80;">APP 自己能直接控制：</span>

```text
eMMC
SD卡
硬盘
USB
网卡
串口
```

吗？

<span style="color:#FF0000; background:#00FF80;">通常也不可以。</span>

为什么？

因为<span style="color:#FF0000; background:#00FF80;">这些资源归：</span>

> <span style="background:#00FF80;">**Linux 内核统一管理。**</span>

------

## 三、所以 APP 想读文件，需要“请求内核帮忙”

这才是系统调用的真正含义。

例如：

```c
read(fd, buf, 100);
```

你可以把它想象成：

```text
APP：

“Linux 内核，
我想从 fd 对应的文件里读取 100 个字节，
请把数据放到我的 buf 里面。”
```

于是发生：

```text
APP
 │
 │ read(fd, buf, 100)
 ↓
系统调用入口
 │
 ↓
CPU进入内核态
 │
 ↓
Linux内核处理读取请求
 │
 ↓
把结果返回APP
```

完成以后：

```text
CPU重新回到用户态
```

你的 APP 接着往下执行。

------

## 四、所以所谓“进入内核”到底是谁进入？

这一点非常关键。

并不是：

```text
你的整个程序被搬进Linux内核
```

也不是：

```text
read函数钻进内核里
```

更准确地讲是：

> <span style="background:#00FF80;">**CPU 的执行状态从用户态切换到了内核态，并开始执行 Linux 内核中的系统调用处理代码。**</span>（<span style="background:#00FF80; color:#FF0000;">处于内核态还是用户态取决于CPU执行哪段程序 ，CPU执行你的app的程序就是处于用户态，CPU执行Linux内核的程序就是处于内核态</span>）

例如：

```c
read(fd, buf, 100);
```

前一刻：

```text
CPU执行你的APP
     ↓
用户态
```

系统调用发生：

```text
CPU切换状态
     ↓
内核态
     ↓
执行Linux内核代码
```

处理完成：

```text
返回
 ↓
CPU切回用户态
 ↓
继续执行APP
```

可以想成：

```text
             用户态
                │
                │
        read(fd, buf, 100)
                │
                ▼
         ┌─────────────┐
         │ 系统调用入口 │
         └──────┬──────┘
                │
===============│================
用户空间       │          内核空间
===============│================
                ▼
         Linux Kernel

         查fd
          ↓
       查找文件
          ↓
       文件系统
          ↓
      必要时读设备
          ↓
       得到数据
          ↓
    拷贝到用户buf
                │
                ▼
===============│================
                │
            返回APP
```

------

## 五、以你的 ARM IMX6ULL 来理解会更直观

你学的是 ARM Linux，所以视频里可能会看到：

```text
svc
```

例如概念上：

```text
read()
 ↓
svc
 ↓
Linux内核
```

`svc` 可以先理解成：

> <span style="background:#00FF80;">**ARM CPU 提供的一扇“进入操作系统内核”的门。**</span>

正常 APP：

```text
用户态
```

执行到系统调用以后，大概经过：

```text
read()
  ↓
准备系统调用参数
  ↓
执行 SVC 指令
  ↓
CPU发生异常/特权级切换
  ↓
进入 Linux 内核
  ↓
Linux判断：
“哦，这是一个 read 请求”
  ↓
执行相应内核代码
```

之后再回来。

所以你可以把 `svc` 想象成：

```text
APP                           Kernel

 用户态                        内核态

   │
   │ read()
   │
   │
   ├────── SVC ───────────────►
   │                            │
   │                         处理read
   │                            │
   ◄────────────────────────────┤
   │
继续执行
```

------

## 六、那 `open()` 到底进入内核干什么？

比如：

```c
int fd = open("hello.txt", O_RDONLY);
```

你可能以前觉得：

> `open()` 就是“打开文件”。

其实里面内核做了不少事情。

APP：

```text
open("hello.txt")
```

<span style="color:#FF0000; background:#00FF80;">进入内核以后，大致需要：</span>

```text
Linux内核
   ↓
解析文件路径
   ↓
找到对应目录
   ↓
找到 hello.txt
   ↓
检查文件是否存在
   ↓
检查访问权限
   ↓
建立内核中的文件管理结构
   ↓
分配文件描述符 fd
   ↓
返回给APP
```

最终：

```c
fd = 3;
```

你的 APP 拿到：

```text
3
```

以后就可以说：

```c
read(3, buf, 100);
```

Linux 内核看到 `3` 就知道：

> “哦，你说的是刚才那个 `hello.txt`。”

------

## 七、`read()` 进入内核以后干什么？

假设：

```c
char buf[100];

read(fd, buf, 100);
```

可以理解为：

```text
APP
 │
 │ fd = 3
 │ buf = 某块用户内存
 │ size = 100
 │
 ▼
read(3, buf, 100)
 │
 ▼
进入内核
 │
 ▼
根据 fd=3
找到内核中的文件对象
 │
 ▼
看看要读取的数据在哪里
 │
 ├── 内核缓存中有
 │       ↓
 │    直接取缓存
 │
 └── 内核缓存中没有
         ↓
      文件系统
         ↓
      设备驱动
         ↓
       SD/eMMC
         ↓
      得到数据
 │
 ▼
把数据复制到
用户空间的 buf
 │
 ▼
返回用户态
```

这里有一个非常重要的纠正：

### `read()` 每次都进入内核

对。

但是：

### `read()` 每次都直接访问硬盘/SD 卡

**不对。**

因为 Linux 有缓存。

可能是：

```text
read()
 ↓
进入内核
 ↓
内核发现数据已经在 page cache
 ↓
直接从内存拿
```

根本不需要访问 SD 卡。

所以：

> **进入内核 ≠ 每次访问物理硬件。**

这两个概念一定要分开。

------

## 八、`write()` 也是一样

例如：

```c
write(fd, buf, 100);
```

APP 不能自己说：

> “我要控制 eMMC，把这些数据写到某个扇区。”

而是：

```text
APP
 │
 │ write(fd, buf, 100)
 ▼
进入Linux内核
 │
 ▼
找到fd对应文件
 │
 ▼
处理文件位置、权限等
 │
 ▼
把数据交给内核文件系统
```

但同样：

> `write()` 返回，并不一定表示数据已经真的写进了 Flash/硬盘。

很多时候是：

```text
APP
 ↓
write
 ↓
内核
 ↓
内核缓存
```

然后内核之后再适当写入存储设备。

这也是为什么后面还会碰到：

```c
fsync(fd);
```

它和“把数据真正同步下去”有关。

------

## 九、为什么普通函数不会进入内核，而 `read()` 会？

比如：

```c
int add(int a, int b)
{
    return a + b;
}
```

你调用：

```c
c = add(a, b);
```

大致就是：

```text
APP
 ↓
add()
 ↓
CPU算一下
 ↓
返回
```

全部：

```text
用户态 → 用户态
```

没有进入内核。

但是：

```c
read(fd, buf, 100);
```

需要 Linux 管理的资源。

于是：

```text
用户态
 ↓
read
 ↓
系统调用
 ↓
内核态
 ↓
Linux内核
 ↓
返回
 ↓
用户态
```

所以真正区别在于：

> **是不是需要操作系统提供服务。**

------

## 十、为什么系统调用不能像普通函数一样直接 `call` 过去？

因为 Linux 故意把：

```text
用户空间
```

和：

```text
内核空间
```

隔离起来。

假设 APP 可以随便执行内核代码、访问内核内存，那就很危险。

例如一个普通程序：

```c
某个小游戏
```

如果能直接：

```text
修改内核内存
修改其他程序内存
关闭网卡
擦除磁盘
操作任意硬件
```

那操作系统基本就没有安全性了。

所以 Linux 的设计是：

```text
              APP
               │
               │
               ▼
       只能走规定的门
         “系统调用”
               │
               ▼
             Kernel
```

就像一栋办公楼：

```text
普通人
 │
 │
 × 不能翻墙进去
 × 不能随便进机房
 × 不能自己拿钥匙
 │
 ▼
前台窗口
 │
 ▼
提出申请
 │
 ▼
工作人员帮你办理
```

对应关系就是：

```text
普通人       = APP
办公区域     = 内核
前台窗口     = 系统调用
工作人员     = Linux Kernel
```

所以我更推荐你把“系统调用”理解成：

> **应用程序向 Linux 内核申请服务的一套官方接口。**

------

## 十一、那为什么说系统调用有“开销”？

现在你应该能理解了。

普通函数：

```text
APP
 ↓
普通函数
 ↓
APP

一直用户态
```

而系统调用：

```text
APP
 ↓
保存当前状态
 ↓
用户态 → 内核态
 ↓
执行Linux内核代码
 ↓
内核态 → 用户态
 ↓
恢复APP执行
```

中间比普通函数多很多东西。

所以：

```c
read(fd, buf, 1);
read(fd, buf, 1);
read(fd, buf, 1);
...
```

假设调用 1000 次：

```text
用户态 → 内核态 → 用户态
用户态 → 内核态 → 用户态
用户态 → 内核态 → 用户态
...
```

1000 次。

这也是为什么前一个视频会讲 `fread()` 的用户空间 Buffer。

------

## 十二、现在就能理解 `fread()` 为什么有 Buffer 了

假设你想读 100 个字节。

### 方法一：不停 `read`

```c
for (i = 0; i < 100; i++)
{
    read(fd, buf, 1);
}
```

可能发生：

```text
APP
↓
Kernel
↑
APP
↓
Kernel
↑
APP
↓
Kernel
↑
...
```

很多次用户态/内核态切换。

------

### 方法二：C 标准库 `fread`

可能底层第一次直接：

```text
Kernel
 ↓
一次给C库 4096字节
 ↓
用户空间Buffer
```

以后程序只需要：

```text
fread
 ↓
Buffer

fread
 ↓
Buffer

fread
 ↓
Buffer
```

只在 Buffer 不够时：

```text
fread
 ↓
read
 ↓
Kernel
```

于是减少：

> **系统调用次数。**

------

## 十三、所以 `fread()` 是不是“不进入内核”？

这个要特别准确。

不能简单说：

> `fread()` 不进入内核。

正确说法是：

> **一次 `fread()` 不一定会触发系统调用。**

例如 C 库 Buffer 已经有数据：

```text
APP
 │
 │ fread()
 ↓
C库 Buffer
 │
 ↓
返回数据
```

完全待在用户空间。

但是 Buffer 没数据：

```text
APP
 │
 │ fread()
 ↓
C库
 │
 │ read()
 ↓
Linux Kernel
```

这时候仍然需要进入内核。

所以：

```text
read()
每次调用基本都会产生一次系统调用
```

而：

```text
fread()
并不是每次都必须产生 read 系统调用
```

这就是两者非常大的差别之一。

------

## 十四、还有一个技术细节：`open()` 这个 C 函数本身通常还是一个“包装器”

严格一点讲：

当你代码写：

```c
open(...)
```

我们一般口语上直接说：

> 调用 `open` 系统调用。

但实际程序里往往先进入：

```text
glibc / C库里的包装函数
```

再通过系统调用机制进入 Linux。

甚至在现代 Linux 上，某些 libc 实现里 `open()` 最终可能转成类似：

```text
openat(...)
```

再进行系统调用。

你现在不用纠结这个细节。

学习阶段可以先记成：

```text
open/read/write
       ↓
系统调用接口
       ↓
Linux Kernel
```

等以后学系统调用实现再细分。

------

## 十五、把整件事情连成一条完整链，你就彻底懂了

假设程序：

```c
int fd = open("hello.txt", O_RDONLY);

char buf[100];

read(fd, buf, 100);

close(fd);
```

### 第一步：程序运行

```text
APP
│
│ 普通代码
│
▼
用户态
```

### 第二步：执行 `open`

```text
open("hello.txt")
        ↓
     系统调用
        ↓
用户态 → 内核态
        ↓
Linux查找文件
        ↓
建立文件对象
        ↓
返回 fd = 3
        ↓
内核态 → 用户态
```

### 第三步：执行 `read`

```text
read(3, buf, 100)
        ↓
     系统调用
        ↓
用户态 → 内核态
        ↓
找到 fd=3 对应文件
        ↓
取得文件数据
        ↓
复制给 buf
        ↓
返回100
        ↓
内核态 → 用户态
```

### 第四步：执行 `close`

```text
close(3)
   ↓
系统调用
   ↓
进入内核
   ↓
释放/减少文件相关内核资源
   ↓
返回用户态
```

于是：

```text
APP
 │
 ├── 普通计算 ───────────── 用户态
 │
 ├── open() ──────────────► 内核态
 │                         ↓
 │ ◄────────────────────── 返回
 │
 ├── 普通代码 ───────────── 用户态
 │
 ├── read() ──────────────► 内核态
 │                         ↓
 │ ◄────────────────────── 返回
 │
 ├── 普通代码 ───────────── 用户态
 │
 └── close() ─────────────► 内核态
                           ↓
                    返回用户态
```

------

# 最后你记住这 4 句话就够了

```text
① APP平时运行在用户态。

② APP没有权限随意访问Linux内核和硬件资源。

③ open/read/write 是APP请求Linux内核提供服务的入口。

④ 调用系统调用时，CPU从用户态切换到内核态；
   内核处理完成后，再返回用户态。
```

而“**每次 `open/read/write` 都会直接进入内核**”这句话，更严谨地改写成：

> **每次真正执行这类系统调用时，都会通过系统调用机制发生一次从用户态到内核态的受控切换，由 Linux 内核执行相应操作；但进入内核并不等于每次都会访问磁盘或硬件。**

你现在可以把它和前面学的 `fread` 连起来看：**`read()` 的昂贵之处之一就是每次都需要跨越“用户态 ↔ 内核态”这条边界，而 `fread()` 的 Buffer 很大一个意义就是尽量少跨几次这条边界。**



# 问题2:（了解就行，不用太过于细致）

处于内核态还是用户态取决于CPU执行哪段程序 ，CPU执行你的app的程序就是处于用户态，CPU执行Linux内核的程序就是处于内核态。我对这句话的理解对吗，还有没有什么需要补充的吗。



对，你这个理解**基本是对的，而且已经抓住核心了**。

更准确一点，可以把它表述成：

> **CPU 当前处于用户态还是内核态，主要取决于它当前以什么特权级执行哪一类代码。**
>
> - 执行普通应用程序代码时，一般处于**用户态**
> - 执行 Linux 内核代码时，一般处于**内核态**

不过有几个细节值得补上，这样你的理解会更完整。

第一，**不是“代码属于谁”单独决定状态，而是 CPU 的特权级决定状态**。也就是说，<span style="color:#FF0000; background:#00FF80;">本质上是 CPU 自己有不同的运行级别。Linux 会让普通 APP 运行在低权限级别，让内核运行在高权限级别。</span>

所以更底层地看是：

```text
CPU执行普通APP
↓
CPU当前特权级较低
↓
用户态
```

而：

```text
CPU执行Linux内核
↓
CPU当前特权级较高
↓
内核态
```

你可以把“执行哪段代码”理解成表面现象，把“CPU 当前特权级”理解成底层原因。

第二，**用户态不是不能执行 CPU 指令，而是不能执行某些特权操作**。比如普通 APP 当然可以做：

```c
a = b + c;
```

也可以：

```c
for (...)
```

这些都完全没问题。

但<span style="color:#FF0000; background:#00FF80;">它不能随意做这类事情：</span>

```text
直接修改内核页表
直接操作任意硬件寄存器
随便访问内核内存
关闭中断
修改某些CPU控制寄存器
```

这些需要更高权限。

所以用户态的核心不是“能力弱”，而是：

> **权限被限制。**

第三<span style="background:#00FF80;">，**从用户态进入内核态，不只有系统调用这一种情况**。</span>

你前面学的：

```c
read()
write()
open()
```

这是最常见的一种：

```text
用户态
↓
系统调用
↓
内核态
```

但<span style="background:#00FF80; color:#FF0000;">还有两种很重要的情况：</span>

```text
中断
异常
```

例如：

```text
网卡收到数据
↓
产生硬件中断
↓
CPU进入内核态
↓
执行中断处理程序
```

或者：

```text
APP访问非法内存
↓
发生异常
↓
CPU进入内核态
↓
Linux处理这个异常
```

所以<span style="color:#FF0000; border:1px solid #330000; background:#00FF80;">以后你最好记成：</span>

```text
进入内核态常见三种原因：

1. 系统调用
2. 中断
3. 异常
```

第四，还有一个特别容易混淆的点：**同一个进程，也可能一会儿运行在用户态，一会儿运行在内核态。**

比如你的程序：

```c
printf("hello");
read(fd, buf, 100);
printf("done");
```

执行过程可以粗略看成：

```text
printf附近的普通用户代码
↓
用户态

read()
↓
系统调用
↓
内核态

read处理完成
↓
返回

继续执行APP
↓
用户态
```

所以不是说：

```text
这个进程是“用户态进程”
```

然后永远待在用户态。

更准确地说是：

> **同一个进程在运行过程中，会随着执行路径在用户态和内核态之间切换。**

第五，你现在可以把整个概念压缩成这一张图：

```text
             CPU

      执行普通APP代码
             ↓
          用户态
             │
             │ 系统调用 / 中断 / 异常
             ▼
          内核态
             │
      执行Linux内核代码
             │
             ▼
          处理完成
             │
             └────────→ 返回用户态
```

所以你原来的这句话：

> “CPU执行你的 APP 程序就是用户态，CPU执行 Linux 内核程序就是内核态”

作为学习阶段的理解，**完全可以这么记**。

我只建议你再补一句：

> **真正决定用户态/内核态的是 CPU 当前的特权级；Linux 只是让 APP 和内核分别运行在不同特权级。**

这样就更严谨了。

如果以后你学 ARM 的话，还会进一步看到类似 **EL0 / EL1** 这种概念，它本质上就是把“用户态/内核态”落实到了 ARM CPU 的具体特权级上。





# open进入内核后的操作

Linux内核
   ↓
解析文件路径
   ↓
找到对应目录
   ↓
找到 hello.txt
   ↓
检查文件是否存在
   ↓
检查访问权限
   ↓
建立内核中的文件管理结构
   ↓
分配文件描述符 fd
   ↓
返回给APP

将上述open()系统调用进入内核的过程从硬件软件结合角度给我给我细致讲解下。



可以。我们就拿你在 **i.MX6ULL（ARM Cortex-A7，ARMv7-A）Linux** 上执行下面这句代码来完整走一遍：

```c
int fd = open("/home/user/hello.txt", O_RDONLY);
```

这一次不只看软件函数，而是看：

**C程序 → CPU指令 → ARM特权级切换 → Linux内核 → VFS → 文件系统 → 缓存/存储硬件 → 内核数据结构 → fd → 返回APP**

------

## 一、先给你一张总地图

整个过程可以先压缩成：

```text
你的 APP
│
│ open("/home/user/hello.txt", O_RDONLY)
│
▼
C库 libc
│
│ 准备系统调用号、参数
│
▼
ARM CPU 执行 SVC 指令
│
│ 用户态 → 内核态
│
▼
Linux 系统调用入口
│
│ 获取 pathname、flags
│
▼
VFS 虚拟文件系统
│
│ 路径解析
│
├─ /
├─ home
├─ user
└─ hello.txt
│
▼
找到对应 dentry / inode
│
│ 检查权限、文件类型等
▼
具体文件系统
ext4 / ubifs / ...
│
│ 如果元数据不在内存缓存
▼
块设备层 / MTD
│
▼
设备驱动
│
▼
eMMC / SD / NAND 等硬件
│
▲
└──────── 得到需要的文件元数据
│
▼
Linux 创建 struct file
│
▼
放入当前进程的文件描述符表
│
▼
分配 fd，例如 3
│
▼
ARM CPU 从内核态返回用户态
│
▼
APP 得到 fd = 3
```

注意一个非常重要的地方：

> **`open()` 的主要任务不是把文件内容读出来。**

它主要是：

> **找到这个文件 + 检查你有没有资格打开 + 建立内核中的“已打开文件对象” + 给你的进程分配一个 fd。**

真正大量读取文件内容，一般是后面的：

```c
read(fd, buf, 100);
```

------

## 二、第一阶段：APP 在用户态执行 `open()`

假设代码是：

```c
int fd;

fd = open("/home/user/hello.txt", O_RDONLY);
```

此时 CPU 正在执行你的 APP。

在你的 i.MX6ULL Cortex-A7 上，这时候 CPU 处于一个**非特权的用户模式**。

可以先理解：

```text
CPU
↓
正在执行 APP 指令
↓
User Mode
↓
用户态
```

这时候 CPU 可以正常执行：

```text
加减乘除
函数调用
访问自己进程的内存
判断
循环
```

但是不能随便：

```text
访问内核内存
修改页表
关闭中断
操作任意硬件寄存器
修改其他进程内存
```

因为这些属于高权限操作。

------

## 三、`open()` 首先其实会进入 C 库

你代码中写：

```c
open(...)
```

不应该简单理解成：

```text
APP
↓
瞬间跳到Linux内核
```

更准确是：

```text
APP
↓
libc中的 open() 包装函数
↓
系统调用
↓
Linux Kernel
```

也就是说：

```c
open("/home/user/hello.txt", O_RDONLY);
```

通常首先是一个**用户空间库函数**。

例如：

```text
你的程序
   │
   ▼
libc
   │
   └── open()
```

libc 的职责之一就是：

> 把你这种方便人使用的 C 函数调用，转换成 Linux 能识别的系统调用请求。

现代 Linux 的具体 libc 实现中，`open()` 还可能最终使用 `openat()` 一类系统调用实现。

你现阶段先记：

```text
open()
↓
libc包装
↓
文件打开系统调用
```

就够了。

------

## 四、那么 Linux 怎么知道你到底想干什么？

调用：

```c
open("/home/user/hello.txt", O_RDONLY);
```

至少包含几个信息：

```text
路径：
"/home/user/hello.txt"

打开方式：
O_RDONLY

系统调用类型：
我要执行“打开文件”
```

所以在真正进入内核前，需要准备：

```text
系统调用号
+
参数
```

CPU 寄存器会参与传递这些信息。

------

## 五、ARM CPU 的寄存器这时候派上用场了

以 ARM 32 位 Linux 的思路理解，系统调用参数通常会通过寄存器传递。

概念上类似：

```text
r0 ─────→ 参数1
r1 ─────→ 参数2
r2 ─────→ 参数3
...

某寄存器 ─→ 系统调用编号
```

例如老式 `open` 的概念可以理解成：

```text
r0 → pathname 地址
r1 → flags
r2 → mode
r7 → 系统调用编号
```

然后执行：

```asm
svc 0
```

注意：

`r0` 里面并不是：

```text
/home/user/hello.txt
```

这串字符本身。

而通常是：

```text
这串字符在用户空间内存中的地址
```

也就是类似：

```text
APP用户空间

0x12340000
│
▼
"/home/user/hello.txt\0"
```

然后：

```text
r0 = 0x12340000
```

内核以后根据这个地址取得路径字符串。

------

## 六、最关键一步：CPU 执行 `SVC`

这就是硬件和软件真正连接起来的地方。

CPU 正在用户态运行：

```text
APP
↓
open()
↓
libc
↓
准备参数
```

接下来执行 ARM 指令：

```asm
svc 0
```

`SVC`：

> **Supervisor Call**

可以理解成：

> “CPU，我这个普通 APP 现在需要操作系统内核服务，请进入内核规定的入口。”

------

## 七、`SVC` 不是普通函数调用

普通函数：

```c
foo();
```

可能就是：

```text
用户态
↓
跳转到 foo
↓
还是用户态
```

CPU 权限没变。

但：

```asm
svc 0
```

不同。

这是 <span style="background:#00FF80;">ARM CPU 专门提供的一种**异常机制**。</span>

CPU 硬件发现：

```text
执行了 SVC
```

于是<span style="background:#00FF80;"> CPU 自己完成重要操作：</span>

```text
用户模式
   ↓
SVC异常发生
   ↓
保存部分现场
   ↓
改变CPU运行模式
   ↓
进入特权模式
   ↓
跳转到异常向量入口
```

注意：

> 这里真正完成“用户态 → 内核态切换”的基础，是 **CPU硬件**。

不是 C 语言自己做到的。

------

## 八、CPU 到底发生了什么？

在 ARMv7-A 这种处理器上，可以粗略理解成：

原来：

```text
CPU Mode = User
PC = APP中的某条指令
```

执行：

```asm
svc 0
```

CPU 硬件会进行类似：

```text
保存原来的执行状态
↓
记录返回地址
↓
切换到异常/特权模式
↓
跳到 SVC 异常入口
```

ARM 有异常向量表。

SVC 对应一个规定的异常入口。

所以：

```text
APP
 │
 │ svc
 ▼
ARM CPU异常向量
 │
 ▼
Linux事先放好的SVC处理代码
```

这里非常漂亮：

### ARM CPU 只规定：

> “发生 SVC 后我要跳到这里。”

### Linux 内核负责：

> “那我就在这里放我的系统调用入口代码。”

硬件和操作系统就是这样接上的。

------

## 九、这里不要误解成“APP 被搬进内核”

不是：

```text
APP
      ↓
被复制到 Kernel
```

而是：

```text
CPU原来执行：
APP代码

        ↓ SVC

CPU现在执行：
Linux内核代码
```

也就是说：

> **CPU 的执行流变了，权限也变了。**

这和你前面说的：

> CPU 执行 APP → 用户态
> CPU 执行 Linux Kernel → 内核态

正好接上。

------

## 十、MMU 这时候在干什么？

这又是硬件里一个非常重要的角色。

你的 ARM Cortex-A7 有：

> **MMU，Memory Management Unit**

它<span style="color:#FF0000; background:#00FF80;">负责虚拟地址到物理地址转换以及内存权限检查。</span>

进程看到的是：

```text
虚拟地址空间
```

大概可以抽象成：

```text
一个进程的虚拟地址空间

低地址
┌─────────────────────┐
│ APP代码              │
│ APP数据              │
│ heap                 │
│ stack                │
│ libc                 │
├─────────────────────┤
│                     │
│ 内核映射区域          │
│                     │
└─────────────────────┘
高地址
```

用户态时：

```text
APP想访问内核区域
↓
MMU权限检查
↓
禁止
```

所以你不能：

```c
随便取个内核地址
然后 *ptr = 123;
```

但<span style="color:#FF0000; background:#00FF80;"> SVC 之后：</span>

```text
CPU进入特权模式
```

<span style="color:#FF0000; background:#00FF80;">Linux 内核拥有访问相应内核地址空间的权限。</span>

所以：

```text
CPU + MMU + 页表
```

共同构成了用户空间和内核空间隔离的重要基础。

------

## 十一、进入 Linux 内核之后，首先不是马上找硬盘

现在 CPU 已经开始执行 Linux 系统调用入口代码了。

Linux 需要先弄明白：

```text
谁调用我？

调用什么系统调用？

参数是什么？
```

所以概念上：

```text
SVC入口
↓
保存APP的寄存器现场
↓
识别系统调用号
↓
获得系统调用参数
↓
分派到对应的内核函数
```

例如：

```text
syscall number
      ↓
系统调用表
      ↓
找到“打开文件”对应处理函数
```

你可以理解成一个巨大菜单：

```text
系统调用号 0    → 某功能
系统调用号 1    → 某功能
...
系统调用号 xxx  → read
...
系统调用号 yyy  → open/openat
```

------

## 十二、Linux 还要给这次系统调用准备“内核栈”

APP 有自己的用户栈：

```text
用户栈
```

进入内核处理系统调用的时候，又需要：

```text
内核栈
```

所以一个进程实际上不只是简单拥有：

```text
用户空间代码
```

内核还为它维护各种信息。

概念上：

```text
进程
├─ 用户虚拟地址空间
├─ 用户栈
├─ 内核管理信息
├─ 文件描述符表
└─ 内核栈
```

当它陷入内核执行系统调用时：

```text
CPU
↓
开始使用相应的内核执行环境
```

于是内核函数就可以层层调用。

------

## 十三、Linux 现在要取得文件路径

APP 给的是：

```text
"/home/user/hello.txt"
```

但是这字符串位于：

> **用户空间内存**

内核不能傻乎乎地完全相信用户提供的指针。

因为 APP 有可能传：

```c
open((char *)0x123, ...);
```

甚至一个非法地址。

所以内核通常要使用安全的用户空间访问机制：

```text
用户提供pathname指针
        ↓
检查
        ↓
安全地从用户空间读取
        ↓
得到内核能够使用的路径字符串
```

概念上你可以把它理解成：

```text
APP内存

"/home/user/hello.txt"
        │
        │ 安全复制/取得
        ▼
Kernel
```

这里会涉及 Linux 中类似：

```text
copy_from_user
getname
```

这一类机制。

------

## 十四、接下来进入 Linux VFS

这部分是 Linux 文件系统非常核心的知识。

Linux 支持很多文件系统：

```text
ext4
FAT
NTFS
UBIFS
NFS
tmpfs
procfs
sysfs
...
```

但 APP 全部写：

```c
open()
read()
write()
close()
```

为什么？

因为中间有：

### VFS

Virtual File System：

> **虚拟文件系统层**

你可以理解成：

```text
                 APP
                  │
                 open
                  │
                  ▼
             ┌─────────┐
             │   VFS   │
             └─────────┘
              /   |   \
             /    |    \
           ext4 UBIFS  FAT
```

VFS 就像统一总台。

APP 不需要知道：

```text
hello.txt到底在ext4？
还是FAT？
还是网络文件系统？
```

VFS 帮你屏蔽这些差异。

------

## 十五、现在真正开始解析路径

路径：

```text
/home/user/hello.txt
```

不是一次直接：

```text
找到 hello.txt
```

而是一级一级走。

因为第一个字符是：

```text
/
```

说明它是：

> **绝对路径**

于是从当前进程的根目录开始。

------

## 十六、第一步：找到 `/`

Linux 内核知道当前进程所看到的：

```text
root
```

于是：

```text
/
```

对应一个 VFS 对象。

这里要开始认识两个非常重要的数据结构：

```text
dentry
inode
```

------

## 十七、dentry 是什么？

`dentry` 可以简单理解：

> **目录项，负责描述“名字与文件对象之间的对应关系”。**

例如目录：

```text
/
├── home
├── bin
├── etc
└── dev
```

这些名字：

```text
home
bin
etc
dev
```

在 VFS 中会涉及 dentry。

所以可以先记：

```text
名字
↓
dentry
↓
找到对应文件对象
```

------

## 十八、inode 又是什么？

inode 更接近：

> **文件本身的元数据描述。**

例如 `hello.txt` 的 inode 里面可能包含：

```text
文件类型
权限
所有者UID
所属组GID
文件大小
时间戳
文件系统相关信息
数据位置相关信息
...
```

所以：

```text
hello.txt
```

这个名字本身和：

```text
这个文件究竟是什么
```

是两个层次。

可以粗略理解：

```text
"hello.txt"
     │
   dentry
     │
     ▼
   inode
     │
     ├─ 权限
     ├─ 类型
     ├─ 大小
     └─ 数据相关信息
```

这个概念以后学 Linux 驱动、VFS 会经常出现。

------

## 十九、继续解析 `/home/user/hello.txt`

Linux 开始：

```text
/
↓
找 home
```

然后：

```text
/home
↓
找 user
```

再：

```text
/home/user
↓
找 hello.txt
```

于是：

```text
/
└── home
     └── user
          └── hello.txt
```

Linux 是一层层做 pathname lookup。

------

## 二十、这时候是不是每找一级目录都要访问 eMMC？

**不一定！**

这是特别重要的地方。

Linux 内核有：

```text
dentry cache
inode cache
page cache
```

所以可能：

```text
Linux想找 home
↓
内存缓存中已经有
↓
直接得到
```

然后：

```text
想找 user
↓
缓存也有
↓
直接得到
```

然后：

```text
想找 hello.txt
↓
缓存也有
```

这样整个路径查找可能主要在：

> **DDR 内存**

里完成。

------

## 二十一、什么情况下才可能真正访问存储硬件？

假设：

```text
hello.txt的信息
```

当前并不在内核缓存。

那么文件系统可能就需要获取存储介质上的元数据。

例如：

```text
VFS
↓
具体文件系统，例如 ext4
↓
需要读取目录块 / inode
↓
块设备层
↓
eMMC/SD 驱动
↓
硬件控制器
↓
eMMC / SD
```

这时候才真正涉及硬件。

------

## 二十二、从文件系统一路走到硬件是什么样？

假设根文件系统在 eMMC 上，概念上：

```text
open()
 │
 ▼
VFS
 │
 ▼
ext4
 │
 │ “我要读取某个元数据块”
 ▼
Linux块设备层
 │
 ▼
MMC子系统
 │
 ▼
i.MX6ULL USDHC驱动
 │
 ▼
i.MX6ULL SoC里的 USDHC 控制器
 │
 ▼
电气信号
 │
 ▼
eMMC / SD卡
```

这里已经真正从：

```text
软件
```

一路走到：

```text
硬件
```

了。

------

## 二十三、CPU 怎么控制这个硬件？

你的 i.MX6ULL 内部有很多硬件控制器。

例如：

```text
CPU
│
├─ GPIO Controller
├─ UART Controller
├─ I2C Controller
├─ USDHC Controller
├─ Ethernet Controller
└─ ...
```

SD/eMMC 一般不是 ARM CPU 自己一根一根产生 SD 信号。

而是：

```text
CPU
↓
Linux驱动
↓
配置USDHC控制器寄存器
↓
USDHC硬件控制器
↓
产生SD/eMMC总线时序
↓
存储芯片
```

所以：

> **驱动程序是内核软件，控制器是真实硬件。**

这个区分特别重要。

------

## 二十四、MMIO 是软件和硬件之间的重要桥梁

Linux 驱动最后想控制：

```text
USDHC
UART
GPIO
```

经常会操作：

> **硬件寄存器**

这些寄存器会映射进 CPU 的地址空间。

这就是：

> Memory-Mapped I/O，MMIO。

概念上：

```c
某个地址
```

不一定对应普通 DDR。

可能对应：

```text
USDHC控制寄存器
```

于是：

```text
CPU执行 store 指令
↓
访问某个物理地址
↓
SoC内部总线判断：
这不是DDR地址
↓
这是USDHC寄存器地址
↓
把数据送给USDHC控制器
```

这就是非常典型的：

> **CPU 指令 → 总线 → 外设寄存器 → 硬件**

------

## 二十五、读取存储数据通常还会涉及 DMA

例如要从 SD/eMMC 读取一块数据。

如果让 CPU 一个字节一个字节搬：

```text
eMMC
↓
CPU
↓
DDR
```

效率很低。

因此通常可以使用：

> **DMA，Direct Memory Access**

概念上变成：

```text
CPU：
“DMA/控制器，你把这一块数据
从设备搬到这块DDR内存。”

      ↓

DMA / USDHC
自己进行数据传输

eMMC
  ↓
USDHC
  ↓
DMA
  ↓
DDR

      ↓

完成后：
产生中断通知CPU
```

CPU 不需要一直搬数据。

------

## 二十六、于是这里又出现“中断”

设备完成后：

```text
USDHC
↓
产生硬件中断
↓
GIC中断控制器
↓
ARM Cortex-A7
↓
CPU执行Linux中断处理程序
```

这里同样体现：

```text
硬件事件
↓
CPU异常/中断机制
↓
Linux内核软件
```

最终 Linux 知道：

> “刚才请求的数据已经读进 DDR 了。”

然后文件系统可以继续工作。

------

# 二十七、但是再次强调：`open()` 不等于读取文件正文

比如：

```c
fd = open("hello.txt", O_RDONLY);
```

Linux 主要需要知道：

```text
这个文件是谁
在哪里
什么类型
权限如何
如何操作它
```

不一定把：

```text
hello.txt所有内容
```

读进来。

真正文件内容一般等：

```c
read(fd, buf, 100);
```

的时候才读。

所以：

```text
open
主要建立“关系”

read
主要取得“数据”
```

这个区别非常值得记。

------

# 二十八、路径找到之后，Linux 检查权限

假设最终找到：

```text
hello.txt
   ↓
inode
```

里面记录：

```text
-rw-r--r--
UID
GID
...
```

你的进程本身也有身份信息：

```text
当前UID
当前GID
安全凭据
```

于是 Linux 比较：

```text
进程身份
    +
文件权限
    +
open flags
```

例如你要求：

```c
O_RDONLY
```

Linux 判断：

```text
你是否有读取权限？
```

如果没有：

```text
open失败
↓
返回负错误
↓
libc转换
↓
APP看到 -1
errno = EACCES
```

所以：

```c
fd = open(...);

if (fd < 0)
    perror("open");
```

背后就是这样的流程。

------

# 二十九、如果权限通过，就要创建“打开文件对象”

这个地方非常关键。

磁盘上本来存在：

```text
hello.txt
```

但你执行：

```c
open()
```

以后，Linux 还会创建一个：

> **struct file**

可以把它理解成：

> “这一次打开行为，在内核中的运行时对象。”

于是关系变成：

```text
磁盘上的 hello.txt
        │
      inode
        ▲
        │
   struct file
```

`struct file` 里面会保存很多与“本次打开”有关的信息，比如概念上的：

```text
当前文件位置
打开方式
flags
对应inode/dentry
操作函数
私有数据
...
```

------

# 三十、为什么 inode 和 struct file 要分开？

因为一个文件可以被打开很多次。

例如：

```c
fd1 = open("hello.txt", O_RDONLY);
fd2 = open("hello.txt", O_RDONLY);
```

磁盘上还是：

```text
一个 hello.txt
```

但：

```text
第一次打开
↓
struct file A

第二次打开
↓
struct file B
```

可以理解：

```text
        hello.txt
            │
          inode
         ↗     ↖
struct file A  struct file B
```

因为两次打开的：

```text
当前位置
flags
状态
```

可能不同。

------

# 三十一、现在终于轮到 fd 了

Linux 每个进程都有：

> **文件描述符表**

可以把它想成一个数组：

```text
当前进程 fd table

索引          指向

0  ───────→ stdin
1  ───────→ stdout
2  ───────→ stderr
3  ───────→ struct file A
4  ───────→ struct file B
5  ───────→ ...
```

所以如果：

```c
fd = open("hello.txt", O_RDONLY);
```

Linux 发现：

```text
0 被用了
1 被用了
2 被用了

第一个可用：
3
```

于是：

```text
fd = 3
```

并建立：

```text
当前进程
   │
fd table
   │
   ├── 0
   ├── 1
   ├── 2
   └── 3 ─────→ struct file
                         │
                         ▼
                      inode
                         │
                         ▼
                    hello.txt
```

这张关系图非常重要。

------

# 三十二、所以 fd=3 本身不是文件

这个你一定要区分。

```text
fd = 3
```

不是：

```text
文件地址
```

也不是：

```text
inode号
```

也不是：

```text
硬盘扇区号
```

而是：

> **当前进程文件描述符表中的一个索引。**

类似：

```text
fd = 3
   ↓
查当前进程fd表第3项
   ↓
struct file *
```

所以后面：

```c
read(3, buf, 100);
```

Linux 就：

```text
3
↓
当前进程fd表
↓
struct file
↓
知道你想操作哪个文件
```

------

# 三十三、为什么不同进程都可能有 fd=3？

比如进程 A：

```text
fd 3 → hello.txt
```

进程 B：

```text
fd 3 → test.txt
```

完全没问题。

因为：

> **每个进程有自己的 fd 表。**

所以：

```text
Process A
fd=3 → hello.txt

Process B
fd=3 → test.txt
```

fd 没有全系统唯一的意义。

必须加一句：

> **“哪个进程的 fd”**

才有意义。

------

# 三十四、如果打开的是设备文件，会发生什么？

这个跟你以后 Linux 驱动尤其重要。

例如：

```c
fd = open("/dev/led", O_RDWR);
```

路径解析：

```text
/
↓
dev
↓
led
```

找到：

```text
/dev/led
```

如果这是字符设备：

```text
inode
↓
识别字符设备
↓
找到对应驱动
↓
file_operations
```

驱动可能定义：

```c
static struct file_operations led_fops = {
    .open  = led_open,
    .read  = led_read,
    .write = led_write,
};
```

于是：

```c
open("/dev/led", ...)
```

最终可能进一步调用：

```c
led_open(...)
```

这时候驱动自己的 `open` 函数甚至可能：

```text
初始化设备
设置寄存器
打开时钟
申请资源
```

所以对于设备文件：

> **`open()` 本身就有可能间接操作真实硬件。**

这也是普通文件与设备文件一个很有意思的区别。

------

# 三十五、现在内核已经完成 open

假设成功：

```text
路径解析完成
✓

文件存在
✓

权限允许
✓

struct file创建
✓

fd table建立关系
✓

fd = 3
✓
```

Linux 准备返回：

```text
3
```

------

# 三十六、返回值怎么从 Kernel 给 APP？

又回到 CPU 寄存器。

Linux 会把系统调用返回值放在约定的寄存器中。

例如概念上：

```text
r0 = 3
```

然后准备：

```text
返回用户态
```

------

# 三十七、CPU 又发生一次状态切换

之前：

```text
APP
↓
SVC
↓
用户态 → 内核态
```

现在反过来：

```text
Linux处理完成
↓
恢复之前保存的APP现场
↓
恢复用户态执行状态
↓
恢复APP的PC
↓
CPU继续执行APP
```

于是：

```text
内核态
↓
返回异常
↓
用户态
```

CPU 又继续执行你的：

```c
fd = open(...);
```

后面的下一条程序指令。

最终 C 变量：

```c
fd
```

得到：

```text
3
```

------

# 三十八、把整个过程从 CPU 视角看一遍

这是我最建议你建立的认识：

```text
【1】
CPU执行APP
User Mode
用户态

        ↓

【2】
调用 libc open()

CPU仍然执行用户空间代码
仍然用户态

        ↓

【3】
libc准备寄存器参数

pathname
flags
系统调用号

        ↓

【4】
CPU执行 SVC

硬件异常机制启动

        ↓

【5】
CPU：
User Mode → 特权模式

PC跳到Linux SVC入口

        ↓

【6】
CPU开始执行Linux Kernel代码

内核态

        ↓

【7】
Linux系统调用分派

open/openat处理函数

        ↓

【8】
VFS路径解析

/ → home → user → hello.txt

        ↓

【9】
查 dentry / inode cache

如果命中：
直接使用DDR中的缓存

如果没命中：
进入具体文件系统

        ↓

【10】
必要时：

文件系统
↓
块设备层
↓
MMC驱动
↓
USDHC硬件
↓
eMMC/SD

        ↓

【11】
找到inode

检查权限

        ↓

【12】
创建 struct file

        ↓

【13】
放入当前进程 fd table

3 → struct file

        ↓

【14】
返回值放寄存器

r0 = 3

        ↓

【15】
Linux恢复APP现场

        ↓

【16】
CPU：
内核态 → 用户态

        ↓

【17】
继续执行APP

fd = 3
```

------

# 三十九、从软件/硬件层级重新整理一次

你以后学 Linux，一定要有这个分层意识：

```text
────────────────────────────
        应用软件层
────────────────────────────

你的C程序
open()

────────────────────────────
       用户空间库
────────────────────────────

glibc / libc

────────────────────────────
       CPU系统调用机制
────────────────────────────

寄存器
SVC指令
异常机制
特权模式

        ↑ 硬件开始非常明显

────────────────────────────
        Linux Kernel
────────────────────────────

系统调用入口
VFS
dentry
inode
struct file
fd table

────────────────────────────
        文件系统
────────────────────────────

ext4
UBIFS
FAT
...

────────────────────────────
        内核驱动
────────────────────────────

MMC驱动
块设备驱动
MTD驱动
...

────────────────────────────
         SoC硬件
────────────────────────────

ARM Cortex-A7
MMU
GIC
DMA
USDHC控制器
SoC内部总线

────────────────────────────
       外部存储硬件
────────────────────────────

DDR
eMMC
SD Card
NAND Flash
```

------

# 四十、这里还有一个特别重要的认识

你之前说：

> “调用 `open()` 就进入内核。”

现在你应该可以把这句话升级成：

```text
APP调用open()
       ↓
不是APP直接访问内核数据结构
       ↓
而是libc按照系统调用协议
       ↓
让CPU执行SVC
       ↓
ARM CPU通过硬件异常机制
       ↓
从低权限用户模式
切换到高权限内核执行环境
       ↓
CPU开始执行Linux内核代码
       ↓
Linux代表APP访问受保护的系统资源
```

所以“**APP访问内核**”这句话其实不太严谨。

真正准确的是：

> **APP通过系统调用向内核提出请求，CPU切换到内核态后，由内核代码代表该进程完成操作。**

------

# 四十一、再看你原来的流程，就能把每一步扩展开

你原来写的是：

```text
Linux内核
   ↓
解析文件路径
   ↓
找到对应目录
   ↓
找到 hello.txt
   ↓
检查文件是否存在
   ↓
检查访问权限
   ↓
建立内核中的文件管理结构
   ↓
分配文件描述符 fd
   ↓
返回给APP
```

现在应该升级成：

```text
APP调用 open()
        │
        ▼
libc包装函数
        │
        ▼
准备pathname、flags、syscall number
        │
        ▼
ARM执行SVC
        │
        ▼
CPU硬件：
用户态 → 内核态
        │
        ▼
Linux系统调用入口
        │
        ▼
安全取得用户pathname
        │
        ▼
进入VFS
        │
        ▼
/ → home → user → hello.txt
        │
        ▼
查dentry/inode缓存
        │
        ├──命中──→ 直接使用内存信息
        │
        └──未命中
              ↓
          具体文件系统
              ↓
         必要时访问存储
              ↓
         驱动 → 控制器
              ↓
           eMMC/SD
        │
        ▼
获得 hello.txt inode
        │
        ▼
检查UID/GID/权限/flags
        │
        ▼
创建 struct file
        │
        ▼
当前进程fd表：
3 → struct file
        │
        ▼
返回 fd=3
        │
        ▼
CPU：
内核态 → 用户态
        │
        ▼
APP继续执行
```

------

# 最后，用一句特别关键的话把硬件软件串起来

你可以把 `open()` 整个过程理解成：

> **C语言中的 `open()` 只是起点；真正完成文件打开的是 Linux 内核。APP 通过 libc 准备系统调用参数，再利用 ARM CPU 的 `SVC` 异常机制从用户态受控地进入内核态。内核通过 VFS、dentry、inode 等软件结构查找文件；如果所需元数据不在内存缓存中，文件系统再通过设备驱动控制 SoC 中的硬件控制器，必要时从 eMMC、SD 或 NAND 中取得数据。最后 Linux 创建 `struct file`，把它挂到当前进程的文件描述符表中，并把索引值比如 `3` 返回给 APP。**

如果你接下来学习 Linux 驱动，最值得继续往下追的一条链就是：

```text
fd
↓
struct file
↓
inode
↓
file_operations
↓
驱动的 open/read/write
↓
寄存器
↓
硬件
```

**这一条链一旦彻底搞懂，你前面学的“应用程序、系统调用、Linux内核、驱动、硬件”就会真正连成一个整体。**
