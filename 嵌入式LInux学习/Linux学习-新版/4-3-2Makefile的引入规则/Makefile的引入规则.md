# 视频分析：4_3-2《Makefile 的引入》

这节比上一节更关键。上一节只是告诉你：**Makefile 要实现“只编译受影响的文件”**；这一节开始真正回答：

> **make 到底凭什么知道哪个文件要重新编译？Makefile 又该怎么写？**

这节视频约 **20分31秒**，核心其实只有一条规则，但老师是从 gcc 的实际编译过程一步一步推出来的。

------

## 1. 视频类型判断

**主要类型：讲解类 / 教程类**

**次要类型：原理分析类 / 命令演示类**

判断依据：

1. 从 `gcc -o test a.c b.c` 的问题开始分析。

2. 把 `.c → .o → 可执行程序` 的过程重新拆开。

3. 最后<span style="color:#FF0000; background:#00FF80;">正式引出 Makefile 的核心格式：</span>

   ```makefile
   目标: 依赖
   	命令
   ```

4. <span style="color:#FF0000; background:#00FF80;">通过实际 `make` 演示“改 a.c 只重新编译 a.c”的效果。</span>

------

## 2. 一句话总结

> **<span style="background:#00FF80;">Makefile 本质上是在描述“文件之间谁依赖谁”；make 再通过<span style="color:#FF0000;">比较目标文件和依赖文件的修改时间</span>，决定哪些命令需要执行。</span>**

这句话基本就是整节课。

------

## 3. 时间段拆解

时间点按视频画面切换整理，个别边界取近似值。

| 时间段          | 内容概括                                | 画面/命令                            | 作用             |
| --------------- | --------------------------------------- | ------------------------------------ | ---------------- |
| **00:00-01:40** | 进入 Makefile 章节，提出问题            | 打开课程目录、示例工程、课程提纲     | 引入             |
| **01:40-03:30** | 建立多源文件程序                        | 展示 `a.c`、`b.c` 等源码             | 构造例子         |
| **03:30-06:30** | 分析 `gcc -o test a.c b.c` 内部做了什么 | 画出 `.c → 汇编 → .o → test`         | 回顾编译+链接    |
| **06:30-09:20** | 指出“一条 gcc 命令”的缺点               | 使用 gcc 演示并画图                  | 引出重复编译问题 |
| **09:20-11:30** | 改成分别编译 `.c` 文件                  | `gcc -c -o a.o a.c` 等               | 引出增量编译     |
| **11:30-12:40** | 提出新问题：怎么自动判断谁变了？        | 板书“比较时间”等                     | 引出 make        |
| **12:40-15:20** | 正式介绍 Makefile 规则                  | `目标: 依赖` + TAB 命令              | 本节核心         |
| **15:20-17:10** | 分析 make 如何根据依赖工作              | `test → a.o/b.o → a.c/b.c`           | 建立依赖树       |
| **17:10-18:25** | 编写真正的 Makefile 并首次执行          | `make`，编译 a.o、b.o、test          | 首次构建         |
| **18:25-19:00** | 什么都没改再次 make                     | 提示目标已经是最新状态               | 验证时间判断     |
| **19:00-20:05** | 分别修改 a.c、b.c 后执行 make           | 只重编对应 `.o`，随后重新链接        | 验证增量编译     |
| **20:05-20:31** | 最终总结 Makefile 核心规则              | 显示“目标不存在或依赖更新则执行命令” | 收束             |

------

## 4. 这节课是怎么一步一步引出 Makefile 的？

这个推导过程特别值得你掌握。

------

### 第一步：最简单的方法

假设有：

```text
a.c
b.c
```

最终生成：

```text
test
```

可以直接：

```bash
gcc -o test a.c b.c
```

看起来非常方便。

但是 GCC 内部实际上还是类似：

```text
a.c
 ↓
预处理
 ↓
编译
 ↓
汇编
 ↓
a.o
        \
         → 链接 → test
        /
b.c
 ↓
预处理
 ↓
编译
 ↓
汇编
 ↓
b.o
```

也就是说：

```bash
gcc -o test a.c b.c
```

<span style="color:#FF0000; background:#00FF80;">并不是“神奇地把两个 `.c` 直接变成 test”。</span>

它<span style="color:#FF0000; background:#00FF80;">内部仍然经过：</span>

```text
.c → .o → 链接
```

------

## 5. `gcc -o test a.c b.c` 有什么问题？

假设：

```text
a.c  没改
b.c  改了一行
```

然后你再次执行：

```bash
gcc -o test a.c b.c
```

那么 GCC 会再次处理：

```text
a.c  → 再编译一遍
b.c  → 再编译一遍
           ↓
          链接
```

问题来了：

> **a.c 明明没改，为什么还要重新编译？**

两个文件还感觉不到。

假设工程有：

```text
file1.c
file2.c
file3.c
...
file1000.c
```

你只修改：

```text
file753.c
```

结果<span style="background:#00FF80;">一条 gcc 命令把 **1000 个文件重新编译一遍**。</span>

那就很浪费时间了。

------

## 6. 所以老师开始<span style="background:#00FF80;">改成“分别编译”</span>

视频中后面采用类似：

```bash
gcc -c -o a.o a.c
gcc -c -o b.o b.c

gcc -o test a.o b.o
```

对应关系：

```text
a.c ──编译──→ a.o ─┐
                    ├──链接──→ test
b.c ──编译──→ b.o ─┘
```

这样好处马上出来了。

例如只改：

```text
a.c
```

那么理论上只需要：

```bash
gcc -c -o a.o a.c
gcc -o test a.o b.o
```

而：

```text
b.c → b.o
```

这一部分根本不需要再执行。

------

## 7. 但是又出现一个新问题

这<span style="color:#FF0000; background:#00FF80;">也是 Makefile 真正出现的原因。</span>

<span style="color:#FF0000;">你现在有1000个文件。</span>

<span style="color:#FF0000; background:#00FF80;">难道每次编译之前，都由程序员自己判断：</span>

```text
a.c 改没改？
b.c 改没改？
c.c 改没改？
...
```

<span style="color:#FF0000; background:#00FF80;">然后自己决定：</span>

```bash
gcc -c ...
```

<span style="color:#FF0000; background:#00FF80;">执行哪几个？</span>

显然不现实。

于是问题变成：

> <span style="background:#00FF80;">**能不能让一个工具自动判断哪些文件发生变化？**</span>

答案就是：

### `make`

而告<span style="color:#FF0000; background:#00FF80;">诉 make 文件关系的，就是：</span>

### `Makefile`

------

## 8. Makefile 最核心的规则

视频后面明确总结成：

```makefile
目标: 依赖1 依赖2 ...
	命令
```

一定把<span style="color:#FF0000; background:#00FF80;">这三个东西搞懂。</span>

------

### ① 目标 target

我<span style="color:#FF0000; background:#00FF80;">要得到什么。</span>

例如：

```text
test
```

或者：

```text
a.o
```

------

### ② 依赖 prerequisites

<span style="color:#FF0000; background:#00FF80;">生成这个目标，需要什么。</span>

例如：

```text
a.o 需要 a.c
```

于是：

```makefile
a.o: a.c
```

------

### ③ 命令 command

<span style="color:#FF0000; background:#00FF80;">如果目标需要重新生成，具体怎么生成？</span>

例如：

```bash
gcc -c -o a.o a.c
```

所以完整就是：

```makefile
a.o: a.c
	gcc -c -o a.o a.c
```

翻译成人话：

> 我要得到 `a.o`；
> `a.o` 依赖 `a.c`；
> 如果需要重新生成 `a.o`，就执行 `gcc -c -o a.o a.c`。

这就是 Makefile。

------

## 9. 视频中的完整例子

可以整理成：

```makefile
test: a.o b.o
	gcc -o test a.o b.o

a.o: a.c
	gcc -c -o a.o a.c

b.o: b.c
	gcc -c -o b.o b.c
```

你现在先别把它当“代码”。

直接翻译成人话。

------

### 第一条

```makefile
test: a.o b.o
	gcc -o test a.o b.o
```

意思：

```text
目标：test

依赖：
a.o
b.o

生成 test 的方法：
gcc -o test a.o b.o
```

------

### 第二条

```makefile
a.o: a.c
	gcc -c -o a.o a.c
```

意思：

```text
a.o 依赖 a.c

需要更新 a.o 时：
编译 a.c
```

------

### 第三条

```makefile
b.o: b.c
	gcc -c -o b.o b.c
```

意思：

```text
b.o 依赖 b.c

需要更新 b.o 时：
编译 b.c
```

------

## 10. make 究竟怎么判断“需要重新编译”？

这才是<span style="background:#00FF80;">这节视频最核心的知识点之一。</span>

视频最后明确给出了判断原则：

> <span style="color:#FF0000; background:#00FF80;">当目标文件不存在，或者某个依赖文件比目标文件“新”，就执行命令。</span>

这里的：

### “新”

非常容易理解错。

它主要指：

### <span style="background:#00FF80;">文件修改时间更新</span>

不是 make 打开两个文件，把代码一行一行比较。

------

<span style="color:#FF0000; background:#00FF80;">例如：</span>

```text
a.c   修改时间：10:30
a.o   修改时间：10:20
```

make 一看：

```text
a.c 比 a.o 新
```

说明：

```text
10:20 生成 a.o
      ↓
10:30 又修改了 a.c
```

那么当前的：

```text
a.o
```

肯定已经跟不上现在的 `a.c` 了。

因此：

```text
a.c 新于 a.o
       ↓
a.o 已过期
       ↓
执行：
gcc -c -o a.o a.c
```

------

## 11. 为什么这种“比较时间”特别巧？

假设：

```text
a.c  09:00
 ↓
a.o  09:10
```

说明：

```text
a.o 是基于当前 a.c 编译出来的
```

所以<span style="color:#FF0000; background:#00FF80;">不需要重新编译。</span>

------

后来你修改：

```text
a.c  10:00
```

变成：

```text
a.c：10:00
a.o：09:10
```

make 一看马上知道：

> `a.o` 是在 `a.c` 上一次修改之前生成的，所以过期了。

<span style="color:#FF0000; background:#00FF80;">这就是为什么 Makefile 能自动判断。</span>

------

## 12. 第一次执行 `make` 会发生什么？

假设最开始目录只有：

```text
a.c
b.c
Makefile
```

没有：

```text
a.o
b.o
test
```

Makefile：

```makefile
test: a.o b.o
	gcc -o test a.o b.o

a.o: a.c
	gcc -c -o a.o a.c

b.o: b.c
	gcc -c -o b.o b.c
```

make 要生成：

```text
test
```

但是它发现 test 依赖：

```text
a.o
b.o
```

而<span style="color:#FF0000; background:#00FF80;">这两个文件根本不存在。</span>

于是：

```text
              test
             /    \
           a.o    b.o
           /        \
         a.c        b.c
```

<span style="color:#FF0000; background:#00FF80;">make 就会往下找。</span>

------

首先：

```text
a.o 不存在
```

执行：

```bash
gcc -c -o a.o a.c
```

------

然后：

```text
b.o 不存在
```

执行：

```bash
gcc -c -o b.o b.c
```

------

最后：

```text
a.o + b.o
```

都准备好了：

```bash
gcc -o test a.o b.o
```

最终：

```text
a.c → a.o ─┐
            ├→ test
b.c → b.o ─┘
```

------

## 13. 什么都不改，再执行一次 `make`

视频大约 **18:25-19:00** 演示了这个情况。

第一次已经有：

```text
a.c
a.o

b.c
b.o

test
```

并且所有目标都比依赖更新。

于是第二次：

```bash
make
```

make 判断：

```text
a.c 比 a.o 旧
→ a.o 不用生成

b.c 比 b.o 旧
→ b.o 不用生成

a.o、b.o 比 test 旧
→ test 也不用重新链接
```

所以<span style="color:#FF0000; background:#00FF80;">它基本什么都不做。</span>

终端中会出现类似：

```text
'test' is up to date
```

也就是：

> <span style="background:#00FF80;">test 已经是最新的。</span>

------

## 14. 只修改 `a.c`，make 到底会干什么？

这是视频后面非常重要的实际演示。

修改：

```text
a.c
```

以后：

```text
a.c：新
a.o：旧
```

因此：

```bash
gcc -c -o a.o a.c
```

生成新的：

```text
a.o
```

但是：

```text
b.c 没有变化
```

所以：

```text
b.o
```

不会重新生成。

随后出现一个很重要的连锁反应：

新的：

```text
a.o
```

比：

```text
test
```

更新。

而：

```makefile
test: a.o b.o
```

所以 test 也过期。

于是再执行：

```bash
gcc -o test a.o b.o
```

整个过程：

```text
修改 a.c
   ↓
a.o 过期
   ↓
重新编译 a.o
   ↓
a.o 变得比 test 新
   ↓
test 过期
   ↓
重新链接 test
```

而：

```text
b.c
 ↓
b.o
```

完全没动。

这就是<span style="background:#00FF80;"> **增量编译**。</span>

------

## 15. 这里有个非常容易搞错的地方

<span style="background:#00FF80;">make **不是看到 Makefile 后，把里面所有命令从上往下执行一遍**。</span>

这是初学者特别容易产生的误解。

例如：

```makefile
test: a.o b.o
	gcc -o test a.o b.o

a.o: a.c
	gcc -c -o a.o a.c

b.o: b.c
	gcc -c -o b.o b.c
```

不是：

```text
第一行执行
第二行执行
第三行执行
```

而是：

```text
先看我要生成 test
       ↓
test 依赖谁？
       ↓
a.o、b.o
       ↓
它们需不需要更新？
       ↓
根据依赖和文件时间判断
       ↓
只执行必要的命令
```

所以更准确地理解：

> <span style="color:#FF0000; background:#00FF80;">**Makefile 描述的是关系，不是一串固定执行步骤。**</span>

这个观念很重要。

------

## 16. 和你前面学的 GCC、编译器、链接器彻底串起来

你之前学的可以整理成：

```text
                源代码
           a.c          b.c
            │            │
            │ gcc -c     │ gcc -c
            ↓            ↓
           a.o          b.o
              \          /
               \        /
                ↓      ↓
                 链接
                  ↓
                 test
```

而现在 Makefile 加进来以后：

```text
                    Makefile
                       │
                 描述依赖关系
                       ↓
                      make
                       │
             判断哪些东西过期
                       │
          ┌────────────┼───────────┐
          ↓            ↓           ↓
     编译 a.c      编译 b.c       链接
          │            │           │
          ↓            ↓           ↓
         a.o          b.o        test
```

所以四个东西的关系：

```text
Makefile = 规则说明书

make = 根据规则做判断的程序

gcc = 真正执行编译/链接工作的程序

.o / test = gcc 产生的结果文件
```

------

## 17. 内容结构

### 开头

从以前 IDE：

```text
点一下按钮就自动编译
```

引出：

> IDE 怎么知道哪些文件应该重新编译？

------

### 发展

分析：

```bash
gcc -o test a.c b.c
```

<span style="color:#FF0000; background:#00FF80;">存在全部重新编译的问题。</span>

------

### 转折

把它拆为：

```bash
gcc -c -o a.o a.c
gcc -c -o b.o b.c
gcc -o test a.o b.o
```

<span style="color:#FF0000; background:#00FF80;">虽然效率提高，但又产生：</span>

> 人怎么判断哪个 `.c` 被修改？

------

### 核心

<span style="color:#FF0000; background:#00FF80;">引入：</span>

```text
make + Makefile
```

<span style="color:#FF0000; background:#00FF80;">建立：</span>

```text
目标 ← 依赖
```

关系。

------

### 结尾

<span style="color:#FF0000; background:#00FF80;">给出最终判断：</span>

```text
目标不存在
        或
依赖文件比目标更新
        ↓
执行命令
```

<span style="color:#FF0000; background:#00FF80;">整体路径：</span>

```text
从【直接 gcc 编译】
→ 发现【每次全部重新编译】
→ 改为【分别生成 .o】
→ 又发现【人工判断太麻烦】
→ 引入【Makefile 描述依赖】
→ make【比较文件时间】
→ 实现【自动增量编译】
```

------

## 18. 观点分析

这节主要是技术结论。

| 观点                                       | 归属   | 明确/可能 | 依据                  |
| ------------------------------------------ | ------ | --------- | --------------------- |
| 一次性编译所有 `.c` 会造成不必要的重复编译 | 讲解者 | 明确      | 中段明确指出缺点      |
| 应把源文件分别编译为 `.o`                  | 讲解者 | 明确      | 后续改用三个 gcc 命令 |
| 人工判断哪些文件需要重新编译很麻烦         | 讲解者 | 明确      | 引出 make 的直接原因  |
| Makefile 最核心的是“目标、依赖、命令”规则  | 讲解者 | 明确      | 视频结尾明确总结      |
| make 根据文件新旧判断是否执行命令          | 讲解者 | 明确      | 最终规则              |

核心观点：

```text
Makefile 的核心不是保存 gcc 命令，
而是描述依赖关系。

make 的核心不是“执行 Makefile”，
而是根据这些依赖关系判断哪些命令需要执行。
```

------

## 19. 重点片段精读

### 片段一：约 03:30-06:30

表面上：

老师画：

```text
a.c → ... → a.o
b.c → ... → b.o
              ↓
             test
```

深层含义：

这是把你前面学的 **编译和链接** 重新拿出来，为 Makefile 做基础。

如果不知道 `.o` 是什么，后面：

```makefile
test: a.o b.o
```

根本理解不了。

------

### 片段二：约 08:00-11:30

表面上：

指出：

```bash
gcc -o test a.c b.c
```

会导致没修改的文件也重新编译。

更深层：

Makefile 最初存在的一个重要原因就是：

> **大型工程的构建效率。**

------

### 片段三：约 12:40-15:20

这是全视频最值得反复看的部分：

```makefile
目标: 依赖
	命令
```

你以后学 Makefile 的：

```text
变量
自动变量
通配符
模式规则
伪目标
函数
```

都是在这个基本规则上继续扩展。

------

### 片段四：约 18:25-20:05

实际执行 `make`：

```text
不改文件 → 什么都不编

改 a.c → 只编 a.c，再链接

改 b.c → 只编 b.c，再链接
```

这一段把前面理论真正验证了。

------

## 20. Who / Whom / Whose / When / Where / What / Why / How

| 维度      | 视频中的答案                                                 |
| --------- | ------------------------------------------------------------ |
| **Who**   | `make` 程序                                                  |
| **Whom**  | Makefile 中描述的目标文件和依赖文件                          |
| **Whose** | 每个目标都有自己的依赖关系                                   |
| **When**  | 目标不存在，或依赖比目标更新时                               |
| **Where** | C/C++ 工程的编译构建过程中                                   |
| **What**  | 判断哪些 `.o`、可执行程序需要重新生成                        |
| **Why**   | 避免整个工程每次全部重新编译                                 |
| **How**   | 通过 Makefile 描述依赖，通过文件修改时间判断新旧，再调用 gcc |

------

## 21. 你现在最应该重点记住的 4 件事

这一节学完，先不要急着背复杂 Makefile。

你只要能非常清楚地说出这四件事就够了：

```text
① .c 编译 → .o
② 多个 .o 链接 → 可执行程序

③ Makefile：
   目标 : 依赖
       命令

④ make 判断：
   目标不存在
   或
   依赖比目标更新
        ↓
   执行命令
```

尤其是这一条：

> **make 并不是“看到 Makefile 就把命令全部执行”，而是先分析依赖关系，再决定哪些命令有必要执行。**

这应该算是这节课最值得你真正理解透的一句话。

------

## 22. 记忆版总结

```text
类型：
Makefile 原理 + 基础规则

主线：
gcc 一次编译所有源文件
→ 浪费
→ 分别生成 .o
→ 人工判断又太麻烦
→ 引入 make 和 Makefile

核心规则：

目标: 依赖
	命令

make 的判断依据：

目标不存在
或
依赖比目标新
→ 执行命令

例子：

test ← a.o + b.o
a.o  ← a.c
b.o  ← b.c

修改 a.c：

a.c
 ↓
a.o 重新编译
 ↓
test 重新链接

b.o 不动

一句话记忆：

Makefile 描述“谁依赖谁”，
make 根据“谁比谁新”决定“谁需要重新生成”。
```

这一节和你前面学的 **GCC 编译过程** 是完全连起来的：前面是在研究“**一个文件怎么被编译出来**”，现在 Makefile 开始研究的是“**一整个工程里，到底哪些文件有必要编译**”。这两个层次一分开，后面 Makefile 就会好理解很多。
