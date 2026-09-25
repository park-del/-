# 视频分析

> 视频时长约 **15分25秒**。以下时间点根据画面、代码和终端操作定位，可能存在数秒误差；终端内容较清晰，个别口头说明无法逐字核验。

## 1. 视频类型判断

**主要类型：** 教程类、讲解类
**次要类型：** 操作演示类、嵌入式开发入门类

**判断依据：**

1. 先解释 Hello 程序及 `argc/argv` 的含义。
2. 实际演示本机编译、复制、挂载和开发板运行。
3. 通过一次失败的运行结果，引出“为什么需要交叉编译”。
4. 最后使用 ARM 交叉编译器重新生成程序，并在开发板上验证成功。

------

## 2. 一句话总结

这个视频通过“**x86 编译的程序在 ARM 开发板上运行失败**”这一现象，说明了交叉编译的必要性，并完整演示了：

```text
编写程序
→ 在 Ubuntu 上普通编译
→ 通过 NFS 共享给开发板
→ 运行失败
→ 使用 ARM 交叉编译器重新编译
→ 开发板运行成功
```

------

## 3. 时间段拆解

| 时间段      | 内容概括                          | 画面/声音/操作表现                                           | 作用                         |
| ----------- | --------------------------------- | ------------------------------------------------------------ | ---------------------------- |
| 00:00-00:44 | 打开课程文件和 `hello.c`          | 从课程页面、文件目录切换到 Source Insight，打开 Hello 程序   | 准备与引入                   |
| 00:44-03:38 | 解释 Hello 程序及命令行参数       | 讲解 `main(int argc, char **argv)`、`argc`、`argv[0]`、`argv[1]`，并手写标注 | 建立程序基础                 |
| 03:38-04:44 | 打开 MobaXterm，连接 Ubuntu       | 切换终端并通过 SSH 进入 `192.168.1.137` 上的 Ubuntu 环境     | 准备编译环境                 |
| 04:44-05:50 | 使用普通 `gcc` 编译并运行         | 执行 `gcc -o hello hello.c`，随后使用不同参数运行程序        | 验证程序逻辑                 |
| 05:50-06:18 | 检查可执行文件架构                | 使用 `file hello`，结果显示程序属于 `x86-64`                 | 引出架构问题                 |
| 06:18-07:56 | 解释 PC 与 ARM 开发板的区别       | 白板绘制 Ubuntu PC 与 ARM 开发板，说明两者处理器架构不同     | 建立交叉编译概念             |
| 07:56-08:52 | 连接 ARM 开发板并检查网络         | 通过串口进入 Buildroot 系统，执行 `ifconfig`、`ping 192.168.1.137` | 确认开发板与 Ubuntu 通信正常 |
| 08:52-10:00 | 解释 `mount` 和共享目录           | 用白板类比 U 盘、SD 卡、服务器目录挂载到 `/a`、`/b`、`/c`    | 解释 NFS 挂载                |
| 10:00-12:17 | 将 Ubuntu 的 NFS 目录挂载到开发板 | 多次输入 `mount -t nfs`，处理锁服务问题，加入 `nolock`、`vers=3` 等参数 | 建立文件共享通道             |
| 12:17-13:08 | 在 ARM 板运行 x86 程序并失败      | 开发板进入 `/mnt`，运行 `./hello`，出现 `Exec format error`  | 制造核心问题                 |
| 13:08-13:37 | 查阅手册中的交叉编译配置          | 打开课程手册，定位工具链和环境变量说明                       | 提供解决方案                 |
| 13:37-14:48 | 配置 ARM 工具链并交叉编译         | 设置 `ARCH`、`CROSS_COMPILE`、`PATH`，调用 ARM GCC；使用 `file` 确认 ARM 架构 | 完成交叉编译                 |
| 14:48-15:25 | 在开发板运行 ARM 程序             | `./hello` 输出 `Hello, world!`；带参数运行输出 `Hello, www.100ask.net!` | 最终验证与收束               |

------

## 4. 内容结构

### 1. 开头

打开示例程序，先让观众理解程序本身在做什么，而不是直接进入交叉编译命令。

### 2. 发展

在 Ubuntu 中用普通 `gcc` 编译，并验证程序能够正常运行。

### 3. 转折

把普通 GCC 生成的程序放到 ARM 开发板上运行，出现：

```text
cannot execute binary file: Exec format error
```

这一错误成为整段视频的核心转折。

### 4. 结尾

换用 ARM 交叉编译器重新编译，确认文件架构已经变成 ARM，再在开发板上运行成功。

### 整体表达路径

```text
从【理解 Hello 程序】开始，
通过【普通编译、NFS 共享和失败实验】展开，
最后落到【交叉编译器必须与目标处理器架构匹配】。
```

------

## 5. 核心程序解释

视频中的程序大致逻辑是：

```c
#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc >= 2)
        printf("Hello, %s!\n", argv[1]);
    else
        printf("Hello, world!\n");

    return 0;
}
```

### `argc` 是什么？

`argc` 表示命令行参数的数量，但它会把程序名称本身也计算进去。

#### 情况一

```bash
./hello
```

对应：

```text
argc = 1
argv[0] = "./hello"
```

没有提供额外名字，所以执行：

```c
printf("Hello, world!\n");
```

输出：

```text
Hello, world!
```

#### 情况二

```bash
./hello weidongshan
```

对应：

```text
argc = 2
argv[0] = "./hello"
argv[1] = "weidongshan"
```

因此输出：

```text
Hello, weidongshan!
```

#### 情况三

视频还演示了类似：

```bash
./hello weidongshan asdfds
```

这时：

```text
argc = 3
argv[0] = "./hello"
argv[1] = "weidongshan"
argv[2] = "asdfds"
```

但程序只使用了 `argv[1]`，所以仍然只输出：

```text
Hello, weidongshan!
```

这说明：

> 参数可以有多个，但程序读取哪些参数，取决于代码具体访问了哪些 `argv` 元素。

------

## 6. 最重要的知识：什么是交叉编译？

### 普通编译

视频先在 Ubuntu PC 中执行：

```bash
gcc -o hello hello.c
```

这里的 GCC：

- 运行在 x86 Ubuntu 电脑上；
- 默认生成给 x86 处理器运行的程序；
- 所以编译结果可以在 Ubuntu PC 上正常运行。

使用：

```bash
file hello
```

可以看到类似：

```text
ELF 64-bit LSB ...
x86-64
```

核心信息是：

```text
x86-64
```

说明这个程序面向的是 x86-64 处理器。

------

### 为什么放到开发板上不能运行？

开发板使用的是 ARM 处理器，而普通 GCC 生成的是 x86 指令。

可以把它理解成：

```text
hello.c 源代码
       ↓ x86 GCC
x86 机器指令
       ↓
只能直接交给 x86 CPU 执行
```

而开发板的 ARM CPU 不认识这些 x86 指令，于是出现：

```text
cannot execute binary file: Exec format error
```

这个错误主要表示：

> 当前可执行文件的格式或处理器架构，与当前系统不匹配。

它通常不是“没有执行权限”。

假如是权限问题，更常见的提示会是：

```text
Permission denied
```

------

### 交叉编译

交叉编译器虽然运行在 x86 Ubuntu 上，但它生成的是 ARM 指令：

```text
交叉编译器运行的平台：x86 Ubuntu
交叉编译器生成程序的平台：ARM 开发板
```

视频使用的编译器名称是：

```bash
arm-buildroot-linux-gnueabihf-gcc
```

名称可以拆开理解：

```text
arm          → 目标处理器是 ARM
buildroot    → 与 Buildroot 工具链有关
linux        → 目标系统是 Linux
gnueabihf    → GNU EABI，使用硬件浮点 ABI
gcc          → C 编译器
```

执行的核心命令是：

```bash
arm-buildroot-linux-gnueabihf-gcc -o hello hello.c
```

之后再次检查：

```bash
file hello
```

结果变成类似：

```text
ELF 32-bit LSB executable, ARM, EABI5
```

最关键的信息是：

```text
ARM
```

这说明新生成的 `hello` 已经面向 ARM 开发板。

------

## 7. 环境变量分别有什么用？

视频配置了三个关键环境变量。

### 1. `ARCH`

```bash
export ARCH=arm
```

表示目标架构是 ARM。

但要注意：

> `ARCH` 通常主要由 Makefile、Linux 内核或其他构建系统读取。直接执行 GCC 编译一个简单的 `.c` 文件时，编译器并不会因为设置了 `ARCH=arm` 就自动变成 ARM 编译器。

所以仅执行：

```bash
export ARCH=arm
gcc -o hello hello.c
```

通常仍然会得到 x86 程序。

真正决定目标架构的是所调用的编译器。

------

### 2. `CROSS_COMPILE`

```bash
export CROSS_COMPILE=arm-buildroot-linux-gnueabihf-
```

它保存的是交叉编译工具前缀。

构建系统可以组合出：

```text
${CROSS_COMPILE}gcc
→ arm-buildroot-linux-gnueabihf-gcc

${CROSS_COMPILE}ld
→ arm-buildroot-linux-gnueabihf-ld

${CROSS_COMPILE}strip
→ arm-buildroot-linux-gnueabihf-strip
```

同样需要注意：

> `CROSS_COMPILE` 一般是 Makefile 主动读取的变量。只设置变量，但仍手动运行普通 `gcc`，不会自动变成交叉编译。

------

### 3. `PATH`

视频将工具链的 `bin` 目录加入 `PATH`：

```bash
export PATH=$PATH:/home/book/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/bin
```

它的作用是：

> 让 Shell 能够找到 `arm-buildroot-linux-gnueabihf-gcc` 这个程序。

没有加入 `PATH` 时，可能出现：

```text
arm-buildroot-linux-gnueabihf-gcc: command not found
```

也可以不用修改 `PATH`，直接写编译器的完整路径，但命令会很长。

------

## 8. NFS 挂载部分在做什么？

视频没有每次都通过 U 盘或串口复制程序，而是使用 NFS 建立共享目录。

Ubuntu 主机上的目录是：

```text
/home/book/nfs_rootfs
```

开发板把它挂载到：

```text
/mnt
```

视频中的完整命令类似：

```bash
mount -t nfs -o nolock,vers=3 \
192.168.1.137:/home/book/nfs_rootfs /mnt
```

各部分含义：

| 部分                    | 含义                |
| ----------------------- | ------------------- |
| `mount`                 | 挂载文件系统        |
| `-t nfs`                | 文件系统类型是 NFS  |
| `-o nolock,vers=3`      | 使用相应的 NFS 参数 |
| `192.168.1.137`         | Ubuntu 主机的 IP    |
| `/home/book/nfs_rootfs` | Ubuntu 共享目录     |
| `/mnt`                  | 开发板上的挂载点    |

挂载成功后：

```text
Ubuntu 的 /home/book/nfs_rootfs
                 ⇅
开发板的 /mnt
```

Ubuntu 把新编译的 `hello` 复制到：

```bash
cp hello /home/book/nfs_rootfs/
```

开发板在 `/mnt` 中就可以直接看到它。

这能大幅减少开发过程中的文件传输步骤。

------

## 9. 观点分析

本视频不是评论或辩论视频，因此主要是技术结论，而不是价值立场。

| 观点或技术结论                         | 观点归属     | 明确/可能 | 判断依据                       |
| -------------------------------------- | ------------ | --------- | ------------------------------ |
| `argc` 包含程序名称本身                | 讲师技术说明 | 明确      | 通过多个命令行示例解释         |
| 普通 GCC 在 Ubuntu 上默认生成 x86 程序 | 讲师技术说明 | 明确      | `file hello` 显示 `x86-64`     |
| x86 程序不能直接在 ARM 开发板运行      | 讲师技术说明 | 明确      | 开发板出现 `Exec format error` |
| 必须使用面向 ARM 的交叉编译器          | 视频核心结论 | 明确      | 更换编译器后成功运行           |
| `file` 命令应当用于检查生成程序的架构  | 讲师操作建议 | 明确      | 编译前后均使用该命令验证       |
| NFS 可以简化主机与开发板之间的文件共享 | 讲师操作思路 | 明确      | 将共享目录挂载到 `/mnt`        |

```text
核心观点：
编译成功不代表程序能在任何机器上运行；
可执行文件的目标架构必须与运行它的处理器架构相匹配。
```

------

## 10. 人物分析

视频主要是屏幕录制和讲师操作，没有足够清晰、持续的出镜人物画面。

因此可以明确分析的是：

- 讲师采取“先演示正常情况，再制造错误，最后解决错误”的教学方式。
- 操作节奏偏实战，重视通过终端输出验证结论。
- 不能仅根据声音和操作片段判断讲师的性格、人格或人品。

不对人物情绪、人格和人品作进一步推断。

------

## 11. Who / Whom / Whose / When / Where / What / Why / How

| 维度  | 视频中的答案                                                 |
| ----- | ------------------------------------------------------------ |
| Who   | 操作主体是讲师；执行环境包括 Ubuntu 主机和 ARM 开发板        |
| Whom  | 教学内容面向嵌入式 Linux 初学者                              |
| Whose | x86 可执行文件属于普通 GCC 的编译结果；ARM 可执行文件属于交叉编译器的编译结果 |
| When  | 核心变化发生在约 12:17，x86 程序在 ARM 板上出现运行错误；约 14:48 后换成 ARM 程序 |
| Where | Windows 上使用 MobaXterm；远程 Ubuntu 虚拟机；ARM 开发板的 Buildroot Linux；NFS 共享目录 |
| What  | 编写、编译、检查、共享并运行一个 Hello C 程序                |
| Why   | 普通 GCC 生成的 x86 指令无法被 ARM 处理器执行，因此需要交叉编译 |
| How   | 通过 ARM 工具链生成 ARM ELF 文件，再通过 NFS 放到开发板上运行 |

------

## 12. 重点片段精读

### 片段一：00:44-03:38

**表面发生了什么：**

讲解 `argc`、`argv` 以及不同命令行参数对应的输出。

**更深层可能意味着：**

这一部分不是交叉编译本身，而是在建立一个可以明确观察运行结果的测试程序。只有先确认程序逻辑正确，后面出现错误时，才能把问题定位到编译架构，而不是程序逻辑。

**这个片段的重要性：**

为后面的架构对比提供相同的测试对象。

------

### 片段二：05:50-07:56

**表面发生了什么：**

使用 `file hello` 检查程序，发现它属于 `x86-64`，随后画出 PC 和 ARM 开发板。

**更深层可能意味着：**

文件扩展名或文件名称不能说明程序属于哪种处理器。Linux 可执行程序通常没有 `.exe` 后缀，必须查看 ELF 文件头中的机器架构。

**这个片段的重要性：**

这是理解交叉编译的理论核心。

------

### 片段三：12:17-13:08

**表面发生了什么：**

开发板运行普通 GCC 生成的 `hello`，出现：

```text
Exec format error
```

**更深层可能意味着：**

源代码是通用的，但机器指令不是通用的。相同的 `hello.c` 使用不同编译器，会生成面向不同处理器的二进制文件。

**这个片段的重要性：**

它用实际失败证明“为什么不能直接用普通 GCC”。

------

### 片段四：13:37-15:25

**表面发生了什么：**

配置工具链，使用：

```bash
arm-buildroot-linux-gnueabihf-gcc
```

重新编译，`file` 显示 ARM，程序在开发板运行成功。

**更深层可能意味着：**

交叉编译不是修改 C 源代码，而是更换生成机器指令的编译工具链。

**这个片段的重要性：**

完成从问题发现到解决验证的闭环。

------

## 13. 你应该重点看什么？

这是一段教程类视频，建议重点抓住四条线：

### 第一条：源程序逻辑

```text
argc 是参数数量
argv 是参数内容
argv[0] 是程序名称
argv[1] 是第一个额外参数
```

### 第二条：区分编译机和运行机

```text
编译机：Ubuntu x86 PC
运行机：ARM 开发板
```

### 第三条：学会检查架构

```bash
file hello
```

不要只看“有没有编译报错”，还要看生成的是：

```text
x86-64
```

还是：

```text
ARM
```

### 第四条：记住真正决定架构的是编译器

```bash
gcc
```

和：

```bash
arm-buildroot-linux-gnueabihf-gcc
```

虽然都叫 GCC，但目标架构不同。

------

## 14. 易错点总结

### 易错点一：把 `Exec format error` 当成权限错误

错误：

```text
cannot execute binary file: Exec format error
```

优先检查：

```bash
file hello
```

而不是立刻只执行：

```bash
chmod +x hello
```

------

### 易错点二：认为设置 `ARCH=arm` 就够了

仅设置：

```bash
export ARCH=arm
```

不会让普通 `gcc` 自动生成 ARM 程序。

仍然需要使用：

```bash
arm-buildroot-linux-gnueabihf-gcc
```

或者让 Makefile 根据 `CROSS_COMPILE` 调用它。

------

### 易错点三：复制后没有确认开发板看到的是新文件

重新编译后应再次复制：

```bash
cp hello /home/book/nfs_rootfs/
```

并在开发板中确认 `/mnt/hello` 已更新。

否则可能一直运行旧的 x86 文件。

------

### 易错点四：只看文件名，不检查架构

两个文件都可以叫：

```text
hello
```

但内部可能分别是：

```text
x86-64 程序
ARM 程序
```

文件名相同不代表内容相同。

------

## 15. 记忆版总结

```text
类型：嵌入式 Linux 交叉编译教程

主线：
普通 GCC 编译 → ARM 板运行失败
→ ARM GCC 交叉编译 → ARM 板运行成功

核心观点：
源代码可以相同，但不同 CPU 需要不同架构的机器指令。

关键错误：
Exec format error

关键检查命令：
file hello

普通编译：
gcc -o hello hello.c

交叉编译：
arm-buildroot-linux-gnueabihf-gcc -o hello hello.c

最重要时间段：
12:17-15:25

一句话记忆：
在电脑上编译、给开发板运行时，编译器必须“生成开发板认识的指令”。
```



# 对视频中提到的3个export命令的理解

可以。视频中的三个命令大致是：

```bash
export ARCH=arm
export CROSS_COMPILE=arm-buildroot-linux-gnueabihf-
export PATH=/工具链目录/bin:$PATH
```

它们不是三个“编译命令”，而是在编译前给当前终端设置三个**环境变量**。

------

## 一、先理解 `export` 是什么

例如：

```bash
export ARCH=arm
```

可以拆成两部分：

```text
ARCH=arm
```

表示创建一个名叫 `ARCH` 的变量，把它的值设为 `arm`。

```text
export
```

表示把这个变量导出为环境变量，使当前终端随后启动的 `make`、编译器、脚本等子程序也能读到它。

你可以把它理解成：

```text
在当前终端里贴一张公共便签：

ARCH = arm

以后从这个终端启动的程序都可以查看这张便签。
```

查看变量：

```bash
echo $ARCH
```

输出：

```text
arm
```

这里的 `$ARCH` 表示“取出变量 `ARCH` 的值”。

## 不加 `export` 有什么区别？

```bash
ARCH=arm
```

当前 Shell 自己知道这个变量，但它启动的某些子进程不一定能读取。

而：

```bash
export ARCH=arm
```

当前 Shell 和随后启动的子进程都能够读取。

编译通常会启动 `make`、编译器、链接器等多个程序，因此视频使用 `export`。

------

## 二、第一个命令：`export ARCH=arm`

```bash
export ARCH=arm
```

### 1. 它表达什么意思？

它表达的是：

```text
这次准备构建的目标处理器架构是 ARM。
```

其中：

```text
ARCH        architecture，架构
arm         ARM 处理器架构
```

视频中的电脑通常是 x86-64，而开发板是 ARM：

```text
Ubuntu 电脑：x86-64
开发板：ARM
```

因此要告诉构建系统：

```text
我现在不是为这台 x86 电脑构建，
而是准备为 ARM 开发板构建。
```

------

### 2. 谁会读取 `ARCH`？

通常是：

- Linux 内核的 Makefile；
- U-Boot 的构建系统；
- 部分嵌入式工程的 Makefile；
- 某些构建脚本。

例如执行：

```bash
make
```

工程的 Makefile 可能会查看：

```makefile
ARCH
```

然后根据它选择 ARM 相关的源代码、头文件或编译选项。

------

### 3. `ARCH=arm` 会自动把普通 GCC 变成 ARM GCC 吗？

**不会。**

这是最容易误解的地方。

下面这样做：

```bash
export ARCH=arm
gcc -o hello hello.c
```

你调用的仍然是普通的：

```text
gcc
```

它通常仍然生成 x86-64 程序。

也就是说：

```text
ARCH=arm
```

只是一条给构建系统看的信息，并不会改变 `gcc` 本身。

真正决定生成 ARM 指令的，是你使用了哪个编译器：

```bash
arm-buildroot-linux-gnueabihf-gcc
```

所以：

```text
ARCH=arm
```

更像是在表格中填写：

```text
目标架构：ARM
```

但真正生产 ARM 程序的机器，仍然是 ARM 交叉编译器。

------

### 4. 一个简单例子

假设 Makefile 中有：

```makefile
ifeq ($(ARCH),arm)
    CFLAGS += -DARM_PLATFORM
endif
```

当你执行：

```bash
export ARCH=arm
make
```

Makefile 发现 `ARCH` 是 `arm`，就会加入 ARM 平台相关配置。

但是，如果只是编译一个特别简单的 `hello.c`，而且直接手动输入完整编译器名字，那么 `ARCH` 不一定真正参与编译。

------

## 三、第二个命令：`export CROSS_COMPILE=...`

视频中类似：

```bash
export CROSS_COMPILE=arm-buildroot-linux-gnueabihf-
```

注意最后有一个：

```text
-
```

这个短横线非常重要。

------

### 1. `CROSS_COMPILE` 是什么意思？

它表示交叉编译工具链的**命令前缀**。

值是：

```text
arm-buildroot-linux-gnueabihf-
```

构建系统会在它后面拼接不同的工具名称。

例如：

```text
CROSS_COMPILE 的值
arm-buildroot-linux-gnueabihf-
```

拼上：

```text
gcc
```

得到：

```text
arm-buildroot-linux-gnueabihf-gcc
```

拼上：

```text
ld
```

得到：

```text
arm-buildroot-linux-gnueabihf-ld
```

拼上：

```text
objcopy
```

得到：

```text
arm-buildroot-linux-gnueabihf-objcopy
```

拼上：

```text
strip
```

得到：

```text
arm-buildroot-linux-gnueabihf-strip
```

------

### 2. 为什么不直接设置成完整的 GCC 名称？

因为构建一个程序不只有 GCC 一个工具。

完整工具链通常包含：

| 工具      | 作用               |
| --------- | ------------------ |
| `gcc`     | 编译 C 代码        |
| `g++`     | 编译 C++ 代码      |
| `as`      | 汇编               |
| `ld`      | 链接               |
| `ar`      | 创建静态库         |
| `objcopy` | 转换目标文件格式   |
| `objdump` | 查看目标文件信息   |
| `strip`   | 删除调试和符号信息 |

如果只写：

```text
arm-buildroot-linux-gnueabihf-gcc
```

构建系统只知道 GCC 的名称。

而设置公共前缀：

```bash
export CROSS_COMPILE=arm-buildroot-linux-gnueabihf-
```

构建系统就可以自动寻找整套工具。

------

### 3. Makefile 如何使用它？

Makefile 中可能写着：

```makefile
CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld
AR := $(CROSS_COMPILE)ar
```

当：

```text
CROSS_COMPILE=arm-buildroot-linux-gnueabihf-
```

这些变量就会展开为：

```makefile
CC = arm-buildroot-linux-gnueabihf-gcc
LD = arm-buildroot-linux-gnueabihf-ld
AR = arm-buildroot-linux-gnueabihf-ar
```

然后执行：

```bash
make
```

时，Makefile 会使用 ARM 工具链。

------

### 4. 为什么结尾的短横线不能漏？

正确：

```bash
export CROSS_COMPILE=arm-buildroot-linux-gnueabihf-
```

随后拼接：

```text
arm-buildroot-linux-gnueabihf- + gcc
```

得到：

```text
arm-buildroot-linux-gnueabihf-gcc
```

假如漏掉最后的 `-`：

```bash
export CROSS_COMPILE=arm-buildroot-linux-gnueabihf
```

拼接后就会变成：

```text
arm-buildroot-linux-gnueabihfgcc
```

这是一个不存在的命令。

终端可能提示：

```text
command not found
```

------

### 5. `CROSS_COMPILE` 会自动生效吗？

它也不是 Shell 自带的特殊魔法变量。

Shell 只负责保存它。真正是否使用它，取决于：

- Makefile 是否读取 `CROSS_COMPILE`；
- 构建脚本是否读取 `CROSS_COMPILE`；
- 你是否在命令中引用它。

例如可以直接这样编译：

```bash
${CROSS_COMPILE}gcc -o hello hello.c
```

Shell 会先把变量展开：

```text
${CROSS_COMPILE}
→ arm-buildroot-linux-gnueabihf-
```

最后实际执行：

```bash
arm-buildroot-linux-gnueabihf-gcc -o hello hello.c
```

但是下面这样写：

```bash
export CROSS_COMPILE=arm-buildroot-linux-gnueabihf-
gcc -o hello hello.c
```

你最终调用的仍然是普通 `gcc`，因为命令中没有使用 `$CROSS_COMPILE`。

------

## 四、第三个命令：`export PATH=...:$PATH`

视频中的命令可能类似：

```bash
export PATH=/home/book/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/bin:$PATH
```

不同课程环境中的具体目录可能不同，但原理相同。

------

### 1. `PATH` 是什么？

`PATH` 是 Shell 的“命令搜索目录列表”。

当你输入：

```bash
gcc
```

Shell 并不知道 `gcc` 文件具体存放在哪里。它会到 `PATH` 中列出的目录里依次寻找。

查看当前 `PATH`：

```bash
echo $PATH
```

可能看到：

```text
/usr/local/bin:/usr/bin:/bin
```

冒号 `:` 用来分隔多个目录：

```text
/usr/local/bin
/usr/bin
/bin
```

当你执行：

```bash
gcc
```

Shell 会按顺序检查：

```text
1. /usr/local/bin/gcc
2. /usr/bin/gcc
3. /bin/gcc
```

找到第一个符合条件的可执行文件后就运行。

------

### 2. 为什么需要修改 `PATH`？

ARM 编译器可能放在一个很深的目录中，例如：

```text
/home/book/100ask_imx6ull-sdk/ToolChain/.../bin
```

如果这个目录不在 `PATH` 中，直接输入：

```bash
arm-buildroot-linux-gnueabihf-gcc
```

Shell 不知道去哪里寻找，可能提示：

```text
arm-buildroot-linux-gnueabihf-gcc: command not found
```

当然，你可以每次输入完整路径：

```bash
/home/book/100ask_imx6ull-sdk/ToolChain/.../bin/arm-buildroot-linux-gnueabihf-gcc \
-o hello hello.c
```

但非常麻烦。

把工具链目录加入 `PATH` 后：

```bash
export PATH=/工具链目录/bin:$PATH
```

就可以直接输入：

```bash
arm-buildroot-linux-gnueabihf-gcc -o hello hello.c
```

------

### 3. `$PATH` 为什么还要写在后面？

假设原来的 `PATH` 是：

```text
/usr/local/bin:/usr/bin:/bin
```

执行：

```bash
export PATH=/toolchain/bin:$PATH
```

Shell 会先替换 `$PATH`：

```text
/toolchain/bin:/usr/local/bin:/usr/bin:/bin
```

也就是说，新工具链目录被加入了，同时保留原来的系统目录。

假如写成：

```bash
export PATH=/toolchain/bin
```

原来的 `/usr/bin`、`/bin` 等目录会被覆盖。

这时很多普通命令都可能找不到，例如：

```text
ls
cp
mkdir
grep
```

因此通常不能随便丢掉原来的 `$PATH`。

------

### 4. 放前面和放后面有什么区别？

#### 放前面

```bash
export PATH=/toolchain/bin:$PATH
```

搜索顺序：

```text
先找 /toolchain/bin
再找原来的目录
```

工具链目录优先级更高。

#### 放后面

```bash
export PATH=$PATH:/toolchain/bin
```

搜索顺序：

```text
先找原来的目录
最后找 /toolchain/bin
```

假如不同目录中存在同名工具，排在前面的那个会被使用。

嵌入式工具链名称通常比较特殊，例如：

```text
arm-buildroot-linux-gnueabihf-gcc
```

一般不会和系统普通工具重名，所以放前放后的区别可能不明显。但通常把指定工具链路径放在前面更明确：

```bash
export PATH=/toolchain/bin:$PATH
```

------

## 五、这三个变量是怎么配合的？

可以把它们分别理解成三个问题。

### 1. `ARCH=arm`

回答：

```text
要为哪种处理器架构构建？
```

答案：

```text
ARM
```

### 2. `CROSS_COMPILE=arm-buildroot-linux-gnueabihf-`

回答：

```text
构建时应该使用哪一套工具？
```

答案：

```text
使用 arm-buildroot-linux-gnueabihf- 这一套工具前缀。
```

### 3. `PATH=/工具链目录/bin:$PATH`

回答：

```text
这些工具具体存放在哪里？
```

答案：

```text
到这个 bin 目录里寻找。
```

所以三者的关系是：

```text
ARCH
告诉构建系统：目标是 ARM

CROSS_COMPILE
告诉构建系统：工具名称以什么开头

PATH
告诉 Shell：去什么目录寻找这些工具
```

------

## 六、用“寄快递”来理解

假设你要从电脑上制作一个程序，然后交给 ARM 开发板。

### `ARCH=arm`

相当于填写收件地区：

```text
目的地：ARM 平台
```

### `CROSS_COMPILE=arm-buildroot-linux-gnueabihf-`

相当于指定生产线：

```text
使用 ARM 专用生产线
```

### `PATH=...`

相当于告诉工作人员：

```text
ARM 专用生产线位于哪一个车间
```

三项配合起来才完整：

```text
做给谁用
→ 用哪套工具做
→ 去哪里找到这套工具
```

------

## 七、完整操作示例

假设工具链目录是：

```text
/home/book/toolchain/bin
```

设置环境变量：

```bash
export ARCH=arm
export CROSS_COMPILE=arm-buildroot-linux-gnueabihf-
export PATH=/home/book/toolchain/bin:$PATH
```

检查：

```bash
echo $ARCH
echo $CROSS_COMPILE
echo $PATH
```

确认交叉编译器能否找到：

```bash
which arm-buildroot-linux-gnueabihf-gcc
```

可能输出：

```text
/home/book/toolchain/bin/arm-buildroot-linux-gnueabihf-gcc
```

查看编译器版本：

```bash
arm-buildroot-linux-gnueabihf-gcc --version
```

编译：

```bash
${CROSS_COMPILE}gcc -o hello hello.c
```

检查结果：

```bash
file hello
```

应该看到类似：

```text
ELF 32-bit LSB executable, ARM
```

假如看到：

```text
x86-64
```

说明你仍然使用了电脑上的普通 GCC，而不是 ARM 交叉编译器。

------

## 八、为什么有时只执行 `make` 就可以？

因为 Makefile 可能已经写好了：

```makefile
CC = $(CROSS_COMPILE)gcc
```

你设置：

```bash
export CROSS_COMPILE=arm-buildroot-linux-gnueabihf-
```

然后执行：

```bash
make
```

Makefile 就会自动组合为：

```text
arm-buildroot-linux-gnueabihf-gcc
```

因此很多嵌入式项目的标准构建方式是：

```bash
export ARCH=arm
export CROSS_COMPILE=arm-buildroot-linux-gnueabihf-
export PATH=/工具链目录/bin:$PATH

make
```

但对于只有一个文件的 Hello 程序，也可以直接写：

```bash
arm-buildroot-linux-gnueabihf-gcc -o hello hello.c
```

此时 `ARCH` 和 `CROSS_COMPILE` 不一定是必需的，因为你已经明确写出了完整的编译器名称。

------

## 九、这些设置会一直存在吗？

通常只对**当前终端会话**有效。

关闭终端或重新登录后，变量可能消失。

可以验证：

```bash
echo $ARCH
```

新终端中可能什么都不输出。

如果每次都需要，可以把命令放入：

```text
~/.bashrc
```

然后执行：

```bash
source ~/.bashrc
```

不过学习不同开发板时，工具链可能不同，不建议把多个不同的 `ARCH`、`CROSS_COMPILE` 随意永久写入 `.bashrc`，否则容易混用工具链。更稳妥的方法是写一个项目专用脚本，例如：

```bash
#!/bin/bash

export ARCH=arm
export CROSS_COMPILE=arm-buildroot-linux-gnueabihf-
export PATH=/home/book/toolchain/bin:$PATH
```

保存为：

```text
setenv.sh
```

使用时执行：

```bash
source setenv.sh
```

这里要用 `source`，这样变量才会进入当前终端。

------

## 十、最容易混淆的三个结论

```text
ARCH=arm
不等于普通 gcc 自动变成 ARM 编译器。
CROSS_COMPILE=...
只是保存工具前缀，必须由 Makefile、脚本或命令主动使用。
PATH=...
只负责让 Shell 找到编译器，不负责决定编译器生成什么架构。
```

最终真正生成 ARM 程序的是：

```text
arm-buildroot-linux-gnueabihf-gcc
```

------

## 记忆版

```text
export：
让当前终端启动的程序也能读取这个变量。

ARCH=arm：
告诉构建系统，目标架构是 ARM。

CROSS_COMPILE=arm-buildroot-linux-gnueabihf-：
告诉构建系统，交叉工具链的命令前缀是什么。

PATH=/工具链/bin:$PATH：
告诉 Shell，到哪里寻找交叉编译器。

三者关系：
目标是谁 → ARCH
工具叫什么 → CROSS_COMPILE
工具在哪里 → PATH
```
