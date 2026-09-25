# 视频分析

这节视频不长，大约 **2分42秒**。它其实是在正式讲 Makefile 语法之前，先回答一个特别关键的问题：

> **我们写 Makefile，最终到底想让它帮我们做到什么？**

答案可以先记成一句：

<span style="background:#00FF80;">**只重新编译“因为文件发生变化而受到影响”的部分，而不是每次把整个工程全部重新编译。**</span>

这正好承接你前面学的 **gcc 编译、`.c/.cpp → .o → 可执行文件`**，所以这节很重要。

------

## 1. 视频类型判断

**主要类型：讲解类 / 教程类**

**次要类型：原理铺垫类 / 示例演示类**

判断依据：

1. 前面直接提出“为什么需要 Makefile”“怎样高效编译程序”。
2. 老师没有马上堆 Makefile 语法，而是先用 **Visual Studio** 演示理想的编译效果。
3. 核心目的，是让你理解后面<span style="color:#FF0000; background:#00FF80;">为什么要写“目标、依赖、命令”。</span>

------

## 2. 一句话总结

这个视频主要讲的是：

> <span style="background:#00FF80;">**Makefile 的目标，是建立文件之间的依赖关系，让 make 知道“哪个文件变了以后，究竟应该重新编译哪些东西”，从而避免无意义的全部重新编译。**</span>

------

## 3. 时间段拆解

| 时间段          | 内容概括                    | 画面/操作                                                    | 作用                                                         |
| --------------- | --------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| **00:00-00:28** | 提出为什么需要 Makefile     | 展示《Makefile 的使用》，出现“怎么高效地编译程序”“参考 Visual Studio”等内容 | 提出核心问题                                                 |
| **00:28-00:45** | 准备用 Visual Studio 举例   | 启动 Visual Studio 2019                                      | 从理论转入演示                                               |
| **00:45-01:01** | 打开示例工程                | 打开一个已有 VS 工程                                         | 搭建演示环境                                                 |
| **01:01-01:25** | 查看工程源文件关系          | 可以看到 `main.cpp`、`sub.cpp`、`sub.h` 等文件               | 建立“多个源文件+头文件”的场景                                |
| **01:25-01:40** | 编译工程                    | 点击生成/编译，可以看到编译输出                              | 展示第一次完整构建                                           |
| **01:40-02:28** | 修改/查看不同文件后再次构建 | 在 `sub.cpp`、`sub.h`、`main.cpp` 等文件之间切换，再使用“生成” | 演示增量编译思想                                             |
| **02:28-02:42** | 返回文档总结目标            | 再次出现<span style="color:#FF0000; background:#00FF80;">“修改源文件或头文件，只需要重新编译牵涉到的文件”</span> | <span style="color:#FF0000; background:#00FF80;">强化 Makefile 的最终目标</span> |

这里最值得看的其实不是 Visual Studio 本身，而是 **01:01-02:28** 这一整段演示。

------

## 4. 这节真正想让你理解什么？

视频中的例子大概可以抽象成这样：

```text
main.cpp
   │
   ↓
main.o
   │
   ├─────────┐
             ↓
sub.cpp → sub.o → 最终程序
   ↑
 sub.h
```

更准确一点，可以写成依赖关系：

```text
main.o 依赖 main.cpp

sub.o 依赖 sub.cpp、sub.h

最终程序 app 依赖：
main.o、sub.o
```

这里就是<span style="color:#FF0000; background:#00FF80;">整个 Makefile 最核心的思想</span>。

------

## 5. 举个视频里的场景，你就很好理解了

比如工程里有：

```text
main.cpp
sub.cpp
sub.h
```

其中画面里能看到，`sub.cpp` 使用了：

```cpp
#include "sub.h"
```

而 `sub.h` 里面定义了一些内容，比如类似：

```cpp
#define A 1
```

那么关系就是：

```text
sub.h
  ↓
sub.cpp
  ↓
sub.o
  ↓
程序
```

------

### 情况一：我只改了 `main.cpp`

那么：

```text
main.cpp ★发生变化
   ↓
重新生成 main.o
   ↓
main.o + 原来的 sub.o
   ↓
重新链接
   ↓
新的程序
```

<span style="background:#00FF80;">**sub.cpp 没变，就没必要重新编译 sub.cpp。**</span>

------

### 情况二：我只改了 `sub.cpp`

```text
sub.cpp ★
   ↓
重新生成 sub.o
   ↓
原来的 main.o + 新 sub.o
   ↓
重新链接
```

这时候：

```text
main.cpp
```

根本没变化。

所以：

> 没必要重新编译 `main.cpp`。

------

### 情况三：我修改了 `sub.h`

这个稍微重要一点。

因为：

```cpp
sub.cpp
```

包含了：

```cpp
#include "sub.h"
```

所以虽然：

```text
sub.cpp 本身没有改
```

但是它依赖的：

```text
sub.h
```

改了。

因此：

```text
sub.h ★
 ↓
影响 sub.cpp 的编译结果
 ↓
重新生成 sub.o
 ↓
重新链接
```

这<span style="background:#00FF80;">就是 Makefile 里的“**依赖关系**”。</span>

------

## 6. 为什么老师要拿 Visual Studio 来讲？

这个设计其实挺好的。

因为你<span style="color:#FF0000; background:#00FF80;">平时在 Visual Studio 里面按：</span>

```text
生成
```

你<span style="color:#FF0000;">一般不会手动想：</span>

```text
先编译 main.cpp
再编译 sub.cpp
再……
最后链接……
```

<span style="color:#FF0000; background:#00FF80;">VS 自动替你判断了。</span>

例如：

```text
第一次编译：

main.cpp → main.obj
sub.cpp  → sub.obj
             ↓
          main.exe
```

然后<span style="color:#FF0000; background:#00FF80;">你只改：</span>

```text
sub.cpp
```

<span style="color:#FF0000; background:#00FF80;">第二次生成可能就变成：</span>

```text
sub.cpp → sub.obj
             ↓
main.obj + sub.obj
             ↓
          main.exe
```

注意：

```text
main.cpp
   ×
不重新编译
```

<span style="background:#00FF80; color:#FF0000;">因为没必要。</span>

而 **<span style="background:#00FF80;">Linux 下 make + Makefile 想做的事情，本质上就很像这个。</span>**

------

## 7. 所以 Makefile 到底在解决什么问题？

这是这节最核心的地方。

假设以后你的工程有：

```text
main.c
a.c
b.c
c.c
d.c
e.c
...
1000 个 .c 文件
```

<span style="color:#FF0000; background:#00FF80;">如果没有正确管理依赖，每次你只改一行：</span>

```c
a.c
```

<span style="background:#00FF80;">然后又：</span>

```text
1000 个文件全部重新编译
```

那<span style="background:#00FF80; color:#FF0000;">会非常浪费时间。</span>

所以<span style="background:#00FF80; color:#FF0000;">理想情况应该是：</span>

```text
a.c 改了
 ↓
只重新生成 a.o
 ↓
然后重新链接
```

也就是：

```text
改了谁
   ↓
判断谁受影响
   ↓
只重新生成这些文件
```

这就是所谓：

> <span style="background:#00FF80;">**增量编译。**</span>

------

## 8. 这里其实已经埋下了 Makefile 最重要的三个概念

后面你马上就会看到类似：

```makefile
target: prerequisites
	command
```

你现在先不要把它当成“奇怪的 Makefile 语法”。

实际上它就是在描述：

```text
我想得到谁？
target
目标

它依赖谁？
prerequisites
依赖

如果需要更新，怎么得到它？
command
命令
```

例如：

```makefile
sub.o: sub.cpp sub.h
	g++ -c sub.cpp
```

翻译成人话：

> <span style="background:#00FF80;">**我要生成 `sub.o`。它依赖 `sub.cpp` 和 `sub.h`。只要这些依赖比 `sub.o` 更新，就执行下面的编译命令。**</span>

一下就通了。

------

## 9. 和你前面学习的 GCC 正好能串起来

你前面一直在学：

```text
源文件
 ↓
预处理
 ↓
编译
 ↓
汇编
 ↓
.o
 ↓
链接
 ↓
可执行程序
```

<span style="color:#FF0000; background:#00FF80;">Makefile 并没有替代 gcc。</span>

这一点非常重要。

关系其实是：

```text
                Makefile
                   │
          描述“谁依赖谁”
                   │
                   ↓
                  make
                   │
        判断现在需要做哪些工作
                   │
          ┌────────┴────────┐
          ↓                 ↓
        gcc/g++             ld
        编译               链接
          ↓                 ↓
         .o ───────────→ 可执行程序
```

所以：

### <span style="color:#FF0000;">gcc 是“干活的人”</span>

比如：

```bash
gcc -c add.c -o add.o
```

------

### <span style="color:#FF0000;">Makefile 是“工作说明书”</span>

它告诉 make：

```text
add.o 依赖 add.c
```

以及：

```text
需要更新 add.o 时执行什么命令
```

------

### <span style="color:#FF0000;">make 是“调度员”</span>

make 看着 Makefile 判断：

```text
add.c 有变化吗？

有
↓
重新编译 add.o

没有
↓
不编译
```

------

## 10. 内容结构

### 1. 开头

提出：

```text
为什么需要 Makefile？
怎样高效编译程序？
```

------

### 2. 发展

拿 Visual Studio 做参照。

------

### 3. 核心演示

展示一个由：

```text
main.cpp
sub.cpp
sub.h
```

组成的小工程，并进行生成操作。

------

### 4. 结尾

重新强调：

> 修改源文件或者头文件后，只重新编译受到影响的文件。

整体表达路径：

```text
从【为什么需要 Makefile】
        ↓
通过【Visual Studio 的自动/增量编译】
        ↓
说明【只编译真正受影响的文件】
        ↓
最后引出【Makefile 必须描述文件依赖关系】
```

------

## 11. 观点分析

这类视频没有社会意义上的“立场”，但有明确的教学结论。

| 观点                                                     | 归属         | 明确/可能 | 依据                                          |
| -------------------------------------------------------- | ------------ | --------- | --------------------------------------------- |
| 大型程序不应该每次全部重新编译                           | 作者/讲解者  | 明确      | 开头围绕“高效编译”展开                        |
| 修改文件以后，只需要重新编译受影响的文件                 | 作者/讲解者  | 明确      | 文档中明确强调                                |
| Visual Studio 的构建行为可以作为 Makefile 目标效果的参照 | 作者/讲解者  | 明确      | 专门打开 VS 演示                              |
| Makefile 的核心之一是维护依赖关系                        | 作者教学意图 | 明确      | 后续规则 `target: prerequisites` 正是为此服务 |

### 核心观点

```text
Makefile 不是为了“少敲几条 gcc 命令”这么简单。

更重要的是：
让 make 能根据依赖关系判断哪些文件需要重新生成，
从而实现高效的增量构建。
```

这一点你最好重点记住。

------

## 12. Who / Whom / Whose / When / Where / What / Why / How

| 维度      | 视频中的答案                                                 |
| --------- | ------------------------------------------------------------ |
| **Who**   | Makefile / make，以及演示中的程序工程                        |
| **Whom**  | 主要针对多文件 C/C++ 工程                                    |
| **Whose** | 文件依赖关系属于各个目标文件                                 |
| **When**  | 当源文件或头文件被修改后                                     |
| **Where** | C/C++ 工程构建过程中                                         |
| **What**  | 判断哪些文件需要重新编译                                     |
| **Why**   | 避免每次全部重新编译，提高编译效率                           |
| **How**   | 描述目标、依赖关系、生成命令，再由 make 判断文件是否需要更新 |

------

## 13. 重点片段精读

### 片段一：00:00-00:28

**表面发生了什么：**

老师提出“为什么需要 Makefile”，并指出目标是高效编译。

**更深层：**

这实际上告诉你：

```text
这一章的核心不是背 Makefile 语法，
而是学会描述工程的依赖关系。
```

------

### 片段二：01:01-01:25

**表面发生了什么：**

展示：

```text
main.cpp
sub.cpp
sub.h
```

等工程文件。

**更深层：**

老师是在故意构造：

```text
源文件 ↔ 头文件 ↔ 目标文件
```

之间的依赖关系。

这正是<span style="color:#FF0000; background:#00FF80;">后面 Makefile 要描述的东西。</span>

------

### 片段三：01:25-02:28

**表面发生了什么：**

执行生成，并查看/修改工程文件。

**更深层：**

你真正应该观察的是：

> <span style="background:#00FF80;">**哪个文件改变之后，Visual Studio 到底重新处理了哪些文件？**</span>

而不是去研究 VS 菜单怎么点。

------

## 14. 你看这节视频时最应该盯住什么？

这一节属于“**讲原理，不是讲语法**”。

所以先不要急着背：

```makefile
$@
$<
$^
%.o
wildcard
patsubst
```

那些是后面的事情。

你这一节只需要真正弄懂下面这条链：

```text
文件发生变化
      ↓
make 根据依赖关系判断
      ↓
哪些目标已经过期？
      ↓
执行相应 gcc 命令
      ↓
只生成必要的 .o
      ↓
重新链接
      ↓
得到新的程序
```

如果这条链完全理解了，后面看到：

```makefile
main.o: main.c
	gcc -c main.c

sub.o: sub.c sub.h
	gcc -c sub.c
```

你就不会觉得 Makefile 是在背语法了。

------

## 15. 记忆版总结

```text
类型：Makefile 原理铺垫

主线：
为什么需要 Makefile
→ Visual Studio 演示
→ 增量编译
→ 引出依赖关系

核心观点：
只重新编译真正受到修改影响的文件。

关键概念：
目标 target
依赖 prerequisites
命令 command

最重要时间：
01:01-02:28

一句话记忆：

Makefile 告诉 make“谁依赖谁、怎么生成谁”，
make 再根据文件是否发生变化决定哪些 gcc 命令真正需要执行。
```

尤其建议你把这一节和你前面问过的 **gcc、编译器、链接器、`.o`、`.a`、`.so`** 串起来理解<span style="background:#00FF80;">：**Makefile 本身不负责“编译”，它更像整个编译/链接过程的调度规则。**</span>

