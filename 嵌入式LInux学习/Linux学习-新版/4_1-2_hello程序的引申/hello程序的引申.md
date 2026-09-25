# 视频分析：4_1-2 Hello程序的引申

> 视频时长约 **6分58秒**。这段视频不是继续演示 Hello 程序怎么运行，而是从 `#include <stdio.h>` 和 `printf()` 出发，解释 **头<span style="background:#00FF80;">文件、函数声明、函数实现、编译链接以及 Linux 软件分层</span>**。

## 1. 视频类型判断

**主要类型：** 讲解类、嵌入式 Linux 基础教程类
**次要类型：** C 语言原理分析类

**判断依据：**

1. 通过 `hello.c` 引出 `.h` 和 `.c` 文件的区别。
2. 通过 `add()` 举例<span style="background:#00FF80; color:#FF0000;">区分函数声明和函数实现</span>。
3. 解释<span style="color:#FF0000; background:#00FF80;">编译器如何找到头文件，以及常见编译、链接错误。</span>
4. 最后从 `printf()` 延伸到 libc、内核、驱动和硬件。

------

## 2. 一句话总结

```text
头文件告诉编译器“函数怎么使用”，
C 文件或库提供“函数具体怎么实现”，
应用程序再通过 libc 和系统调用进入 Linux 内核，最终操作硬件。
```

------

## 3. 时间段拆解

| 时间段      | 内容概括                                                     | 画面与板书                                                   | 作用                                                         |
| ----------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 00:00-00:40 | 从 `#include <stdio.h>` 引出 `.h` 与 `.c` 的区别             | 展示 `hello.c`，圈出 `stdio.h`，板书“.h/.c 差别”             | 提出核心问题                                                 |
| 00:40-02:10 | 解释函数声明                                                 | 写出 `int add(int a, int b);`，并用 `add(...)` 调用举例      | 说明<span style="color:#FF0000; background:#00FF80;">声明的作用</span> |
| 02:10-02:45 | 解释函数实现/定义                                            | 写出 `int add(...) { return a+b; }`                          | 区分<span style="color:#FF0000; background:#00FF80;">声明与实现</span> |
| 02:45-03:40 | 头文件保存在哪里                                             | 板书“<span style="background:#00FF80;">系统目录</span>”“<span style="background:#00FF80;">指定目录 `-I dir`</span>” | 解释<span style="color:#FF0000; background:#00FF80;">编译器的头文件搜索路径</span> |
| 03:40-05:15 | `printf` 的<span style="color:#FF0000; background:#00FF80;">声明和实现</span>分别在哪里 | 圈出 `printf()`，联系 `stdio.h`，总结常见的“<span style="background:#00FF80;">头文件找不到</span>”“<span style="background:#00FF80;">函数未定义</span>”问题 | 引入<span style="color:#FF0000; background:#00FF80;">编译和链接的区别</span> |
| 05:15-05:40 | 短暂切换网页后返回板书                                       | 画面过渡                                                     | 转向 Linux 软件结构                                          |
| 05:40-06:58 | <span style="color:#FF0000; background:#00FF80;">应用、libc、内核和硬件</span>的分层 | 板书 <span style="color:#FF0000; background:#00FF80;">APP、API、libc、`printf/open/read`、user、kernel、FS/driver、硬件</span> | 建立 <span style="background:#00FF80; color:#FF0000;">Linux 应用程序整体运行模型</span> |

------

## 4. 这段视频最核心的知识

### 4.1 `.h` 和 `.c` 到底有什么区别？

可以先用一句话记：

```text
.h 主要放“说明书”
.c 主要放“具体实现”
```

例如我们自己写一个加法函数。

#### `add.h`

```c
#ifndef ADD_H
#define ADD_H

int add(int a, int b);

#endif
```

这里的：

```c
int add(int a, int b);
```

只是**函数声明**，<span style="color:#FF0000; background:#00FF80;">它告诉编译器：</span>

```text
函数名：add
返回值：int
参数一：int
参数二：int
```

但是<span style="color:#FF0000; background:#00FF80;">它没有告诉编译器，加法函数内部究竟做什么。</span>

#### `add.c`

```c
#include "add.h"

int add(int a, int b)
{
    return a + b;
}
```

这里才是<span style="background:#00FF80;">函数的**定义或实现**。</span>

------

### 4.2 声明与实现分别有什么作用？

#### 声明：告诉编译器“它长什么样”

```c
int add(int a, int b);
```

<span style="color:#FF0000; background:#00FF80;">编译器看到这条声明后，就知道怎样检查下面的调用：</span>

```c
int result = add(1, 2);
```

它可以检查：

- <span style="color:#FF0000;">参数数量</span>是否正确；
- <span style="color:#FF0000;">参数类型</span>是否基本匹配；
- <span style="color:#FF0000;">返回值</span>是什么类型；
- 应该<span style="color:#FF0000;">怎样生成函数调用代码</span>。

#### 实现：告诉系统“它具体做什么”

```c
int add(int a, int b)
{
    return a + b;
}
```

真正运行时，需要执行这里面的机器指令。

所以：

```text
声明解决：怎么调用？
实现解决：调用后具体执行什么？
```

------

## 5. 为什么<span style="background:#00FF80;">有了声明还必须有实现</span>？

假设只有头文件：

```c
int add(int a, int b);
```

主程序写：

```c
#include "add.h"

int main(void)
{
    return add(1, 2);
}
```

<span style="color:#FF0000; background:#00FF80;">编译这一个源文件时，编译器通常能够通过，因为它知道 `add()` 的调用规则。</span>

但是<span style="background:#00FF80;">最终链接时，链接器会问：</span>

```text
add() 的真正代码在哪里？
```

假如<span style="color:#FF0000; background:#00FF80;">没有编译并链接 `add.c`，可能出现：</span>

```text
undefined reference to `add'
```

这表示：

```text
我知道 add() 是什么样的函数，
但是找不到它的具体实现。
```

正确编译：

```bash
gcc main.c add.c -o app
```

也可以先分别编译：

```bash
gcc -c main.c -o main.o
gcc -c add.c -o add.o
gcc main.o add.o -o app
```

过程是：

```text
main.c → main.o
add.c  → add.o

main.o + add.o → app
```

------

## 6. `#include <stdio.h>` 实际干了什么？

Hello 程序中有：

```c
#include <stdio.h>
```

很多初学者会以为：

```text
这条命令把 printf 的完整实现复制了进来。
```

实际通常不是这样。

`stdio.h` 中<span style="color:#FF0000; background:#00FF80;">主要提供的是：</span>

- `printf()` 等<span style="color:#FF0000; background:#00FF80;">函数的声明；</span>
- `FILE` 等<span style="color:#FF0000; background:#00FF80;">类型声明；</span>
- <span style="color:#FF0000; background:#00FF80;">宏定义</span>；
- <span style="color:#FF0000; background:#00FF80;">常量</span>；
- 与<span style="color:#FF0000; background:#00FF80;">标准输入输出相关的接口说明。</span>

可以简化理解为：

```c
int printf(const char *format, ...);
```

<span style="color:#FF0000; background:#00FF80;">`stdio.h` 告诉编译器：</span>

```text
printf 是一个可以调用的函数；
第一个参数是格式字符串；
它返回一个 int。
```

<span style="background:#00FF80;">但 `printf()` 的主要实现并不在 `stdio.h` 中，而是在 C 标准库里，<span style="color:#FF0000;">Linux 中通常是</span> **libc**。</span>

因此：

```text
stdio.h → printf 的说明书
libc    → printf 的主要实现
```

------

## 7.`#include` 的<span style="background:#00FF80;">本质是文本包含</span>

预处理器看到：

```c
#include <stdio.h>
```

<span style="color:#FF0000; background:#00FF80;">会找到 `stdio.h`，把需要的头文件内容包含进当前源文件，再交给编译器处理。</span>

大致过程：

```text
hello.c
   ↓ 预处理
展开 #include、宏定义、条件编译
   ↓
编译
   ↓
hello.o
   ↓
链接 libc 等需要的实现
   ↓
hello
```

因此 `#include` 和“链接库”是两个不同动作：

```text
#include <stdio.h>
解决声明和类型信息

链接 libc
解决 printf 的具体实现
```

------

## 8. 编译器在哪里寻找 `stdio.h`？

视频中<span style="background:#00FF80; color:#FF0000;">板书了两类位置：</span>

```text
系统目录
指定目录：-I dir
```

### 8.1 系统默认目录

<span style="color:#FF0000; background:#00FF80;">编译器自带一组默认头文件搜索路径，例如可能包括：</span>（<span style="color:#FF0000; background:#00FF80;">补充理解：这里可以把编译器理解为一个软件，这里配置的默认头文件搜索路径相当于是软件自身的配置的路径，即下面所说的 include 目录。</span>）

```text
/usr/include
/usr/local/include
编译器自己的 include 目录
工具链 sysroot 中的 include 目录
```

所以写：

```c
#include <stdio.h>
```

<span style="color:#FF0000; background:#00FF80;">通常不需要告诉编译器 `stdio.h` 的绝对路径。</span>

------

### 8.2 使用 `-I` 添加头文件目录

假设项目结构是：

```text
project/
├── include/
│   └── add.h
└── src/
    ├── main.c
    └── add.c
```

主程序中写：

```c
#include "add.h"
```

编译时可以指定：

```bash
gcc -I ./include src/main.c src/add.c -o app
```

其中：

```text
-I ./include
```

表示：

```text
请额外到 ./include 目录中寻找头文件。
```

注意<span style="background:#00FF80;">，`-I` 后面跟的是**目录**，通常不是具体头文件：</span>

```bash
gcc -I ./include
```

而不是：

```bash
gcc -I ./include/add.h
```

------

## 9. `<stdio.h>` 和 `"add.h"` 有什么区别？

### 尖括号

```c
#include <stdio.h>
```

通常<span style="color:#FF0000; background:#00FF80;">优先在编译器配置的系统头文件目录中寻找。</span>

适合：

- 标准库头文件；
- 系统头文件；
- 已安装到工具链中的库头文件。

### 双引号

```c
#include "add.h"
```

通常<span style="color:#FF0000; background:#00FF80;">先从当前源文件附近或用户指定目录中寻找，找不到再检查系统目录。</span>

适合：

- 自己项目的头文件；
- 当前工程内部模块的头文件。

简单记忆：

```text
<...> 主要找系统提供的头文件
"..." 主要找项目自己的头文件
```

这是<span style="color:#FF0000; background:#00FF80;">常用规则，具体搜索顺序也会受编译器参数影响。</span>

------

## 10. 视频中涉及的两类常见错误

### 10.1 <span style="background:#00FF80;">找不到头文件</span>

例如：

```text
fatal error: add.h: No such file or directory
```

这说明<span style="color:#FF0000;">问题发生在预处理或编译阶段。</span>

含义是：

```text
编译器不知道 add.h 文件在哪里。
```

解决方向：

```bash
gcc -I 头文件目录 ...
```

例如：

```bash
gcc -I ./include main.c add.c -o app
```

<span style="color:#FF0000; background:#00FF80;">也要检查：</span>

- 文件名是否拼写正确；
- 大小写是否一致；
- 目录是否写错；
- 交叉工具链是否完整；
- 是否用了错误的头文件环境。

------

### 10.2 <span style="background:#00FF80;">找不到函数实现</span>

例如：

```text
undefined reference to `add'
```

这<span style="color:#FF0000; background:#00FF80;">通常是链接阶段的问题。</span>

含义是：

```text
已经看到 add() 的声明，
但是没有找到 add() 的机器代码。
```

<span style="color:#FF0000; background:#00FF80;">解决方向：</span>

- 把对应 `.c` 文件加入编译；
- 把对应 `.o` 文件加入链接；
- 把对应函数库加入链接；
- 确认函数名称完全一致。

例如错误命令：

```bash
gcc main.c -o app
```

正确命令：

```bash
gcc main.c add.c -o app
```

------

## 11. `-I` 和 `-L`、`-l` 不要混淆

这是视频内容进一步延伸后非常重要的区别。

| 参数     | 作用                                                         | 对象                        |
| -------- | ------------------------------------------------------------ | --------------------------- |
| `-I目录` | 增加<span style="color:#FF0000; background:#00FF80;">头文件搜索路径</span> | `.h`                        |
| `-L目录` | 增加<span style="color:#FF0000; background:#00FF80;">库文件搜索路径</span> | `.a`、`.so`                 |
| `-l名称` | 指定<span style="color:#FF0000; background:#00FF80;">需要链接的库</span> | `lib名称.so` 或 `lib名称.a` |

例如：

```bash
gcc main.c \
    -I ./include \
    -L ./lib \
    -lmath \
    -o app
```

可以理解为：

```text
-I：说明书去哪里找
-L：函数实现库去哪里找
-l：具体要使用哪个库
```

------

## 12. 这和上一节的交叉编译有什么关系？

上一节使用：

```bash
arm-buildroot-linux-gnueabihf-gcc
```

为 <span style="color:#FF0000; background:#00FF80;">ARM 开发板编译。</span>

这里<span style="color:#FF0000; background:#00FF80;">不仅意味着编译器生成 ARM 指令，还意味着它应该使用与目标开发板相匹配的：</span>

- ARM 头文件；
- ARM libc；
- ARM 启动文件；
- ARM 动态链接器；
- ARM 库文件。

不能把电脑中的 x86 库直接交给 ARM 程序。

可以理解为：

```text
x86 gcc
├── x86 头文件环境
├── x86 libc
└── 生成 x86 程序

ARM 交叉编译器
├── 目标系统头文件
├── ARM libc
└── 生成 ARM 程序
```

<span style="color:#FF0000; background:#00FF80;">工具链中一般有一个目标根目录环境，常称为：</span>

```text
sysroot
```

里面可能包括：

```text
sysroot/
├── usr/include
├── usr/lib
├── lib
└── ...
```

<span style="color:#FF0000; background:#00FF80;">交叉编译器会从这个目标环境中寻找 `stdio.h` 和 libc，而不是随意使用宿主机的 x86 库。</span>

------

## 13. `printf()` 从程序到屏幕经历了什么？

视频结尾画出了这样的层次：

```text
应用程序 APP
      ↓
API / libc
printf、open、read 等
      ↓
用户空间与内核空间边界
      ↓
Linux 内核
文件系统 / 驱动
      ↓
硬件
```

需要稍微细分一下。

### 第一层：应用程序

你写：

```c
printf("Hello, world!\n");
```

这是<span style="color:#FF0000; background:#00FF80;">应用层代码。</span>

程序不需要自己知道：

- 显示终端的驱动怎么写；
- 串口寄存器地址是多少；
- 文件描述符底层怎样管理；
- 当前输出设备具体是什么硬件。

------

### 第二层：libc

`printf()` 是<span style="color:#FF0000; background:#00FF80;"> libc 提供的高级接口。</span>

它会处理：

- `%d`、`%s` 等格式转换；
- 把数字转换成字符；
- 缓冲；
- 拼接最终输出内容。

例如：

```c
printf("num = %d\n", 123);
```

libc 会把整数 `123` 转换成字符：

```text
'1' '2' '3'
```

然后准备写出：

```text
num = 123\n
```

------

### 第三层：系统调用

<span style="color:#FF0000; background:#00FF80;">libc 最终需要请求内核执行输出。</span>

在简化模型中，可以理解为最终会调用类似：

```c
write(...)
```

系统调用。

这一步跨越：

```text
用户空间 → 内核空间
```

注意：

```text
printf 是 C 库函数
write 是 Linux 系统调用接口
```

二者不是同一层。

------

### 第四层：Linux 内核

<span style="color:#FF0000; background:#00FF80;">内核接收到请求后，会进一步处理：</span>

- 文件描述符；
- 终端；
- 文件系统；
- 设备；
- 调度；
- 缓冲；
- 权限。

------

### 第五层：驱动和硬件

<span style="color:#FF0000; background:#00FF80;">假如输出最终对应串口，内核会通过串口驱动操作硬件。</span>

假如输出重定向到文件：

```bash
./hello > output.txt
```

则<span style="color:#FF0000; background:#00FF80;">内核可能通过文件系统和存储设备驱动把内容写入存储设备。</span>

整体过程可以简化为：

```text
printf()
   ↓
libc 格式化
   ↓
write 系统调用
   ↓
Linux 内核
   ↓
文件系统/终端/设备驱动
   ↓
硬件或显示终端
```

------

## 14. 为什么应用程序不直接操作硬件？

主要原因包括：

1. **安全隔离**：普通程序不能随意破坏硬件和系统。
2. **统一接口**：应用不必为每一种硬件重写全部代码。
3. **资源管理**：多个程序需要共享 CPU、内存、文件和设备。
4. **可移植性**：应用调用相对稳定的 API，底层硬件差异交给驱动处理。

例如应用程序调用：

```c
read(fd, buf, size);
```

它<span style="color:#FF0000; background:#00FF80;">并不需要知道文件来自：</span>

- SD 卡；
- eMMC；
- U 盘；
- 网络文件系统；
- 虚拟文件系统。

<span style="color:#FF0000; background:#00FF80;">底层差异主要由内核、文件系统和驱动负责。</span>

------

## 15. 内容结构

### 开头

从 Hello 程序中的：

```c
#include <stdio.h>
```

提出 `.h` 和 `.c` 的区别。

### 发展

用 `add()` 示例说明：

```text
声明 ≠ 实现
```

### 转折

进一步追问：

```text
stdio.h 在哪里？
printf 的实现又在哪里？
```

从单个 C 程序转向编译、链接和库。

### 结尾

把 `printf()` 放入完整 Linux 软件栈：

```text
APP → libc → 内核 → 驱动 → 硬件
```

整体表达路径：

```text
从【一行 #include】开始，
通过【声明、实现、头文件搜索和函数链接】展开，
最后落到【Linux 应用程序与内核、硬件之间的分层关系】。
```

------

## 16. 观点分析

本视频以技术说明为主，没有明显争论性观点。

| 技术结论                                                     | 归属         | 明确/可能 | 依据                                                         |
| ------------------------------------------------------------ | ------------ | --------- | ------------------------------------------------------------ |
| `.h` 文件主要<span style="color:#FF0000; background:#00FF80;">提供声明</span> | 讲师技术说明 | 明确      | 以 `stdio.h` 和 `add()` 为例                                 |
| `.c` 文件或库中<span style="color:#FF0000; background:#00FF80;">包含函数实现</span> | 讲师技术说明 | 明确      | 写出 `add()` 函数体                                          |
| 编译器会在<span style="color:#FF0000; background:#00FF80;">系统目录</span>和 <span style="color:#FF0000; background:#00FF80;">`-I` 指定目录</span>中找头文件 | 讲师技术说明 | 明确      | 板书“<span style="color:#FF0000; background:#00FF80;">系统目录</span>、<span style="color:#FF0000; background:#00FF80;">指定目录 `-I dir`</span>” |
| <span style="color:#FF0000; background:#00FF80;">看到函数声明不等于已经找到函数实现</span> | 核心技术结论 | 明确      | 区分<span style="color:#FF0000; background:#00FF80;">头文件问题</span>和<span style="color:#FF0000; background:#00FF80;">函数未定义问题</span> |
| 应用通过<span style="color:#FF0000; background:#00FF80;"> libc、内核和驱动间接使用硬件</span> | 讲师技术说明 | 明确      | 结尾分层图                                                   |

```text
核心观点：
一个简单的 printf 调用背后，包含了预处理、编译、链接、
C 库、系统调用、Linux 内核、设备驱动和硬件等多个层次。
```

------

## 17. Who / Whom / Whose / When / Where / What / Why / How

| 维度  | 视频中的答案                                                 |
| ----- | ------------------------------------------------------------ |
| Who   | 讲师围绕 Hello C 程序展开讲解                                |
| Whom  | 面向刚开始学习嵌入式 Linux 和 C 编译过程的学习者             |
| Whose | `stdio.h` 中的声明属于标准 C 库接口；`printf` 的实现主要由 libc 提供 |
| When  | 约 02:45 开始转向头文件搜索；约 05:40 开始讲 Linux 软件分层  |
| Where | C 源文件、头文件目录、工具链库目录、用户空间、Linux 内核空间 |
| What  | 解释声明、定义、头文件搜索、函数链接和应用运行层次           |
| Why   | 为了说明 Hello 程序能够运行，不只是因为写了几行 C 代码，还依赖工具链和操作系统 |
| How   | 通过 `stdio.h`、`add()` 和 `printf()` 三个例子逐层引申       |

------

## 18. 重点片段精读

### 片段一：00:40-02:10

**表面发生了什么：**

讲师写出：

```c
int add(int a, int b);
```

并讨论函数调用。

**深层含义：**

<span style="color:#FF0000; background:#00FF80;">函数声明是一份“调用合同”。编译器依据它检查参数和返回值，但声明本身并不包含可执行逻辑。</span>

**重要性：**

帮助区分两个经常被混为一谈的概念：

```text
编译器知道怎么调用
≠
链接器已经找到具体实现
```

------

### 片段二：02:45-03:40

**表面发生了什么：**

讲师说明头文件可能处于系统目录，也可以通过：

```bash
-I dir
```

指定搜索目录。

**深层含义：**

<span style="color:#FF0000; background:#00FF80;">`#include "xxx.h"` 中并没有记录头文件的绝对位置，编译器是按照搜索规则寻找的。</span>

**重要性：**

这是以后解决：

```text
xxx.h: No such file or directory
```

的基础。

------

### 片段三：03:40-05:15

**表面发生了什么：**

从 `printf()` 追问声明和实现在哪里，并总结不同错误。

**深层含义：**

头文件和库承担不同职责：

```text
头文件：让编译器认识接口
库文件：让链接器找到实现
```

**重要性：**

这是理解 `-I`、`-L`、`-l` 和 Makefile 链接过程的基础。

------

### 片段四：05:40-06:58

**表面发生了什么：**

画出 APP、libc、kernel、FS、driver 和硬件。

**深层含义：**

<span style="color:#FF0000; background:#00FF80;">应用程序通常不会直接操作硬件，而是通过层层接口请求底层服务。</span>

**重要性：**

它把一个简单 Hello 程序放进了完整 Linux 系统结构中。

------

## 19. 记忆版总结

```text
类型：
C语言与嵌入式Linux基础讲解

主线：
stdio.h → 声明与实现 → 头文件搜索 → libc → 内核与硬件

.h：
主要放声明、类型和宏，相当于说明书

.c：
主要放函数实现，相当于具体代码

-I：
增加头文件搜索目录

stdio.h：
提供 printf 的声明

libc：
提供 printf 的主要实现

printf 的大致运行路径：
APP → libc → 系统调用 → 内核 → 驱动/文件系统 → 硬件

两个错误要分开：
找不到 .h → 头文件搜索问题
undefined reference → 函数实现或链接问题

一句话记忆：
头文件让编译器认识函数，库让链接器找到函数，
内核和驱动让函数最终作用到硬件。
```





# 问题1

编译器相当于一个软件，编译器自带一组默认头文件搜索路径，其中编译器自己的include目录就相当于自己给个软件添加的头文件路径。在终端里面的gcc，arm-linux-gcc 这些就是编译器这个软件，如果这个软件找不到头文件目录，比如自己随便创建的一个文件夹，那么我们就需要使用-I来指定头文件的目录是吧。还有我们编译的时候用的是gcc，那么链接的时候用的是什么命令呢，有链接器这个软件吗。另外执行的时候用的是./hello，直接hello不行吗，为什么还要加上一个./呢。



你的理解**基本正确**，但有几个地方可以再精确一点。我分三部分解释。

## 一、关于头文件搜索路径和 `-I`

### 1. `gcc`、`arm-linux-gcc` 确实是编译工具

在终端输入：

```bash
gcc
```

<span style="color:#FF0000; background:#00FF80;">Shell 会从 `PATH` 目录中找到 `gcc` 这个可执行程序并运行。</span>

交叉编译器可能叫：

```bash
arm-linux-gcc
```

或者：

```bash
arm-buildroot-linux-gnueabihf-gcc
```

<span style="color:#FF0000; background:#00FF80;">它们也是运行在电脑上的软件，只不过生成的机器指令面向 ARM。</span>

------

### 2. 编译器有默认的头文件搜索路径

例如编译：

```c
#include <stdio.h>
```

时，GCC 会去默认目录寻找 `stdio.h`，可能包括：

```text
/usr/include
/usr/local/include
GCC 自己的 include 目录
工具链 sysroot 中的 usr/include
```

<span style="color:#FF0000;">交叉编译器通常不会使用电脑上普通的 x86 库环境，而是使用交叉工具链自己的目标系统目录</span>，例如：

```text
工具链/sysroot/usr/include
```

这里保存的是适用于 ARM 目标系统的头文件。

可以查看 GCC 实际的默认搜索路径：

```bash
gcc -E -v -xc /dev/null
```

其中会显示类似：

```text
#include <...> search starts here:
 /usr/lib/gcc/...
 /usr/local/include
 /usr/include
End of search list.
```

------

### 3. 自己随便创建的目录，GCC 默认不知道

例如目录结构：

```text
project/
├── main.c
└── myinclude/
    └── add.h
```

`main.c` 中写：

```c
#include "add.h"
```

假如直接执行：

```bash
gcc main.c -o main
```

GCC 可能找不到 `add.h`，因为 `myinclude` 不在默认搜索路径中。

这时需要：

```bash
gcc -I ./myinclude main.c -o main
```

这里：

```text
-I ./myinclude
```

就是告诉编译器：

```text
除了默认目录以外，还要去 ./myinclude 寻找头文件。
```

所以你的理解可以总结为：

```text
系统或工具链头文件
→ 编译器通常知道去哪里找

自己创建的头文件目录
→ 通常需要用 -I 指定
```

不过，`-I` 指定的是**目录**，不是具体头文件： 

```bash
gcc -I ./myinclude main.c
```

而不是：

```bash
gcc -I ./myinclude/add.h main.c
```

------

## 二、编译时用 GCC，链接时用什么？

### 1. 确实有专门的链接器软件

<span style="color:#FF0000;">Linux 中常见的链接器叫：</span>

```bash
ld
```

<span style="color:#FF0000;">ARM 交叉工具链里的链接器可能叫：</span>

```bash
arm-linux-ld
```

或者：

```bash
arm-buildroot-linux-gnueabihf-ld
```

因此，<span style="color:#FF0000; background:#00FF80;">链接器确实是一个独立的软件。</span>

查看它的位置：

```bash
which ld
```

查看版本：

```bash
ld --version
```

------

### 2. <span style="background:#00FF80;">为什么平时编译和链接都写 `gcc`？</span>

<span style="background:#00FF80;">因为 `gcc` 不只是狭义上的“C 编译器”，它还是一个**编译驱动程序**。</span>

你执行：

```bash
gcc hello.c -o hello
```

背后实际上包含多个步骤：

```text
hello.c
   ↓ 预处理
hello.i
   ↓ 编译
hello.s
   ↓ 汇编
hello.o
   ↓ 链接
hello
```

对应工具大致是：

| 阶段       | 工具                       |
| ---------- | -------------------------- |
| 预处理     | `cpp`                      |
| 编译为汇编 | GCC 内部编译程序，如 `cc1` |
| 汇编       | `as`                       |
| 链接       | `ld`                       |

也就是说，<span style="color:#FF0000; background:#00FF80;">输入一条：</span>

```bash
gcc hello.c -o hello
```

<span style="color:#FF0000; background:#00FF80;">GCC 会在背后调用汇编器和链接器。</span>

可以使用：

```bash
gcc -v hello.c -o hello
```

查看 GCC 实际调用了哪些程序。

------

## 3. 如何把编译和链接分开？

### 第一步：只编译，不链接

```bash
gcc -c hello.c -o hello.o
```

`-c` 表示：

```text
只生成目标文件，不进行最终链接。
```

结果是：

```text
hello.o
```

它还不是可以直接运行的完整程序。

### 第二步：链接

<span style="color:#FF0000;">通常仍然使用：</span>

```bash
gcc hello.o -o hello
```

这一步<span style="color:#FF0000; background:#00FF80;">没有 `.c` 文件，只有 `.o` 文件，所以 GCC 主要是在组织链接过程。</span>

它<span style="background:#00FF80; color:#FF0000;">最终会调用链接器 `ld`。</span>

------

## 4. 可以直接使用 `ld` 吗？

可以，但一般不建议初学时直接使用。

例如理论上可以：

```bash
ld hello.o -o hello
```

但<span style="color:#FF0000; background:#00FF80;">这条命令通常不能直接生成正常的 C 程序，因为还缺少很多内容</span>，例如：

- 程序启动代码；
- C 标准库；
- 动态链接器信息；
- 正确的入口点；
- 一些默认库路径；
- GCC 运行时库。

使用：

```bash
gcc hello.o -o hello
```

时，<span style="color:#FF0000; background:#00FF80;">GCC 会替你加入这些必要内容。</span>

因此通常建议：

```text
编译 C 程序：使用 gcc
链接 C 程序：也使用 gcc
底层真正执行链接：ld
```

可以把二者理解成：

```text
gcc：项目负责人，负责安排整个流程
ld：专门负责最终拼装的工作人员
```

------

## 5. 多文件程序的例子

有三个文件：

```text
main.c
add.c
add.h
```

分别编译：

```bash
gcc -c main.c -o main.o
gcc -c add.c -o add.o
```

此时：

```text
main.o 中调用了 add()
add.o 中实现了 add()
```

链接：

```bash
gcc main.o add.o -o app
```

底层链接器会把两个目标文件以及必要的库组合成：

```text
app
```

假如只执行：

```bash
gcc main.o -o app
```

可能出现：

```text
undefined reference to `add'
```

因为链接器找不到 `add()` 的具体实现。

------

## 三、为什么执行程序要写 `./hello`？

### 1. Shell 不会默认在当前目录寻找命令

当你输入：

```bash
hello
```

<span style="color:#FF0000; background:#00FF80;">Shell 会去 `PATH` 中列出的目录寻找它。</span>

查看：

```bash
echo $PATH
```

可能看到：

```text
/usr/local/bin:/usr/bin:/bin
```

Shell 会寻找：

```text
/usr/local/bin/hello
/usr/bin/hello
/bin/hello
```

但是它通常不会自动查找：

```text
当前目录/hello
```

<span style="color:#FF0000; background:#00FF80;">所以即使当前目录明明有 `hello`，直接输入：</span>

```bash
hello
```

<span style="color:#FF0000; background:#00FF80;">也可能提示：</span>

```text
hello: command not found
```

------

### 2. `.` 表示当前目录

Linux 路径中：

```text
.   当前目录
..  上一级目录
```

因此：

```bash
./hello
```

表示：

```text
运行当前目录中的 hello 文件。
```

这里的 `/` 是目录与文件名的分隔符：

```text
.       当前目录
./hello 当前目录下面的 hello
```

------

### 3. 为什么 Linux 不默认搜索当前目录？

主要是出于安全考虑。

假设当前目录中有人放了一个恶意程序，名字叫：

```text
ls
```

你本来想执行系统命令：

```bash
ls
```

<span style="color:#FF0000; background:#00FF80;">如果当前目录优先被搜索，Shell 可能会运行这个恶意的 `./ls`，而不是正常的：</span>

```text
/bin/ls
```

<span style="color:#FF0000; background:#00FF80;">因此 Linux 通常不把当前目录 `.` 默认放进 `PATH`。</span>

这迫使你明确写：

```bash
./hello
```

表示：

```text
我确认要运行当前目录中的这个文件。
```

------

### 4. 怎样才能直接输入 `hello`？

有几种方法。

#### 方法一：使用绝对路径

```bash
/home/book/project/hello
```

#### 方法二：把程序复制到 `PATH` 中的目录

例如：

```bash
sudo cp hello /usr/local/bin/
```

之后可以直接执行：

```bash
hello
```

因为 `/usr/local/bin` 通常在 `PATH` 中。

#### 方法三：把当前目录加入 `PATH`

可以执行：

```bash
export PATH=.:$PATH
```

之后当前目录中的程序可以直接输入名称运行。

但一般不推荐长期这样设置，因为存在前面提到的安全风险。

------

### 5. 即使写了 `./hello`，还需要执行权限

假如程序没有执行权限：

```bash
./hello
```

可能提示：

```text
Permission denied
```

可以查看权限：

```bash
ls -l hello
```

例如：

```text
-rwxr-xr-x 1 book book 16000 hello
```

其中的：

```text
x
```

表示具有执行权限。

如果没有，可以添加：

```bash
chmod +x hello
```

不过使用 GCC 正常生成的可执行文件通常已经具有执行权限。

------

## 四、把这三个问题连起来理解

假设有：

```text
project/
├── main.c
├── add.c
└── include/
    └── add.h
```

编译 `main.c`：

```bash
gcc -I ./include -c main.c -o main.o
```

解释：

```text
gcc             运行 GCC 编译驱动程序
-I ./include    告诉它去哪里找 add.h
-c              只编译，不链接
main.c          输入源文件
-o main.o       输出目标文件
```

编译 `add.c`：

```bash
gcc -I ./include -c add.c -o add.o
```

链接：

```bash
gcc main.o add.o -o hello
```

解释：

```text
GCC 负责组织链接
底层调用 ld
把 main.o、add.o 和必要的 C 库组合成 hello
```

运行：

```bash
./hello
```

解释：

```text
./ 表示当前目录
hello 是当前目录中的可执行程序
```

完整流程：

```text
头文件在哪里？
→ 使用 -I 指定

源代码怎样变成目标文件？
→ gcc -c

目标文件怎样组合成程序？
→ gcc 组织链接，底层调用 ld

怎样运行当前目录中的程序？
→ ./hello
```

## 最简记忆

```text
-I：
告诉编译器头文件目录在哪里。

gcc：
表面上负责整个编译流程，也会组织链接过程。

ld：
真正的链接器软件。

gcc hello.o -o hello：
GCC 调用链接器并自动加入启动文件和 C 库。

./hello：
运行当前目录中的 hello。

hello：
只在 PATH 指定的目录中寻找，不一定找当前目录。
```

