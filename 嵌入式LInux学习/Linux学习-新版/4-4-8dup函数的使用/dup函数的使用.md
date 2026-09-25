# 视频分析：P47《4_4-8 dup函数的使用》

这节大约 **20分54秒**。它正好接着你上一节学的：

```text
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
```

这一节真正要回答的问题是：

> <span style="background:#00FF80;">**“复制一个文件描述符，到底复制了什么？为什么两个 fd 会共享同一个文件偏移？`dup2()` 又为什么可以实现标准输出重定向？”**</span>

------

## 1. 视频类型判断

**主要类型：讲解类 / Linux 文件 I/O 机制**

**次要类型：源码机制分析 + 编程实验**

判断依据：

1. 先从 `struct file` 中的 `f_pos` 出发解释文件偏移。
2. 对比“同一个文件 `open()` 两次”和“使用 `dup()` 复制 fd”。
3. 用 `task_struct → files_struct → fdtable → struct file` 解释底层。
4. 最后<span style="background:#00FF80; color:#FF0000;">通过 `dup2(fd, 1)` 演示标准输出重定向。</span>

------

## 2. 一句话总结

这个视频最核心的一句话是：

> <span style="color:#FF0000; background:#00FF80;">**`dup()` 不是重新 `open()` 一次文件，而是在当前进程的 fd 表中增加一个新的 fd，让它和原 fd 指向同一个 `struct file`；因此两个 fd 会共享同一个 `f_pos`。**</span>

也就是：

```text
open两次同一个文件：

fd=3 ─→ struct file A ─→ f_pos A
fd=4 ─→ struct file B ─→ f_pos B

两个偏移独立
```

而：

```text
fd=3
fd2=dup(fd)

fd=3 ─┐
       ├─→ 同一个 struct file ─→ 同一个 f_pos
fd=4 ─┘
```

这就是整节课的灵魂。

------

## 3. 时间段拆解

| 时间段      | 内容概括                        | 画面/操作                            | 作用                      |
| ----------- | ------------------------------- | ------------------------------------ | ------------------------- |
| 00:00-01:30 | 介绍 `dup/dup2/dup3`            | 查看函数说明和原型                   | 建立接口概念              |
| 01:30-03:20 | 回顾 `struct file` 和 `f_pos`   | 查看内核 `struct file` 源码          | 为共享偏移铺垫            |
| 03:20-07:50 | 同一个文件 `open()` 两次        | 板书两个 `struct file`，编写实验程序 | 证明两次 open 的偏移独立  |
| 07:50-11:30 | 引入 `dup()`                    | 画 `fdtable → struct file` 关系      | 解释 dup 的底层本质       |
| 11:30-13:30 | 编写并运行 dup 实验             | `fd=3, fd2=4, fd3=5`，读出 `1、1、2` | 实验证明 dup 共享 `f_pos` |
| 13:30-16:00 | 引入标准 fd 0/1/2               | `stdin/stdout/stderr` 板书           | 为重定向准备              |
| 16:00-20:40 | 使用 `dup2(fd,1)` 重定向 stdout | 打开文件、替换 fd 1、`printf()`      | 解释输出重定向            |
| 20:40-20:54 | 回到函数说明                    | 总结 dup/dup2/dup3                   | 收束                      |

时间是根据视频画面阶段划分，可能有几十秒误差。

------

## 4. 第一部分最关键：两次 `open()` 为什么不是共享 `f_pos`？

视频先写类似：

```c
fd  = open("1.txt", O_RDONLY);
fd2 = open("1.txt", O_RDONLY);
```

假设：

```text
fd  = 3
fd2 = 4
```

<span style="color:#FF0000;">很多人第一反应可能是：</span>

> <span style="background:#00FF80; color:#FF0000;">都是 `1.txt`，那应该共用一个文件位置吧？</span>

<span style="color:#FF0000;">其实不是。</span>

<span style="color:#FF0000; background:#00FF80;">通常关系是：</span>

```text
fdtable

3 ─→ struct file A
       └─ f_pos = 0

4 ─→ struct file B
       └─ f_pos = 0
```

虽然<span style="background:#00FF80;">最终两个 `struct file` 都可能关联到：</span>

```text
同一个 inode
↓
同一个 1.txt
```

但是：

> <span style="color:#FF0000; background:#00FF80;">**每次独立 `open()` 通常会建立独立的 open file description，也就是 Linux 中对应一个独立的 `struct file`。**</span>

所以：

```c
read(fd, buf, 1);
```

以后：

```text
struct file A:
f_pos = 1
```

但是：

```text
struct file B:
f_pos仍然 = 0
```

所以再：

```c
read(fd2, buf2, 1);
```

还是从文件第一个字节开始。

------

## 5. 视频实验为什么会读出 `1` 和 `1`？

文件内容类似：

```text
123
```

第一次：

```c
read(fd, buf, 1);
```

读：

```text
1
```

然后：

```text
fd对应f_pos:
0 → 1
```

但 `fd2` 是第二次独立 `open()` 得到的：

```text
fd2对应f_pos仍然 = 0
```

于是：

```c
read(fd2, buf2, 1);
```

仍然读：

```text
1
```

所以：

```text
fd 读到：1
fd2读到：1
```

这一步是在给后面的 `dup()` 做对比。

------

## 6. `dup(fd)` 到底做了什么？

例如：

```c
int fd3 = dup(fd);
```

假设：

```text
fd = 3
```

而：

```text
0、1、2 已经占用
3 已占用
4 也可能已占用
```

那么 `dup()` 会寻找：

> <span style="color:#FF0000; background:#00FF80;">**当前进程中最小的、尚未使用的 fd。**</span>

视频实验里可以看到类似：

```text
fd  = 3
fd2 = 4
fd3 = 5
```

<span style="color:#FF0000; background:#00FF80;">但是最重要的不是 `5`。</span>

而是：

```text
fdtable：

3 ──────────┐
             │
             ▼
          struct file A
             │
             └── f_pos
             ▲
             │
5 ──────────┘
```

所以：

```text
fd=3
fd3=5
```

<span style="color:#FF0000; background:#00FF80;">是两个不同的整数，</span>

但：

> <span style="color:#FF0000; background:#00FF80;">**它们指向同一个 `struct file`。**</span>

------

## 7. 所以 `dup()` “复制”的到底是什么？

这句话一定要说准确。

它不是：

```text
复制磁盘文件
```

也不是：

```text
重新创建一个完全独立的 struct file
```

而更接近：

> <span style="color:#FF0000; background:#00FF80;">**复制一个文件描述符引用。**</span>

也就是在：

```text
fdtable
```

<span style="color:#FF0000; background:#00FF80;">中增加一项：</span>

```text
原来：

3 → struct file A
```

执行：

```c
dup(3);
```

以后：

```text
3 → struct file A
5 → struct file A
```

<span style="color:#FF0000; background:#00FF80;">Linux 也会维护这个 `struct file` 的引用计数，所以两个 fd 都引用它时，不会因为关闭其中一个 fd 就马上销毁这个打开文件对象。</span>

------

## 8. 为什么 `dup()` 出来的 fd 会<span style="color:#FF0000; background:#00FF80;">共享文件偏移？</span>

因为：

```text
f_pos
```

<span style="color:#FF0000; background:#00FF80;">不是放在：</span>

```text
fdtable[3]
fdtable[5]
```

里面。

<span style="color:#FF0000; background:#00FF80;">它放在：</span>

```c
struct file
```

里面。

所以：

```text
fd 3 ─┐
       ▼
    struct file
       │
       └─ f_pos
       ▲
fd 5 ─┘
```

<span style="color:#FF0000; background:#00FF80;">两个 fd 最后访问的是：</span>

> <span style="color:#FF0000; background:#00FF80;">**同一个 `f_pos`。**</span>

这就解释视频里的实验结果。

------

## 9. 视频中的 `1、1、2` 是怎么来的？

程序大致做：

```c
int fd  = open(argv[1], O_RDONLY);
int fd2 = open(argv[1], O_RDONLY);
int fd3 = dup(fd);

read(fd,  buf,  1);
read(fd2, buf2, 1);
read(fd3, buf3, 1);
```

文件：

```text
123
```

第一次：

```c
read(fd, ...)
```

因为：

```text
fd → struct file A
f_pos = 0
```

读到：

```text
1
```

然后：

```text
f_pos A = 1
```

------

第二次：

```c
read(fd2, ...)
```

因为：

```text
fd2 → struct file B
f_pos B = 0
```

所以也读：

```text
1
```

然后：

```text
f_pos B = 1
```

------

第三次：

```c
read(fd3, ...)
```

但是：

```text
fd3
↓
还是 struct file A
↓
它的 f_pos 已经 = 1
```

所以从第二个字符开始：

```text
2
```

最终实验：

```text
data get from fd  : 1
data get from fd2 : 1
data get from fd3 : 2
```

这三个数字其实已经把 `dup()` 的本质证明得非常漂亮了。

------

## 10. 和你上一节学的结构完全连起来

你刚刚才问：

```text
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
```

现在 `dup()` 就可以直接画进去。

执行：

```c
fd3 = dup(fd);
```

本质上是在：

```text
current
↓
task_struct
↓
files_struct
↓
fdtable
```

这里增加新的 fd 映射：

```text
fdtable

0 → file *
1 → file *
2 → file *

3 ────────┐
           ▼
       struct file
           │
           └── f_pos
           ▲
5 ────────┘
```

所以：

> <span style="color:#FF0000; background:#00FF80;">`dup()` 主要操作的是 **文件描述符表这一层**，而不是重新从文件系统路径查找并建立一次独立打开。</span>

------

## 11. `dup2()` 和 `dup()` 最大区别

`dup()`：

```c
newfd = dup(oldfd);
```

你只告诉它：

```text
我要复制oldfd
```

至于新 fd 是多少：

> 内核自动选择最小空闲 fd。

------

而：

```c
dup2(oldfd, newfd);
```

你是在说：

> <span style="background:#00FF80; color:#FF0000;">**我就要让 `newfd` 变成 `oldfd` 的副本。**</span>

例如视频：

```c
dup2(fd, 1);
```

<span style="color:#FF0000; background:#00FF80;">含义：</span>

```text
让 fd=1
↓
改成和 fd 指向同一个打开文件对象
```

------

## 12. `dup2(fd,1)` 为什么特别厉害？

因为 Linux 中：

```text
fd 0 = stdin
fd 1 = stdout
fd 2 = stderr
```

原本：

```text
fdtable

1 ─→ 终端/console对应的 struct file
```

假设：

```c
fd = open("result.txt",
          O_RDWR | O_CREAT | O_TRUNC,
          0777);
```

得到：

```text
fd = 3
```

此时：

```text
3 ─→ result.txt对应struct file
```

然后：

```c
dup2(3, 1);
```

就变成：

```text
fdtable：

1 ─┐
   ├─→ result.txt对应struct file
3 ─┘
```

<span style="color:#FF0000; background:#00FF80;">于是：</span>

```text
stdout
↓
fd 1
↓
不再去终端
↓
去 result.txt
```

这就叫：

> <span style="color:#FF0000; background:#00FF80;">**标准输出重定向。**</span>

------

## 13. `dup2()` 如果 newfd 原来已经被占用了怎么办？

例如：

```c
dup2(3, 1);
```

但 `1` 原本已经是：

```text
stdout → terminal
```

`dup2()` 的语义大致是：

```text
如果 oldfd != newfd：

先关闭newfd原来的关联
↓
再让newfd引用oldfd所引用的打开文件对象
```

所以：

```text
原来：

1 → terminal
3 → result.txt
```

以后：

```text
1 → result.txt
3 → result.txt
```

视频板书里就在强调这个替换过程。

<span style="color:#FF0000;">实际 `dup2()` 对“关闭旧 `newfd` 并重新绑定”的处理是原子化设计的，避免中间窗口被别的线程/信号抢占 fd，这一点以后学多线程再深入。</span>

------

## 14. 为什么 `printf()` 也会被重定向？

例如视频里：

```c
dup2(fd, 1);

printf("hello, world\n");
```

你可能会觉得：

> `printf()` 又没有传 fd，怎么知道要写文件？

因为：

```text
printf()
↓
stdout
↓
最终输出到底层 fd 1
```

而你已经：

```c
dup2(fd, 1);
```

把：

```text
fd 1
```

改成指向文件了。

<span style="color:#FF0000; background:#00FF80;">所以：</span>

```text
printf
↓
stdout
↓
fd=1
↓
struct file
↓
result.txt
```

<span style="color:#FF0000; background:#00FF80;">最终 `"hello, world"` 就进入文件，而不是终端。</span>

------

## 15. 一个很重要的细节：<span style="background:#00FF80;">`dup2()` 改的是 fd，不是 `printf()` 函数</span>

不要理解成：

```text
dup2
↓
修改printf
```

实际是：

```text
printf逻辑不变
```

变化的是：

```text
fdtable[1]
```

原来：

```text
fdtable[1]
↓
terminal
```

现在：

```text
fdtable[1]
↓
文件
```

所以所有最终往 fd 1 写数据的东西都会跟着改变。

这就是“重定向”特别漂亮的地方：

> **上层程序根本不需要知道输出设备换了。**

------

## 16.<span style="background:#00FF80; color:#FF0000;"> 这和 Shell 的 `>` 是同一类思想</span>

你<span style="color:#FF0000; background:#00FF80;">以后执行：</span>

```bash
./hello > result.txt
```

<span style="color:#FF0000; background:#00FF80;">程序里面：</span>

```c
printf("hello\n");
```

<span style="color:#FF0000;">并没有修改。</span>

Shell 在启动程序之前，就可以利用类似：

```text
open()
dup2()
```

的机制，把：

```text
stdout
fd 1
```

连接到：

```text
result.txt
```

所以你的程序感觉：

```text
“我还是正常往stdout输出。”
```

实际：

```text
stdout → 文件
```

这就是 Linux Unix I/O 设计非常重要的一点。

------

## 17. `dup3()` 是什么？

视频开头的资料还列了：

```c
int dup3(int oldfd, int newfd, int flags);
```

它和 `dup2()` 很像，但增加了：

```text
flags
```

常见可以使用：

```c
O_CLOEXEC
```

用于控制：

```text
exec()以后是否自动关闭这个fd
```

现在你知道它存在即可。

可以先记：

```text
dup
→ 内核帮你选newfd

dup2
→ 你自己指定newfd

dup3
→ 类似dup2，再加flags控制
```

------

## 18. 一个容易混淆的点：dup 共享什么、不共享什么？

最核心共享：

```text
同一个 open file description
↓
Linux中对应同一个 struct file
```

所以共享：

```text
f_pos
文件状态 flags 等打开文件状态
```

但：

```text
fd 3
fd 5
```

毕竟仍然是两个不同的 fdtable 项。

因此“文件描述符本身的一些属性”可以分别存在。

学习阶段最重要的仍然是：

> **dup 后共享 `struct file` 和 `f_pos`。**

------

### 19. 内容结构

```text
开头：
介绍dup系列接口

↓
发展：
从struct file的f_pos说明
“两次open为什么独立”

↓
核心转折：
引入dup
“两个fd竟然可以指向同一个struct file”

↓
实验：
读出1、1、2验证共享偏移

↓
应用：
dup2(fd,1)

↓
结尾：
解释标准输出重定向
```

整体表达路径：

> 从【文件偏移 `f_pos`】开始，通过【两次 open 与 dup 的对比】解释 fd 和 `struct file` 的关系，最后落到【利用 `dup2` 实现标准输出重定向】。

------

## 20. 观点分析

这节不是评论视频，因此没有价值观点，主要是技术结论。

| 技术结论                                       | 明确/可能 | 依据                    |
| ---------------------------------------------- | --------- | ----------------------- |
| 独立 `open()` 同一文件通常得到独立打开文件对象 | 明确      | 两个 `struct file` 板书 |
| 独立 open 的 `f_pos` 相互独立                  | 明确      | `1、1` 实验             |
| `dup()` 创建新 fd，但引用同一个打开文件对象    | 明确      | fdtable 图              |
| dup 出来的 fd 共享 `f_pos`                     | 明确      | 第三个读取结果为 `2`    |
| `dup()` 默认使用最小空闲 fd                    | 明确      | 函数说明                |
| `dup2()` 可以指定目标 fd                       | 明确      | `dup2(fd,1)`            |
| 修改 fd 1 的映射可以改变标准输出方向           | 明确      | `printf()` 重定向实验   |

------

## 21. Who / Whom / Whose / When / Where / What / Why / How

| 维度  | 视频中的答案                                                 |
| ----- | ------------------------------------------------------------ |
| Who   | 当前 Linux 进程及其 fdtable                                  |
| Whom  | 对某个已有 fd 进行复制                                       |
| Whose | fd 属于当前进程的文件描述符表；`f_pos` 属于对应 `struct file` |
| When  | 调用 `dup/dup2` 时                                           |
| Where | Linux 内核的文件描述符管理体系中                             |
| What  | 新增/替换一个 fd 到 `struct file` 的映射                     |
| Why   | 让多个 fd 共享一个打开文件状态，或进行 I/O 重定向            |
| How   | 修改 fdtable，使不同 fd 指向同一 `struct file`               |

------

## 22. 最值得你精读的三个片段

### 片段一：约 03:20-07:50

重点看：

```text
open("1.txt")
open("1.txt")
```

为什么对应：

```text
两个struct file
两个f_pos
```

这决定你是否真正理解“inode”和“open file description”的区别。

------

### 片段二：约 08:00-13:20

这是整节最重要。

```c
fd3 = dup(fd);
```

之后：

```text
fd
 \
  → struct file → f_pos
 /
fd3
```

然后实验得到：

```text
1
1
2
```

这三个数字建议直接记下来。

------

### 片段三：约 15:00-20:40

```c
dup2(fd, 1);
printf("hello, world\n");
```

这里真正展示了：

> **“文件描述符只是一个可重新连接的接口编号。”**

这比单纯记 dup 函数更加重要。

------

## 23. 你现在应该形成的完整 fd 模型

结合 P46 + P47：

```text
current
↓
task_struct
↓
files_struct
↓
fdtable
│
├─ fd 0 ─→ struct file
├─ fd 1 ─→ struct file
├─ fd 2 ─→ struct file
│
├─ fd 3 ──────┐
│             ▼
│         struct file
│             │
│             ├─ f_pos
│             ├─ flags
│             └─ path
│             ▲
└─ fd 5 ──────┘
```

这里：

```text
fd 3
fd 5
```

虽然号码不同，

但是：

```text
同一个struct file
↓
同一个f_pos
```

------

## 24. 记忆版总结

```text
类型：
Linux文件描述符机制 / dup

主线：
open两次 vs dup一次

核心观点：
dup不是重新open，
而是让新fd引用同一个struct file

独立open：
fd3 → file A → f_pos A
fd4 → file B → f_pos B

dup：
fd3 ─┐
      ├→ file A → 同一个f_pos
fd5 ─┘

实验：
文件123

read(fd)  → 1
read(fd2) → 1
read(fd3) → 2

dup：
自动找最小空闲fd

dup2：
可以指定newfd

dup2(fd,1)：
把stdout重新指向文件

printf：
仍然写stdout
但stdout的fd=1已经指向文件
所以输出进入文件
```

### 一句话记忆

> **`open()` 决定“创建一个新的打开文件状态”，而 `dup()` 决定“再给已有的打开文件状态增加一个 fd 入口”；因此 dup 出来的 fd 会共享同一个 `struct file` 和 `f_pos`，而 `dup2()` 进一步允许你指定这个新入口的号码，于是可以用 `dup2(fd, 1)` 把标准输出重定向到文件。**





# 补充：close文件的操作

可以，而且 `close()` 正好能和你刚学的 `dup()`、`fdtable`、`struct file` 串起来。<span style="color:#FF0000;">它真正做的事情不是“把磁盘上的文件删掉”</span>，而是：

> <span style="background:#00FF80; color:#FF0000;">**关闭当前进程里的某个文件描述符 fd，解除 `fd → struct file` 的引用关系；如果这是最后一个引用，这个打开文件对象才会真正被释放。**</span>

先看最简单的：

```c
int fd = open("a.txt", O_RDONLY);

/* 使用文件 */

close(fd);
```

可以理解成：

```text
open()

fdtable
3 ─────→ struct file
             │
             ├─ f_pos
             ├─ flags
             ├─ path
             └─ ...
```

执行：

```c
close(3);
```

以后：

```text
fdtable
3 ─────→ 空闲
```

也就是：

> <span style="color:#FF0000; background:#00FF80;">**fd=3 这个编号被释放，以后可以再次被 `open()`、`dup()` 等操作重新利用。**</span>

------

## 1. `close(fd)` 最先做的事情：找到 fd 对应的文件对象

比如：

```c
close(3);
```

<span style="color:#FF0000; background:#00FF80;">内核大致会沿着：</span>

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
struct file *
```

找到：

```text
fd 3
到底指向哪个 struct file
```

如果 fd 根本无效，比如：

```c
close(100);
```

<span style="color:#FF0000; background:#00FF80;">而 100 没有打开：</span>

```text
return -1
errno = EBADF
```

意思就是：

> Bad File Descriptor。

------

## 2. 然后把 fdtable 里的这个入口清掉

原来：

```text
fdtable

0 → stdin
1 → stdout
2 → stderr
3 → struct file A
4 → struct file B
```

执行：

```c
close(3);
```

<span style="color:#FF0000; background:#00FF80;">以后：</span>

```text
fdtable

0 → stdin
1 → stdout
2 → stderr
3 → 空
4 → struct file B
```

所以：

```text
fd 3
```

<span style="color:#FF0000; background:#00FF80;">以后不能再拿来：</span>

```c
read(3, ...)
write(3, ...)
```

<span style="color:#FF0000; background:#00FF80;">除非后来又有别的 `open()` 恰好重新分配到 3。</span>

------

## 3. <span style="background:#00FF80; color:#FF0000;">但是 `struct file` 不一定立刻被释放</span>

这里<span style="color:#FF0000; background:#00FF80;">正好和 `dup()` 有关系。</span>

假设：

```c
int fd1 = open("a.txt", O_RDONLY);   // fd1 = 3
int fd2 = dup(fd1);                  // fd2 = 4
```

关系：

```text
fdtable

3 ─────┐
       │
       ▼
   struct file A
       ▲
       │
4 ─────┘
```

也就是说：

```text
fd 3
fd 4
```

都指向同一个：

```text
struct file A
```

现在执行：

```c
close(fd1);
```

也就是：

```c
close(3);
```

结果：

```text
3 → 空

4 ─────→ struct file A
```

所以：

> **struct file A 不能被释放。**

因为 fd 4 还在使用它。

------

## 4. Linux 会维护引用计数

你可以先把 `struct file` 想成有一个：

```text
reference count
```

比如：

```text
fd 3 ─┐
      ├→ struct file
fd 4 ─┘

引用数 = 2
```

执行：

```c
close(3);
```

以后：

```text
引用数：
2 → 1
```

所以：

```text
struct file
仍然存在
```

再：

```c
close(4);
```

以后：

```text
引用数：
1 → 0
```

<span style="color:#FF0000; background:#00FF80;">这时候才意味着：</span>

> <span style="color:#FF0000; background:#00FF80;">没有文件描述符再引用这个打开文件对象了。</span>

<span style="color:#FF0000; background:#00FF80;">于是内核可以进行最终释放。</span>

------

## 5. <span style="color:#FF0000; background:#00FF80;">最后一次 `close()` 会发生什么？</span>

当这个 `struct file` 的最后一个引用消失时，内核会做类似：

```text
最后一个fd关闭
↓
引用计数到0
↓
执行文件对象释放流程
↓
文件系统/驱动相关 release
↓
释放 struct file
```

如果这是普通文件：

```text
VFS
↓
filesystem
↓
释放打开状态
```

如果这是设备文件：

```c
open("/dev/xxx", ...)
```

那么最后释放时还可能最终调用驱动里的：

```c
.release
```

回调。

所以以后学驱动时，你会看到：

```c
struct file_operations {
    .open    = xxx_open,
    .read    = xxx_read,
    .write   = xxx_write,
    .release = xxx_release,
};
```

这里：

```text
.release
```

就和“最后关闭这个打开实例”密切相关。

------

## 6. `close()` 不等于删除文件

比如：

```c
fd = open("a.txt", O_RDONLY);
close(fd);
```

只是：

```text
你这个进程
不再通过这个fd访问文件
```

<span style="color:#FF0000; background:#00FF80;">磁盘上的：</span>

```text
a.txt
```

<span style="color:#FF0000; background:#00FF80;">仍然存在。</span>

<span style="color:#FF0000; background:#00FF80;">删除文件是：</span>

```c
unlink("a.txt");
```

<span style="color:#FF0000; background:#00FF80;">之类的操作。</span>

所以：

```text
close
≠
delete
```

这个一定要分开。

------

## 7. `close()` 也不等于“强制把数据写入硬盘”

这个和我们之前讲的 Page Cache 很重要。

假设：

```c
write(fd, buf, 100);
close(fd);
```

很多人会理解成：

```text
write
↓
Page Cache
↓
close
↓
一定写进eMMC
```

这个理解不准确。

`close()` 的主要语义是：

> **释放文件描述符以及对应引用。**

它通常不等价于：

```c
fsync(fd);
```

如果你特别要求：

> “这个文件的数据现在就尽量同步到稳定存储”

应该在 close 前：

```c
fsync(fd);
close(fd);
```

所以：

```text
write()
→ 数据可能先进入Page Cache

close()
→ 关闭fd

fsync()
→ 请求同步文件数据/必要元数据到稳定存储
```

三者职责不同。

------

## 8. 那 `close()` 以后 `f_pos` 怎么了？

如果这是最后一个引用：

```text
fd 3
↓
struct file
↓
f_pos = 500
```

执行：

```c
close(3);
```

引用数变成 0：

```text
struct file被释放
```

那么：

```text
f_pos = 500
```

这个“本次打开状态”也就随 `struct file` 一起没了。

以后重新：

```c
open("a.txt", O_RDONLY);
```

这是一个新的：

```text
struct file
```

新的：

```text
f_pos
```

通常从：

```text
0
```

开始。

也就是：

```text
第一次open
→ struct file A
→ f_pos = 500
→ close
→ A释放


第二次open
→ struct file B
→ f_pos = 0
```

------

## 9. 但是 dup 后 close 一个 fd，`f_pos` 不会消失

比如：

```c
fd1 = open("a.txt", O_RDONLY);
fd2 = dup(fd1);
```

然后：

```c
read(fd1, buf, 100);
```

于是：

```text
struct file
f_pos = 100
```

再：

```c
close(fd1);
```

现在：

```text
fd1 → 被关闭

fd2 → 仍然指向同一个struct file
```

所以：

```text
f_pos仍然 = 100
```

接下来：

```c
read(fd2, buf, 100);
```

会继续从：

```text
offset 100
```

开始。

这就是：

> **close 的是 fd，不是一定马上销毁整个 `struct file`。**

------

## 10. `close()` 和 fd 重用也很重要

比如：

```c
fd1 = open("a.txt", O_RDONLY);
```

得到：

```text
fd1 = 3
```

然后：

```c
close(fd1);
```

于是：

```text
3空出来了
```

接下来：

```c
fd2 = open("b.txt", O_RDONLY);
```

很可能：

```text
fd2 = 3
```

所以：

```text
fd编号不是永久身份
```

它只是：

> 当前进程 fdtable 中一个当前可用的索引。

这也是为什么：

```c
close(fd);
```

以后最好不要继续使用旧 fd 变量。

因为：

```text
这个数字以后可能已经代表另一个文件
```

------

## 11. 和 `dup2()` 一起看会更清楚

比如：

```c
dup2(fd, 1);
```

如果：

```text
fd 1
```

原来已经指向 terminal，

那么 `dup2()` 会把：

```text
fd 1原来的关联
```

关闭/替换掉，再让：

```text
1 → oldfd对应的struct file
```

所以它内部也涉及类似：

```text
解除旧fd引用
↓
减少旧struct file引用
↓
建立新引用
```

这就是为什么你刚学的 `dup()` 和 `close()` 是一对特别重要的知识点。

------

## 12. 整条路径你可以这样记

执行：

```c
close(fd);
```

概念上：

```text
用户空间

close(fd)
↓
libc
↓
系统调用
↓
SVC
════════════════════
Linux Kernel
↓
current
↓
task_struct
↓
files_struct
↓
fdtable
↓
找到 fdtable[fd]
↓
得到 struct file *
↓
把这个fd表项清掉
↓
降低struct file引用计数
↓
如果还有引用
    → struct file继续存在

如果引用变成0
    → 最终释放文件对象
    → 必要时调用文件系统/驱动release
```

------

### 最后一句话记忆

> **`close(fd)` 的核心不是“关闭磁盘上的文件”，而是“让当前进程的这个 fd 不再引用对应的 `struct file`”。如果还有其他 fd（例如 `dup()` 出来的 fd）引用同一个 `struct file`，文件打开状态仍然存在；只有最后一个引用消失后，这个 `struct file` 才会最终被释放。**

然后再额外记住：

```text
close()
≠ unlink()     // 不是删除文件

close()
≠ fsync()      // 不保证等价于强制落盘
```

这样 `open → dup → read/write/lseek → close` 这整套 fd 生命周期就基本串起来了。
