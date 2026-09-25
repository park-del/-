# 视频分析：嵌入式 Linux 开发环境介绍

> 视频时长约 **3分12秒**。
> 这一节正好回答了你上一节的疑问：**Windows、Ubuntu和Linux开发板分别负责什么，命令究竟由谁执行。**

------

## 1. 视频类型判断

**主要类型：** 讲解类、嵌入式Linux基础教程类
**次要类型：** 开发流程分析类、环境架构说明类

**判断依据：**

1. 先<span style="color:#FF0000; background:#00FF80;">比较单片机开发与嵌入式Linux开发的区别。</span>
2. 将“<span style="color:#FF0000; background:#00FF80;">编辑、编译、运行</span>”拆分到<span style="color:#FF0000; background:#00FF80;">Windows、Ubuntu和开发板</span>中。
3. 介绍<span style="color:#FF0000; background:#00FF80;"> Source Insight、FileZilla、MobaXterm、VMware Ubuntu 等工具的作用。</span>
4. 通过<span style="color:#FF0000; background:#00FF80;">箭头说明电脑、Ubuntu环境、开发板以及代码仓库之间</span>的关系。

------

## 2. 一句话总结

这个视频主要讲的是：

> **嵌入式Linux开发通常不是在一个软件里完成全部工作，而是<span style="color:#FF0000; background:#00FF80;">先在Windows中编辑代码</span>，<span style="color:#FF0000; background:#00FF80;">再在Ubuntu中交叉编译</span>，最后<span style="color:#FF0000; background:#00FF80;">把程序放到Linux开发板上运行</span>。**

最核心的流程是：

```text
Windows编辑代码
        ↓
传到Ubuntu
        ↓
Ubuntu交叉编译
        ↓
传到开发板
        ↓
开发板运行程序
```

------

## 3. 时间段拆解

| 时间段      | 内容概括                                                     | 画面/声音/人物表现                                           | 作用                                                         |
| ----------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 00:00-00:06 | briefly回顾嵌入式Linux系统组成                               | 画面<span style="color:#FF0000;">短暂出现 <span style="background:#00FF80;">bootloader</span>、<span style="background:#00FF80;">Linux内核</span>、<span style="background:#00FF80;">根文件系统和APP的对应关系</span></span> | 承接上一部分内容                                             |
| 00:06-00:41 | 介绍单片机或普通PC软件开发                                   | 画面展示集成开发环境IDE和单片机开发板，文字<span style="color:#FF0000; background:#00FF80;">强调IDE同时具有编辑、编译和运行功能</span> | 与嵌入式Linux开发形成对比                                    |
| 00:41-01:06 | 提出嵌入式Linux开发的三部分                                  | 画面分别<span style="color:#FF0000;">展示Windows电脑</span>、<span style="color:#FF0000;">服务器</span>和<span style="color:#FF0000;">开发板</span>，标出“<span style="color:#FF0000; background:#00FF80;">编辑、编译、运行”</span> | 建立<span style="color:#FF0000; background:#00FF80;">整个开发环境框架</span> |
| 01:06-01:33 | 介绍具体软件工具                                             | Windows端出现 <span style="color:#FF0000; background:#00FF80;">Source Insight、FileZilla、MobaXterm</span>；右侧是运行Ubuntu的VMware；下方是开发板 | 解释各工具分别承担什么任务                                   |
| 01:33-02:00 | 解释编辑、编译、运行的分工                                   | 讲师在画面上标出“1.编辑、2.编译、3.运行”，并画线连接电脑、Ubuntu和开发板 | 强化三阶段工作流                                             |
| 02:00-02:31 | 解释<span style="color:#FF0000; background:#00FF80;">远程操作Ubuntu</span>和<span style="color:#FF0000; background:#00FF80;">串口操作开发板</span> | 画面出现“<span style="background:#00FF80; color:#FF0000;">远程登录Ubuntu执行命令比较方便</span>”“<span style="color:#FF0000; background:#00FF80;">通过串口执行开发板的命令</span>”等说明 | 区分<span style="color:#FF0000; background:#00FF80;">Ubuntu命令</span>与<span style="color:#FF0000; background:#00FF80;">开发板命令</span> |
| 02:31-02:49 | 汇总电脑、Ubuntu和开发板之间的连接                           | 蓝色箭头将Windows、Ubuntu和开发板连接起来                    | 形成完整开发闭环                                             |
| 02:49-03:12 | 引入GitHub/Coding代码仓库                                    | 画面顶部出现“<span style="color:#FF0000; background:#00FF80;">代码在云端：github/coding”</span>，<span style="color:#FF0000; background:#00FF80;">箭头连接Windows和Ubuntu开发环境</span> | 补充<span style="color:#FF0000;">多人协作</span>和<span style="color:#FF0000;">代码同步方式</span> |

------

## 4. 内容结构

1. **开头：** 用单片机开发说明传统IDE可以集成编辑、编译和运行。
2. **发展：** 指出嵌入式Linux开发通常需要多个系统和工具共同配合。
3. **转折：** 将Windows、Ubuntu和开发板的职责明确拆分。
4. **结尾：** 加入GitHub或Coding，说明代码也可以通过云端仓库同步。

整体表达路径：

```text
从【单片机IDE的一体化开发】开始，
通过【嵌入式Linux编辑、编译、运行分离】展开，
最后落到【Windows、Ubuntu、开发板和云端代码仓库协同工作】。
```

------

## 5. 这节视频最核心的图应该怎么理解？

视频中展示的结构大致如下：

```text
┌────────────────── Windows电脑 ──────────────────┐
│                                                  │
│  Source Insight：编辑代码                         │
│  FileZilla：传输文件                              │
│  MobaXterm：远程登录Ubuntu                         │
│  MobaXterm：通过串口操作开发板                     │
│                                                  │
│  VMware中运行Ubuntu                               │
│       └── 使用交叉编译器编译程序                   │
└──────────────────────────────────────────────────┘
                   │
                   │ 网络、USB、串口等
                   ▼
             Linux开发板
               └── 运行程序
```

注意，视频画面虽然把Ubuntu称为“服务器”，但它不一定真的是另一台物理服务器。

它也可能是：

```text
你的Windows电脑
   └── VMware虚拟机
          └── Ubuntu系统
```

也就是说，一台电脑中同时存在：

- Windows宿主机；
- VMware；
- Ubuntu虚拟机。

------

## 6. 为什么单片机可以在一个IDE里完成，而嵌入式Linux要分开？

### 单片机开发

单片机开发软件，例如 Keil，通常把很多功能集中在一起：

```text
写代码
  ↓
点击编译
  ↓
下载程序
  ↓
在单片机上运行或调试
```

一个IDE中通常集成了：

- 代码编辑器；
- 编译器；
- 下载工具；
- 调试器；
- 工程管理工具。

所以初学时会感觉：

> 打开一个软件就能完成所有事情。

------

### 嵌入式Linux开发

嵌入式Linux开发更复杂，通常涉及三个不同环境：

```text
Windows：方便编辑和管理代码
Ubuntu：提供Linux编译环境和交叉编译工具
开发板：真正运行生成的程序
```

它们的处理器、系统和工具可能不同，因此工作会被拆开。

------

## 7.Windows、Ubuntu、开发板分别负责什么？

### 7.1Windows：主要负责编辑和操作工具

视频中Windows端展示了这些软件：

#### Source Insight

作用：

```text
阅读代码
编辑代码
跳转函数
查找变量
分析大型工程
```

它<span style="color:#FF0000; background:#00FF80;">主要负责“写代码”，一般不负责生成最终在ARM开发板上运行的程序。</span>

------

#### FileZilla

作用：

```text
把Windows文件传到Ubuntu
把Ubuntu文件下载回Windows
```

例如你在Windows中编辑：

```text
hello.c
```

然后<span style="color:#FF0000; background:#00FF80;">使用FileZilla上传到Ubuntu：</span>

```text
/home/book/project/hello.c
```

------

#### MobaXterm

视频中MobaXterm有两个主要作用。

##### 作用一：远程操作Ubuntu

例如<span style="color:#FF0000; background:#00FF80;">使用SSH登录Ubuntu：</span>

```bash
ssh book@Ubuntu的IP地址
```

登录后出现：

```bash
book@ubuntu:~$
```

<span style="color:#FF0000; background:#00FF80;">此时输入的命令由Ubuntu执行：</span>

```bash
ls
cd project
arm-linux-gcc hello.c -o hello
```

------

##### 作用二：通过串口操作开发板

<span style="color:#FF0000; background:#00FF80;">连接开发板串口后，终端提示符可能是：</span>

```bash
root@board:~#
```

此时<span style="color:#FF0000; background:#00FF80;">输入的命令由开发板执行：</span>

```bash
ls
chmod +x hello
./hello
```

因此，<span style="color:#FF0000; background:#00FF80;">虽然都使用MobaXterm窗口，但命令执行者不同。</span>

------

### 7.2Ubuntu：主要负责编译

Ubuntu中通常安装：

- GCC；
- Make；
- 交叉编译器；
- Linux内核编译环境；
- U-Boot编译环境；
- 根文件系统制作工具；
- Shell和其他Linux工具。

例如<span style="color:#FF0000; background:#00FF80;">开发板使用ARM处理器，Ubuntu是x86电脑，那么普通：</span>

```bash
gcc hello.c -o hello
```

通常<span style="color:#FF0000; background:#00FF80;">生成的是给Ubuntu电脑自身运行的程序。</span>

开发板需要的则可能是：

```bash
arm-linux-gnueabihf-gcc hello.c -o hello
```

或<span style="color:#FF0000; background:#00FF80;">课程提供的其他交叉编译器。</span>

这种过程叫：

> <span style="background:#00FF80; color:#FF0000;">**交叉编译。**</span>

即：

```text
在一种处理器平台上编译
生成给另一种处理器平台运行的程序
```

例如：

```text
Ubuntu x86电脑上编译
        ↓
生成ARM程序
        ↓
放到ARM开发板运行
```

------

### 7.3 Linux开发板：负责运行和测试

Ubuntu编译完成后，会生成可执行程序，例如：

```text
hello
```

然后<span style="color:#FF0000; background:#00FF80;">把它传到开发板。</span>

<span style="color:#FF0000; background:#00FF80;">在开发板中执行：</span>

```bash
chmod +x hello
./hello
```

<span style="color:#FF0000; background:#00FF80;">真正运行程序的是开发板的CPU。</span>

所以：

```text
Ubuntu生成程序
开发板执行程序
```

它们不是同一件事。

------

## 8. “编辑、编译、运行”完整举例

假设你要编写一个程序：

```c
#include <stdio.h>

int main(void)
{
    printf("Hello, embedded Linux!\n");
    return 0;
}
```

文件名是：

```text
hello.c
```

------

### 第一步：在Windows中编辑

使用Source Insight编写并保存：

```text
D:\project\hello.c
```

这里完成的是：

> 编辑源代码。

程序还没有变成开发板可以执行的文件。

------

### 第二步：把源代码传到Ubuntu

使用FileZilla上传：

```text
Windows：
D:\project\hello.c

        ↓

Ubuntu：
/home/book/project/hello.c
```

------

### 第三步：在Ubuntu中编译

通过MobaXterm远程登录Ubuntu：

```bash
ssh book@192.168.1.10
```

然后编译：

```bash
cd /home/book/project
arm-linux-gnueabihf-gcc hello.c -o hello
```

生成：

```text
hello
```

这个 `hello` 是二进制可执行程序。

------

### 第四步：把程序传到开发板

可以<span style="color:#FF0000; background:#00FF80;">通过网络、NFS、TFTP、SCP、U盘或其他方式传输。</span>

例如：

```bash
scp hello root@192.168.1.100:/root/
```

------

### 第五步：在开发板上运行

<span style="color:#FF0000; background:#00FF80;">通过串口或SSH登录开发板：</span>

```bash
root@board:~#
```

执行：

```bash
cd /root
chmod +x hello
./hello
```

输出：

```text
Hello, embedded Linux!
```

完整过程就是：

```text
hello.c
  │
  │ Windows编辑
  ▼
hello.c
  │
  │ 上传到Ubuntu
  ▼
hello.c
  │
  │ Ubuntu交叉编译
  ▼
hello
  │
  │ 传到开发板
  ▼
开发板运行 ./hello
```

------

## 9. 结合你上一节的疑问：命令到底是谁执行的？

必须<span style="color:#FF0000; background:#00FF80;">看当前终端登录到了谁。</span>

### 情况一：Ubuntu终端

提示符：

```bash
book@ubuntu:~$
```

输入：

```bash
vi hello.c
```

是<span style="color:#FF0000; background:#00FF80;">Ubuntu执行VI。</span>

输入：

```bash
gcc hello.c -o hello
```

也是<span style="color:#FF0000; background:#00FF80;">Ubuntu执行编译。</span>

------

### 情况二：开发板终端

提示符：

```bash
root@board:~#
```

输入：

```bash
vi config.txt
```

是<span style="color:#FF0000; background:#00FF80;">开发板执行VI。</span>

输入：

```bash
./hello
```

是<span style="color:#FF0000; background:#00FF80;">开发板执行程序。</span>

------

### 情况三：Windows命令行

提示符类似：

```text
C:\Users\book>
```

输入的是Windows命令。

因此，判断方式不是看“终端窗口在哪台电脑上显示”，而是看：

```text
当前终端会话连接的是哪一个系统。
```

------

## 10. 视频中“服务器”是什么意思？

视频画面写的是：

```text
2.编译：在服务器
```

右侧又标注：

```text
服务器：用VMware运行Ubuntu
```

这里的<span style="color:#FF0000; background:#00FF80;">“服务器”</span>更像是在强调：

> <span style="color:#FF0000; background:#00FF80;">使用一个Linux环境来集中存放代码并完成编译。</span>

它可能有三种形式：

### 形式一：本机Ubuntu虚拟机

```text
Windows电脑
  └── VMware
       └── Ubuntu
```

这是课程初学阶段最常见的情况。

### 形式二：局域网中的Ubuntu电脑

```text
Windows电脑
      │
      │ SSH
      ▼
另一台Ubuntu电脑
```

### 形式三：远程Linux服务器

```text
Windows电脑
      │
      │ Internet/SSH
      ▼
云服务器或公司编译服务器
```

视频重点不是要求你必须买一台服务器，而是告诉你：

> <span style="color:#FF0000; background:#00FF80;">编译工作通常需要一个Ubuntu/Linux环境。</span>

------

## 11. 为什么不直接在开发板上编译？

理论上，有些开发板可以直接安装GCC并编译：

```bash
gcc hello.c -o hello
```

但嵌入式开发通常不这样做，原因包括：

- 开发板CPU性能较弱；
- 内存和存储空间较小；
- 编译Linux内核等大型工程非常慢；
- 开发板文件系统可能很精简；
- 不一定安装完整编译工具；
- PC上的Ubuntu更方便备份和管理工程。

所以常见做法是：

```text
高性能电脑负责编译
开发板只负责运行和测试
```

------

##  12.GitHub /Coding在视频中的作用

视频最后在顶部写了：

```text
代码在云端：github/coding
```

这表示<span style="color:#FF0000; background:#00FF80;">源代码不一定只保存在Windows或Ubuntu中，还可以上传到代码仓库。</span>

结构变成：

```text
             GitHub / Coding
               ↙       ↘
        Windows          Ubuntu
          编辑             编译
             ↘           ↙
               Linux开发板
                    运行
```

例如：

在<span style="color:#FF0000; background:#00FF80;">Windows中修改后上传：</span>

```bash
git add .
git commit -m "update hello"
git push
```

<span style="color:#FF0000; background:#00FF80;">Ubuntu中拉取：</span>

```bash
git pull
```

然后<span style="color:#FF0000; background:#00FF80;">编译。</span>

这样<span style="color:#FF0000; background:#00FF80;">不需要每次都依赖FileZilla手动上传文件，同时还能：</span>

- 保存版本历史；
- 多人协作；
- 回退错误修改；
- 在不同电脑间同步代码。

<span style="color:#FF0000; background:#00FF80;">视频这里只是介绍概念，没有完整演示Git命令。</span>

------

## 13. 观点分析

视频没有争议性观点，主要是开发方法上的判断。

| 观点内容                                          | 观点归属       | 明确/可能 | 判断依据                                   |
| ------------------------------------------------- | -------------- | --------- | ------------------------------------------ |
| 单片机开发通常可以在集成IDE中完成编辑、编译和运行 | 讲师观点       | 明确      | 00:06后画面直接标注                        |
| 嵌入式Linux开发通常将编辑、编译和运行分开         | 讲师观点       | 明确      | Windows、服务器和开发板三部分图示          |
| Windows更适合进行代码编辑和使用图形工具           | 讲师观点       | 明确      | Source Insight、FileZilla等均放在Windows端 |
| Ubuntu主要负责Linux环境下的编译                   | 讲师观点       | 明确      | 图中“编译：在服务器”和Ubuntu虚拟机         |
| 程序最终应在开发板上运行和验证                    | 讲师观点       | 明确      | 图中“运行：在开发板”                       |
| 远程登录Ubuntu比直接切换虚拟机窗口方便            | 讲师观点       | 明确      | 画面出现相应说明                           |
| 使用GitHub或Coding能更方便地同步代码              | 可能是作者观点 | 可能      | 结尾将云端代码仓库连接两个开发环境         |

**核心观点：**

```text
嵌入式Linux开发是多环境协作：
Windows负责方便地编辑和管理，
Ubuntu负责构建和交叉编译，
开发板负责真实运行与测试。
```

------

## 14. 人物分析

视频中没有清晰出镜人物，主要是教学画面和手写标注。

因此不能可靠分析人物的情绪、心理、性格或人格。

明确能判断的是：

- 讲师使用对比法讲解；
- 先展示单片机IDE，再说明Linux开发为什么需要多个环境；
- 通过箭头和工具截图解释不同系统之间的联系；
- 教学重点是建立整体框架，而不是立即讲具体命令。

------

## 15. Who / Whom / Whose / When / Where / What / Why / How

| 维度  | 视频中的答案                                                 |
| ----- | ------------------------------------------------------------ |
| Who   | 嵌入式Linux开发者                                            |
| Whom  | 面向准备学习嵌入式Linux开发的初学者                          |
| Whose | 源代码由开发者编写；编译任务由Ubuntu环境完成；运行任务属于开发板 |
| When  | 编写、构建和测试嵌入式Linux程序时                            |
| Where | Windows电脑、Ubuntu虚拟机或服务器、Linux开发板               |
| What  | 编辑代码、传输文件、交叉编译、下载程序、运行测试             |
| Why   | 开发板资源有限，而且Windows、Ubuntu和开发板各自适合不同任务  |
| How   | 使用Source Insight编辑，FileZilla或Git传输，MobaXterm远程登录，Ubuntu编译，串口或网络操作开发板 |

------

## 16. 重点片段精读

### 片段一：00:06-00:41

**表面发生了什么：**
讲师介绍单片机或普通PC软件开发使用集成IDE。

**更深层可能意味着：**
这是后面理解嵌入式Linux开发环境的对照组。单片机开发看起来只有一个IDE，而Linux开发需要多个系统配合。

**重要性：**
解释“为什么嵌入式Linux环境看起来比单片机复杂”。

------

### 片段二：00:41-01:06

**表面发生了什么：**
画面将任务拆成：

```text
编辑：Windows
编译：服务器
运行：开发板
```

**更深层可能意味着：**
同一项目的三个步骤不一定由同一个系统完成。源代码、可执行程序以及最终运行环境需要明确区分。

**重要性：**
这是整段视频最核心的框架。

------

### 片段三：01:06-02:31

**表面发生了什么：**
展示Source Insight、FileZilla、MobaXterm、VMware Ubuntu以及开发板。

**更深层可能意味着：**
<span style="color:#FF0000; background:#00FF80;">工具只是帮助你连接三个环境：</span>

```text
Source Insight解决编辑
FileZilla解决传输
MobaXterm解决远程终端和串口
VMware解决Ubuntu环境
```

**重要性：**
避免把所有软件都误认为编译器，或者误认为所有终端都连接开发板。

------

### 片段四：02:49-03:12

**表面发生了什么：**
加入GitHub和Coding，并连接Windows、Ubuntu环境。

**更深层可能意味着：**
<span style="color:#FF0000; background:#00FF80;">代码传输不只有FileZilla。实际工程中通常更推荐使用Git进行版本管理和同步</span>。

**重要性：**
从个人练习流程扩展到更规范的工程开发流程。

------

## 17. 我应该重点看什么？

学习这节时，重点不是记住所有软件名称，而是记住“职责划分”。

```text
Source Insight：编辑代码
FileZilla：传输文件
MobaXterm：远程登录Ubuntu、串口连接开发板
VMware Ubuntu：提供Linux编译环境
开发板：运行程序
GitHub/Coding：保存和同步代码
```

还要特别分清三种文件：

```text
hello.c
源代码，Windows和Ubuntu都可以查看、编辑。

hello
经过交叉编译生成的可执行程序。

开发板上的hello
最终由开发板CPU执行的程序。
```

------

## 18. 记忆版总结

```text
类型：
嵌入式Linux开发环境讲解

主线：
比较单片机的一体化IDE与嵌入式Linux的多环境协作。

三个核心环境：
Windows：编辑
Ubuntu：交叉编译
开发板：运行

四个常见工具：
Source Insight：写代码
FileZilla：传文件
MobaXterm：远程终端和串口
VMware：运行Ubuntu虚拟机

完整流程：
Windows写hello.c
→ 上传到Ubuntu
→ Ubuntu交叉编译
→ 传到开发板
→ 开发板执行

最重要时间段：
00:41-01:06 三环境分工
01:06-02:31 具体工具及连接关系
02:49-03:12 GitHub/Coding代码同步

一句话记忆：
代码在Windows写，在Ubuntu编译，在开发板运行；
终端命令由谁执行，要看终端当前登录的是Ubuntu还是开发板。
```





# 问题1

<span style="color:#FF0000; background:#00FF80;">我有个问题，就是开发板上和VM虚拟机上安装的都是linux系统是吗，VM虚拟机上的是ubuntu这个Linux系统，但是它们是不同架构的，一个x86架构，一个是arm架构，为什么可以在x86架构上的ubuntu系统中编译然后上传到amr架构上的Linux开发板上运行呢？</span>



对，通常情况下：

- **VMware 虚拟机里安装的是 Ubuntu Linux**，一般运行在电脑的 **x86-64** 处理器架构上。
- **嵌入式开发板里也运行 Linux**，但处理器往往是 **ARM、AArch64** 等架构。

它们都是 Linux，但处理器架构不同。<span style="color:#FF0000; background:#00FF80;">之所以能在 x86 Ubuntu 上编译出 ARM 开发板能运行的程序，是因为使用了：</span>

> **交叉编译器 Cross Compiler**

------

## 一、先理解普通编译为什么不行

假设在 x86 Ubuntu 中有一个程序：

```c
#include <stdio.h>

int main(void)
{
    printf("Hello\n");
    return 0;
}
```

文件名为：

```text
hello.c
```

如果在 Ubuntu 中使用普通 GCC：

```bash
gcc hello.c -o hello
```

这个 `gcc` 默认认为：

> 我现在运行在 x86-64 电脑上，所以生成给 x86-64 CPU 执行的程序。

因此生成的 `hello` 是：

```text
x86-64机器指令
```

它可以在 Ubuntu 虚拟机中执行：

```bash
./hello
```

但把它复制到 ARM 开发板后，通常不能运行，可能出现：

```text
cannot execute binary file: Exec format error
```

因为 <span style="color:#FF0000; background:#00FF80;">ARM CPU看不懂 x86-64机器指令。</span>

------

## 二、交叉编译器解决了什么问题？

在<span style="color:#FF0000; background:#00FF80;"> x86 Ubuntu 中安装一个专门面向 ARM 的编译器</span>，例如：

```bash
arm-linux-gnueabihf-gcc
```

或者64位ARM的：

```bash
aarch64-linux-gnu-gcc
```

它们<span style="color:#FF0000;">本身是 **x86-64程序**，所以能够在 Ubuntu 虚拟机中运行。</span>

但是<span style="color:#FF0000;">它们生成的结果是 **ARM机器指令**。</span>

例如：

```bash
arm-linux-gnueabihf-gcc hello.c -o hello
```

整个过程是：

```text
x86 Ubuntu
    │
    │ 运行一个x86版本的ARM交叉编译器
    ▼
读取 hello.c
    │
    ▼
生成ARM机器指令
    │
    ▼
得到ARM可执行文件 hello
    │
    │ 传到开发板
    ▼
ARM开发板运行 ./hello
```

所以，<span style="color:#FF0000; background:#00FF80;">关键并不是：</span>

> “<span style="background:#00FF80;">在哪里编译就一定生成哪里的程序。</span>”

而是：

> **<span style="background:#00FF80;">使用什么目标编译器，决定生成哪种架构的机器指令</span>。**

------

## 三、一个容易理解的比喻

可以把源代码理解成中文原稿：

```text
hello.c = 中文原稿
```

普通 x86 GCC 像一位翻译员，它把中文翻译成“x86语言”：

```text
中文源代码
   ↓ x86 GCC
x86机器指令
```

ARM交叉编译器则把同一份中文原稿翻译成“ARM语言”：

```text
中文源代码
   ↓ ARM交叉编译器
ARM机器指令
```

翻译员本人工作在哪里，与翻译结果使用哪种语言不是一回事。

例如：

- 翻译员本人可以在中国工作；
- 但他可以翻译出英语文档；
- 英语文档最后交给英语使用者阅读。

同理：

- 交叉编译器运行在 x86 Ubuntu；
- 但它生成 ARM机器码；
- 最后交给 ARM开发板运行。

------

## 四、交叉编译器本身是什么架构？

这是最容易绕的地方。

以：

```bash
arm-linux-gnueabihf-gcc
```

为例，它包含两个不同概念。

### 编译器自己

<span style="color:#FF0000; background:#00FF80;">编译器程序</span>运行在 x86 Ubuntu中，因此它本身通常是：

```text
x86-64可执行程序
```

Ubuntu的CPU能够执行它。

### 编译器生成的程序

它生成的是：

```text
ARM 32位可执行程序
```

ARM开发板能够执行它。

因此：

```text
交叉编译器自身架构：x86-64
交叉编译器输出架构：ARM
```

这两个架构可以不同。

------

## 五、可以用 `file` 命令验证

### 普通编译

在 x86 Ubuntu中执行：

```bash
gcc hello.c -o hello_x86
file hello_x86
```

可能显示：

```text
hello_x86: ELF 64-bit LSB executable, x86-64
```

说明它是 x86-64程序。

------

### ARM交叉编译

执行：

```bash
arm-linux-gnueabihf-gcc hello.c -o hello_arm
file hello_arm
```

可能显示：

```text
hello_arm: ELF 32-bit LSB executable, ARM
```

说明它是32位ARM程序。

如果使用：

```bash
aarch64-linux-gnu-gcc hello.c -o hello_arm64
file hello_arm64
```

可能显示：

```text
hello_arm64: ELF 64-bit LSB executable, ARM aarch64
```

此时三个文件虽然都在 x86 Ubuntu中生成，但目标架构不同：

| 文件          | 目标架构        | 在哪里运行    |
| ------------- | --------------- | ------------- |
| `hello_x86`   | x86-64          | Ubuntu虚拟机  |
| `hello_arm`   | 32位ARM         | 32位ARM开发板 |
| `hello_arm64` | 64位ARM/AArch64 | 64位ARM开发板 |

------

## 六、“都是Linux”在这里有什么作用？

<span style="color:#FF0000; background:#00FF80;">开发板和Ubuntu都是Linux，主要意味着它们使用相似的：</span>

- Linux系统调用；
- ELF可执行文件格式；
- C语言运行环境；
- 文件、进程、线程、网络等编程接口；
- Shell和常见Linux工具。

例如下面的程序调用：

```c
printf("Hello\n");
```

交叉编译器会按照目标开发板的Linux环境进行编译和链接。

但是需要注意：

> 都是Linux，并不表示二进制程序可以直接互相运行。

<span style="color:#FF0000; background:#00FF80;">二进制程序能否运行，还要同时满足：</span>

```text
CPU架构兼容
操作系统接口兼容
位数兼容
ABI兼容
动态链接库兼容
```

例如：

```text
Ubuntu x86-64程序
```

和：

```text
开发板ARM Linux程序
```

虽然都属于Linux程序，但机器指令不同，因此不能直接互相运行。

------

## 七、交叉编译实际上经历了什么？

以：

```bash
arm-linux-gnueabihf-gcc hello.c -o hello
```

为例，大致经过以下步骤。

### 1. 预处理

处理：

```c
#include <stdio.h>
#define ...
```

得到展开后的C代码。

### 2. 编译

把C语言转换成ARM汇编语言。

例如产生类似ARM指令：

```asm
mov
ldr
bl
```

而不是x86指令。

### 3. 汇编

把ARM汇编转换成ARM机器码。

### 4. 链接

把你的程序与目标开发板需要的：

- C标准库；
- 启动代码；
- 动态链接器信息；
- 其他库文件；

组合成ARM ELF可执行文件。

最后得到：

```text
hello
```

上传到ARM Linux开发板执行。

------

## 八、为什么不能只告诉普通GCC“生成ARM程序”？

因为编译器不仅需要知道ARM指令，还需要配套的目标环境。

完整的交叉编译工具链通常包括：

```text
ARM编译器
ARM汇编器
ARM链接器
ARM标准库
ARM头文件
目标系统的启动文件
调试工具
```

这套东西统称为：

> **交叉编译工具链 Cross Toolchain**

例如：

```text
arm-linux-gnueabihf-gcc
arm-linux-gnueabihf-as
arm-linux-gnueabihf-ld
arm-linux-gnueabihf-gdb
```

其中：

| 前缀部分    | 含义                        |
| ----------- | --------------------------- |
| `arm`       | 目标是32位ARM               |
| `linux`     | 目标运行Linux               |
| `gnueabihf` | 使用GNU EABI，采用硬浮点ABI |
| `gcc`       | C语言编译器                 |

------

## 九、不只是“ARM”就一定能运行

即使都是ARM，也还要继续匹配。

### 1. 32位和64位

32位ARM编译器：

```bash
arm-linux-gnueabihf-gcc
```

生成的是<span style="color:#FF0000; background:#00FF80;">32位ARM程序。</span>

64位ARM编译器：

```bash
aarch64-linux-gnu-gcc
```

生成的是<span style="color:#FF0000; background:#00FF80;">64位AArch64程序。</span>

<span style="color:#FF0000; border:1px solid #330000; background:#00FF80;">如果开发板只运行32位系统，64位程序通常无法执行。</span>

------

### 2. 浮点ABI

32位ARM中常见：

```text
gnueabi
gnueabihf
```

其中 `hf` 表示 hard float，即硬浮点ABI。

如果开发板的系统库和程序使用的ABI不匹配，也可能无法运行。

------

### 3. C语言库

开发板可能使用：

```text
glibc
musl
uClibc
```

交叉编译器链接的库必须与开发板环境兼容。

否则可能出现：

```text
No such file or directory
```

即使文件明明存在。

这种情况下，真正缺少的可能不是程序，而是程序需要的动态加载器，例如：

```text
/lib/ld-linux-armhf.so.3
```

------

### 4. 处理器指令集版本

例如同为ARM：

- ARMv7；
- ARMv8；
- Cortex-A7；
- Cortex-A53；
- Cortex-A72。

如果程序使用了开发板CPU不支持的高级指令，也可能出现：

```text
Illegal instruction
```

因此交叉编译时有时还会指定：

```bash
-march=armv7-a
-mcpu=cortex-a7
```

------

## 十、动态链接为什么可能出问题？

假设编译出一个ARM程序：

```bash
arm-linux-gnueabihf-gcc hello.c -o hello
```

这个程序可能没有把所有库代码包含在自身内部，而是在运行时寻找开发板上的：

```text
libc.so
动态加载器
其他共享库
```

因此即使CPU架构正确，如果开发板缺少对应库，程序仍然不能运行。

可以用：

```bash
file hello
```

查看架构，也可以使用交叉工具：

```bash
arm-linux-gnueabihf-readelf -l hello
```

查看程序需要的动态加载器。

在开发板上可以使用：

```bash
ldd hello
```

查看依赖库，但某些精简系统可能没有 `ldd`。

------

## 十一、静态编译是什么？

为了减少开发板缺少动态库的问题，可以尝试静态编译：

```bash
arm-linux-gnueabihf-gcc hello.c -static -o hello
```

<span style="color:#FF0000; background:#00FF80;">静态编译会尽量把所需库代码放进可执行文件中。</span>

优点：

```text
上传到开发板后更容易直接运行
不太依赖开发板上的动态库
```

缺点：

```text
文件更大
并非所有库都适合静态链接
某些功能仍可能依赖系统配置
```

所以静态编译不是万能方案，但很适合用来理解依赖关系。

------

## 十二、三个重要概念：Build、Host、Target

交叉编译中经常出现三个术语。

### Build

编译工具链是在什么系统上构建出来的。

### Host

编译器当前运行在哪里。

本课程通常是：

```text
Host = x86-64 Ubuntu
```

### Target

编译结果最终在哪里运行。

例如：

```text
Target = ARM Linux开发板
```

简化理解：

```text
x86 Ubuntu运行编译器
        │
        │ 输入C源代码
        ▼
生成ARM Linux程序
        │
        ▼
ARM开发板执行
```

在初学阶段，先记住：

```text
Host：编译发生的平台
Target：程序最终运行的平台
```

即可。

------

## 十三、完整实例

假设：

```text
Ubuntu虚拟机：x86-64
开发板：ARM 32位
```

### 在Ubuntu中查看自己的架构

```bash
uname -m
```

可能输出：

```text
x86_64
```

------

### 使用普通GCC编译

```bash
gcc hello.c -o hello_x86
file hello_x86
```

结果：

```text
ELF 64-bit, x86-64
```

它应该在Ubuntu中运行：

```bash
./hello_x86
```

不能直接放到ARM开发板运行。

------

### 使用交叉编译器

```bash
arm-linux-gnueabihf-gcc hello.c -o hello_arm
file hello_arm
```

结果：

```text
ELF 32-bit, ARM
```

把它传到开发板：

```bash
scp hello_arm root@开发板IP:/root/
```

登录开发板：

```bash
ssh root@开发板IP
```

确认开发板架构：

```bash
uname -m
```

可能输出：

```text
armv7l
```

运行：

```bash
chmod +x hello_arm
./hello_arm
```

这样才是完整的交叉编译流程。

------

## 十四、为什么不直接在开发板上编译？

开发板上理论上也可以安装本地ARM GCC：

```bash
gcc hello.c -o hello
```

此时：

- GCC本身是ARM程序；
- GCC在ARM开发板上运行；
- 输出也是ARM程序。

这叫：

> **本地编译 Native Compilation**

但开发板通常：

- CPU比电脑慢；
- 内存比较小；
- 存储空间有限；
- 编译大型项目耗时很长；
- 系统可能没有安装完整开发工具。

所以更常见的是：

```text
x86电脑承担繁重编译
ARM开发板只负责运行和测试
```

------

## 十五、把你的理解修正成一句准确的话

你原来的理解可以修正为：

> VMware中的Ubuntu和开发板运行的都是Linux，但它们面向不同的CPU架构。Ubuntu中的普通GCC会生成x86程序，而ARM交叉编译器虽然运行在x86 Ubuntu上，却会生成ARM机器指令。只要生成程序的架构、位数、ABI和库环境与开发板兼容，就可以传到ARM Linux开发板上运行。

------

## 十六、最简单的记忆方法

```text
源代码本身通常不属于x86或ARM。
编译器输出的机器码才区分x86和ARM。

普通gcc：
在x86 Ubuntu运行，生成x86程序。

ARM交叉gcc：
在x86 Ubuntu运行，生成ARM程序。

上传到开发板的不是x86编译器，
而是交叉编译器生成的ARM可执行文件。
```

最关键的一句话：

> **编译器“运行在哪个平台”和它“为哪个平台生成程序”，是两个不同的问题。**





# 问题2

<span style="color:#FF0000; background:#00FF80;">为什么不直接在windows下载一个 arm-linux的软件直接编译呢，还套上一个ubuntu系统，不是感觉多了一层有点麻烦吗？</span>



可以，**并不是必须套一层 Ubuntu 才能交叉编译 ARM 程序**。

你完全可以在 Windows 上安装一个“运行在 Windows、生成 ARM Linux 程序”的交叉编译器，例如：

```text
Windows x86-64
    ↓ 运行 Windows 版 ARM 交叉编译器
生成 ARM Linux 可执行文件
    ↓
传到 ARM 开发板运行
```

<span style="color:#FF0000; background:#00FF80;">但嵌入式 Linux 课程通常仍然使用 Ubuntu，主要不是因为 Windows 做不到，而是因为：</span>

> <span style="color:#FF0000; background:#00FF80;">**嵌入式 Linux 开发不只是编译一个 `.c` 文件，还会涉及大量原本面向 Linux 环境设计的工具、脚本和工程。**</span>

------

## 1. Windows上直接编译，技术上完全可行

假设有一个专门为 Windows 制作的交叉编译器：

```text
arm-linux-gnueabihf-gcc.exe
```

它本身是 Windows 程序，所以可以在 Windows 中运行：

```powershell
arm-linux-gnueabihf-gcc.exe hello.c -o hello
```

它输出的 `hello` 则是 ARM Linux 程序。

这里有两个不同平台：

```text
编译器运行平台：Windows x86-64
生成程序的平台：ARM Linux
```

只要工具链制作正确，就没有原理上的障碍。

------

## 2. 编译一个简单C程序时，Windows确实更直接

例如只编译：

```c
#include <stdio.h>

int main(void)
{
    printf("Hello\n");
    return 0;
}
```

Windows安装好交叉编译器后，可以直接：

```powershell
arm-linux-gnueabihf-gcc hello.c -o hello
```

再上传到开发板。

这种简单情况下，Ubuntu虚拟机看起来确实多了一层：

```text
Windows
  ↓
Ubuntu虚拟机
  ↓
交叉编译
```

而直接使用Windows工具链可以变成：

```text
Windows
  ↓
交叉编译
```

所以你的感觉是对的：

> **只看单文件编译，Ubuntu虚拟机不是绝对必要的。**

------

## 3. 真正的问题在于：嵌入式Linux工程不只有GCC

<span style="color:#FF0000; background:#00FF80;">开发中经常还会用到：</span>

```text
make
CMake
Bash脚本
Shell命令
sed
awk
grep
find
tar
rsync
pkg-config
Git
Python脚本
设备树编译器
内核构建工具
根文件系统制作工具
```

<span style="color:#FF0000; background:#00FF80;">这些工具在Linux中原生存在，并且大量工程默认就是按照Linux环境写的。</span>

例如一个构建脚本可能是：

```bash
#!/bin/bash

export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-

make clean
make -j8
```

它直接依赖：

```text
Bash
export
make
Linux路径规则
Linux命令
```

在Windows的普通CMD或PowerShell中不能原样执行。

你需要：

- 修改脚本；
- 安装额外兼容工具；
- 处理路径差异；
- 处理命令差异。

这时<span style="color:#FF0000; background:#00FF80;">反而可能比使用Ubuntu更麻烦。</span>

------

## 4. Windows和Linux的路径规则不同

Linux路径：

```text
/home/book/project/main.c
```

Windows路径：

```text
C:\Users\book\project\main.c
```

很多嵌入式工程的Makefile中会出现：

```makefile
include /home/book/toolchain/config.mk
```

或者Shell脚本中写：

```bash
cp ./output/uImage /home/book/tftpboot/
```

<span style="color:#FF0000; background:#00FF80;">直接放到Windows中，路径就无法正常使用。</span>

另外，Windows使用反斜杠：

```text
\
```

Linux主要使用正斜杠：

```text
/
```

某些构建工具虽然可以兼容，但复杂工程中更容易出现问题。

------

## 5. Windows和Linux对文件名大小写的处理不同

Linux通常严格区分：

```text
Main.c
main.c
```

它们是两个不同的文件。

Windows常见文件系统默认不严格区分大小写，因此可能把二者当作同一个文件。

假设工程中同时存在：

```text
include/Config.h
include/config.h
```

<span style="color:#FF0000; background:#00FF80;">在Linux中可以区分，在Windows中可能产生冲突。</span>

<span style="color:#FF0000; background:#00FF80;">嵌入式Linux的：</span>

- Linux内核；
- U-Boot；
- Buildroot；
- Yocto；
- 开源库；

<span style="color:#FF0000; background:#00FF80;">大多是在Linux文件系统规则下开发和测试的，所以Ubuntu环境更稳妥。</span>

------

## 6. 很多构建脚本默认使用Linux Shell

例如：

```makefile
SOURCE_FILES := $(shell find ./src -name "*.c")
```

这里的：

```text
find
```

是<span style="color:#FF0000; background:#00FF80;">Linux命令。</span>

或者：

```makefile
VERSION := $(shell git describe --always)
```

又或者：

```bash
sed -i 's/DEBUG=0/DEBUG=1/g' config.txt
```

<span style="color:#FF0000; background:#00FF80;">这些命令在Linux里直接能用。</span>

<span style="color:#FF0000; background:#00FF80;">Windows原生CMD中不一定有：</span>

```text
find
sed
awk
grep
chmod
ln
```

你<span style="color:#FF0000; background:#00FF80;">可以安装替代工具，但最后实际上是在Windows中重新搭一个类似Linux的环境。</span>

------

## 7. Linux内核和U-Boot本身就是Linux风格工程

如果你只开发普通应用程序，Windows交叉编译比较容易。

但嵌入式Linux还可能需要编译：

```text
U-Boot
Linux内核
设备树
内核驱动
Buildroot
Yocto
根文件系统
```

这些项目通常依赖：

- GNU Make；
- Bash；
- Perl；
- Python；
- GNU工具链；
- 符号链接；
- Linux权限位；
- 大小写敏感文件系统。

例如Linux中的符号链接：

```bash
ln -s libfoo.so.1 libfoo.so
```

在<span style="color:#FF0000; background:#00FF80;">嵌入式根文件系统中很常见。</span>

<span style="color:#FF0000; background:#00FF80;">Windows对符号链接、可执行权限和Linux文件属性的处理方式不同，因此直接制作根文件系统可能出问题。</span>

------

## 8. Linux文件权限在Windows上不完全一样

Linux文件有：

```text
读权限
写权限
执行权限
用户
用户组
```

例如：

```bash
chmod +x start.sh
```

表示让脚本能够执行。

嵌入式根文件系统中的一些程序和脚本必须保留执行权限。

如果用不合适的Windows工具复制、解压或重新打包，可能丢失：

- 执行权限；
- 符号链接；
- 所有者信息；
- 设备节点。

最终放到开发板后，可能出现：

```text
Permission denied
```

或者系统启动失败。

Ubuntu对这些Linux属性是原生支持的。

------

## 9. 开发板厂商通常直接提供Linux版工具链

很多开发板资料会给出类似：

```text
arm-linux-gnueabihf-gcc
aarch64-linux-gnu-gcc
```

这些工具链通常是给Linux x86-64主机使用的。

也就是：

```text
工具链自身：x86-64 Linux程序
目标程序：ARM Linux程序
```

如果你在Windows直接运行，可能看到：

```text
不是有效的 Win32 应用程序
```

因为厂商提供的编译器本身是Linux程序，不是Windows程序。

此时你需要自己另找Windows版工具链，而不同工具链的：

- GCC版本；
- C库；
- ABI；
- 默认配置；
- 头文件；
- 库文件；

可能不完全一致。

这可能导致“老师那里能编译，你这里报错”。

------

## 10. Ubuntu让所有学员的环境保持一致

课程最怕出现这种情况：

```text
学生A：Windows 10
学生B：Windows 11
学生C：安装了MSYS2
学生D：使用不同版本GCC
学生E：环境变量配置不同
```

如果统一发一个Ubuntu虚拟机镜像：

```text
相同Ubuntu版本
相同交叉编译器
相同目录
相同依赖库
相同脚本
相同账号
```

老师可以直接告诉所有人：

```bash
cd /home/book/project
make
```

每个人得到的结果基本相同。

所以虚拟机的一个重要价值是：

> **提供可复制、可恢复、统一的开发环境。**

它不只是为了“多装一个系统”。

------

## 11. Ubuntu虚拟机还可以保护Windows主系统

<span style="color:#FF0000; background:#00FF80;">嵌入式开发经常需要：</span>

- 修改环境变量；
- 安装很多依赖；
- 配置TFTP；
- 配置NFS；
- 修改网络；
- 运行脚本；
- 解压大型源码；
- 使用root权限。

如果全部直接装在Windows上，可能比较混乱。

虚拟机中可以随便折腾：

```text
配置错了
依赖装乱了
系统坏了
```

直接恢复虚拟机快照即可，不影响Windows。

------

## 12. 那现在最好的方案一定是VMware吗？

也不是。

<span style="color:#FF0000; background:#00FF80;">目前常见有三种方案。</span>

### 方案一：Windows原生交叉编译器

结构：

```text
Windows
  ↓
Windows版ARM交叉编译器
  ↓
ARM开发板
```

适合：

- 简单C/C++程序；
- 厂商提供Windows工具；
- 使用IDE开发；
- 不需要复杂Linux构建脚本。

优点：

- 少一层系统；
- 文件访问直接；
- 配置简单时效率高。

缺点：

- Linux工程兼容性可能较差；
- 脚本和Makefile可能需要修改；
- 内核、根文件系统等开发不方便。

------

### 方案二：Windows + WSL2

结构：

```text
Windows
  └── WSL2 Ubuntu
         ↓
       交叉编译
```

<span style="color:#FF0000; background:#00FF80;">WSL2是在Windows中运行Linux环境，使用体验通常比完整虚拟机更轻便。</span>

适合：

- Linux应用程序开发；
- 交叉编译；
- CMake、Make、Bash脚本；
- 不需要复杂图形桌面的情况。

优点：

- 启动快；
- 占用通常比完整虚拟机小；
- Windows和Linux文件交互方便；
- 可以直接使用Ubuntu软件包。

需要注意：

- 某些USB、串口、网络、内核级功能配置起来可能更复杂；
- 部分旧课程是按VMware环境写的，目录和操作不完全相同。

------

## 方案三：Windows + VMware Ubuntu

结构：

```text
Windows宿主机
  └── 完整Ubuntu虚拟机
         ↓
       交叉编译
```

适合：

- 跟随课程学习；
- 编译内核和U-Boot；
- Buildroot、Yocto；
- NFS、TFTP等完整网络环境；
- 需要尽量接近独立Linux电脑。

优点：

- 是完整Linux系统；
- 与教材环境一致；
- 隔离性强；
- 可以做快照；
- 网络和文件系统行为更接近真实Linux。

缺点：

- 占用内存和磁盘；
- 文件传输多一步；
- 启动比WSL慢；
- 虚拟化有少量性能开销。

------

## 13. 为什么不直接安装双系统Ubuntu？

也可以。

结构：

```text
开机选择
├── Windows
└── Ubuntu
```

Ubuntu直接运行在真实硬件上，性能通常比虚拟机更好。

但缺点是：

- 切换系统要重启；
- 分区操作有风险；
- 初学者配置难度更大；
- Windows和Ubuntu不能同时使用。

所以课程通常选择VMware，是因为它更安全、更统一。

------

## 14. 一个具体对比

假设只编译：

```text
hello.c
```

Windows原生方案可能只需要：

```powershell
arm-linux-gcc hello.c -o hello
```

确实很方便。

但如果编译整个Linux内核，可能需要：

```bash
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-

make board_defconfig
make menuconfig
make -j8 zImage
make -j8 dtbs
make -j8 modules
```

过程中还可能调用：

```text
bash
make
perl
python
awk
sed
grep
bc
bison
flex
openssl
```

在Ubuntu中可以通过包管理器安装：

```bash
sudo apt install build-essential bison flex libssl-dev bc
```

而在Windows原生环境中，要逐个寻找兼容版本，还要解决路径和脚本问题。

所以：

```text
简单程序：Windows原生可能更方便
完整嵌入式Linux工程：Ubuntu通常更方便
```

------

## 15. 课程为什么选择Ubuntu虚拟机？

结合你正在看的课程，主要原因可能是：

```text
1. 厂商的交叉编译工具链是Linux版
2. 教材中的命令是Linux命令
3. 后面需要编译U-Boot、Linux内核和驱动
4. 需要使用Makefile和Shell脚本
5. 需要搭建NFS、TFTP等服务
6. 所有人使用相同虚拟机环境，便于教学和排错
```

所以课程不是为了简单编译一个 `hello.c` 才安装Ubuntu，而是在为后续整个嵌入式Linux开发流程准备统一环境。

------

## 16. 你可以怎样选择？

如果你现在只是练习：

```text
C语言小程序
简单ARM应用
单个源文件
```

可以尝试Windows原生交叉编译器。

如果你接下来要学习：

```text
Linux驱动
U-Boot
Linux内核
设备树
Buildroot
根文件系统
Makefile
Shell脚本
```

建议继续使用课程提供的Ubuntu虚拟机，避免后面因为环境不同产生大量额外问题。

更现代、轻便的折中方式是：

```text
Windows负责编辑
WSL2 Ubuntu负责编译
开发板负责运行
```

但跟课程时，最好先保持与讲师环境一致。

------

## 17. 最准确的一句话

> Windows上确实可以安装ARM交叉编译器，简单程序没有必要必须经过Ubuntu；但完整的嵌入式Linux开发依赖大量Linux工具、脚本、文件权限和构建系统，所以使用Ubuntu通常不是多余的一层，而是在提供一个与工程原生环境一致、稳定且容易复现的开发平台。

可以记成：

```text
Windows原生：能编译，但复杂Linux工程可能不方便。

Ubuntu虚拟机：多一层系统，
但少很多兼容性、脚本、依赖和环境问题。
```
