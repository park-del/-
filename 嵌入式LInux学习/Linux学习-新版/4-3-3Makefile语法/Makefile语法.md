# 视频分析：4_3-3《Makefile 的语法》

这节视频大约 **19分43秒**。它是在上一节“目标:依赖 + 命令”的基础上，把 Makefile 从“能写”推进到“写得简洁、可复用”。

先记住这节最核心的四块：

```text
① %.o : %.c        → 模式规则，少写重复规则
② $@ / $< / $^     → 自动变量
③ .PHONY            → 伪目标
④ := / = / ?= / += → Makefile变量赋值方式
```

------

## 1. 视频类型判断

**主要类型：教程类 / 讲解类**

**次要类型：代码演示类 / 原理分析类**

判断依据：

1. 老师直接修改 Makefile，并在 Linux 终端执行 `make` 验证。
2. 每讲一种语法，都配合实际例子解释。
3. 重点不是“背语法”，而是解释这些语法为什么能把 Makefile 写得更简洁。

------

## 2. 一句话总结

> **这节是在教你如何把上一节那种重复、笨重的 Makefile，利用模式规则、自动变量、伪目标和变量机制逐步简化。**

------

## 3. 时间段拆解

时间边界按视频中的画面、Makefile 编辑和终端演示自然划分，个别位置取近似值。

| 时间段          | 内容概括                       | 关键内容                           | 作用                       |
| --------------- | ------------------------------ | ---------------------------------- | -------------------------- |
| **00:00-01:20** | 回顾上一节并列出 Makefile 语法 | `%.o`、变量、`.PHONY`              | 给出本节路线               |
| **01:20-03:20** | 引入模式规则                   | `%.o: %.c`                         | 消除大量重复 `.o` 规则     |
| **02:00-04:30** | 引入自动变量                   | `$@`、`$<`、`$^`                   | 进一步通用化规则           |
| **03:20-06:30** | 增加 `c.c` 并实际验证          | 编译、链接、出现链接错误后修改依赖 | 展示规则如何扩展到更多文件 |
| **06:30-10:15** | 讲 `clean` 和 `.PHONY`         | `clean:`、`.PHONY: clean`          | 解释伪目标                 |
| **10:15-12:30** | 引入 Makefile 变量             | 即时变量、延时变量                 | 进入变量机制               |
| **12:30-16:20** | 对比 `:=` 和 `=`               | `A := $(C)`、`B = $(C)`            | 理解“什么时候求值”         |
| **16:20-19:30** | 介绍 `?=` 和 `+=`              | 条件赋值、追加                     | 补充常见变量写法           |
| **19:30-19:43** | 返回笔记总结                   | 四种赋值方式                       | 收束本节                   |

------

## 4. 第一部分：`%.o : %.c` 到底是什么意思？

上一节可能需要这样写：

```makefile
a.o: a.c
	gcc -c -o a.o a.c

b.o: b.c
	gcc -c -o b.o b.c

c.o: c.c
	gcc -c -o c.o c.c
```

你应该马上发现：

```text
a.o ← a.c
b.o ← b.c
c.o ← c.c
```

<span style="color:#FF0000; background:#00FF80;">结构完全一样。</span>

<span style="color:#FF0000; background:#00FF80;">如果有100个 `.c` 文件，难道写100遍？</span>

所以视频大约 **01:20之后** 引入：

```makefile
%.o: %.c
	gcc -c -o $@ $<
```

这里的<span style="color:#FF0000; background:#00FF80;"> `%` 可以理解成</span>：

> **<span style="background:#00FF80;">“这一部分叫什么名字都行，但前后要对应。”</span>**

例如：

```text
a.o → % = a → a.c
b.o → % = b → b.c
c.o → % = c → c.c
```

于是：

```makefile
%.o: %.c
```

相当于一次写出了：

```text
a.o: a.c
b.o: b.c
c.o: c.c
d.o: d.c
……
```

这叫 **模式规则（pattern rule）**。

------

## 5. `$@`、`$<`、`$^` 是什么？

这是这节最值得你记住的地方之一。

视频笔记里明确写了：

```text
$@  表示目标
$<  表示第1个依赖文件
$^  表示所有依赖文件
```

------

### `$@`：当前目标

例如：

```makefile
%.o: %.c
	gcc -c -o $@ $<
```

现在 make 正准备生成：

```text
a.o
```

那么：

```text
$@ = a.o
```

------

### `$<`：第一个依赖

对于：

```text
a.o: a.c
```

那么：

```text
$< = a.c
```

所以：

```makefile
gcc -c -o $@ $<
```

实际执行就变成：

```bash
gcc -c -o a.o a.c
```

如果轮到 `b.o`：

```text
$@ = b.o
$< = b.c
```

于是自动变成：

```bash
gcc -c -o b.o b.c
```

------

## 6. 所以这一条为什么这么厉害？

```makefile
%.o: %.c
	gcc -c -o $@ $<
```

<span style="color:#FF0000; background:#00FF80;">表面只有两行。</span>

<span style="color:#FF0000; background:#00FF80;">实际可以处理：</span>

```text
a.c → a.o
b.c → b.o
c.c → c.o
d.c → d.o
……
```

你可以<span style="color:#FF0000; background:#00FF80;">把它理解成一个“函数模板”：</span>

```text
给我一个 xxx.o
        ↓
我去找 xxx.c
        ↓
执行：
gcc -c -o xxx.o xxx.c
```

这就是 Makefile 开始变得真正实用的地方。

------

## 7. `$^`：为什么链接的时候特别好用？

比如：

```makefile
test: a.o b.o c.o
	gcc -o test $^
```

这一条中的：

```text
$^
```

代表：

```text
a.o b.o c.o
```

所以实际执行：

```bash
gcc -o test a.o b.o c.o
```

而：

```text
$@ = test
```

所以甚至还能进一步写：

```makefile
test: a.o b.o c.o
	gcc -o $@ $^
```

make 看到以后自动替换：

```text
$@ → test
$^ → a.o b.o c.o
```

最终：

```bash
gcc -o test a.o b.o c.o
```

------

## 8. 三个自动变量放一起记

建议你直接这样记：

| 自动变量 | 含义                 | 举例          |
| -------- | -------------------- | ------------- |
| `$@`     | **我是谁**——当前目标 | `test`、`a.o` |
| `$<`     | **我首先依赖谁**     | `a.c`         |
| `$^`     | **我依赖的所有东西** | `a.o b.o c.o` |

可以记成：

```text
$@ → 自己
$< → 第一个
$^ → 全部
```

------

## 9. 为什么视频中增加 `c.c` 后会出现链接错误？

大约 **03:20-04:30**，老师增加了 `c.c`。

终端中可以看到类似：

```text
undefined reference to `func_c'
```

这个地方其实非常有教学价值。

程序里调用了：

```c
func_c();
```

而 `func_c()` 的实现存在于：

```text
c.c → c.o
```

但是如果 Makefile 还是：

```makefile
test: a.o b.o
	gcc -o test a.o b.o
```

链接器只拿：

```text
a.o
b.o
```

去链接。

它根本没有：

```text
c.o
```

当然找不到 `func_c()` 的实现。

于是要修改为：

```makefile
test: a.o b.o c.o
	gcc -o test $^
```

这样才形成：

```text
a.c → a.o ─┐
b.c → b.o ─┼→ test
c.c → c.o ─┘
```

这和你前面学的**链接器必须拿到相应 `.o` / 库，才能找到函数定义**完全是同一件事。

------

## 10. 第二个重点：`clean` 为什么还需要 `.PHONY`？

视频大约 **06:30-10:15** 进入这个问题。

我们通常会写：

```makefile
clean:
	rm *.o test
```

然后：

```bash
make clean
```

它<span style="color:#FF0000; background:#00FF80;">不是在“编译一个叫 clean 的程序”。</span>

而是<span style="color:#FF0000; background:#00FF80;">借用了 Makefile 的目标机制，想执行：</span>

```bash
rm *.o test
```

所以：

```text
clean
```

<span style="color:#FF0000; background:#00FF80;">其实只是一个“命令名字”。</span>

------

## 11. 那问题来了：如果目录里真的存在一个叫 `clean` 的文件呢？

视频里专门演示了这个情况。

老师创建：

```bash
> clean
```

于是目录里面真的多出一个：

```text
clean
```

文件。

然后执行：

```bash
make clean
```

make 可能告诉你：

```text
'clean' is up to date
```

也就是：

> “clean 已经是最新的，不需要做任何事情。”

为什么？

<span style="color:#FF0000; background:#00FF80;">因为 make 原本把：</span>

```makefile
clean:
	rm *.o test
```

<span style="color:#FF0000; background:#00FF80;">理解成：</span>

```text
我要生成一个目标文件：clean
```

现在它发现：

```text
咦？
clean 文件已经存在！
```

<span style="color:#FF0000; background:#00FF80;">又没有依赖文件比它更新。</span>

按照上一节规则：

```text
不需要执行命令
```

于是：

```bash
rm *.o test
```

反而不执行了。

------

## 12. `.PHONY` 就是解决这个问题

加上：

```makefile
.PHONY: clean

clean:
	rm *.o test
```

意思基本就是告诉 make：

> **c<span style="background:#00FF80;">lean 不是真正的文件目标，它只是一个命令型目标。不要拿文件时间去判断它。</span>**

所以以后：

```bash
make clean
```

<span style="color:#FF0000; background:#00FF80;">无论目录里有没有：</span>

```text
clean
```

<span style="color:#FF0000; background:#00FF80;">这个文件，都执行：</span>

```bash
rm *.o test
```

------

## 13. `.PHONY` 应该怎么理解？

`.PHONY` 直译容易让人迷糊。

你可以简单理解：

```text
普通目标：
“我要生成一个文件。”

伪目标：
“我不是为了生成这个名字的文件，
我只是想让你执行一组操作。”
```

常见的：

```makefile
.PHONY: clean
.PHONY: install
.PHONY: all
```

都是这个思路。

------

## 14. 第三个重点：Makefile变量

视频后半部分开始讲变量。

Makefile 里面可以：

```makefile
C = abc
```

使用：

```makefile
$(C)
```

例如：

```makefile
@echo $(C)
```

就能得到：

```text
abc
```

但是 Makefile 有一个容易让初学者困惑的地方：

> <span style="background:#00FF80;">**不同赋值符号，变量“什么时候得到值”不一样。**</span>

这就是视频后半段重点。

------

## 15. `:=` 和 `=` 是这节最容易混淆的地方

视频用的例子非常典型：

```makefile
A := $(C)
B = $(C)

C = abc

all:
	@echo A = $(A)
	@echo B = $(B)
```

乍一看：

```text
A 和 B 不都是 $(C) 吗？
```

但结果不一样。

------

### `:=`：立即展开

```makefile
A := $(C)
```

执行到这一行时，Make 立即查看：

```text
C 现在是多少？
```

问题是此时：

```makefile
C = abc
```

还没出现。

所以此时 C 没有值。

因此：

```text
A = 空
```

后来即使：

```makefile
C = abc
```

也不会回头重新计算 A。

------

## 16. `=`：延迟展开

再看：

```makefile
B = $(C)
```

它不是立即把 `$(C)` 换掉。

更像先保存：

```text
B 的内容是“$(C)”
```

等真正使用：

```makefile
$(B)
```

的时候，再去问：

```text
此时 C 是多少？
```

这时已经执行到了：

```makefile
C = abc
```

所以：

```text
B = abc
```

最终：

```text
A =
B = abc
```

视频终端里也演示出了这个结果。

------

## 17. 用时间线理解 `:=` 和 `=`

这个方法最好记。

### `:=`

```text
A := $(C)
      ↑
      现在立刻看 C

此刻 C 还没定义
      ↓
A = 空
```

------

### `=`

```text
B = $(C)
     ↓
先记着，不急着查

后来：
C = abc

使用 B
 ↓
现在才查 C
 ↓
B = abc
```

所以可以记：

```text
:= → 现在算

=  → 用的时候再算
```

------

## 18. 为什么这个区别以后很重要？

假设：

```makefile
C = abc
B = $(C)

C = 123
```

<span style="color:#FF0000; background:#00FF80;">因为 `B` 是延迟变量，它最终使用时看到：</span>

```text
C = 123
```

那么 B 也可能得到：

```text
123
```

这就是为什么 Makefile 变量不像普通 C 变量那么直观。

C语言里：

```c
a = b;
```

通常就是马上把当前值给过去。

Makefile 的：

```makefile
A = $(B)
```

却可能是：

> “以后需要 A 时，再来找 B。”

这是两个完全不同的思维。

------

## 19. `?=` 是什么意思？

视频最后笔记里写得很明确：

```makefile
?=
```

核心意思：

> **只有这个变量之前没有被定义过，我才给它赋值。**

例如：

```makefile
D ?= 100ask
```

如果前面没有：

```makefile
D = ...
```

那么：

```text
D = 100ask
```

但如果前面已经：

```makefile
D = weidongshan
D ?= 100ask
```

那么第二句不会把第一句覆盖掉。

最终：

```text
D = weidongshan
```

所以可以把：

```text
?=
```

记成：

> <span style="color:#FF0000; background:#00FF80;">**“你没值的话，我才给你一个默认值。”**</span>

它<span style="background:#00FF80;">特别适合写默认配置。</span>

------

## 20. `+=` 是追加

例如：

```makefile
C = abc
C += 123
```

最终：

```text
C = abc 123
```

不是覆盖掉原值，而是在后面添加。

视频终端最后也演示出了类似：

```text
B = abc 123
```

这样的结果。

所以：

```text
=   → 赋值
+=  → 往后追加
```

------

## 21. 四种变量赋值方式放一起

这是本节建议直接收藏的一张表：

| 写法       | 含义         | 最简单记法 |
| ---------- | ------------ | ---------- |
| `A := xxx` | 立即展开     | **现在算** |
| `A = xxx`  | 延迟展开     | **以后算** |
| `A ?= xxx` | 没定义才赋值 | **默认值** |
| `A += xxx` | 追加内容     | **往后加** |

视频笔记还特别提醒：

> <span style="background:#00FF80;">`+=` 最终表现为立即还是延迟，与这个变量原先采用的定义方式有关。</span>

这个属于稍微深入一点的 Makefile 规则，目前先知道即可。

------

## 22. 那 `export` 呢？

视频提纲中确实把：

```text
即时变量、延时变量、export
```

放在了一组。

不过这段视频画面中，重点实际落在：

```text
:=
=
?=
+=
```

这些变量赋值规则以及实际运行结果上。

所以这一节你先不要把 `export` 当成主线。

后续只需要知道一个方向：

> Makefile 中的变量默认主要属于 make 自己；`export` 涉及把变量传给 make 启动的子进程/子 make。

等课程真正用到多级 Makefile 时，再深入理解会更自然。

------

## 23. 这一节完整的 Makefile 思想已经开始成型

把前两节和这一节合起来，你现在已经能看懂类似：

```makefile
test: a.o b.o c.o
	gcc -o $@ $^

%.o: %.c
	gcc -c -o $@ $<

clean:
	rm *.o test

.PHONY: clean
```

我们逐句翻译。

------

### 第一条

```makefile
test: a.o b.o c.o
	gcc -o $@ $^
```

翻译：

```text
我要生成 test。

它依赖：
a.o
b.o
c.o

需要重新生成时：
gcc -o test a.o b.o c.o
```

------

### 第二条

```makefile
%.o: %.c
	gcc -c -o $@ $<
```

翻译：

```text
任何 xxx.o
都可以由 xxx.c 编译得到。

例如：
a.o ← a.c
b.o ← b.c
c.o ← c.c
```

------

### 第三条

```makefile
clean:
	rm *.o test
```

翻译：

```text
make clean 时：
删除 .o 和 test。
```

------

### 第四条

```makefile
.PHONY: clean
```

翻译：

```text
clean 是命令型目标，
不是一个真正想生成的 clean 文件。
```

------

## 24. 一个非常重要的认识

到这里 Makefile 已经明显不是：

> “把 gcc 命令保存到一个文本文件里。”

而是三层东西：

```text
第一层：依赖关系

test
 ├── a.o
 ├── b.o
 └── c.o
      ↓
各自依赖 .c
```

加上：

```text
第二层：通用生成规则

%.o : %.c
```

再加：

```text
第三层：变量和自动变量

$@
$<
$^
$(变量)
```

最终就是：

> <span style="background:#00FF80;">**用少量规则描述一个可能非常大的工程。**</span>

这才是 Makefile 真正强大的地方。

------

## 25. 内容结构

### 开头

回顾：

```text
目标不存在
或
依赖比目标新
→ 执行命令
```

然后进入 Makefile 语法。

### 第一阶段

从大量重复：

```makefile
a.o: a.c
b.o: b.c
c.o: c.c
```

抽象成：

```makefile
%.o: %.c
```

### 第二阶段

再使用：

```text
$@
$<
$^
```

让命令本身也通用化。

### 第三阶段

通过 `clean` 文件冲突，引出：

```makefile
.PHONY
```

### 第四阶段

通过变量例子解释：

```text
:=
=
?=
+=
```

整体路径：

```text
从【重复的 Makefile】
→ 用【模式规则】减少规则
→ 用【自动变量】减少命令重复
→ 用【.PHONY】处理命令型目标
→ 用【变量】提高配置和复用能力
```

------

## 26. 观点/教学结论

| 教学结论                                | 归属   | 明确/可能 | 依据                   |
| --------------------------------------- | ------ | --------- | ---------------------- |
| 重复的 `.c → .o` 规则可以抽象为模式规则 | 讲解者 | 明确      | `%.o: %.c` 演示        |
| `$@/$</$^` 可以代替具体文件名           | 讲解者 | 明确      | 实际修改 Makefile      |
| `clean` 应定义为伪目标                  | 讲解者 | 明确      | 创建同名文件后实际验证 |
| `:=` 与 `=` 的求值时机不同              | 讲解者 | 明确      | A/B/C 变量实验         |
| `?=` 适合“变量没定义时才赋值”           | 讲解者 | 明确      | 结尾笔记和实验         |
| `+=` 用于追加变量内容                   | 讲解者 | 明确      | 终端输出验证           |

------

## 27. 重点片段精读

### 片段一：约 01:20-03:20

核心：

```makefile
%.o: %.c
	gcc -c -o $@ $<
```

这是全节最值得反复看的地方。

因为从这里开始你第一次真正见到：

> **用一条规则描述无数条具体编译规则。**

------

### 片段二：约 03:20-04:30

增加 `c.c` 后出现：

```text
undefined reference to func_c
```

这段很值得注意，因为它再次验证：

> `.c` 文件存在不代表链接器自动知道它。

最终参与链接的是：

```text
.o / 库
```

必须把相应目标加入依赖和链接命令。

------

### 片段三：约 07:00-10:00

老师故意创建一个真正的：

```text
clean
```

文件。

然后让：

```bash
make clean
```

失效。

最后再通过：

```makefile
.PHONY: clean
```

修复。

这一段对理解 `.PHONY` 非常直观。

------

### 片段四：约 13:20-16:20

```makefile
A := $(C)
B = $(C)
C = abc
```

最后：

```text
A =
B = abc
```

这段把 Makefile 中最反直觉的“立即展开”和“延迟展开”讲出来了。

------

## 28. Who / Whom / Whose / When / Where / What / Why / How

| 维度      | 视频中的答案                                         |
| --------- | ---------------------------------------------------- |
| **Who**   | GNU make / Makefile                                  |
| **Whom**  | C程序中的 `.c`、`.o`、最终可执行文件                 |
| **Whose** | `$@/$</$^` 的含义取决于当前规则                      |
| **When**  | make 分析规则和执行命令时                            |
| **Where** | Linux C工程的构建过程中                              |
| **What**  | 模式规则、自动变量、伪目标、变量赋值                 |
| **Why**   | 减少重复、增强通用性、避免目标冲突                   |
| **How**   | 使用 `%`、自动变量、`.PHONY` 和变量机制抽象 Makefile |

------

## 29. 你这节最应该真正掌握什么？

如果现在让我只让你背 **6行**，我会让你记：

```makefile
test: a.o b.o c.o
	gcc -o $@ $^

%.o: %.c
	gcc -c -o $@ $<

.PHONY: clean
```

然后知道：

```text
$@ = 当前目标
$< = 第一个依赖
$^ = 所有依赖

%  = 匹配相同的“文件主体”

.PHONY = 这不是文件目标
```

变量再记：

```text
:= 现在算
=  以后算
?= 没有才赋值
+= 追加
```

这样这一节的骨架基本就抓住了。

------

## 30. 记忆版总结

```text
类型：
Makefile语法教程

主线：
普通规则
→ 模式规则
→ 自动变量
→ 伪目标
→ Makefile变量

核心语法：

%.o: %.c
	gcc -c -o $@ $<

$@：目标
$<：第一个依赖
$^：所有依赖

.PHONY：
声明伪目标，例如 clean

变量：
:= 立即展开
=  延迟展开
?= 未定义才赋值
+= 追加

一句话记忆：

% 负责“批量匹配”，
$@/$</$^ 负责“自动代入文件名”，
.PHONY 负责“告诉 make 这不是文件”，
变量负责“减少硬编码和重复”。
```

而且你可以发现，这节已经和你前面学的 **`.c → .o → 链接 → 可执行文件`** 完全串起来了：Makefile 的这些语法并不是凭空出现的，几乎每一种语法都是为了更方便地描述这条编译依赖链。



# 问题1

如果makefile文件里面有多个目标的话，我在终端里面输入make默认是执行哪个呢，我是不是可以通过make 目标文件名 来选择执行哪个makefile文件中的特定目标呢；执行make命令默认生成的是一个文件是吗，如果是要执行的目标文件的操作的命令是命令行里面的指令的话，此时也是会生成目标文件的吧，是根据目标文件执不执行来决定这个命令对应的指令来执不执行的吧。



对，你的理解已经很接近了，关键是把 **“目标 target”** 和 **“Makefile 文件”** 这两个东西分开。

假设 Makefile 是：

```makefile
test: a.o b.o
	gcc -o test a.o b.o

a.o: a.c
	gcc -c -o a.o a.c

b.o: b.c
	gcc -c -o b.o b.c

clean:
	rm -f *.o test

.PHONY: clean
```

## 1. 只输入 `make`，默认执行哪个目标？

```bash
make
```

<span style="background:#00FF80;">默认会选择 **Makefile 里的第一个普通目标**，这里就是：</span>

```makefile
test: a.o b.o
```

所以可以先简单记成：

```text
make
 ↓
找到 Makefile
 ↓
默认选择第一个目标 test
 ↓
分析 test 的依赖
 ↓
a.o、b.o 是否需要更新？
 ↓
必要时编译
 ↓
必要时生成 test
```

当然，Makefile 也可以专门设置默认目标，比如 `.DEFAULT_GOAL`，不过你现在先记“默认第一个目标”就够了。

------

## 2. `make xxx` 是<span style="background:#00FF80;">选择特定目标，不是选择 Makefile</span>

比如：

```bash
make clean
```

意思不是：

> 找一个叫 clean 的 Makefile。

而是：

> 在当前 Makefile 中，找到 `clean` 这个目标，然后尝试完成它。

同理：

```bash
make a.o
```

就是：

> 我这次只要求 make 把目标 `a.o` 搞定。

于是它找到：

```makefile
a.o: a.c
	gcc -c -o a.o a.c
```

如果 `a.o` 不存在，或者：

```text
a.c 比 a.o 新
```

就执行：

```bash
gcc -c -o a.o a.c
```

------

如果<span style="background:#00FF80;">你真的想指定 **使用哪个 Makefile 文件**，那是：</span>

```bash
make -f 文件名
```

例如：

```bash
make -f Makefile.test
```

甚至：

```bash
make -f Makefile.test clean
```

意思是：

```text
使用 Makefile.test
       +
执行 clean 目标
```

所以这两个不要混：

```text
make clean
     ↑
   目标名

make -f xxx.mk
        ↑
   Makefile文件名
```

------

## 3. `make` 默认是不是一定生成一个文件？

**不一定。**

这是你这个问题里最关键的一点。

Makefile 的目标叫：

```text
target
```

但：

> **target 不一定真的对应一个文件。**

最常见的情况确实是文件：

```makefile
test: a.o b.o
	gcc -o test a.o b.o
```

这里目标：

```text
test
```

确实是最终生成出来的文件。

执行：

```bash
gcc -o test a.o b.o
```

以后目录里真的出现：

```text
test
```

------

再比如：

```makefile
a.o: a.c
	gcc -c -o a.o a.c
```

这里：

```text
目标 = a.o
```

命令执行以后也确实生成：

```text
a.o
```

这是最标准的：

```text
目标名
  ↕
实际生成的文件名
```

------

## 4. 但目标也可以只是“一个操作名字”

比如：

```makefile
clean:
	rm -f *.o test
```

执行：

```bash
make clean
```

真正执行的是：

```bash
rm -f *.o test
```

<span style="color:#FF0000; background:#00FF80;">它反而是在**删除文件**，根本不会生成一个叫：</span>

```text
clean
```

<span style="background:#00FF80; color:#FF0000;">的文件。</span>

所以：

```text
target
```

更准确地说应该理解为：

> <span style="background:#00FF80;">**“我希望 make 完成的一个目标/任务。”**</span>

这个目标可能是：

```text
生成一个文件
```

也可能是：

```text
删除文件
安装程序
运行测试
烧写程序
打包程序
```

等等。

------

## 5. 那 make 到底是根据什么决定“命令执不执行”？

你后面这一句话其实基本对：

> “是根据目标文件执不执行来决定这个命令对应的指令执不执行。”

不过可以稍微修正成更准确的表达：

> <span style="background:#00FF80;">**make 先判断“目标是否需要重新构建”，如果需要，才执行该目标下面的命令。**</span>

例如：

```makefile
a.o: a.c
	gcc -c -o a.o a.c
```

make 会判断：

```text
a.o 存在吗？
```

### 情况一：不存在

```text
a.o 不存在
 ↓
这个目标需要生成
 ↓
执行命令
 ↓
gcc -c -o a.o a.c
```

------

### 情况二：存在，但 `a.c` 更新

比如：

```text
a.c：10:30修改
a.o：10:20生成
```

说明：

```text
a.o 已经过期
```

于是：

```text
执行命令
↓
重新生成 a.o
```

------

### 情况三：`a.o` 比 `a.c` 新

```text
a.c：10:00
a.o：10:20
```

说明：

```text
当前 a.o 已经是根据最新 a.c 编译出来的
```

所以：

```text
不用执行 gcc
```

------

## 6. 一个非常重要的细节：<span style="background:#00FF80;">make 不检查“命令到底有没有生成目标”</span>

这个地方特别值得你理解。

假设你写一个奇怪的 Makefile：

```makefile
hello:
	echo "你好"
```

然后：

```bash
make hello
```

会打印：

```text
你好
```

但：

```bash
echo "你好"
```

<span style="color:#FF0000; background:#00FF80;">并没有生成一个</span>：

```text
hello
```

<span style="color:#FF0000; background:#00FF80;">文件。</span>

于是<span style="color:#FF0000; background:#00FF80;">下一次：</span>

```bash
make hello
```

<span style="color:#FF0000; background:#00FF80;">make 又发现：</span>

```text
hello 文件还是不存在
```

<span style="color:#FF0000; background:#00FF80;">所以它又会执行：</span>

```bash
echo "你好"
```

也就是说：

> <span style="background:#00FF80;">make **不会验证你的命令是否真的按照目标名生成了文件**。</span>

<span style="color:#FF0000;">Makefile 是你写的，你告诉 make：</span>

```text
执行这个命令可以完成这个目标
```

<span style="color:#FF0000;">make 就相信你。</span>

------

## 7. 这也是为什么有 `.PHONY`

像：

```makefile
clean:
	rm -f *.o test
```

我们的本意根本就不是生成 `clean` 文件。

所以应该明确写：

```makefile
.PHONY: clean

clean:
	rm -f *.o test
```

相当于告诉 make：

```text
clean 不是文件
不要拿 clean 这个文件的存在与否、
修改时间来判断。

只要我明确执行：

make clean

你就执行它下面的命令。
```

所以：

```bash
make clean
```

基本就是：

```text
选择 clean 目标
 ↓
发现它是 PHONY
 ↓
直接执行 recipe
 ↓
rm -f *.o test
```

------

## 8. 再来看一个完整例子，你应该就彻底通了

Makefile：

```makefile
test: a.o b.o
	gcc -o test a.o b.o

a.o: a.c
	gcc -c -o a.o a.c

b.o: b.c
	gcc -c -o b.o b.c

run: test
	./test

clean:
	rm -f a.o b.o test

.PHONY: run clean
```

这里其实有 **5个目标**：

```text
test
a.o
b.o
run
clean
```

------

你输入：

```bash
make
```

默认：

```text
test
```

因为它是第一个目标。

------

你输入：

```bash
make a.o
```

只要求：

```text
a.o
```

于是：

```text
检查 a.c → a.o
```

------

你输入：

```bash
make clean
```

执行：

```bash
rm -f a.o b.o test
```

------

你输入：

```bash
make run
```

make 发现：

```makefile
run: test
```

所以：

```text
我要 run
 ↓
run 依赖 test
 ↓
先保证 test 是最新的
 ↓
test 又依赖 a.o b.o
 ↓
需要的话先编译它们
 ↓
生成/确认 test
 ↓
最后执行 ./test
```

所以可能出现：

```text
a.c → a.o ─┐
            ├→ test
b.c → b.o ─┘
              ↓
            ./test
```

注意 `run` 自己并没有生成 `run` 文件。

------

## 9.<span style="background:#00FF80;"> 你现在可以把 Makefile 理解成这样</span>

这比“make 就是执行命令”准确很多：

```text
                 Makefile
                    │
         ┌──────────┼──────────┐
         ↓          ↓          ↓
       test        run        clean
        │           │
     a.o b.o       test
      │   │
     a.c b.c
```

当你输入：

```bash
make 某个目标
```

其实是在告诉 make：

> **“请把这个目标搞定。”**

<span style="color:#FF0000; background:#00FF80;">然后 make：</span>

```text
① 找到这个目标
       ↓
② 看它依赖谁
       ↓
③ 递归把依赖搞定
       ↓
④ 判断目标是否需要更新
       ↓
⑤ 如果需要，执行下面的命令
```

------

## 最后给你一个特别好记的版本

```text
make
→ 默认完成 Makefile 的第一个目标

make xxx
→ 完成 Makefile 中的 xxx 目标

make -f xxx.mk
→ 指定使用哪个 Makefile

目标不一定是文件：
test、a.o → 通常是真的文件
clean、run → 通常只是任务名字

普通文件目标：
目标不存在
或
依赖比目标更新
→ 执行命令

.PHONY 目标：
明确 make 它时
→ 执行对应命令
```

所以你那句话我帮你改成最准确的一版就是：

> <span style="background:#00FF80; color:#FF0000;">**Makefile 中每个目标下面都有一组命令，但 make 并不是看到命令就执行；它先判断这个目标是否需要被完成/更新，只有需要时才执行对应的命令。而这些命令可能生成目标文件，也可能只是执行某种操作，本身并不一定生成任何文件。**</span>





# 问题2

如果在当前目录里面有多个makefile文件的话，那么我在终端里面执行make命令怎么知道执行的是哪个makefile文件呢，另外makefile文件的命名有什么特殊的吗，后缀名什么的，我怎么识别一个文件是不是makefile文件呢？



对，这个问题很关键。你现在其实是在问两个东西：**`make` 怎么自动找 Makefile**，以及 **Makefile 到底有没有固定文件名/后缀**。

在你现在学的 Linux 环境里，一般用的是 **GNU make**。<span style="color:#FF0000; background:#00FF80;">当你在当前目录直接执行：</span>

```bash
make
```

<span style="color:#FF0000; background:#00FF80;">GNU make 会按顺序尝试寻找：</span>

```text
GNUmakefile
    ↓
makefile
    ↓
Makefile
```

找到第一个存在的，就使用它。

比如当前目录有：

```text
a.c
b.c
GNUmakefile
makefile
Makefile
test.mk
```

你直接：

```bash
make
```

<span style="color:#FF0000; background:#00FF80;">那么它会优先使用：</span>

```text
GNUmakefile
```

因为搜索顺序是：

```text
GNUmakefile → makefile → Makefile
```

如果没有 `GNUmakefile`，但同时有：

```text
makefile
Makefile
```

那就使用：

```text
makefile
```

------

## 1. 那平时为什么大家几乎都叫 `Makefile`？

因为这是最常见、最推荐的名字：

```text
Makefile
```

注意：

> <span style="background:#00FF80;">**没有后缀名。**</span>

不是：

```text
Makefile.txt
Makefile.mk
Makefile.c
```

而就是：

```text
Makefile
```

例如：

```bash
ls
```

看到：

```text
Makefile
main.c
add.c
add.h
```

这个<span style="background:#00FF80;"> `Makefile` 本质上只是一个**普通文本文件**。</span>

你可以：

```bash
vi Makefile
```

或者：

```bash
cat Makefile
```

查看它。

------

## 2. Makefile 必须叫 `Makefile` 吗？

**完全不必须。**

你甚至可以起一个很随意的名字：

```text
abc
hello.mk
build.mk
my_makefile
123.txt
```

<span style="color:#FF0000; background:#00FF80;">里面只要写的是合法 Makefile 内容，都可以被 make 使用。</span>

但<span style="color:#FF0000;">如果它不是：</span>

```text
GNUmakefile
makefile
Makefile
```

<span style="background:#00FF80;">这三个默认名字之一，那么直接：</span>

```bash
make
```

<span style="color:#FF0000; background:#00FF80;">不会自动找到它。</span>

<span style="color:#FF0000; background:#00FF80;">你需要通过：</span>

```bash
make -f 文件名
```

<span style="color:#FF0000; background:#00FF80;">指定。</span>

比如：

```text
build.mk
```

里面：

```makefile
test: main.o
	gcc -o test main.o
```

执行：

```bash
make -f build.mk
```

就会使用：

```text
build.mk
```

------

## 3. 如果目录里有多个 Makefile，我可以自己选吗？

当然可以。

例如：

```text
Makefile
Makefile.debug
Makefile.release
test.mk
```

直接：

```bash
make
```

会自动使用：

```text
Makefile
```

但你可以：

```bash
make -f Makefile.debug
```

或者：

```bash
make -f Makefile.release
```

或者：

```bash
make -f test.mk
```

所以你可以把：

```text
-f
```

理解成：

> **file：这次你给我指定一个 Makefile 文件。**

------

## 4. `make -f xxx` 和 `make xxx` 一定不要混

这个非常重要。

假设：

```text
Makefile
```

里面：

```makefile
test:
	gcc -o test main.c

clean:
	rm -f test
```

那么：

```bash
make clean
```

意思是：

> <span style="background:#00FF80;">使用默认的 `Makefile`，执行其中的 **clean目标**。</span>

而：

```bash
make -f clean
```

意思完全不同：

> <span style="background:#00FF80;">把名字叫 `clean` 的这个**文件当成 Makefile读取**。</span>

所以：

```text
make clean
     ↑
     目标

make -f clean
        ↑
        Makefile文件
```

再比如：

```bash
make -f build.mk clean
```

完整意思：

```text
使用 build.mk
       ↓
寻找里面的 clean 目标
       ↓
执行 clean
```

这个组合以后你会经常遇到。

------

## 5. Makefile 有固定后缀吗？

**没有。**

这是和 C 文件非常不一样的地方。

C语言：

```text
xxx.c     → 源文件
xxx.h     → 头文件
xxx.o     → 目标文件
xxx.a     → 静态库
xxx.so    → 动态库
```

这些通常通过后缀就很好判断。

但是 Makefile：

```text
Makefile
```

通常：

> <span style="background:#00FF80;">**没有扩展名。**</span>

<span style="color:#FF0000; background:#00FF80;">当然你也经常会看到：</span>

```text
rules.mk
config.mk
common.mk
```

<span style="color:#FF0000; background:#00FF80;">`.mk` 是一种非常常见的习惯命名，意思大概就是：</span>

```text
make相关文件
```

但：

> `.mk` 不是 GNU make 强制要求的后缀。

比如：

```text
hello.xyz
```

<span style="color:#FF0000; background:#00FF80;">只要里面内容是：</span>

```makefile
all:
	echo hello
```

<span style="color:#FF0000;">执行：</span>

```bash
make -f hello.xyz
```

<span style="color:#FF0000; background:#00FF80;">照样可以。</span>

------

## 6. 那我看到一个文件，怎么知道它是不是 Makefile？

这就有点意思了。

严格来说：

> **单靠文件名或后缀，不一定能100%判断。**

因为 Makefile 本质就是一个普通文本文件。

比如文件叫：

```text
abc.txt
```

里面却写：

```makefile
test: a.o b.o
	gcc -o test a.o b.o

%.o: %.c
	gcc -c -o $@ $<

.PHONY: clean

clean:
	rm -f *.o test
```

那么：

```bash
make -f abc.txt
```

完全可以正常使用。

所以判断是不是 Makefile，通常看两方面。

### 第一，看名字

看到这些：

```text
Makefile
makefile
GNUmakefile
xxx.mk
```

基本可以高度怀疑它是 make 相关文件。

其中：

```text
Makefile
```

最典型。

### 第二，看内容

打开文件以后，如果看到：

```makefile
目标: 依赖
	命令
```

比如：

```makefile
test: main.o add.o
	gcc -o test main.o add.o
```

或者：

```makefile
CC = gcc
CFLAGS = -Wall

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
```

出现这些东西：

```text
目标: 依赖
<TAB>命令

$(变量)

$@
$<
$^

.PHONY

%.o: %.c
```

基本一眼就能认出：

> “哦，这是 Makefile 语法。”

------

## 7. 这里还有个容易误解的地方

你可能会想：

> 那系统是不是根据 Makefile 的“文件类型”判断它？

其实不是。

比如：

```bash
make
```

GNU make 并不是扫描：

```text
当前目录所有文件
↓
检查谁里面写的是 Makefile语法
↓
选择它
```

<span style="color:#FF0000; background:#00FF80;">它没这么智能。</span>

<span style="color:#FF0000; background:#00FF80;">它其实就是按照约定的名字找：</span>

```text
GNUmakefile
makefile
Makefile
```

<span style="color:#FF0000;">所以如果你写：</span>

```text
我的编译文件
```

<span style="background:#00FF80;">内容完全正确：</span>

```makefile
test:
	gcc -o test main.c
```

直接：

```bash
make
```

make 还是会说找不到 Makefile。

必须：

```bash
make -f 我的编译文件
```

------

## 8. 给你一个完整场景

假设当前目录：

```text
project/
├── main.c
├── add.c
├── Makefile
├── debug.mk
└── release.mk
```

### 输入

```bash
make
```

相当于：

```text
当前目录
 ↓
寻找 GNUmakefile
没有
 ↓
寻找 makefile
没有
 ↓
寻找 Makefile
有！
 ↓
读取 Makefile
 ↓
执行默认目标
```

------

输入：

```bash
make -f debug.mk
```

则：

```text
不用再自动寻找
 ↓
直接读取 debug.mk
 ↓
执行它的默认目标
```

------

输入：

```bash
make -f release.mk clean
```

则：

```text
读取 release.mk
 ↓
寻找 clean 目标
 ↓
执行 clean
```

------

## 9. 你可以把整个关系这样记

```text
                    make
                      │
       ┌──────────────┴─────────────┐
       │                            │
    没有 -f                       有 -f
       │                            │
       ↓                            ↓
自动寻找                       使用指定文件
       │
GNUmakefile
   ↓
makefile
   ↓
Makefile
       │
       ↓
读取里面的目标和规则
       │
       ↓
make           → 默认目标
make clean     → clean目标
make test      → test目标
```

------

## 最后给你压缩成一句特别好记的

```text
Makefile没有强制后缀，
它本质就是普通文本文件。

直接 make：
按 GNUmakefile → makefile → Makefile 顺序寻找。

make 目标名：
选择这个 Makefile 里的某个目标。

make -f 文件名：
指定“使用哪个 Makefile”。

make -f 文件名 目标名：
既指定 Makefile，又指定目标。
```

所以以后你看到像 `Makefile`、`rules.mk`、`config.mk` 这种文件，先别把 `.mk` 当成什么特殊二进制格式——**它们本质上基本都是给 `make` 读取的文本规则文件。**
