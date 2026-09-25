# 视频分析：4_3-5《Makefile 实例》

这节大约 **21分13秒**。它非常重要，因为前面几节讲的 Makefile 知识，在这一节第一次真正组合起来了。

这节的主线不是再教新语法，而是解决一个实际工程问题：

> <span style="background:#00FF80;">**`.c` 文件变化，make 会重新编译；但是 `.h` 头文件变化时，怎样让 make 自动知道哪些 `.o` 也应该重新编译？**</span>

然后老师一步步引出了：

```text
gcc 自动生成依赖关系
        ↓
.d 依赖文件
        ↓
include 到 Makefile
        ↓
自动处理头文件依赖
        ↓
CFLAGS 统一管理编译选项
```

------

## 1. 视频类型判断

**主要类型：教程类 / 实例分析类**

**次要类型：Makefile 工程实践类**

判断依据：

1. 不再单独介绍 `%`、`$@`、函数等语法，而是把它们组合起来。
2. 通过 `a.c / b.c / c.c / c.h` 的真实工程演示头文件依赖。
3. 使用 GCC 的 `-M / -MD / -MF` 自动生成依赖。
4. 后半部分加入 `CFLAGS`，最后查看一个更大的实际 Makefile。

------

## 2. 一句话总结

> <span style="background:#00FF80;">**这节是在教你让 Makefile 不仅知道 `.c → .o` 的依赖，还能自动知道 `.o` 依赖了哪些 `.h`，从而真正实现可靠的增量编译。**</span>

这个问题是前面那个简单 Makefile 的一个明显缺口。

------

## 3. 时间段拆解

| 时间段          | 内容概括                                         | 核心作用                     |
| --------------- | ------------------------------------------------ | ---------------------------- |
| **00:00-02:30** | 打开实例工程，查看 `a.c/b.c/c.c/c.h` 和 Makefile | 建立工程                     |
| **02:30-04:30** | 手工给 `c.o` 增加 `c.h` 依赖                     | 暴露“头文件依赖要手写”的问题 |
| **04:30-07:30** | 使用 GCC 生成依赖信息                            | 引出 `-M/-MD/-MF`            |
| **07:30-10:00** | 编译 `.o` 时同时生成 `.d` 文件                   | 自动保存依赖                 |
| **10:00-13:00** | 使用 `patsubst`、`wildcard` 得到 `.d` 文件列表   | 把上一节函数用起来           |
| **13:00-15:30** | 使用 `include` 把 `.d` 依赖读回 Makefile         | 自动建立头文件依赖           |
| **15:30-16:30** | 添加 `clean/distclean` 清理规则并测试            | 完善 Makefile                |
| **16:30-19:30** | 引入 `CFLAGS`，使用 `-Werror`、`-Iinclude`       | 统一管理编译参数             |
| **19:30-21:13** | 查看一个更大型的真实 Makefile                    | 展示实际工程中的写法         |

------

## 4. 为什么以前的 Makefile 还“不够好”？

前面我们可能写：

```makefile
objs = a.o b.o c.o

test: $(objs)
	gcc -o test $^

%.o: %.c
	gcc -c -o $@ $<
```

看起来已经不错了：

```text
a.c → a.o
b.c → b.o
c.c → c.o
```

但是现在假设：

```c
// c.c
#include "c.h"
```

那么真正的关系其实是：

```text
c.c ─────┐
         ├──→ c.o
c.h ─────┘
```

而原来的：

```makefile
%.o: %.c
```

只告诉 make：

```text
c.o 依赖 c.c
```

却没告诉它：

```text
c.o 还依赖 c.h
```

于是问题出现了。

------

## 5. 如果只修改 `c.h` 会怎么样？

假设：

```text
10:00  c.c
10:10  c.o
10:20  c.h  ← 后来修改
```

实际上：

```text
c.h 修改
 ↓
c.c 编译出来的结果应该变化
 ↓
c.o 应该重新编译
```

但是 Makefile 如果只有：

```makefile
c.o: c.c
```

make 只比较：

```text
c.c 和 c.o
```

它根本不知道 `c.h` 的存在。

结果可能是：

```text
c.h 改了
 ↓
make
 ↓
“c.c 没改啊”
 ↓
c.o 不重新编译   ← 错
```

这就是这一节真正要解决的问题。

------

## 6. 最简单的方法：手工写头文件依赖

视频前面先使用类似：

```makefile
c.o: c.c c.h
```

于是：

```text
目标：
c.o

依赖：
c.c
c.h
```

现在只要：

```text
c.c 更新
或者
c.h 更新
```

都能导致：

```text
c.o 重新生成
```

这确实能解决问题。

但是又有新问题：

> 工程里面如果有几百个 `.c` 和几百个 `.h`，难道我要自己研究每个 `.c` include 了哪些 `.h`，然后全部手写吗？

显然很麻烦。

所以老师接下来开始利用 **GCC 自动分析依赖关系**。

------

## 7. `gcc -M c.c` 是干什么的？

视频大约 **04:30-07:00** 开始演示这一块。

例如：

```bash
gcc -M c.c
```

GCC 可以分析：

```c
#include <stdio.h>
#include "c.h"
```

等内容，然后输出类似：

```makefile
c.o: c.c c.h /usr/include/stdio.h ...
```

注意这个输出非常有意思。

它本身就是：

```makefile
目标: 依赖
```

的形式。

也就是说：

> **GCC 本身就知道一个 `.c` 文件编译时到底依赖哪些头文件。**

所以我们根本不需要自己一个一个找。

------

## 8. `-MF` 又是什么？

例如：

```bash
gcc -M -MF c.d c.c
```

可以简单理解：

```text
-M
→ 帮我分析依赖

-MF c.d
→ 不要只打印到屏幕
→ 把依赖结果写到 c.d 文件
```

于是产生：

```text
c.d
```

文件。

里面保存类似：

```makefile
c.o: c.c c.h ...
```

所以 `.d` 可以先理解成：

> **dependency file，依赖关系文件。**

它实际上通常就是一小段 Makefile 规则。

------

# 9. `-MD`：编译的同时生成依赖

更实用的方法是：

```bash
gcc -c -o c.o c.c -MD -MF c.d
```

这一次一次干两件事情：

```text
                  gcc
                   │
           ┌───────┴────────┐
           ↓                ↓
        编译 c.c          分析依赖
           ↓                ↓
          c.o              c.d
```

也就是说：

> 每次正常编译 `.c → .o` 时，顺便把它依赖哪些头文件记录下来。

这就非常适合 Makefile。

------

# 10. 视频中最终把它塞进模式规则

老师把原来的：

```makefile
%.o: %.c
	gcc -c -o $@ $<
```

改成类似：

```makefile
%.o: %.c
	gcc -c -o $@ $< -MD -MF .$@.d
```

这里你现在已经能看懂前半部分：

```text
$@ = 当前目标
$< = 第一个依赖
```

如果现在编译：

```text
a.o
```

那么：

```text
$@ = a.o
$< = a.c
```

最终相当于：

```bash
gcc -c -o a.o a.c -MD -MF .a.o.d
```

于是同时生成：

```text
a.o
.a.o.d
```

------

编译 `b.o`：

```bash
gcc -c -o b.o b.c -MD -MF .b.o.d
```

得到：

```text
b.o
.b.o.d
```

------

编译 `c.o`：

```text
c.o
.c.o.d
```

于是目录最终类似：

```text
a.c
b.c
c.c

a.o
b.o
c.o

.a.o.d
.b.o.d
.c.o.d
```

------

# 11. 那 `.d` 文件生成以后怎么让 make 使用？

这是下一步最关键的地方。

生成：

```text
.a.o.d
.b.o.d
.c.o.d
```

还不够。

Make 必须把这些文件里的规则读进来。

所以需要：

```makefile
include ...
```

但首先得知道有哪些 `.d`。

------

# 12. `patsubst` 在这里终于真正派上用场

原来：

```makefile
objs = a.o b.o c.o
```

老师使用类似：

```makefile
dep_files := $(patsubst %,.%.d,$(objs))
```

来转换。

输入：

```text
a.o b.o c.o
```

经过：

```text
% → .%.d
```

变成：

```text
.a.o.d
.b.o.d
.c.o.d
```

所以：

```text
objs
 ↓ patsubst
dep_files
```

关系就是：

```text
a.o → .a.o.d
b.o → .b.o.d
c.o → .c.o.d
```

这就是上一节 `patsubst` 真正在工程里的应用。

------

# 13. 为什么这里还要用 `wildcard`？

视频接下来又有：

```makefile
dep_files := $(wildcard $(dep_files))
```

这个地方非常值得理解。

第一次编译以前：

```text
.a.o.d
.b.o.d
.c.o.d
```

这些文件根本不存在。

但是：

```text
dep_files
```

理论上已经计算出了它们的名字。

所以 `wildcard` 的作用就是：

> **只把实际已经存在的依赖文件留下来。**

比如第一次：

```text
理论列表：
.a.o.d .b.o.d .c.o.d

实际磁盘：
一个都没有

wildcard 后：
空
```

编译一次以后：

```text
实际磁盘：
.a.o.d
.b.o.d
.c.o.d

wildcard 后：
.a.o.d .b.o.d .c.o.d
```

这正好呼应上一节：

```text
patsubst
→ 处理字符串名字

wildcard
→ 检查这些文件实际上存不存在
```

------

# 14. `include $(dep_files)` 是这套机制真正闭环的地方

视频最终写类似：

```makefile
ifneq ($(dep_files),)
include $(dep_files)
endif
```

这里的：

```makefile
include
```

可以简单理解成：

> **把另一个 Makefile/Makefile片段的内容，插进当前 Makefile。**

假设：

```text
.c.o.d
```

里面写：

```makefile
c.o: c.c c.h
```

那么：

```makefile
include .c.o.d
```

以后，相当于当前 Makefile 里面也拥有：

```makefile
c.o: c.c c.h
```

于是 make 终于知道：

```text
                  c.o
                 /   \
               c.c   c.h
```

这样以后：

```text
修改 c.h
   ↓
make 发现 c.h 比 c.o 新
   ↓
重新执行 c.o 的编译规则
   ↓
重新生成 c.o
   ↓
重新链接 test
```

整个自动头文件依赖机制就闭环了。

------

# 15. 把这套机制完整串起来

这是这节最重要的一张逻辑图：

```text
                 c.c
                  │
                  │ gcc编译
                  ↓
                 c.o
                  │
          同时用 -MD/-MF
                  ↓
               .c.o.d
                  │
          里面记录类似：
                  │
           c.o: c.c c.h
                  │
                  ↓
      Makefile include .c.o.d
                  │
                  ↓
      make知道 c.o 还依赖 c.h
                  │
                  ↓
              修改 c.h
                  │
                  ↓
          自动重新编译 c.o
```

这就是这节的核心。

------

# 16. `clean` 和 `distclean`

视频中 Makefile 后来出现类似：

```makefile
clean:
	rm *.o test
```

以及：

```makefile
distclean:
	rm $(dep_files)
```

可以把它们理解为两个程度不同的清理动作。

### `clean`

清理主要构建结果：

```text
*.o
test
```

### `distclean`

进一步清除自动产生的依赖文件：

```text
.a.o.d
.b.o.d
.c.o.d
```

也就是说：

```text
clean
→ 清主要编译结果

distclean
→ 再清理辅助生成文件
```

实际项目中具体怎么定义由 Makefile 作者决定，并不是 `distclean` 这个名字天然自带某个固定操作。

------

# 17. 后面为什么突然讲 `CFLAGS`？

大约 **16:30以后**，老师开始把：

```text
编译参数
```

也抽出来。

例如：

```makefile
CFLAGS = -Werror
```

编译规则改成：

```makefile
%.o: %.c
	gcc $(CFLAGS) -c -o $@ $< -MD -MF .$@.d
```

这样：

```text
CFLAGS
```

就集中保存了编译 C 文件使用的选项。

以后不用在每一条 gcc 命令里重复修改。

------

# 18. `-Werror` 是什么？

视频实际演示：

```makefile
CFLAGS = -Werror
```

`-Werror` 的核心作用：

> **把编译器的 warning（警告）当成 error（错误）处理。**

例如代码里出现：

```text
implicit declaration of function ...
```

原来可能只是：

```text
warning
```

程序还能继续编译。

加入：

```text
-Werror
```

以后就会变成：

```text
error
```

然后：

```text
编译失败
```

这可以迫使程序员及时修复潜在问题。

------

# 19. `CFLAGS` 和你前面学的 `-I` 又串起来了

视频后面又修改：

```makefile
CFLAGS = -Werror -Iinclude
```

这里：

```text
-Werror
→ 警告当错误

-Iinclude
→ 增加头文件搜索目录 include
```

如果项目结构：

```text
project/
├── a.c
├── b.c
├── c.c
├── include/
│   └── c.h
└── Makefile
```

那么编译：

```bash
gcc -Iinclude ...
```

就是告诉 GCC：

> 除了默认头文件目录之外，也去当前工程的 `include/` 目录寻找头文件。

所以这又和你之前一直问的：

```text
-I
头文件搜索路径
```

完全接上了。

------

# 20. `CFLAGS` 本质是什么？

这一点别把它想复杂。

```makefile
CFLAGS = -Werror -Iinclude
```

本质就是：

> **一个普通 Makefile 变量。**

GNU make 并不是看到名字叫 `CFLAGS` 才允许使用。

你完全可以写：

```makefile
ABC = -Werror -Iinclude
```

然后：

```makefile
gcc $(ABC) -c ...
```

一样可以。

只不过：

```text
CFLAGS
```

是大家约定俗成用于保存 **C编译器选项** 的常见变量名。

以后还经常看到：

```text
CC
CFLAGS
CPPFLAGS
LDFLAGS
LDLIBS
```

这些都属于构建系统里很常见的约定名称。

------

# 21. 最终 Makefile 的骨架

视频中逐步形成的结构大致可以整理成：

```makefile
objs = a.o b.o c.o

dep_files := $(patsubst %,.%.d,$(objs))
dep_files := $(wildcard $(dep_files))

CFLAGS = -Werror -Iinclude

test: $(objs)
	gcc -o test $^

ifneq ($(dep_files),)
include $(dep_files)
endif

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $< -MD -MF .$@.d

clean:
	rm *.o test

distclean:
	rm $(dep_files)

.PHONY: clean
```

这份 Makefile 已经比前几节的：

```makefile
test: a.o b.o c.o
	gcc -o test a.o b.o c.o
```

成熟很多了。

------

# 22. 逐层看它到底干了什么

### 第一层：有哪些 `.o`

```makefile
objs = a.o b.o c.o
```

------

### 第二层：这些 `.o` 对应哪些依赖文件

```makefile
dep_files := $(patsubst %,.%.d,$(objs))
```

得到：

```text
.a.o.d .b.o.d .c.o.d
```

------

### 第三层：只找真正已经存在的

```makefile
dep_files := $(wildcard $(dep_files))
```

------

### 第四层：最终程序依赖所有 `.o`

```makefile
test: $(objs)
	gcc -o test $^
```

------

### 第五层：加载头文件依赖

```makefile
include $(dep_files)
```

------

### 第六层：统一编译 `.c → .o`

```makefile
%.o: %.c
	gcc $(CFLAGS) -c -o $@ $< -MD -MF .$@.d
```

并顺手生成：

```text
.d
```

------

### 第七层：清理

```makefile
clean:
	...

distclean:
	...
```

整个 Makefile 已经形成一个完整构建流程。

------

# 23. 最后为什么给你看那么大的 Makefile？

视频最后大约 **19:30以后** 打开了一个明显更大的实际 Makefile。

里面出现大量：

```makefile
objs += ...
objs += ...
objs += ...
```

例如 LCD、ADC touchscreen、I2C 等模块的目标文件。

以及：

```makefile
all: $(objs)
```

再调用类似：

```text
arm-linux-ld
arm-linux-objcopy
arm-linux-objdump
```

这些工具。

老师真正想让你看到的是：

> 前面学的那些“小例子语法”，到了真正的嵌入式工程里还是同一套思想。

只不过：

```text
3个 .o
```

变成：

```text
几十个甚至几百个 .o
```

普通：

```text
gcc
```

可能变成：

```text
arm-linux-gcc
arm-linux-ld
```

但：

```text
目标
依赖
命令
变量
模式规则
自动变量
```

这些基本思想完全没有变。

------

# 24. 内容结构

整体表达路径：

```text
从【简单 Makefile 的缺陷】
          ↓
发现【头文件变化无法自动触发编译】
          ↓
先【手工写 .h 依赖】
          ↓
发现【大型工程手写不现实】
          ↓
利用【gcc 自动生成依赖】
          ↓
产生【.d 文件】
          ↓
利用【patsubst + wildcard】
          ↓
利用【include 加载依赖】
          ↓
加入【CFLAGS统一编译选项】
          ↓
最后看【实际大型 Makefile】
```

------

# 25. 这节你最应该重点理解的地方

如果这节只让我让你带走 **3个知识点**，我会选：

### 第一：`.o` 不仅依赖 `.c`

真实关系通常是：

```text
             main.c
               │
        ┌──────┴──────┐
        ↓             ↓
     main.h         xxx.h
        \             /
         \           /
          ↓         ↓
            main.o
```

所以头文件变了，也可能必须重新编译。

------

### 第二：`.d` 文件就是让 GCC 帮 Makefile 记录依赖

```text
gcc
 ↓
自动分析 include
 ↓
生成 .d
 ↓
.d 里面写“谁依赖谁”
 ↓
Makefile include 它
```

------

### 第三：Makefile 开始真正“自动化”

前面：

```text
自己写依赖
```

现在：

```text
GCC 自动发现依赖
+
函数自动生成文件列表
+
Make 自动判断更新时间
```

这才是一个比较完整的构建系统思路。

------

# 26. 记忆版总结

```text
类型：
Makefile综合实例

核心问题：
.h 改了以后，怎么让对应 .o 自动重新编译？

解决方案：

.c
 ↓ gcc -MD/-MF
.o + .d
      ↓
.d记录：
.o : .c .h
      ↓
include进Makefile
      ↓
make自动知道头文件依赖

关键工具：

-M
→ 输出依赖

-MF
→ 指定依赖文件

-MD
→ 编译同时生成依赖

patsubst
→ 从 .o 名称构造 .d 名称

wildcard
→ 找真正存在的 .d

include
→ 把 .d 中的规则读进Makefile

CFLAGS
→ 集中保存 gcc 编译参数

-Werror
→ warning 当 error

-Iinclude
→ 增加头文件搜索目录
```

### 一句话记忆

> **前几节解决的是“怎么自动编译 `.c → .o`”，这一节进一步解决“怎么自动知道 `.o` 还依赖哪些 `.h`”。有了 `.d + include`，Makefile 才真正能可靠地管理头文件变化。**
