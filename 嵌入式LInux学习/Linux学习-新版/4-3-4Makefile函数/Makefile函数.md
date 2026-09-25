# 视频分析：4_3-4《Makefile 函数》

这节大约 **12分43秒**，正好接着上一节的“Makefile 变量”继续往下讲。

这一节最值得你抓住的不是“Makefile 还有函数”这么简单，而是：

> <span style="color:#FF0000;">**Makefile 函数主要是在帮我们批量处理“文件名列表”。**</span>

老师重点讲了 4 类函数：

```makefile
$(foreach var,list,text)
$(filter pattern...,text)
$(filter-out pattern...,text)
$(wildcard pattern)
$(patsubst pattern,replacement,text)
```

它们以后写稍微复杂一点的 Makefile 会非常常见。

------

## 1. 视频类型判断

**主要类型：教程类 / 讲解类**

**次要类型：代码演示类**

判断依据：

1. 老师先查 GNU make 官方手册，再解释函数语法。
2. 每个函数都通过变量和 `echo` 实际观察结果。
3. 重点围绕“列表处理、文件匹配、文件名转换”展开。

------

## 2. 一句话总结

> <span style="background:#00FF80;">Makefile 函数就是帮助 make 对一堆字符串、文件名进行**遍历、筛选、查找和批量替换**，从而避免我们手工一个一个写文件名。</span>

可以先把这几个函数记成：

| 函数         | 最简单理解                   |
| ------------ | ---------------------------- |
| `foreach`    | **每个都处理一遍**           |
| `filter`     | **把符合条件的留下**         |
| `filter-out` | **把符合条件的删掉**         |
| `wildcard`   | **去目录里找实际存在的文件** |
| `patsubst`   | **批量改名字/后缀**          |

------

## 3. 时间段拆解

| 时间段          | 内容                         | 核心                  |
| --------------- | ---------------------------- | --------------------- |
| **00:00-01:20** | 回顾变量并查阅 GNU make 文档 | 引入 Makefile 函数    |
| **01:20-03:30** | `foreach`                    | 遍历列表              |
| **03:30-06:40** | `filter`、`filter-out`       | 筛选列表              |
| **06:40-09:20** | `wildcard`                   | 查找实际存在的文件    |
| **09:20-12:20** | `patsubst`                   | 批量字符串/文件名替换 |
| **12:20-12:43** | 返回笔记总结                 | 汇总常用函数          |

------

## 4. 第一个函数：`foreach`

视频前半部分使用类似：

```makefile
A = a b c

B = $(foreach f,$(A),$(f).o)
```

最终：

```text
B = a.o b.o c.o
```

它的标准格式：

```makefile
$(foreach var,list,text)
```

翻译成人话：

> <span style="color:#FF0000; background:#00FF80;">从 `list` 里面一个一个取东西，每次放进变量 `var`，然后按照 `text` 描述的方法处理。</span>

------

### 把视频中的例子拆开

```makefile
A = a b c
```

那么：

```makefile
$(foreach f,$(A),$(f).o)
```

相当于循环：

```text
第一次：
f = a
$(f).o → a.o

第二次：
f = b
$(f).o → b.o

第三次：
f = c
$(f).o → c.o
```

最后拼起来：

```text
a.o b.o c.o
```

------

### 你可以把它类比成 C 语言

Makefile：

```makefile
$(foreach f,$(A),$(f).o)
```

思想上有点像：

```c
for (每一个 f in A) {
    把 f 变成 f.o;
}
```

当然 Makefile 并不是 C 语言，这只是帮助理解。

所以记：

> <span style="color:#FF0000; background:#00FF80;">`foreach` = **遍历列表，对每一个元素执行相同的字符串处理。**</span>

------

## 5. 为什么这个函数对 Makefile 特别有用？

假设工程源文件：

```text
a.c
b.c
c.c
```

我们需要：

```text
a.o
b.o
c.o
```

你当然可以手写：

```makefile
objs = a.o b.o c.o
```

但文件多起来：

```text
a.c
b.c
c.c
...
100.c
```

就很麻烦。

这时候 Makefile 的思维变成：

```text
我已经有一份文件列表
        ↓
用函数批量处理
        ↓
得到另一份列表
```

这是这一节真正开始建立的思想。

------

## 6. 第二个函数：`filter`

视频中使用类似：

```makefile
C = a b c d/

D = $(filter %/,$(C))
```

这里：

```text
%/
```

表示：

> <span style="color:#FF0000; background:#00FF80;">匹配“以 `/` 结尾”的字符串。</span>

所以：

```text
a
b
c
d/
```

只有：

```text
d/
```

符合。

最终：

```text
D = d/
```

------

### 标准格式

```makefile
$(filter pattern...,text)
```

可以翻译：

> <span style="color:#FF0000; background:#00FF80;">从 `text` 里面找出所有符合 `pattern` 的内容，并把它们留下来。</span>

所以：

```text
filter
   ↓
符合条件的
   ↓
留下
```

------

## 7. `%` 在这里又是什么意思？

上一节你刚学过：

```makefile
%.o: %.c
```

里面<span style="color:#FF0000; background:#00FF80;"> `%` 是模式匹配。</span>

这里也类似：

```makefile
%/
```

表示：

```text
任意内容 + /
```

所以：

```text
abc/   √
home/  √
123/   √

abc    ×
a.c    ×
```

------

## 8. `filter-out` 正好相反

视频接着写类似：

```makefile
E = $(filter-out %/,$(C))
```

还是：

```text
C = a b c d/
```

但是现在意思变成：

> <span style="color:#FF0000; background:#00FF80;">把所有符合 `%/` 的删掉。</span>

所以：

```text
d/
```

被删除。

最终：

```text
E = a b c
```

------

### 两个放一起最好记

```text
原始列表：

a  b  c  d/
         ↑
        符合 %/
```

执行：

```makefile
$(filter %/,$(C))
```

得到：

```text
d/
```

而：

```makefile
$(filter-out %/,$(C))
```

得到：

```text
a b c
```

所以：

```text
filter     → 要符合条件的

filter-out → 要不符合条件的
```

------

## 9. 第三个函数：`wildcard`

这个函数特别重要，而且<span style="color:#FF0000; background:#00FF80;">很容易和 `filter` 混。</span>

标准格式：

```makefile
$(wildcard pattern)
```

例如：

```makefile
files = $(wildcard *.c)
```

它不是单纯在处理字符串。

它会真的去：

> <span style="color:#FF0000; background:#00FF80;">**当前目录里寻找实际存在、并且匹配 `\*.c` 的文件。**</span>

假设目录：

```text
a.c
b.c
c.c
hello.h
Makefile
test
```

那么：

```makefile
files = $(wildcard *.c)
```

得到：

```text
a.c b.c c.c
```

------

## 10. `wildcard` 和 `filter` 的区别非常重要

你现在最好就把它们分开。

### `filter`

针对的是：

> <span style="color:#FF0000; background:#00FF80;">**已经存在于变量里的“字符串列表”。**</span>

例如：

```makefile
A = a.c b.c hello.h

B = $(filter %.c,$(A))
```

得到：

```text
a.c b.c
```

它没有必要去硬盘上检查：

```text
a.c 到底真的存在吗？
```

它只是过滤字符串。

------

### `wildcard`

针对的是：

> <span style="color:#FF0000; background:#00FF80;">**文件系统。**</span>

```makefile
A = $(wildcard *.c)
```

<span style="color:#FF0000; background:#00FF80;">它会去当前目录寻找真正存在的：</span>

```text
*.c
```

<span style="color:#FF0000; background:#00FF80;">文件。</span>

所以：

```text
filter
→ 筛字符串

wildcard
→ 找真实文件
```

这是一个特别好的区分。

------

## 11. 视频里的 `wildcard` 实验还有一个很有意思的点

老师不仅写类似：

```makefile
files = $(wildcard *.c)
```

还构造了一份名字列表，比如：

```makefile
files2 = a.c b.c c.c d.c e.c
```

然后再把它交给 `wildcard`。

这里想说明：

> <span style="color:#FF0000; background:#00FF80;">`wildcard` 会按照给出的模式/文件名去实际文件系统查找，**不存在的文件不会出现在结果中。**</span>

假设：

```text
a.c √
b.c √
c.c √
d.c ×
e.c ×
```

那么<span style="background:#00FF80;">最终不会因为变量里写了：</span>

```text
d.c e.c
```

<span style="background:#00FF80;">就凭空把它们当成存在。</span>

这<span style="color:#FF0000; background:#00FF80;">也是它和纯字符串处理函数的本质区别。</span>

------

## 12. 第四个：`patsubst`

这是这一节最值得重点学的函数之一。

标准格式：

```makefile
$(patsubst pattern,replacement,text)
```

名字可以拆开：

```text
pat      = pattern
subst    = substitute

patsubst
= pattern substitution
= 按模式替换
```

------

## 13. 视频中的例子

类似：

```makefile
files2 = a.c b.c c.c d.c e.c abc

dep_files = $(patsubst %.c,%.d,$(files2))
```

意思：

> <span style="color:#FF0000; background:#00FF80;">把 `files2` 中所有符合 `%.c` 的字符串，变成 `%.d`。</span>

于是：

```text
a.c → a.d
b.c → b.d
c.c → c.d
d.c → d.d
e.c → e.d
```

而：

```text
abc
```

不符合：

```text
%.c
```

所以不改变。

最终：

```text
a.d b.d c.d d.d e.d abc
```

------

## 14. `%` 在 `patsubst` 里到底发生了什么？

例如：

```makefile
$(patsubst %.c,%.o,a.c)
```

首先：

```text
pattern = %.c
```

拿：

```text
a.c
```

匹配。

这里：

```text
% = a
```

然后 replacement：

```text
%.o
```

把刚才 `%` 捕获到的：

```text
a
```

放进去：

```text
a + .o
↓
a.o
```

所以：

```text
a.c
 ↓
%.c 中 % = a
 ↓
%.o
 ↓
a.o
```

这和你上一节的：

```makefile
%.o: %.c
```

其实是<span style="color:#FF0000; background:#00FF80;">同一类模式思想。</span>

------

## 15. `foreach` 和 `patsubst` 看起来都能改文件名，有什么区别？

这是学完这节很容易产生的疑问。

比如：

```text
a.c b.c c.c
```

我要变成：

```text
a.o b.o c.o
```

### 用 `foreach`

可能写：

```makefile
$(foreach f,...)
```

它强调：

> <span style="color:#FF0000; background:#00FF80;">**一个一个遍历，然后对每一个执行表达式。**</span>

功能更灵活。

------

### 用 `patsubst`

写：

```makefile
$(patsubst %.c,%.o,$(files))
```

它强调：

> <span style="color:#FF0000; background:#00FF80;">**符合某种模式的字符串统一替换。**</span>

如果只是：

```text
.c → .o
```

<span style="background:#00FF80; color:#FF0000;">这种规则性的后缀转换，`patsubst` 会非常自然。</span>

------

## 16. 把这四类函数放到一个实际工程里

假设目录：

```text
project/
├── main.c
├── add.c
├── sub.c
├── test.txt
├── Makefile
```

我们想自动找所有 `.c`：

```makefile
srcs := $(wildcard *.c)
```

得到：

```text
main.c add.c sub.c
```

然后：

```makefile
objs := $(patsubst %.c,%.o,$(srcs))
```

得到：

```text
main.o add.o sub.o
```

然后：

```makefile
test: $(objs)
	gcc -o $@ $^
```

再配合上一节：

```makefile
%.o: %.c
	gcc -c -o $@ $<
```

最终：

```makefile
srcs := $(wildcard *.c)
objs := $(patsubst %.c,%.o,$(srcs))

test: $(objs)
	gcc -o $@ $^

%.o: %.c
	gcc -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(objs) test
```

你注意这里发生了一个非常大的变化。

以前你要写：

```makefile
test: main.o add.o sub.o
```

以后每多一个：

```text
hello.c
```

还得修改 Makefile：

```makefile
test: main.o add.o sub.o hello.o
```

<span style="color:#FF0000; background:#00FF80;">但是现在：</span>

```makefile
srcs := $(wildcard *.c)
objs := $(patsubst %.c,%.o,$(srcs))
```

<span style="color:#FF0000; background:#00FF80;">以后你往目录里放：</span>

```text
hello.c
```

<span style="color:#FF0000; background:#00FF80;">`wildcard` 自动找到：</span>

```text
main.c add.c sub.c hello.c
```

`patsubst` 自动变成：

```text
main.o add.o sub.o hello.o
```

你甚至不需要手动修改文件列表。

**这才是这一节 Makefile 函数真正解决的问题。**

------

## 17. 这几节视频其实已经串成了一条完整路线

你从 4_3-1 到现在学的东西，可以整理为：

```text
第一步：
为什么要 Makefile？

→ 大工程不能每次全部重新编译
```

然后：

```text
第二步：
目标 : 依赖
    命令

→ 描述文件之间的依赖关系
```

然后：

```text
第三步：
%.o : %.c
$@
$<
$^

→ 把重复的编译规则抽象掉
```

现在：

```text
第四步：
wildcard
patsubst
foreach
filter

→ 把重复的“文件列表管理”也自动化
```

所以 Makefile 是一步一步从：

```text
手工写每一个文件
```

发展成：

```text
让 make 自动发现文件
       ↓
自动生成文件列表
       ↓
自动推导依赖规则
       ↓
只编译需要重新编译的部分
```

这条主线很重要。

------

## 18. 内容结构分析

整体表达路径：

```text
从【Makefile变量】
        ↓
引入【Makefile函数】
        ↓
foreach：遍历
        ↓
filter/filter-out：筛选
        ↓
wildcard：寻找实际文件
        ↓
patsubst：批量转换文件名
        ↓
最终实现【自动构造文件列表】
```

------

## 19. 观点 / 教学结论

| 内容                                          | 归属   | 明确/可能 |
| --------------------------------------------- | ------ | --------- |
| Makefile 提供大量函数用于字符串和文件列表处理 | 讲解者 | 明确      |
| `foreach` 可以遍历列表                        | 讲解者 | 明确      |
| `filter` 保留符合模式的内容                   | 讲解者 | 明确      |
| `filter-out` 删除符合模式的内容               | 讲解者 | 明确      |
| `wildcard` 可以得到实际存在、符合模式的文件   | 讲解者 | 明确      |
| `patsubst` 可以按照模式进行批量替换           | 讲解者 | 明确      |

------

## <span style="background:#00FF80;">20. 你这节最应该记住的“人话版”</span>

```text
foreach
→ 每一个都处理一下

filter
→ 符合条件的留下

filter-out
→ 符合条件的扔掉

wildcard
→ 去目录里看看实际有哪些文件

patsubst
→ 按统一规律批量改名字
```

再配上例子：

```makefile
A = a b c

$(foreach f,$(A),$(f).o)
→ a.o b.o c.o
A = a.c b.c hello.h

$(filter %.c,$(A))
→ a.c b.c
$(filter-out %.c,$(A))
→ hello.h
$(wildcard *.c)
→ 当前目录真正存在的所有 .c 文件
$(patsubst %.c,%.o,a.c b.c)
→ a.o b.o
```

------

## 21. 记忆版总结

```text
类型：
Makefile函数教程

核心目的：
批量处理文件名和字符串列表

foreach：
遍历

filter：
留下匹配项

filter-out：
删除匹配项

wildcard：
查找真实存在的文件

patsubst：
按模式批量替换

最重要组合：

srcs := $(wildcard *.c)

objs := $(patsubst %.c,%.o,$(srcs))

作用：

目录中自动寻找 .c
        ↓
得到所有源文件
        ↓
自动把 .c 转成 .o 文件名
        ↓
交给链接目标使用
```

### 一句话记忆

> <span style="color:#FF0000;"><span style="background:#00FF80;">**上一节的 `%`、`$@/lt;/$^` 是在自动化“编译规则”，这一节的 `wildcard + patsubst` 则是在自动化“文件列表”。两者结合起来，Makefile 才开始真正具有管理大工程的能力。</span>**</span>



# 问题1

makefile文件中的 @echo 的作用是什么呢。



`@echo` 其实是 **两部分东西叠在一起**：

```makefile
@echo hello
```

可以拆成：

```text
@    +    echo hello
↑         ↑
make语法   shell命令
```

其中<span style="background:#00FF80;"> `echo` 的作用很简单：**在终端输出文字**。</span>

比如：

```makefile
all:
	echo hello
```

执行：

```bash
make
```

终端通常会看到：

```text
echo hello
hello
```

<span style="color:#FF0000; background:#00FF80;">为什么出现两行？</span>

因为 <span style="background:#00FF80;">make 默认会先把它准备执行的命令显示出来：</span>

```text
echo hello
```

然后 shell 真正执行 `echo hello`，输出：

```text
hello
```

所以你看到两行。

<span style="color:#FF0000; background:#00FF80;">而如果写成：</span>

```makefile
all:
	@echo hello
```

执行：

```bash
make
```

<span style="color:#FF0000; background:#00FF80;">你只会看到：</span>

```text
hello
```

这里的 `@` 就是在告诉 make：

> <span style="color:#FF0000; background:#00FF80;">**这条命令你执行就行，不要先把命令本身打印出来。**</span>

所以可以记成：

```text
echo
→ 输出内容

@
→ 隐藏这条命令本身
```

组合起来：

```makefile
@echo hello
```

就是：

> <span style="background:#00FF80;">**静默执行 echo，只显示 echo 的输出结果。**</span>

比如视频里经常会有这种：

```makefile
all:
	@echo A = $(A)
	@echo B = $(B)
```

假设：

```makefile
A = abc
B = 123
```

执行：

```bash
make
```

会直接看到：

```text
A = abc
B = 123
```

而不会看到：

```text
echo A = abc
echo B = 123
```

这就比较干净，特别适合老师演示“变量最后展开成什么值”。

还有一点很重要：**`@` 不是 `echo` 的一部分。**

你也可以写：

```makefile
all:
	@gcc -o test main.c
```

它的意思同样是：

> 执行 gcc，但不要把 `gcc -o test main.c` 这条命令打印出来。

或者：

```makefile
clean:
	@rm -f *.o test
```

也是一样。

所以最终你可以这样记：

```text
echo xxx
= 把 xxx 输出到终端

@命令
= 执行命令，但不显示命令本身

@echo xxx
= 只显示 xxx，不显示 “echo xxx”
```

顺便注意，Makefile 里的命令前面通常需要用 **Tab** 缩进，这个和 `@` 是两回事：

```makefile
all:
<Tab>@echo hello
```

Tab 是在告诉 make“这是目标下面的命令”，`@` 是告诉 make“执行时别把命令本身显示出来”。
