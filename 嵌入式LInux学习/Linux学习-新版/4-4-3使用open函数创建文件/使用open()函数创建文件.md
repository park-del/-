# 视频分析：P42 `4_4-3 使用 open 函数创建文件`

这节视频大约 **14 分 15 秒**。它其实是在上一节“用 `open()` 打开已有文件”的基础上继续回答一个新问题：

> **如果文件根本不存在，怎么让 `open()` 顺便把它创建出来？创建出来以后，文件权限又是怎么决定的？**

这一节真正的重点有三个：

**`O_CREAT`、`mode` 权限参数、`umask`。**

而且它和我们刚刚聊的 `fd → struct file → inode` 也能很好地接起来。

------

## 1. 视频类型判断

**主要类型：教程类 / Linux 系统编程讲解**
**次要类型：实验验证类**

判断依据：

1. 在上一节 `open()` 程序基础上直接修改代码。
2. 加入 `O_CREAT` 实际创建不存在的文件。
3. 通过 `ls -l` 观察文件权限。
4. 用八进制权限、`S_IRUSR` 等宏解释 `mode`。
5. 最后重点引出 `umask`，解释为什么你写 `0777`，真正创建出来却不一定是 `0777`。

------

## 2. 一句话总结

这节视频可以压缩成一句：

> **使用 `open(..., O_CREAT, mode)` 创建文件时，`mode` 指定“希望赋予”的权限，但 Linux 还会用当前进程的 `umask` 屏蔽部分权限，所以最终权限约等于 `mode & ~umask`。**

比如：

```c
open("1.txt", O_RDWR | O_CREAT, 0777);
```

如果：

```text
umask = 0002
```

最终：

```text
0777 & ~0002
=
0775
```

所以看到：

```text
-rwxrwxr-x
```

------

## 3. 时间段拆解

| 时间段      | 内容概括                 | 画面/操作                                                  | 作用                                                |
| ----------- | ------------------------ | ---------------------------------------------------------- | --------------------------------------------------- |
| 00:00-00:50 | 回顾 `open()` 原型       | 手册展示两个 `open` 原型                                   | 引出创建文件需要第三个参数                          |
| 00:50-01:50 | 修改上一节程序           | `O_RDWR` 后加入 `O_CREAT`                                  | 让不存在的文件也能被创建                            |
| 01:50-03:20 | 编译、运行、观察创建结果 | `./create 1.txt`、`ls -l`                                  | 发现只加 `O_CREAT` 而没正确指定 mode 会出现异常权限 |
| 03:20-05:45 | 讲解权限表示             | `S_IRWXU`、`S_IRUSR`、`S_IWUSR` 等，以及 owner/group/other | 解释第三个参数 `mode`                               |
| 05:45-08:30 | 修改程序指定权限         | `open(..., O_CREAT                                         | O_TRUNC, 0644)` 等                                  |
| 08:30-10:30 | 继续测试各种权限         | `ls -l`、重新创建文件                                      | 引出“指定权限 ≠ 最终权限”                           |
| 10:30-12:00 | 引出 `umask`             | 用户环境下查看 `umask`，如 `0002`                          | 解释权限为什么会被屏蔽                              |
| 12:00-14:15 | 深入解释 `umask`         | 对比普通用户 `0002`、root `0022`，板书 `mode & ~umask`     | 建立最终权限计算规则                                |

------

## 4. 为什么 `open()` 有两个函数原型？

视频一开始展示：

```c
int open(const char *pathname, int flags);

int open(const char *pathname, int flags, mode_t mode);
```

你可以理解成两种场景。

### 场景一：只是打开已经存在的文件

```c
fd = open("1.txt", O_RDWR);
```

只需要告诉 Linux：

```text
打开哪个文件
+
以什么方式打开
```

例如：

```text
pathname = "1.txt"
flags    = O_RDWR
```

------

### 场景二：文件不存在时还要创建它

```c
fd = open("1.txt",
          O_RDWR | O_CREAT,
          0644);
```

<span style="color:#FF0000; background:#00FF80;">这时候 Linux 多了一个问题：</span>

> <span style="background:#00FF80;">“我要创建一个全新的 inode，那么这个新文件的权限应该是什么？”</span>

所以必须再告诉内核：

```text
mode = 0644
```

因此：

```text
pathname
   │
   ├─ 文件叫什么
   │
flags
   │
   ├─ 怎么打开、是否创建
   │
mode
   │
   └─ 新创建文件希望有什么权限
```

------

## 5. `O_CREAT` 到底是什么意思？

例如：

```c
open("1.txt", O_RDWR | O_CREAT, 0644);
```

`O_CREAT` 的含义是：

> <span style="background:#00FF80;">**如果指定文件不存在，就创建它。**</span>

所以：

```text
open("1.txt", O_RDWR)
```

遇到不存在的文件：

```text
1.txt不存在
↓
open失败
↓
返回 -1
```

而：

```text
open("1.txt", O_RDWR | O_CREAT, 0644)
```

则：

```text
1.txt不存在
↓
发现 O_CREAT
↓
创建 1.txt
↓
再建立 struct file
↓
分配 fd
↓
返回 APP
```

这和你前面学的 `open()` 内核过程可以直接接起来。

------

## 6. 加入 `O_CREAT` 后，内核比原来多做了什么？

以前文件已经存在：

```text
pathname
↓
路径解析
↓
dentry
↓
inode已经存在
↓
检查权限
↓
创建 struct file
↓
fd
```

但现在：

```c
open("1.txt", O_CREAT | ...);
```

而 `1.txt` 不存在：

```text
pathname
↓
找到父目录
↓
发现 1.txt 不存在
↓
flags中存在 O_CREAT
↓
在文件系统中创建新的目录项 dentry
↓
分配新的 inode
↓
设置 inode 权限
↓
建立 struct file
↓
放入当前进程 fd table
↓
返回 fd
```

所以非常重要：

> **`O_CREAT` 创建的不只是一个“文件名”，还涉及文件系统为这个新文件建立 inode 等元数据。**

------

## 7. 视频里有一个特别好的“错误示范”

在大约 **01:50-03:20**，可以看到创建出的文件权限非常奇怪，例如画面中出现了类似：

```text
-r-SrwS--T
```

这种明显不像普通文件的权限组合。

为什么？

因为程序一开始大概只有：

```c
open(argv[1], O_RDWR | O_CREAT);
```

也就是用了：

```text
O_CREAT
```

却没有正确提供第三个：

```text
mode
```

------

## 8. 为什么没写 `mode` 会产生奇怪权限？

这是这一节特别值得理解的地方。

只要使用：

```c
O_CREAT
```

并且真的需要创建新文件，Linux 就需要知道：

> <span style="background:#00FF80;">新文件权限是多少？</span>

正确：

```c
open("1.txt",
     O_RDWR | O_CREAT,
     0644);
```

内核收到：

```text
mode = 0644
```

但是如果你写：

```c
open("1.txt",
     O_RDWR | O_CREAT);
```

就没有正确提供 mode。

那么创建权限就是不可依赖的，可能得到各种意外结果。

所以建议你直接形成条件反射：

```text
看到 O_CREAT
     ↓
立刻检查
     ↓
有没有第三个 mode 参数
```

------

## 9. 为什么是 `0644`，前面那个 `0` 是什么？

这一点非常重要。

Linux 文件权限通常用：

> **八进制**

表示。

所以：

```c
0644
```

<span style="color:#FF0000; background:#00FF80;">前面的 `0` 表明：</span>

> <span style="background:#00FF80;">这是八进制常量。</span>

拆开：

```text
0 6 4 4
  │ │ │
  │ │ └── other
  │ └──── group
  └────── owner
```

其中：

```text
r = 4
w = 2
x = 1
```

所以：

```text
6 = 4 + 2
  = rw-

4 = r--

4 = r--
```

最终：

```text
0644
↓
rw-r--r--
```

------

## 10. 为什么 `r=4、w=2、x=1`？

因为实际是三个二进制位：

```text
r   w   x
│   │   │
4   2   1

100 = 4
010 = 2
001 = 1
```

例如：

```text
rwx
111（二进制）
↓
7（八进制）
rw-
110
↓
6
r-x
101
↓
5
```

因此：

```text
0755
```

等于：

```text
owner : rwx = 7
group : r-x = 5
other : r-x = 5
```

所以：

```text
-rwxr-xr-x
```

------

## 11. 视频里那些 `S_IRUSR` 又是什么？

视频大约 **03:20-05:45** 展示了类似：

```text
S_IRWXU  00700
S_IRUSR  00400
S_IWUSR  00200
S_IXUSR  00100

S_IRWXG  00070
S_IRGRP  00040
S_IWGRP  00020
...
```

这些<span style="background:#00FF80;">实际上就是 Linux 给你准备好的**权限宏**。</span>

比如：

```text
USR = user / owner
GRP = group
OTH = other
```

以及：

```text
R = read
W = write
X = execute
```

所以：

```c
S_IRUSR
```

拆开：

```text
S_I R USR
    │  │
    │  └── owner
    └───── read
```

意思就是：

> owner 有读权限。

------

## 12. 所以这两种写法其实可以表达相同意思

直接写八进制：

```c
open("1.txt",
     O_CREAT | O_RDWR,
     0644);
```

也可以用宏组合：

```c
open("1.txt",
     O_CREAT | O_RDWR,
     S_IRUSR | S_IWUSR |
     S_IRGRP |
     S_IROTH);
```

因为：

```text
S_IRUSR = 0400
S_IWUSR = 0200
S_IRGRP = 0040
S_IROTH = 0004
```

按位或：

```text
0400
0200
0040
0004
────
0644
```

所以<span style="color:#FF0000; background:#00FF80;">两者是等价的权限表达思路。</span>

------

## 13. 视频为什么加入 `O_TRUNC`？

视频代码后来可以看到类似：

```c
fd = open(argv[1],
          O_RDWR | O_CREAT | O_TRUNC,
          0644);
```

`O_TRUNC`：

> <span style="background:#00FF80;">**如果文件已经存在，并且以允许写入的方式打开，就把文件长度截断为 0。**</span>

比如原来：

```text
1.txt

内容：
100ask
```

长度：

```text
7 bytes
```

执行：

```c
open("1.txt",
     O_RDWR | O_TRUNC);
```

之后：

```text
文件仍然存在
但内容长度 → 0
```

所以：

```text
O_CREAT
```

解决：

> 不存在怎么办？

而：

```text
O_TRUNC
```

解决：

> 已经存在时，要不要清空原来的内容？

两者不是一个功能。

------

## 14. `O_CREAT` 也不会每次都“新建一个文件”

例如：

```c
open("1.txt",
     O_RDWR | O_CREAT,
     0644);
```

如果：

```text
1.txt不存在
```

那么：

```text
创建
```

如果：

```text
1.txt已经存在
```

则：

> `O_CREAT` 本身不会再次创建第二个同名文件。

而是<span style="color:#FF0000;">直接打开现有文件。</span>

也就是说第三个参数：

```text
mode
```

主要是在：

> **“真的创建新文件”**

的时候发挥作用。

现有文件原本的权限不会因为你：

```c
open(existing_file, O_CREAT, 0777);
```

就自动被改成 `0777`。

这一点也很容易混。

------

## 15. 接下来就是整节视频最重要的第二部分：`umask`

大约 **10:30 之后**，视频开始重点解释：

```bash
umask
```

例如普通用户环境：

```text
$ umask
0002
```

root 环境：

```text
# umask
0022
```

这东西到底是什么？

你可以把它理解成：

> <span style="background:#00FF80; color:#FF0000;">**“创建新文件时，禁止自动赋予哪些权限”的屏蔽码。**</span>

关键词是：

> **屏蔽。**

------

## 16. `mode` 和 `umask` 是什么关系？

假设程序说：

```c
open(..., 0777);
```

APP 的意思是：

> “我希望新文件拥有 rwxrwxrwx。”

但是 Linux 还会看看：

```text
当前进程的 umask
```

假设：

```text
umask = 0002
```

意思是：

> 屏蔽 other 的写权限。

因此最后：

```text
0777
   &
~0002
```

得到：

```text
0775
```

即：

```text
rwxrwxr-x
```

这正是视频后面实验中看到的结果。

------

## 17. `mode & ~umask` 到底怎么算？

这是整节课最建议你真正理解的公式：

```text
最终权限 = mode & ~umask
```

例如：

```text
mode  = 0777
umask = 0002
```

只看权限 9 位：

```text
mode:

111 111 111
rwx rwx rwx
```

`umask 0002`：

```text
000 000 010
```

取反：

```text
111 111 101
```

然后：

```text
111 111 111
&
111 111 101
───────────
111 111 101
```

就是：

```text
rwx rwx r-x
```

即：

```text
0775
```

------

## 18. 为什么一定要先 `~umask`？

因为 `umask` 表示：

> **哪些权限“不允许有”。**

比如：

```text
umask = 0022
```

意思是禁止：

```text
group 写
other 写
```

不是说：

```text
0022
```

是“最终权限”。

恰恰相反，它代表的是：

> 要去掉哪些位。

所以需要：

```text
~umask
```

变成：

> 哪些位允许保留。

再和 `mode` 做：

```text
&
```

------

## 19. 视频里的普通用户 `0002`

例如：

```text
umask = 0002
```

可以理解成：

```text
owner    group    other

000      000      010

                    │
                    └── 去掉 other 的 write
```

如果：

```text
mode = 0777
```

最终：

```text
0775
```

所以视频中可以看到：

```text
-rwxrwxr-x
```

------

## 20. root 的 `0022`

后面视频又展示：

```text
root
↓
umask
↓
0022
```

这时候：

```text
mode = 0777
```

最终：

```text
0777 & ~0022
=
0755
```

所以：

```text
-rwxr-xr-x
```

视频大约 **13:30** 附近正好展示了这样的权限结果。

------

## 21. 为什么普通用户和 root 的 `umask` 可以不同？

因为：

> **`umask` 属于进程的属性。**

shell 本身就是一个进程。

比如你登录后：

```text
bash
```

拥有：

```text
umask = 0002
```

然后：

```bash
./create
```

这个新进程继承 shell 的 umask。

于是你的：

```text
create进程
```

也是：

```text
umask = 0002
```

而 root shell：

```text
umask = 0022
```

启动程序后：

```text
程序继承 0022
```

所以即使执行完全相同的：

```c
open(..., 0777);
```

最终文件权限也可能不同。

------

## 22. 这和 `open()` 进入内核的过程怎么接起来？

我们之前已经讲：

```text
APP
↓
open()
↓
系统调用
↓
进入内核
↓
VFS
```

现在加上 `O_CREAT` 后，可以把它补完整：

```text
APP

open("1.txt",
     O_RDWR | O_CREAT,
     0777)
        │
        ▼
系统调用进入Linux内核
        │
        ▼
VFS解析路径
        │
        ▼
找到父目录
        │
        ▼
查找 1.txt
        │
        ├─ 已存在
        │     ↓
        │   直接打开
        │
        └─ 不存在
              ↓
          看到 O_CREAT
              ↓
           创建 inode
              │
              │ mode = 0777
              │ umask = 0002
              ▼
        最终权限 = 0775
              │
              ▼
          创建 dentry
              │
              ▼
        建立 struct file
              │
              ▼
        放进当前进程 fdtable
              │
              ▼
            fd = 3
              │
              ▼
           返回 APP
```

这就和你最近问的：

```text
fd table
struct file
inode
```

完全连接起来了。

------

## 23. 为什么权限属于 inode，而不是 `struct file`？

这个也可以顺便帮你把前面的知识串起来。

比如：

```text
hello.txt权限 = rw-r--r--
```

这个属于：

> **文件本身的元数据。**

所以主要在：

```text
inode
```

这一层。

而：

```text
struct file
```

保存的是：

> 这一次打开的状态。

比如：

```text
O_RDWR
O_APPEND
当前文件偏移 f_pos
```

所以：

```text
            struct file
            │
            ├── O_RDWR
            ├── f_pos
            │
            ▼
           inode
            │
            ├── 文件权限
            ├── owner
            ├── group
            ├── 大小
            └── 时间戳
```

这两类“权限/flag”不要混在一起。

------

## 24. 特别容易混淆：`O_RDWR` 和 `0644` 不是一回事

比如：

```c
open("1.txt",
     O_RDWR | O_CREAT,
     0644);
```

这里：

### `O_RDWR`

描述：

> <span style="background:#00FF80;">**这一次打开以后，我希望怎么访问它。**</span>

属于：

```text
open flags
```

------

### `0644`

描述：

> <span style="background:#00FF80;">**如果要新创建文件，新文件本身具有哪些权限。**</span>

属于：

```text
file mode
```

这两个完全不同。

可以这样记：

```text
O_RDWR
↓
“我怎么使用它？”


0644
↓
“新文件允许谁怎么使用它？”
```

这个区别非常重要。

------

## 25. 还有一个容易犯错的地方

很多初学者看到：

```c
open(..., O_RDWR | O_CREAT, 0777);
```

会认为：

> “0777 就表示我自己现在能够读写这个文件。”

其实不完全是这个意思。

`0777` 描述的是：

```text
owner
group
other
```

<span style="color:#FF0000; background:#00FF80;">三类主体对新文件的权限。</span>

而<span style="background:#00FF80;">你这次 `open()` 使用：</span>

```text
O_RDWR
```

是在请求：

> <span style="color:#FF0000; background:#00FF80;">当前这个打开实例既能读又能写。</span>

<span style="color:#FF0000; background:#00FF80;">它们属于不同层级。</span>

------

## 26. 内容结构分析

视频整体路径很清晰：

```text
上一节已经会 open 已有文件
              ↓
如果文件不存在怎么办？
              ↓
加入 O_CREAT
              ↓
发现创建权限异常
              ↓
引出 mode 第三个参数
              ↓
解释 rwx 和八进制
              ↓
实际指定 0644 / 0777
              ↓
发现最终权限仍可能不一样
              ↓
引出 umask
              ↓
mode & ~umask
```

所以这一节其实是层层制造问题，再解释原因。

------

## 27. 观点分析

这是纯技术教程，没有明显立场型观点。

明确技术结论包括：

| 技术内容                             | 明确/可能 | 依据                   |
| ------------------------------------ | --------- | ---------------------- |
| `O_CREAT` 可在文件不存在时创建文件   | 明确      | 代码和实验             |
| 使用 `O_CREAT` 创建文件时需提供 mode | 明确      | 权限异常实验及后续修改 |
| mode 可以使用八进制或权限宏表示      | 明确      | `S_IRUSR` 等讲解       |
| `O_TRUNC` 可把已有文件长度截为 0     | 明确      | 程序中加入 flag        |
| 最终创建权限会受到 `umask` 影响      | 明确      | 普通用户/root实验      |
| 权限计算可理解为 `mode & ~umask`     | 明确      | 视频最后板书           |

------

## 28. Who / Whom / Whose / When / Where / What / Why / How

| 维度  | 视频中的答案                                          |
| ----- | ----------------------------------------------------- |
| Who   | 用户态应用程序和 Linux 内核                           |
| Whom  | 应用程序请求内核创建/打开文件                         |
| Whose | mode 是新文件的权限请求；umask 是当前进程的权限屏蔽值 |
| When  | 使用 `O_CREAT` 且目标文件不存在时创建                 |
| Where | 用户空间 → 系统调用 → VFS/文件系统                    |
| What  | 创建文件并指定权限                                    |
| Why   | 新建 inode 时必须确定文件权限                         |
| How   | `O_CREAT + mode`，再由 `mode & ~umask` 得出最终权限   |

------

## 29. 这节最值得反复看的三个片段

### 片段一：约 01:00-03:20

```c
open(argv[1], O_RDWR | O_CREAT);
```

创建出了权限很奇怪的文件。

这个片段很重要，因为它告诉你：

> **`O_CREAT` 不是简单加个 flag 就完事了。**

创建新文件还要考虑：

```text
mode
```

------

### 片段二：约 03:20-05:45

重点讲：

```text
owner
group
other

r w x
4 2 1
```

以及：

```text
S_IRUSR
S_IWUSR
S_IXUSR
...
```

这是理解：

```text
0644
0755
0777
```

的基础。

------

### 片段三：约 10:30-14:15

整节视频最重要的部分：

```text
umask = 0002 / 0022
```

以及：

```text
final mode = mode & ~umask
```

如果这一段没彻底懂，后面经常会困惑：

> “我明明写 0777，为什么 `ls -l` 出来的不是 777？”

------

## 30. 这节课你最需要掌握的 7 个问题

学完以后你应该能自己回答：

```text
1. O_CREAT 到底有什么作用？

2. 为什么用了 O_CREAT 以后需要第三个 mode 参数？

3. 0644、0755、0777 分别是什么意思？

4. rwx 为什么对应 4、2、1？

5. O_RDWR 和 0644 为什么是完全不同的东西？

6. umask 到底是在“加权限”还是“去权限”？

7. 为什么 open(..., 0777) 最后可能得到 0775 或 0755？
```

------

## 31. 最终记忆版

```text
创建文件：

open("1.txt",
     O_RDWR | O_CREAT,
     0666);


O_CREAT：
不存在就创建


第三个参数 mode：
新文件希望拥有的权限


八进制权限：

r = 4
w = 2
x = 1

0644
=
rw-r--r--


但是最终权限还要经过：

umask


最终权限：

mode & ~umask


例如：

mode  = 0777
umask = 0002

最终：
0775
=
rwxrwxr-x


如果：

umask = 0022

则：

0777 → 0755
=
rwxr-xr-x
```

## 一句话记忆

> <span style="background:#00FF80; color:#FF0000;">**`O_CREAT` 决定“要不要创建”，`mode` 决定“希望新文件有什么权限”，`umask` 决定“其中哪些权限必须被拿掉”。**</span>

而从内核结构来看，这一节和我们前面讨论的内容可以最终串成：

```text
open(path, O_CREAT, mode)
          ↓
进入内核
          ↓
路径查找
          ↓
文件不存在
          ↓
创建 dentry + inode
          ↓
inode 权限 = mode & ~umask
          ↓
创建 struct file
          ↓
fdtable[fd] → struct file
          ↓
返回 fd 给 APP
```

这条链已经把 **`open()`、文件权限、inode、`struct file`、fd 表** 基本全部连起来了。
