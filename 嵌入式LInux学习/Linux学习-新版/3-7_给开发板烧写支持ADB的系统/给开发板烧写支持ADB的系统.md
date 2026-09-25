# 视频分析：给开发板烧写支持 ADB 的系统（2025）

> 视频时长约 **7分54秒**。
> 这一节的核心不是单纯“安装ADB”，而是：**<span style="color:#FF0000; background:#00FF80;">把开发板eMMC中原来的Linux系统整体替换为一个已经集成 `adbd` 的新系统，然后验证ADB连接和ARM程序运行</span>。**

------

## 1. 视频类型判断

**主要类型：** 教程类、系统烧写操作演示类
**次要类型：** 开发板环境配置类、ADB连接验证类

**判断依据：**

1. 演示了IMX6ULL开发板切换USB启动模式。
2. 使用Windows烧写工具把完整系统写入eMMC。
3. 烧写完成后切回eMMC启动并通过串口验证。
4. 将USB OTG设备交给Ubuntu虚拟机，再执行 `adb devices`、`adb shell`。
5. 最后交叉编译Hello程序并通过ADB传到开发板运行。

------

## 2. 一句话总结

这个视频主要讲的是：

> **让IMX6ULL先进入USB下载模式，由<span style="color:#FF0000;">Windows烧写工具把支持ADB的新系统写入eMMC</span>；<span style="background:#00FF80;">烧写完成后切回eMMC启动，再让Ubuntu通过USB OTG使用ADB连接开发板</span>。**

完整流程：

```text
开发板设置USB启动
        ↓
Windows识别IMX6ULL下载设备
        ↓
Windows烧写工具向开发板发送临时启动程序
        ↓
把U-Boot、Linux内核、设备树、根文件系统写入eMMC
        ↓
切回eMMC启动并重启
        ↓
开发板启动新Linux系统和adbd
        ↓
USB OTG连接给Ubuntu虚拟机
        ↓
adb devices / adb shell
        ↓
交叉编译程序并adb push到开发板
```

------

## 3. 时间段拆解

| 时间段      | 内容概括                         | 画面/命令/操作                                               | 作用                                                         |
| ----------- | -------------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 00:00-00:18 | 判断当前系统是否已经支持ADB      | 教材提示在开发板中检查 `/usr/bin/adbd`                       | 避免不必要地重新烧写                                         |
| 00:18-00:40 | 介绍支持ADB的系统下载位置        | 展示2025版系统资料、下载入口和镜像说明                       | 准备烧写文件                                                 |
| 00:40-01:15 | 设置USB启动并连接开发板          | <span style="color:#FF0000; background:#00FF80;">拨码切换USB模式，连接USB OTG和电源</span>；强调不要插TF卡 | <span style="color:#FF0000; background:#00FF80;">让芯片进入USB下载模式</span> |
| 01:15-01:45 | 处理Windows与VMware的USB设备归属 | 查看USB设备，说明烧写时应把IMX6ULL设备留给Windows主机        | 让Windows烧写工具访问开发板                                  |
| 01:45-02:10 | 打开系统镜像和烧写软件           | Windows目录中打开 `100ASK IMX6ULL Flashing Tool V4.0`        | 启动烧写工具                                                 |
| 02:10-02:35 | 选择eMMC并开始烧写完整系统       | 工具显示“设备已连接”，目标选择eMMC，执行“烧写整个系统”       | 正式写入系统                                                 |
| 02:35-03:55 | 展示烧写进度和常见USB重连问题    | 进度从0%到100%；日志出现U-Boot、fastboot、eMMC写入；教材说明VMware可能抢占重新枚举的USB设备 | 解释烧写过程和失败原因                                       |
| 03:55-04:20 | 烧写完成                         | 工具提示 `Execution complete, Please set to EMMC mode and restart the board` | 提示切回正常启动模式                                         |
| 04:20-04:50 | 切回eMMC启动并登录新系统         | MobaXterm串口出现Buildroot登录界面，使用 `root` 登录         | 验证新系统能够正常启动                                       |
| 04:50-05:25 | 重新连接USB OTG                  | 教材标出OTG口、电源和启动拨码；将USB设备连接给Ubuntu虚拟机   | 准备由Ubuntu使用ADB                                          |
| 05:25-06:10 | 检查ADB工具                      | Ubuntu执行 `adb --help`，显示Android Debug Bridge版本和命令帮助 | 确认Ubuntu端ADB可用                                          |
| 06:10-06:35 | 验证开发板连接                   | 执行 `adb devices`，显示 `100ask_IMX6ULL device`             | 证明主机ADB和开发板adbd已连接                                |
| 06:35-06:55 | 进入开发板Shell                  | 执行 `adb shell`，提示符变为 `sh-5.0#`，执行 `ls`、`pwd`     | 证明命令由开发板执行                                         |
| 06:55-07:35 | 编写和交叉编译Hello程序          | 创建 `hello.c`，使用 `arm-buildroot-linux-gnueabihf-gcc` 编译 | 生成ARM开发板程序                                            |
| 07:35-07:54 | 通过ADB传输并运行                | `adb push hello /root`，进入shell后执行程序                  | 验证烧写、ADB和交叉编译流程                                  |

------

## 4. 最关键的两个阶段

这一节很容易混乱，因为**<span style="background:#00FF80;">同一条USB OTG线在不同阶段交给不同系统使用</span>**。

### 阶段一：烧写系统

此时<span style="color:#FF0000; background:#00FF80;">使用的是Windows烧写软件：</span>

```text
开发板USB OTG
      ↓
Windows宿主机
      ↓
100ASK IMX6ULL Flashing Tool
```

因此，<span style="color:#FF0000;">VMware弹出USB设备选择时，应让烧写设备连接到：</span>

```text
Windows主机
```

<span style="color:#FF0000;">而不是Ubuntu虚拟机。</span>

------

### 阶段二：使用ADB

<span style="color:#FF0000; background:#00FF80;">系统烧写并正常启动以后，ADB命令运行在Ubuntu中：</span>

```text
开发板USB OTG
      ↓
Ubuntu虚拟机
      ↓
adb devices / adb shell
```

这时<span style="color:#FF0000; background:#00FF80;">才需要把USB设备切换给：</span>

```text
Ubuntu虚拟机
```

可以记成：

```text
Windows烧系统，USB归Windows。

Ubuntu用ADB，USB归Ubuntu。
```

------

## 5. 为什么要先切换到USB启动模式？

正常使用时，开发板从eMMC启动：

```text
Boot ROM
  ↓
读取eMMC中的U-Boot
  ↓
启动Linux
```

但是你<span style="color:#FF0000; background:#00FF80;">现在要覆盖eMMC中的系统。如果eMMC里的系统：</span>

- 不支持ADB；
- 已经损坏；
- 无法正常启动；

就不能依赖原系统来更新自己。

<span style="color:#FF0000; background:#00FF80;">因此把拨码切换成USB启动，IMX6ULL上电后会：</span>

```text
执行芯片内部Boot ROM
        ↓
不从eMMC正常启动
        ↓
等待电脑通过USB发送下载程序
```

<span style="color:#FF0000;">Windows此时会识别出类似的USB下载设备。</span>

这样<span style="color:#FF0000; background:#00FF80;">即使eMMC完全是空的，电脑仍可以向开发板发送临时启动程序并重新烧写系统。</span>

------

## 6. 烧写工具内部大致做了什么？

视频没有逐行解释烧写工具源码，但从日志中的：

```text
uuu
USB Device Attached
u-boot
fastboot
eMMC
```

以及最后的完成提示，可以基本判断内部过程如下：

### 第一步：Boot ROM被Windows发现

<span style="color:#FF0000; background:#00FF80;">开发板处于USB启动模式时，真正运行的是：</span>

```text
IMX6ULL芯片内部Boot ROM
```

Windows烧写工具通过USB找到该设备。

------

### 第二步：发送一个临时启动程序

烧写工具不会让Boot ROM直接处理整个Linux系统，而是先传入一个临时的：

```text
U-Boot / 下载引导程序
```

大致过程：

```text
Windows烧写工具
      ↓ USB
IMX6ULL Boot ROM
      ↓
把临时U-Boot加载到DDR
      ↓
跳转运行临时U-Boot
```

这个临时U-Boot只存在于DDR中，断电后会消失。

------

### 第三步：USB设备可能重新识别

Boot ROM阶段和临时U-Boot阶段向电脑呈现的USB设备身份可能不一样。

所以Windows可能先看到类似：

```text
NXP USB Download Device
```

过一会儿又看到类似：

```text
Freescale SE Blank gadget
fastboot设备
```

这叫：

> **USB重新枚举。**

设备虽然还是同一块开发板，但内部运行的程序变了，所以它重新向电脑报告自己的身份。

这也是VMware容易造成烧写失败的地方：

```text
第一次USB设备归Windows
        ↓
临时U-Boot运行后USB重新枚举
        ↓
VMware把新出现的USB设备抢给Ubuntu
        ↓
Windows烧写工具失去连接
        ↓
烧写中断
```

所以视频特别演示了USB归属和失败处理。

------

### 第四步：写入eMMC

临时U-Boot开始运行后，烧写工具会把系统镜像的数据传给开发板。

写入内容通常包括：

```text
U-Boot
Linux内核
设备树
根文件系统
分区表或其他系统数据
```

对应结构大致是：

```text
Windows系统镜像
      ↓ USB
开发板DDR中的临时U-Boot
      ↓
eMMC控制器
      ↓
开发板eMMC存储器
```

也就是说，Windows不是直接像写U盘一样访问eMMC。

真正执行eMMC写操作的是开发板上临时运行的程序。

------

## 7. “烧写整个系统”意味着什么？

工具中能看到：

```text
烧写整个系统
更新内核
更新设备树
更新Uboot
```

它们的区别是：

| 操作         | 更新内容                                   |
| ------------ | ------------------------------------------ |
| 烧写整个系统 | U-Boot、内核、设备树、根文件系统等整体写入 |
| 更新内核     | 只替换Linux内核                            |
| 更新设备树   | 只替换DTB硬件描述                          |
| 更新U-Boot   | 只替换启动加载程序                         |

本节是<span style="color:#FF0000; background:#00FF80;">为了让旧系统获得ADB支持，因此使用：</span>

```text
烧写整个系统
```

因为<span style="color:#FF0000; background:#00FF80;"> `adbd` 位于根文件系统中，仅更新内核通常不能让旧系统自动获得ADB工具和服务。</span>

------

## 8. 烧写完成后为什么必须切回eMMC？

工具最后提示：

```text
Execution complete,
Please set to EMMC mode and restart the board.
```

意思是：

> <span style="background:#00FF80;">写入已经完成，请把拨码重新设置成eMMC启动，然后重新上电或复位。</span>

<span style="color:#FF0000; background:#00FF80;">如果仍保持USB启动模式，下一次上电时：</span>

```text
Boot ROM仍然等待USB下载
```

它<span style="color:#FF0000; background:#00FF80;">不会自动从刚刚写好的eMMC启动。</span>

正确顺序是：

```text
烧写进度到100%
      ↓
关闭或结束烧写
      ↓
开发板断电
      ↓
拨码切换到eMMC
      ↓
重新上电
      ↓
从新系统启动
```

------

## 9. 为什么要用串口验证？

烧写完成后，视频<span style="color:#FF0000; background:#00FF80;">先通过MobaXterm串口登录，而不是立刻使用ADB。</span>

因为<span style="color:#FF0000; background:#00FF80;">串口：</span>

- 不依赖ADB；
- 不依赖开发板网络；
- 可以观察启动日志；
- 能验证系统是否真正启动成功。

视频出现：

```text
Welcome to imx6ull buildroot system!
100ask login:
```

然后输入：

```text
root
```

进入：

```text
[root@100ask:~]#
```

这说明：

```text
eMMC中的U-Boot正常
Linux内核正常
根文件系统正常
串口控制台正常
新系统已经启动
```

------

## 10. 如何确认新系统确实支持ADB？

最直接的开发板端检查是：

```bash
ls /usr/bin/adbd
```

<span style="color:#FF0000; background:#00FF80;">如果存在：</span>

```text
/usr/bin/adbd
```

<span style="color:#FF0000; background:#00FF80;">说明根文件系统中包含ADB设备端程序。</span>

还可以检查它是否运行：

```bash
ps | grep adbd
```

只存在文件但没有运行，主机仍可能无法连接。

完整ADB关系是：

```text
Ubuntu
├── adb命令
└── adb server
        ↓ USB
开发板
└── adbd后台进程
```

------

## 11. `adb devices`为什么最关键？

视频在Ubuntu执行：

```bash
adb devices
```

输出类似：

```text
List of devices attached
100ask_IMX6ULL    device
```

每一部分含义是：

```text
100ask_IMX6ULL
开发板的ADB设备标识

device
设备在线、已连接，可以正常操作
```

如果显示为空：

```text
List of devices attached
```

说明Ubuntu没有连接到开发板。

如果显示：

```text
offline
```

说明发现了设备，但ADB通信未正常建立。

------

## 12. `adb shell`以后，命令由谁执行？

Ubuntu提示符原本是：

```bash
book@100ask:~/Desktop/...$
```

执行：

```bash
adb shell
```

提示符变成：

```text
sh-5.0#
```

此后执行：

```bash
ls
pwd
```

是开发板执行，而不是Ubuntu执行。

视频中：

```bash
pwd
```

返回：

```text
/
```

并且 `ls` 显示：

```text
bin boot dev etc lib linuxrc media mnt proc root sbin sys usr var
```

这些是开发板的根文件系统内容。

退出开发板：

```bash
exit
```

才会回到Ubuntu。

------

## 13. 最后为什么还要编译一个Hello程序？

烧写成功和ADB连接成功，只能证明：

```text
系统能启动
USB通信正常
文件可以传输
```

还不能证明整个开发流程全部正常。

所以视频最后进行了完整验证：

```text
在Ubuntu写hello.c
        ↓
ARM交叉编译
        ↓
adb push到开发板
        ↓
开发板运行
```

视频使用的编译命令类似：

```bash
arm-buildroot-linux-gnueabihf-gcc -o hello hello.c
```

然后：

```bash
adb push hello /root
adb shell
cd /root
./hello
```

这<span style="color:#FF0000; background:#00FF80;">同时验证了：</span>

- 交叉编译工具链；
- ADB文件传输；
- 新系统的动态库；
- ARM程序执行环境。

------

## 14. 烧写过程中最容易出错的地方

| 现象                         | 可能原因                               |
| ---------------------------- | -------------------------------------- |
| 烧写工具显示设备未连接       | 拨码不是USB模式、OTG线错误、板子没上电 |
| Windows识别不到设备          | USB驱动未安装、线只能充电、接口接错    |
| 烧写到一半停止               | VMware抢走了重新枚举后的USB设备        |
| 工具提示找不到USB设备        | 开发板USB设备被连接到Ubuntu虚拟机      |
| 进度100%后仍无法启动         | 忘记切回eMMC模式                       |
| 启动后没有ADB                | 镜像不支持ADB，或者 `adbd`未启动       |
| `adb devices`为空            | USB OTG没交给Ubuntu、adbd未运行        |
| `adb push`成功但程序运行失败 | 没有使用ARM交叉编译器或库不兼容        |

------

## 15. 烧写时需要特别注意

烧写完整系统通常会覆盖eMMC中的原数据。

因此操作前要确认：

```text
目标选择的是eMMC
镜像确实对应当前开发板型号
重要文件已经备份
烧写期间不要断电
不要随意拔掉USB OTG线
完成后再切换拨码
```

视频中的开发板和工具是：

```text
100ASK IMX6ULL Pro
100ASK IMX6ULL Flashing Tool V4.0
```

其他板卡不能仅因为处理器同为IMX6ULL，就随意使用相同镜像。不同板卡的：

- DDR参数；
- 引脚；
- 网卡；
- eMMC布局；
- 设备树；

都可能不同。

------

## 16. 观点分析

| 观点内容                                    | 观点归属           | 明确/可能 | 依据                        |
| ------------------------------------------- | ------------------ | --------- | --------------------------- |
| 2025年后的新系统默认支持ADB                 | 视频创作者         | 明确      | 开头勘误说明                |
| 老系统不支持ADB时需要重新烧写               | 视频创作者         | 明确      | 本节整体流程                |
| 烧写时USB设备应由Windows宿主机控制          | 讲师观点           | 明确      | 演示VMware USB归属          |
| 使用ADB时USB设备应交给Ubuntu                | 讲师观点           | 明确      | 后半段重新连接USB设备       |
| 串口应先用于验证系统启动，ADB再用于日常开发 | 作者可能暗含的观点 | 可能      | 烧写后先串口登录，再测试ADB |
| 完成一次Hello程序运行才算开发环境完整可用   | 作者可能暗含的观点 | 可能      | 结尾进行完整应用验证        |

<span style="background:#00FF80; color:#FF0000;">**核心观点：**</span>

```text
烧写解决“开发板里运行什么系统”；
串口解决“系统是否成功启动”；
ADB解决“系统启动后如何方便地操作和传文件”。
```

------

## 17. Who / Whom / Whose / When / Where / What / Why / How

| 维度  | 视频中的答案                                      |
| ----- | ------------------------------------------------- |
| Who   | Windows烧写工具、Ubuntu ADB主机、IMX6ULL开发板    |
| Whom  | 烧写操作指向开发板eMMC；ADB操作指向开发板adbd     |
| Whose | 系统镜像保存在Windows，最终系统写入开发板eMMC     |
| When  | 当前开发板系统不支持ADB或需要恢复系统时           |
| Where | Windows宿主机、VMware Ubuntu虚拟机、IMX6ULL开发板 |
| What  | 烧写U-Boot、内核、设备树、根文件系统，并验证ADB   |
| Why   | 为了让开发板拥有支持ADB的完整Linux环境            |
| How   | USB启动、Windows烧写工具、eMMC重启、USB OTG和ADB  |

------

## 18. 记忆版总结

```text
类型：
IMX6ULL系统烧写与ADB验证教程

烧写阶段：
拨码切USB
→ USB OTG连接Windows
→ 打开烧写工具
→ 选择eMMC
→ 烧写整个系统
→ 等待100%

完成后：
断电
→ 拨码切回eMMC
→ 重新上电
→ 串口登录root

ADB阶段：
USB OTG交给Ubuntu虚拟机
→ adb devices
→ adb shell

程序验证：
ARM交叉编译
→ adb push
→ 开发板运行

关键区别：
Windows负责烧写；
Ubuntu负责ADB；
串口负责观察启动；
eMMC负责长期保存系统。

一句话记忆：
USB模式把系统写进去，eMMC模式把系统启动起来，
串口确认板子活着，ADB负责后续开发。
```



# 问题1

这个板子好像有两个接口，一个是OTG，一个是USB 串口 好像，这两个有什么区别呢，和启动方式又有什么联系呢，从USB下载自动不能够接串口吗。



对，这块板子上的 **USB OTG口** 和 **USB串口口** 是<span style="color:#FF0000; background:#00FF80;">两条完全不同的通信通道</span>。它们虽然外形都可能是Micro-USB接口，但<span style="color:#FF0000; background:#00FF80;">连接到开发板内部的硬件不同，作用也不同。</span>

最核心的区别是：

```text
USB串口：电脑通过UART观察、控制开发板
USB OTG：电脑直接通过USB协议与IMX6ULL通信
```

## 一、两个接口内部连接到哪里？

### 1. USB串口接口

USB串口通常先连接一颗USB转UART芯片，例如：

```text
CP2104
CH340
CH9102
```

内部链路是：

```text
电脑USB口
   ↓ USB协议
CP2104等USB转串口芯片
   ↓ UART的TX/RX信号
IMX6ULL的串口控制器
```

Windows安装驱动后，会显示为：

```text
COM17
COM19
```

然后使用MobaXterm：

```text
Serial → COM17 → 115200
```

它主要用于：

- 查看U-Boot和Linux启动日志；
- 操作U-Boot命令行；
- 登录Linux；
- 系统启动失败时排查问题；
- 开发板没有IP时执行命令。

------

### 2. USB OTG接口

USB OTG口通常<span style="color:#FF0000; background:#00FF80;">直接连接到IMX6ULL处理器内部的USB控制器：</span>

```text
电脑USB口
   ↓ USB协议
IMX6ULL USB OTG控制器
```

<span style="color:#FF0000;">中间没有CP2104这类UART转换芯片。</span>

它<span style="color:#FF0000; background:#00FF80;">可以根据开发板当前运行的程序，表现成不同USB设备：</span>

```text
USB下载模式：
表现为NXP下载设备，供烧写工具使用

正常Linux系统：
可能表现为ADB设备、USB网卡、U盘等

U-Boot阶段：
可能表现为fastboot设备
```

因此<span style="color:#FF0000; border:1px solid #330000; background:#00FF80;">OTG口主要用于：</span>

- USB烧写系统；
- ADB通信；
- fastboot；
- USB Gadget功能；
- <span style="color:#FF0000; background:#00FF80;">某些情况下连接U盘或其他USB设备。</span>

------

## 二、最直观的对比

| 对比项             | USB串口                                                      | USB OTG                                  |
| ------------------ | ------------------------------------------------------------ | ---------------------------------------- |
| 内部连接           | USB转UART芯片，再接IMX6ULL UART                              | 直接接IMX6ULL USB控制器                  |
| Windows中常见表现  | COM端口                                                      | NXP下载设备、ADB设备等                   |
| 常用软件           | MobaXterm串口终端                                            | 烧写工具、ADB                            |
| 是否需要波特率     | 需要，如115200                                               | 不需要设置UART波特率                     |
| 是否依赖Linux启动  | 不依赖，U-Boot阶段也能使用                                   | ADB依赖Linux和adbd，但USB下载不依赖Linux |
| 能否看上电启动日志 | 可以                                                         | 通常不用于显示启动日志                   |
| 主要任务           | <span style="color:#FF0000; background:#00FF80;">观察和控制</span> | 下载、烧写、传输数据                     |

可以记成：

```text
串口像“开发板的显示器和键盘”。

OTG像“开发板的USB数据通道”。
```

## 三、它们和启动方式有什么关系？

<span style="color:#FF0000; background:#00FF80;">启动方式由拨码开关决定，不是由“插了哪根USB线”决定。</span>

### 1. eMMC启动

拨码选择eMMC后：

```text
上电
 ↓
Boot ROM读取eMMC
 ↓
启动U-Boot
 ↓
启动Linux
```

此时：

- USB串口可以连接，用来查看整个启动过程；
- OTG口可以暂时不用；
- Linux启动后，OTG口可能用于ADB。

结构是：

```text
USB串口 → 查看启动日志
eMMC     → 提供启动系统
OTG      → Linux启动后可用于ADB
```

------

### 2. SD卡启动

拨码选择SD卡：

```text
上电
 ↓
Boot ROM读取SD卡
 ↓
启动SD卡中的U-Boot和Linux
```

USB串口仍然可以连接并查看日志。

<span style="color:#FF0000; background:#00FF80;">OTG接口也不是启动所必需的，除非启动后的系统要使用ADB等功能。</span>

------

### 3. USB下载模式

拨码选择USB启动后：

```text
上电
 ↓
IMX6ULL内部Boot ROM运行
 ↓
通过USB OTG等待电脑发送程序
 ↓
电脑烧写工具发送临时U-Boot
 ↓
临时U-Boot写入eMMC
```

这里真正承担下载任务的是：

> <span style="color:#FF0000; background:#00FF80;">**USB OTG接口。**</span>

<span style="color:#FF0000; border:1px solid #330000; background:#00FF80;">USB串口</span>不负责把系统镜像写进eMMC。

## 四、USB下载时能不能同时连接串口？

<span style="background:#00FF80;">**可以，通常应该同时连接。**</span>

连接方式可以是：

```text
USB OTG线
开发板 ↔ 电脑
负责烧写数据

USB串口线
开发板 ↔ 电脑
负责显示日志和提供终端
```

它们<span style="color:#FF0000;">连接的是不同硬件控制器，通常不会冲突。</span>

<span style="background:#00FF80; color:#FF0000;">完整结构：</span>

```text
                    ┌── USB OTG ── Windows烧写工具
IMX6ULL开发板 ──────┤
                    └── UART → CP2104 → COM口 → MobaXterm
```

也就是说，<span style="color:#FF0000; background:#00FF80;">USB下载模式下可以同时：</span>

- OTG口<span style="color:#FF0000;">连接Windows烧写工具</span>；
- USB串口<span style="color:#FF0000;">连接Windows的MobaXterm</span>；
- 通过串口观察临时U-Boot或烧写程序输出。

## 五、为什么USB下载初期，串口可能没有任何输出？

虽然串口可以连接，但不代表一上电就一定有日志。

USB下载启动最初运行的是IMX6ULL芯片内部的Boot ROM：

```text
上电
 ↓
Boot ROM等待USB下载
```

Boot ROM通常不会像U-Boot和Linux一样输出大量串口信息，所以此时串口窗口可能是空白的。

当电脑通过OTG传入临时U-Boot后：

```text
Boot ROM
 ↓ 加载临时U-Boot
临时U-Boot开始运行
 ↓
初始化串口
 ↓
串口可能开始显示日志
```

因此可能出现：

```text
烧写工具已经识别到开发板，
但MobaXterm串口窗口暂时没有输出。
```

这是可能正常的，并不一定代表串口连接失败。

## 六、烧写完成后的接口变化

整个过程中，OTG接口的身份可能变化：

```text
第一阶段：
Boot ROM
→ NXP USB下载设备

第二阶段：
临时U-Boot
→ fastboot或烧写设备

第三阶段：
从eMMC启动Linux
→ ADB设备
```

同一个OTG物理接口，在不同阶段由不同程序控制，所以电脑会发现它“断开后又重新连接”。

这叫：

> USB重新枚举。

但USB串口接口通常一直是：

```text
CP2104 → COM17
```

不会因为Boot ROM、U-Boot和Linux切换而改变其基本USB身份。

## 七、Windows和Ubuntu应怎样分配两个USB设备？

结合课程中的操作：

### 烧写系统时

```text
OTG下载设备 → 交给Windows
USB串口设备 → 也可以留给Windows
```

<span style="color:#FF0000; background:#00FF80;">Windows中分别使用：</span>

```text
OTG：100ASK烧写工具
串口：MobaXterm
```

它们<span style="color:#FF0000; background:#00FF80;">可以同时工作。</span>

------

### 烧写完成、<span style="background:#00FF80;">使用ADB时</span>

```text
USB串口设备 → 留给Windows
USB OTG的ADB设备 → 交给Ubuntu虚拟机
```

<span style="color:#FF0000; background:#00FF80;">这样：</span>

```text
Windows MobaXterm
→ 通过COM口操作串口

Ubuntu
→ 通过adb操作OTG
```

<span style="color:#FF0000; background:#00FF80;">两条连接可以同时存在：</span>

```text
Windows串口终端             Ubuntu ADB
       ↓                       ↓
USB串口接口                USB OTG接口
       └────── IMX6ULL开发板 ────┘
```

## 八、这两个接口甚至<span style="border:1px solid #330000; background:#00FF80;">可以同时进入开发板Shell</span>

系统正常启动后，你<span style="color:#FF0000; background:#00FF80;">可能同时开着两个终端。</span>

### Windows串口窗口

```text
[root@100ask:~]#
```

通过：

```text
USB串口 → UART控制台
```

### Ubuntu ADB窗口

```bash
adb shell
```

进入：

```text
sh-5.0#
```

这<span style="color:#FF0000; background:#00FF80;">两个Shell都在同一块开发板上运行，只是入口不同。</span>

例如：

```text
串口终端创建文件：
touch /root/a.txt

ADB终端执行：
ls /root
```

<span style="color:#FF0000; background:#00FF80;">ADB终端能够看到 `a.txt`，因为它们访问的是同一个开发板文件系统。</span>

## 九、最容易混淆的三个“USB”

课程中实际上出现了三种概念：

### 1. USB串口

```text
USB线
→ CP2104
→ UART
→ COM口
```

负责<span style="color:#FF0000; background:#00FF80;">终端和日志。</span>

### 2. USB下载

```text
USB OTG
→ Boot ROM
→ 烧写eMMC
```

负责<span style="color:#FF0000; background:#00FF80;">系统烧写。</span>

### 3. USB ADB

```text
USB OTG
→ Linux USB Gadget
→ adbd
```

负责<span style="color:#FF0000; background:#00FF80;">系统启动后的Shell和文件传输。</span>

其中：

```text
USB下载和ADB使用的是同一个OTG物理接口，
但发生在不同启动阶段，由不同软件控制。

USB串口是另一个独立接口。
```

## 十、一句话回答你的问题

> 开发板上的USB串口口通过CP2104等芯片转换成UART，主要用于查看启动日志和操作终端；USB OTG口直接连接IMX6ULL的USB控制器，USB启动时用于下载和烧写系统，Linux启动后可用于ADB。USB下载模式下完全可以同时连接USB串口，OTG负责传输烧写数据，串口负责观察日志，两者互不替代。

记忆版：

```text
拨码开关：决定从哪里启动。

OTG口：负责USB下载或ADB。

USB串口：负责日志和命令行。

USB下载时：
OTG接烧写工具，
串口仍然可以接MobaXterm。
```





# 问题2

我有个问题，这两个接口可以同时进入开发版Shell，一个是Windows串口窗口，一个是Ubuntu ADB窗口，这两个窗口感觉功能是不是重合了啊。



对，**在Linux已经正常启动之后，这两个窗口的“执行Shell命令”功能确实有较大重合**。但它们不是同一个Shell，也不是完全可以互相替代。

可以把它们理解成：

> <span style="background:#00FF80;">**同一栋房子有两个入口。串口是一扇应急门，ADB是一扇快速通道。进去以后，看到的是同一个开发板系统。**</span>

## 一、两个窗口进入的是同一块开发板

连接关系分别是：

```text
Windows MobaXterm
      ↓ USB串口
CP2104/CH9102
      ↓ UART
开发板Linux串口控制台
```

以及：

```text
Ubuntu终端
      ↓ USB OTG
ADB
      ↓
开发板adbd
      ↓
开发板Linux Shell
```

<span style="color:#FF0000; background:#00FF80;">两边最终都在IMX6ULL开发板上启动一个Shell进程。</span>

所以你可以分别看到类似：

<span style="color:#FF0000; background:#00FF80;">串口窗口：</span>

```bash
[root@100ask:~]#
```

<span style="color:#FF0000; background:#00FF80;">ADB窗口：</span>

```bash
sh-5.0#
```

虽然提示符不同，但<span style="color:#FF0000; background:#00FF80;">访问的是同一套</span>：

- Linux内核；
- 进程；
- 文件系统；
- eMMC；
- 内存；
- 网络设备。

------

## 二、它们是两个独立的Shell会话

不是说你在串口中输入一条命令，ADB窗口就会同步显示这条命令。

例如两个窗口初始都在 `/root`：

### 串口窗口执行

```bash
cd /etc
```

串口窗口当前目录变为：

```text
/etc
```

但ADB窗口的当前目录不会跟着变化，因为：

> <span style="background:#00FF80;">`cd`改变的是当前Shell进程自己的工作目录。</span>

同样，串口窗口执行：

```bash
export TEST=123
```

<span style="color:#FF0000; background:#00FF80;">这个环境变量一般只属于串口中的Shell，不会自动出现在ADB的Shell中。</span>

因此<span style="color:#FF0000;">它们更像两个独立登录用户同时操作同一台Linux电脑。</span>

------

## 三、哪些东西是共享的？

虽然Shell会话独立，但底层系统是共享的。

### 1. 文件系统共享

串口窗口执行：

```bash
touch /root/a.txt
```

ADB窗口执行：

```bash
ls /root
```

能看到：

```text
a.txt
```

因为<span style="color:#FF0000; background:#00FF80;">两个窗口访问的是同一个开发板文件系统。</span>

------

### 2. 进程共享

串口窗口启动程序：

```bash
./hello &
```

ADB窗口执行：

```bash
ps
```

能看到这个进程。

如果ADB窗口执行：

```bash
kill 进程号
```

串口窗口启动的程序也会被终止。

------

### 3. 系统状态共享

任何一边执行下面这些操作，都会影响整个开发板：

```bash
reboot
poweroff
ifconfig eth0 down
mount ...
rm 文件
```

例如ADB窗口执行：

```bash
reboot
```

开发板会整体重启，串口窗口也会立刻看到重新启动的日志。

------

## 四、它们重合的功能有哪些？

Linux正常启动后，两边通常都能执行：

```bash
ls
cd
pwd
cat
vi
ps
mount
chmod
./hello
```

所以在“<span style="color:#FF0000; background:#00FF80;">进入Shell执行普通命令</span>”这件事上，<span style="background:#00FF80;">两者确实重合</span>。

| 功能         | 串口Shell      | ADB Shell      |
| ------------ | -------------- | -------------- |
| 查看文件     | 可以           | 可以           |
| 运行程序     | 可以           | 可以           |
| 修改配置     | 可以           | 可以           |
| 查看进程     | 可以           | 可以           |
| 重启开发板   | 可以           | 可以           |
| 获取root权限 | 取决于登录账号 | 取决于adbd配置 |

在课程提供的镜像中，两边很可能都是root权限，因此日常命令区别更小。

------

## 五、<span style="background:#00FF80;">为什么还要同时保留两个接口？</span>

因为它们的**可用阶段、稳定性和擅长任务不同**。

### 1. 串口可以覆盖完整启动过程

串口可以看到：

```text
开发板上电
→ U-Boot日志
→ Linux内核日志
→ 根文件系统启动
→ 登录界面
```

甚至可以在启动时按键进入U-Boot命令行：

```text
Hit any key to stop autoboot
```

随后执行：

```bash
printenv
setenv
saveenv
boot
```

ADB做不到这一点。

<span style="color:#FF0000; background:#00FF80;">因为ADB必须等到：</span>

```text
Linux内核已经启动
→ 根文件系统已经挂载
→ adbd进程已经运行
```

之后才能连接。

所以：

> **<span style="background:#00FF80;">板子启动失败时，ADB往往不可用，串口仍可能帮助排错。</span>**

------

### 2. ADB传文件更方便、更快

ADB可以直接执行：

```bash
adb push hello /root
adb pull /root/log.txt
```

<span style="color:#FF0000; background:#00FF80;">USB数据传输速度通常明显高于115200波特率的串口。</span>

<span style="color:#FF0000;">串口理论上也能使用协议传文件，但不适合日常频繁传输，操作麻烦、速度也慢。</span>

所以：

```text
串口：更适合查看日志、排错和救援
ADB：更适合传文件、运行程序和日常开发
```

------

### 3. 串口不依赖开发板中的adbd

串口依赖的是：

```text
UART硬件
串口控制台配置
USB转串口芯片
```

ADB依赖的是：

```text
USB Gadget驱动
adbd程序
根文件系统
USB OTG连接
```

如果你误删了：

```text
/usr/bin/adbd
```

或者系统没有启动ADB服务，ADB就无法连接，但串口仍可能正常。

------

## 六、什么时候使用哪一个？

### 开发板刚上电或启动异常

优先使用串口：

```text
查看U-Boot
查看Linux启动日志
检查Kernel panic
检查根文件系统挂载失败
修改U-Boot环境变量
```

------

### 系统已经正常启动，频繁测试程序

优先使用ADB：

```bash
arm-buildroot-linux-gnueabihf-gcc hello.c -o hello
adb push hello /root
adb shell /root/hello
```

<span style="color:#FF0000; background:#00FF80;">ADB更适合形成快速开发流程。</span>

------

### 调试时两个一起开

这是很常见的方式：

```text
串口窗口：
持续观察内核日志、驱动日志

ADB窗口：
上传程序、执行命令、启动测试
```

例如ADB窗口加载驱动：

```bash
insmod my_driver.ko
```

同时串口窗口可能显示：

```text
my_driver: module loaded
device initialized successfully
```

这样非常方便。

------

## 七、两个窗口同时操作会不会冲突？

可能会。

因为<span style="color:#FF0000; background:#00FF80;">两边操作的是同一个系统。</span>

### 情况一：同时修改同一个文件

串口使用VI打开：

```bash
vi /etc/profile
```

ADB也同时修改 `/etc/profile`，<span style="color:#FF0000; background:#00FF80;">后保存的一方可能覆盖前一方的修改。</span>

------

### 情况二：一个运行，一个删除

串口准备运行：

```bash
./hello
```

ADB同时执行：

```bash
rm hello
```

就<span style="color:#FF0000; background:#00FF80;">可能造成混乱。</span>

------

### 情况三：同时控制同一设备

串口执行：

```bash
ifconfig eth0 down
```

ADB依赖的虽然是USB，不一定马上断开，但系统网络会被修改。

如果一边执行：

```bash
reboot
```

两个会话都会断开或受到影响。

所以同时使用时，要知道另一窗口正在做什么。

------

## 八、一个更准确的对比

| 对比项           | Windows串口Shell                | Ubuntu ADB Shell    |
| ---------------- | ------------------------------- | ------------------- |
| 物理通道         | USB转UART                       | USB OTG             |
| 开发板端入口     | 串口控制台/getty                | adbd                |
| 可用时间         | U-Boot、内核启动、Linux运行期间 | Linux及adbd启动后   |
| 查看早期启动日志 | 可以                            | 不可以              |
| 进入U-Boot       | 可以                            | 不可以              |
| 执行Linux命令    | 可以                            | 可以                |
| 传输文件         | 不方便                          | `push/pull`非常方便 |
| 传输速度         | 较慢                            | 通常更快            |
| 系统故障救援     | 更可靠                          | 系统故障后可能失效  |
| 日常应用测试     | 可以，但不够方便                | 更适合              |

## 九、最适合你的使用方式

学习过程中可以这样分工：

```text
串口窗口一直开着：
观察启动过程、内核日志和错误信息。

Ubuntu ADB窗口：
上传程序、进入Shell、运行测试和下载结果。
```

例如：

```bash
# Ubuntu中编译并上传
arm-buildroot-linux-gnueabihf-gcc test.c -o test
adb push test /root

# ADB中运行
adb shell
cd /root
./test
```

同时观察串口窗口是否出现驱动或内核日志。

所以你的判断可以总结为：

> **Linux正常启动后，两者在执行Shell命令方面确实重合；但串口覆盖启动早期并适合故障排查，ADB只在系统和adbd启动后可用，却更适合快速传文件和日常测试。两者不是多余重复，而是“串口负责保底和观察，ADB负责效率”。**
