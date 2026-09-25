# 视频分析：P44 `4_4-5 使用 read 函数读文件`

这节视频约 **11 分 19 秒**。它和上一节 `write()` 基本是一对：<span style="background:#00FF80;">上一节是“用户空间 → 内核 → 文件”，这一节则重点讲 **文件 → 内核 → 用户空间 `buf`**。</span>

这节最值得你抓住的其实有 4 个点：

> **`read()` 的三个参数、返回值三种情况、文件偏移 `f_pos` 会自动后移、`read()` 读出的内容不是 C 字符串所以要自己补 `'\0'`。**

------

## 1. 视频类型判断

**主要类型：教程类 / Linux 文件 I/O 实操**
**次要类型：讲解类 / 错误处理实验**

判断依据：

1. 先通过 `man 2 read` 查看 `read()` 原型和说明。
2. 在上一节程序基础上编写读取文件代码。
3. 讲解连续 `read()` 时文件当前位置如何变化。
4. 处理 `read()` 的 `<0、=0、>0` 三种返回情况。
5. 实际读取文本文件并制造“文件不存在”等错误进行验证。

------

## 2. 一句话总结

> <span style="background:#00FF80; color:#FF0000;">**`read(fd, buf, count)` 从 fd 对应文件的当前偏移 `f_pos` 开始，最多读取 `count` 个字节到用户空间的 `buf` 中；成功读取后 `f_pos` 自动后移，返回值表示实际读到多少字节，返回 0 表示普通文件已经读到 EOF。**</span>

------

## 3. 时间段拆解

| 时间段      | 内容概括                                 | 主要作用                                       |
| ----------- | ---------------------------------------- | ---------------------------------------------- |
| 00:00-01:20 | 回顾 `read/write/lseek` 等函数原型       | 进入 `read()` 主题                             |
| 01:20-03:10 | 编写读取程序，使用 `open(..., O_RDONLY)` | 建立“先 open，再 read”的基本流程               |
| 03:10-04:10 | 查看 `man 2 read`                        | 理解参数、返回值和文件偏移                     |
| 04:10-05:20 | 板书连续两次 `read()` 与 `pos` 的变化    | 解释 `f_pos` 自动向后移动                      |
| 05:20-07:00 | 分析 `read()` 返回值                     | 区分失败、读到数据、EOF                        |
| 07:00-09:40 | 完善循环读取、字符串结束符处理           | 正确打印每次读取的数据                         |
| 09:40-10:40 | 编译并构造测试文件                       | 实际验证读取程序                               |
| 10:40-11:19 | 测试不存在文件和权限等情况               | 展示错误处理，同时暴露代码中一个值得注意的问题 |

------

## 4. `read()` 到底是什么？

函数：

```c
ssize_t read(int fd, void *buf, size_t count);
```

你可以直接翻译成人话：

```text
read(
    从哪个已打开文件读,
    读出来放在哪里,
    最多读多少字节
)
```

也就是：

```text
read(fd, buf, count)
      │    │      │
      │    │      └── 最多读取多少字节
      │    │
      │    └────────── 用户空间缓冲区
      │
      └─────────────── 文件描述符
```

例如：

```c
char buf[100];

len = read(fd, buf, 99);
```

意思是：

> <span style="background:#00FF80; color:#FF0000;">从 `fd` 对应的文件当前位置开始，最多读取 99 字节，放进 `buf`。</span>

------

## 5. 这里的数据方向正好和 `write()` 相反

上一节：

```c
write(fd, buf, 100);
```

是：

```text
用户空间 buf
      ↓
   write()
      ↓
Linux Kernel
      ↓
Page Cache / 文件系统
```

这一节：

```c
read(fd, buf, 100);
```

是反过来：

```text
文件 / Page Cache
      ↓
Linux Kernel
      ↓
   read()
      ↓
用户空间 buf
```

所以<span style="color:#FF0000; background:#00FF80;">你最近问的用户空间与内核空间关系，可以直接用这两个函数对照：</span>

```text
write：

用户buf ─────────→ 内核


read：

用户buf ←───────── 内核
```

<span style="color:#FF0000; border:1px solid #330000; background:#00FF80;">当然这里所谓“内核”，不是这个进程有一块专属 Page Cache，而是 Linux 内核统一管理文件数据。</span>

------

## 6. 从你前面学的 `fd` 开始看，`read()` 怎么找到文件？

假设：

```c
int fd = open("1.txt", O_RDONLY);
```

返回：

```text
fd = 3
```

然后：

```c
read(3, buf, 99);
```

内核大概沿着：

```text
当前进程
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
   ↓
dentry / inode
   ↓
文件系统
```

找到：

> “你说的 fd 3 到底是哪个文件，以及现在读到哪里了。”

所以这节课实际上是在继续使用我们前面讨论的：

```text
fd
 ↓
struct file
 ↓
f_pos
```

------

## 7. 视频板书里的 `pos` 就是你前面学的 `f_pos`

视频大约 **04:10-05:20** 画了一条文件位置线。

假设刚：

```c
open(...)
```

通常：

```text
f_pos = 0
```

第一次：

```c
read(fd, buf, N);
```

假设真的读到 N 字节：

```text
原来：

0
↓
┌────────────────────────────┐
│          文件              │
└────────────────────────────┘

read N字节

0               N
│───────────────│
                ↑
              新f_pos
```

所以：

```text
f_pos：
0 → N
```

再：

```c
read(fd, buf, M);
```

则从 N 开始：

```text
0              N              N+M
│──────────────│───────────────│
               ↑               ↑
             开始读          读完后
```

最后：

```text
f_pos = N + M
```

所以：

> <span style="color:#FF0000; background:#00FF80;">**连续调用 `read()` 不会每次重新从文件头开始，而是接着上次的位置继续读。**</span>

------

## 8. 为什么 `read()` 会知道“上次读到哪里”？

因为这个状态就保存在：

```text
struct file
```

对应的：

```text
f_pos
```

里面。

所以：

```text
read(fd, ...)
      ↓
找到struct file
      ↓
读取当前f_pos
      ↓
从f_pos读取
      ↓
实际读len字节
      ↓
f_pos += len
```

这和上一节 `write()` 完全一样：

```text
read  → 使用并推进 f_pos
write → 使用并推进 f_pos
lseek → 主动修改 f_pos
```

这三个函数现在可以放在一起理解了。

------

## 9. `read()` 的返回值特别重要

假设：

```c
len = read(fd, buf, 99);
```

这里有 **3 种情况**。

### `len > 0`

表示：

> <span style="color:#FF0000; background:#00FF80;">成功读到了 `len` 个字节。</span>

例如：

```text
len = 25
```

说明：

```text
buf[0]
...
buf[24]
```

这 25 个字节有效。

同时：

```text
f_pos += 25
```

------

### `len == 0`

对于普通文件通常表示：

> <span style="color:#FF0000; background:#00FF80;">**已经到达文件末尾 EOF。**</span>

例如文件只有 150 字节。

程序每次最多读 99：

```text
第一次：

read
↓
99字节
↓
len = 99
↓
f_pos = 99


第二次：

read
↓
剩下51字节
↓
len = 51
↓
f_pos = 150


第三次：

read
↓
已经没有数据
↓
len = 0
```

这时候：

```c
if (len == 0)
    break;
```

循环结束。

------

### `len < 0`

表示：

> <span style="color:#FF0000; background:#00FF80;">`read()` 发生错误。</span>

例如：

```c
if (len < 0)
{
    perror("read");
}
```

可能是：

```text
Bad file descriptor
I/O error
...
```

具体看 `errno`。

------

## 10. 一个很容易误解的地方：`count` 不是“保证读这么多”

比如：

```c
read(fd, buf, 99);
```

`99` 的意思是：

> <span style="color:#FF0000; background:#00FF80;">**最多读 99 字节。**</span>

<span style="color:#FF0000; background:#00FF80;">不是：</span>

> 必须读满 99 字节才返回。

所以完全可能：

```text
请求：

99

实际：

35

返回：

35
```

比如文件就只剩 35 字节。

所以：

```text
count
=
最大请求长度


返回值 len
=
这一次实际得到的数据长度
```

一定要分开。

------

## 11. 为什么视频用 `sizeof(buf)-1`？

视频代码后面是：

```c
unsigned char buf[100];

len = read(fd, buf, sizeof(buf)-1);
```

也就是：

```text
buf总共100字节

read最多只使用99字节
```

故意留下：

```text
1 byte
```

干什么？

给：

```c
'\0'
```

用。

------

## 12. 为什么一定要自己加 `'\0'`？

这是这节视频特别值得理解的一点。

`read()` 的工作是：

> <span style="color:#FF0000; background:#00FF80;">**读取原始字节。**</span>

它<span style="color:#FF0000; background:#00FF80;">不知道你是不是想把这些数据当：</span>

```text
C字符串
```

使用。

<span style="background:#00FF80;">假设文件：</span>

```text
hello
```

<span style="color:#FF0000; background:#00FF80;">实际读回来可能只是：</span>

```text
buf[0] = 'h'
buf[1] = 'e'
buf[2] = 'l'
buf[3] = 'l'
buf[4] = 'o'
```

`read()` 不会替你自动做：

```c
buf[5] = '\0';
```

<span style="color:#FF0000; background:#00FF80;">但是：</span>

```c
printf("%s", buf);
```

<span style="color:#FF0000; background:#00FF80;">要求：</span>

> `buf` 必须是以 `'\0'` 结尾的 C 字符串。

所以代码必须：

```c
buf[len] = '\0';
```

然后才能：

```c
printf("%s", buf);
```

------

## 13. 所以视频最终代码为什么是这样？

大概：

```c
unsigned char buf[100];

while (1)
{
    len = read(fd, buf, sizeof(buf)-1);

    if (len < 0)
    {
        perror("read");
        close(fd);
        return -1;
    }
    else if (len == 0)
    {
        break;
    }
    else
    {
        buf[len] = '\0';
        printf("%s", buf);
    }
}
```

这里的逻辑特别漂亮：

```text
read
 ↓
len < 0 ?
 ├─ 是 → 错误
 │
 └─ 否
     ↓
   len == 0 ?
     ├─ 是 → EOF，结束
     │
     └─ 否 → 真正读到了len字节
              ↓
          buf[len] = '\0'
              ↓
           printf
```

------

## 14. 为什么不是 `buf[len-1] = '\0'`？

视频中间编辑代码的时候一度出现过：

```c
buf[len-1] = '\0';
```

随后改成了正确的：

```c
buf[len] = '\0';
```

这个细节非常值得你注意。

假设：

```text
len = 5
```

那么真正有效数据是：

```text
buf[0]
buf[1]
buf[2]
buf[3]
buf[4]
```

所以：

```text
buf[5]
```

才应该放：

```text
'\0'
```

如果写：

```c
buf[len-1] = '\0';
```

就是：

```c
buf[4] = '\0';
```

<span style="color:#FF0000; background:#00FF80;">会把最后一个真正读到的字符直接覆盖掉。</span>

例如：

```text
hello
```

会变成：

```text
hell\0
```

少一个 `o`。

------

## 15. 这也解释了为什么最多只读 99 个

因为：

```text
buf[100]
```

合法下标：

```text
0 ~ 99
```

如果：

```c
read(fd, buf, 100);
```

刚好返回：

```text
len = 100
```

你再：

```c
buf[len] = '\0';
```

实际上就是：

```c
buf[100] = '\0';
```

越界了。

所以正确思路：

```c
read(fd, buf, sizeof(buf)-1);
```

最多：

```text
len = 99
```

那么：

```c
buf[99] = '\0';
```

合法。

因此：

```text
100字节buf

99字节
→ 放真正数据

1字节
→ 给'\0'
```

非常典型。

------

## 16. <span style="background:#00FF80;">但如果不当字符串打印，就不需要 `'\0'`</span>

这个是视频基础之上我建议你补充的。

<span style="background:#00FF80; color:#FF0000;">例如你读取：</span>

```text
图片
音频
二进制程序
结构体
压缩包
```

数据中间本来就可能存在：

```text
0x00
```

这时候根本不应该使用：

```c
printf("%s", buf);
```

而应该按照：

```text
read返回了多少字节
```

处理多少字节。

例如：

```c
len = read(fd, buf, sizeof(buf));

write(STDOUT_FILENO, buf, len);
```

这里就可以把 100 字节全部拿来读。

所以：

> <span style="background:#00FF80;">`sizeof(buf)-1 + buf[len]='\0'` 是为了方便**把读取结果当字符串打印**，不是 `read()` 自己要求必须这样做。</span>

这个区别很重要。

------

## 17. 从你刚刚学的 Page Cache 看 `read()`

这正好和上一轮问题完全接起来。

假设：

```c
read(fd, buf, 100);
```

第一种情况：

### <span style="background:#00FF80; color:#FF0000;">数据已经在 Page Cache</span>

```text
磁盘
      （这次可能完全不访问）

Linux Page Cache
       │
       │ 数据已经存在
       ▼
用户空间 buf
```

所以：

```text
read()
≠
每次都去读取eMMC
```

和 `write()` 一样。

------

## 18. 如果 Page Cache 没有呢？

那么大致：

```text
APP
 │
 │ read
 ▼
Linux内核
 │
 ▼
发现Page Cache没有需要的数据
 │
 ▼
文件系统
 │
 ▼
块设备层
 │
 ▼
MMC/设备驱动
 │
 ▼
eMMC / SD / SSD
 │
 │ 读取
 ▼
物理RAM中的Page Cache
 │
 ▼
复制给用户空间buf
```

<span style="color:#FF0000; background:#00FF80;">所以可以把 `read()` 的数据链理解成：</span>

```text
存储设备
↓
Page Cache
↓
用户空间 buf
```

------

## 19. `read()` 为什么还要从 Page Cache 复制到用户 buf？

因为：

```text
Page Cache
```

属于：

> <span style="color:#FF0000; background:#00FF80;">Linux 内核管理的内存。</span>

你的：

```c
char buf[100];
```

属于：

> <span style="color:#FF0000; background:#00FF80;">当前进程的用户虚拟地址空间。</span>

<span style="color:#FF0000; background:#00FF80;">不能简单让用户程序随便访问内核 Page Cache。</span>

<span style="color:#FF0000; background:#00FF80;">所以通常需要：</span>

```text
内核Page Cache
      ↓
安全复制
      ↓
用户空间buf
```

大方向上你可以理解成：

```text
read = 内核 → 用户
write = 用户 → 内核
```

------

## 20. 和 `write()` 放一起就特别清楚

现在你可以画：

```text
                  用户空间

                 APP buf
                    ▲
                    │
         read()     │     write()
                    │
                    ▼

════════════ 用户态 / 内核态 ════════════

                Page Cache
                    ▲
                    │
                    ▼

                 文件系统
                    │
                    ▼
                块设备层
                    │
                    ▼
                  驱动
                    │
                    ▼

                eMMC / SSD
```

### `read`

```text
Page Cache → 用户buf
```

### `write`

```text
用户buf → Page Cache
```

这个模型值得你直接记住。

------

## 21. 视频结尾有一个非常值得注意的代码问题

视频最后测试：

```bash
./read 1111.txt
```

文件不存在。

终端先出现：

```text
can not open file 1111.txt
errno = 2
No such file or directory
```

这是合理的。

但是后面又出现：

```text
read: Bad file descriptor
```

为什么？

因为从视频代码结构来看，`open()` 失败以后虽然打印了错误，**但当时没有立即退出程序**。

于是：

```text
open失败
↓
fd = -1
```

程序仍然继续：

```c
read(fd, buf, ...);
```

其实相当于：

```c
read(-1, buf, ...);
```

于是 Linux 又告诉你：

```text
Bad file descriptor
```

所以更合理的代码应该：

```c
fd = open(argv[1], O_RDONLY);

if (fd < 0)
{
    perror("open");
    return -1;
}
```

不要再继续进入 `read()` 循环。

这是这个视频结尾一个特别好的“反向教学点”。

------

## 22. `O_RDONLY` 也可以从英文理解

视频中：

```c
fd = open(argv[1], O_RDONLY);
```

就是：

```text
O_
→ open flag

RD
→ Read

ONLY
→ Only
```

所以：

```text
O_RDONLY
=
Read Only
=
只读方式打开
```

因为这一节只需要：

```c
read()
```

不需要修改文件。

------

## 23. 视频结构

整体路径：

```text
认识 read()
      ↓
open(O_RDONLY)
      ↓
read(fd, buf, count)
      ↓
理解f_pos变化
      ↓
理解返回值
      ↓
循环读取
      ↓
给文本补 '\0'
      ↓
printf显示
      ↓
遇到len==0结束
      ↓
实际测试
      ↓
错误场景验证
```

------

## 24. 观点分析

这是技术教学，没有争议性价值观点。

明确技术结论主要是：

| 技术内容                         | 明确/可能 | 依据                     |
| -------------------------------- | --------- | ------------------------ |
| `read()` 最多读取 `count` 个字节 | 明确      | man 手册                 |
| 返回值是实际读取字节数           | 明确      | 手册和程序               |
| 对普通文件，返回 0 表示 EOF      | 明确      | 代码循环                 |
| 成功读取后文件偏移向后移动       | 明确      | 板书                     |
| `read()` 不自动追加字符串结束符  | 明确      | 程序手工 `buf[len]='\0'` |
| 失败返回负值并设置 errno         | 明确      | 错误实验                 |

------

## 25. Who / Whom / Whose / When / Where / What / Why / How

| 维度  | 视频中的答案                                           |
| ----- | ------------------------------------------------------ |
| Who   | 用户态应用程序                                         |
| Whom  | 向 Linux 内核请求读取 fd 对应文件                      |
| Whose | `f_pos` 属于这次打开对应的 `struct file`               |
| When  | 调用 `read()` 时                                       |
| Where | 文件系统/Page Cache → Linux 内核 → 用户空间 buf        |
| What  | 从文件读取最多 count 个字节                            |
| Why   | APP 需要取得文件内容                                   |
| How   | `fd → struct file → f_pos → Page Cache/文件系统 → buf` |

------

## 26. 最值得看的 4 个片段

### ① 约 03:10-04:10：`man 2 read`

重点看：

```c
ssize_t read(int fd, void *buf, size_t count);
```

以及：

> “up to count bytes”

也就是：

**最多 count，不保证一定 count。**

------

### ② 约 04:10-05:20：板书 `pos`

这是整节理论核心：

```text
第一次read：
pos → pos+N

第二次read：
pos+N → pos+N+M
```

正好对应：

```text
struct file->f_pos
```

------

### ③ 约 08:30-09:40：`buf[len] = '\0'`

这个特别关键：

```c
read(fd, buf, sizeof(buf)-1);

buf[len] = '\0';

printf("%s", buf);
```

它解释了：

> **原始字节流和 C 字符串并不是一回事。**

------

### ④ 约 10:40-11:19：错误测试

```text
open失败
↓
fd=-1
↓
又调用read
↓
Bad file descriptor
```

可以顺便培养一个系统编程习惯：

> **前一步系统调用失败，就不要拿无效结果继续往下用。**

------

## 27. 这节最需要你掌握的 8 个问题

学完以后你最好能自己回答：

1. `read(fd, buf, count)` 三个参数分别是什么？
2. `count=100` 是否保证一定读到 100 字节？
3. `read()` 返回 50 表示什么？
4. `read()` 返回 0 为什么不是“失败”？
5. `read()` 返回负数表示什么？
6. 连续调用 `read()` 为什么不会一直从文件头开始？
7. `buf[len]='\0'` 为什么必须自己写？
8. `read()` 每次都会访问 eMMC 吗？

其中第 6、8 个和你最近讨论的 **`struct file`、Page Cache、内核空间**关系最深。

------

## 28. 记忆版总结

```text
read：

ssize_t read(fd, buf, count)


fd
↓
指定哪个打开文件

buf
↓
用户空间接收缓冲区

count
↓
最多读取多少字节


返回：

> 0
读到了这么多字节

= 0
普通文件EOF

< 0
读取失败


文件位置：

fd
↓
fdtable
↓
struct file
↓
f_pos
↓
read从这里开始
↓
实际读len字节
↓
f_pos += len


文本打印：

read(..., sizeof(buf)-1)

↓
buf[0 ... len-1] = 有效数据

↓
buf[len] = '\0'

↓
printf("%s", buf)
```

## 一句话记忆

> <span style="background:#00FF80; color:#FF0000;">**`read()` 就是“根据 fd 找到 `struct file`，从它当前的 `f_pos` 位置取数据，优先从 Linux Page Cache 得到文件内容，再把实际读到的字节复制到用户空间 `buf`，最后把 `f_pos` 向后推进”。**</span>

你最近几节已经可以完整串成：

```text
open()
↓
fd
↓
struct file
├── flags
└── f_pos
      │
      ├── read()  → 文件/Page Cache → 用户buf
      │
      ├── write() → 用户buf → Page Cache/文件
      │
      └── lseek() → 修改f_pos
↓
inode
↓
文件系统
↓
Page Cache
↓
必要时访问eMMC / SSD
```

这张图其实已经把 **应用程序、系统调用、fd、内核对象、文件系统、RAM缓存和存储硬件** 全串起来了。



