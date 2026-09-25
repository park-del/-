# 视频分析：烧写完整系统到 IMX6ULL Pro（2025）

> 视频时长约 **4分55秒**。
> 本节承接上一节Buildroot编译：把Ubuntu生成的完整系统镜像传到Windows，通过USB OTG和烧写工具写入开发板eMMC，最后切回eMMC启动并用串口验证。

------

## 1. 视频类型判断

**主要类型：** 教程类、硬件操作演示类
**次要类型：** 系统部署类、故障排查类、Buildroot构建结果应用类

**判断依据：**

1. 使用Buildroot生成的完整 `.img` 镜像。
2. 通过FileZilla把镜像从Ubuntu传到Windows。
3. 使用100ASK烧写工具和USB OTG写入eMMC。
4. 专门讲解VMware抢占USB设备导致的烧写失败。
5. 最后通过串口启动日志验证新系统。

------

## 2. 一句话总结

这个视频主要讲的是：

> **将Buildroot生成的完整系统镜像按烧写工具要求命名为 `emmc.img`，传到Windows烧写工具的 `files` 目录；开发板切换到USB启动后，通过OTG口把镜像写入eMMC，完成后再切回eMMC启动并验证。**

完整流程：

```text
Ubuntu Buildroot
output/images/完整系统.img
        ↓ 重命名为emmc.img
FileZilla下载到Windows
        ↓
烧写工具files目录
        ↓
开发板切换USB启动
        ↓ USB OTG
Windows烧写工具
        ↓
临时U-Boot/Fastboot运行
        ↓
完整系统写入eMMC
        ↓
拨码切回eMMC
        ↓
重启并通过串口验证
```

------

## 3. 时间段拆解

| 时间段      | 内容概括                 | 画面/操作表现                                                | 作用                   |
| ----------- | ------------------------ | ------------------------------------------------------------ | ---------------------- |
| 00:00-00:12 | 查看Buildroot镜像目录    | Ubuntu终端进入 `output/images`，执行 `pwd`、`ls`             | 确认完整系统镜像位置   |
| 00:12-00:38 | 说明应选择哪个镜像       | 文档指出完整镜像 `100ask-imx6ull-pro-512d-systemv-v1.img`，可改名为 `emmc.img` 或 `sdcard.img` | 区分完整镜像和单独组件 |
| 00:38-01:08 | 将镜像传到Windows        | FileZilla右侧为Ubuntu，左侧为Windows烧写工具的 `files` 目录；旧镜像改为 `emmc_bak.img`，下载新的 `emmc.img` | 准备烧写文件           |
| 01:08-01:45 | 设置开发板连接方式       | 文档标出USB OTG、电源、USB串口和拨码开关；设置为USB启动，不预先插TF卡 | 让芯片进入USB下载模式  |
| 01:45-02:10 | 开始烧写完整系统         | 打开 `100ASK IMX6ULL Flashing Tool V4.0`，目标选择EMMC，点击“烧写整个系统” | 正式部署完整镜像       |
| 02:10-02:25 | 烧写完成                 | UUU日志显示进度和Bootloader写入，最终提示切换至EMMC并重启    | 确认数据写入完成       |
| 02:25-03:45 | 讲解VMware抢占USB故障    | 展示 `Failure open usb device`，说明 `Freescale SE Blank 6ULL` 和 `Netchip USB download gadget`应连接到主机Windows | 解决烧写中途断开问题   |
| 03:45-04:20 | 复习镜像、接口和启动拨码 | 再次展示烧写文件说明、USB OTG接线和EMMC/SD/USB拨码组合       | 强化关键配置           |
| 04:20-04:55 | 切回eMMC并验证启动       | 串口中显示U-Boot读取内核和DTB，Linux挂载eMMC根文件系统并启动服务 | 验证新系统真正生效     |

------

## 4. 内容结构

1. **开头：** 从Buildroot的 `output/images` 中找到完整系统镜像。
2. **发展：** 把镜像传到Windows烧写工具目录，并设置USB启动。
3. **核心操作：** 使用Windows烧写工具把完整镜像写入eMMC。
4. **故障排查：** 说明VMware为什么可能导致USB烧写中断。
5. **结尾：** 切回eMMC启动，通过串口日志验证系统。

整体表达路径：

```text
从【Buildroot生成完整系统镜像】开始，
通过【FileZilla传输、USB启动和UUU烧写】展开，
最后落到【从eMMC启动新系统并验证】。
```

------

## 5. 为什么使用完整 `.img` 镜像？

视频中<span style="color:#FF0000; background:#00FF80;">Buildroot生成了多个文件：</span>

```text
zImage
*.dtb
u-boot-dtb.imx
rootfs.ext4
rootfs.tar
100ask-imx6ull-pro-512d-systemv-v1.img
```

<span style="color:#FF0000; background:#00FF80;">这些文件不是同一种东西。</span>

| 文件             | 作用                                             |
| ---------------- | ------------------------------------------------ |
| `u-boot-dtb.imx` | U-Boot启动镜像                                   |
| `zImage`         | Linux内核                                        |
| `*.dtb`          | 设备树                                           |
| `rootfs.ext4`    | 根文件系统分区镜像                               |
| 完整 `.img`      | 包含分区布局、根文件系统和相关启动组件的整盘镜像 |

完整镜像大致可以理解为：

```text
完整系统.img
├── 分区表
├── Bootloader相关区域
├── 启动分区
│   ├── zImage
│   └── DTB
├── 根文件系统分区
└── 其他数据分区
```

所以“烧写整个系统”适合：

- 第一次安装系统；
- 恢复损坏的eMMC；
- 修改了根文件系统和分区布局；
- 需要部署Buildroot新生成的整套系统。

------

## 6. 为什么要把镜像命名为 `emmc.img`？

文档要求将：

```text
100ask-imx6ull-pro-512d-systemv-v1.img
```

改成：

```text
emmc.img
```

再放入烧写工具的：

```text
files/
```

目录。

这里要特别注意：

> **重命名不是格式转换。**

<span style="color:#FF0000; background:#00FF80;">镜像内部内容没有改变，只是烧写工具可能按照固定文件名寻找目标镜像：</span>

```text
选择EMMC
    ↓
烧写工具自动寻找files/emmc.img
```

同理，如果烧写SD卡，工具可能寻找：

```text
files/sdcard.img
```

因此：

```text
完整系统镜像的内容
≠ 文件名

文件名只是烧写工具识别它的约定
```

------

## 7. 为什么要从Ubuntu传到Windows？

Buildroot在Ubuntu中编译，所以镜像生成在：

```text
/home/book/100ask_imx6ull-sdk/
Buildroot_2020.02.x/output/images/
```

但是视频使用的烧写工具运行在Windows中：

```text
100ASK IMX6ULL Flashing Tool V4.0
```

所以需要完成：

```text
Ubuntu镜像
      ↓ FileZilla
Windows烧写工具files目录
```

FileZilla界面：

```text
左侧：Windows本地文件
右侧：Ubuntu远程文件
```

本节传输方向是：

```text
右侧Ubuntu → 左侧Windows
```

画面中把旧文件改成：

```text
emmc_bak.img
```

然后下载新的：

```text
emmc.img
```

这样保留了旧系统镜像，出现问题时仍可回退。

------

## 8. USB启动和“烧写到eMMC”并不矛盾

这是本节最容易混淆的地方。

开发板拨码设置为：

```text
USB启动
```

<span style="color:#FF0000; background:#00FF80;">并不表示最终系统要安装在USB设备中。</span>

它表示：

> <span style="color:#FF0000; background:#00FF80;">上电后，不从eMMC运行旧系统，而是让芯片内部Boot ROM通过USB OTG等待电脑发送临时下载程序。</span>

实际关系是：

```text
当前启动通道：USB OTG
最终写入目标：eMMC
```

完整过程：

```text
拨码设置USB启动
        ↓
IMX6ULL内部Boot ROM运行
        ↓
电脑通过OTG发送临时U-Boot
        ↓
临时U-Boot在DDR中运行
        ↓
它控制eMMC并写入emmc.img
```

所以：

```text
USB启动
→ 描述当前从哪里取得烧写程序

烧写到eMMC
→ 描述系统最终保存在哪里
```

这是两个不同的问题。

------

## 9. 烧写工具内部大致做了什么？

画面日志显示使用：

```text
uuu (Universal Update Utility)
```

UUU是面向NXP i.MX系列处理器的USB下载工具。

<span style="color:#FF0000; background:#00FF80;">烧写过程通常分成两个阶段。</span>

### 第一阶段：Boot ROM下载阶段

开发板处于USB启动时，电脑识别为类似：

```text
Freescale SE Blank 6ULL
```

烧写工具执行类似：

```text
SDP: boot -f u-boot-dtb_fastboot_100ask.imx
```

含义是：

```text
Windows读取临时U-Boot
        ↓ USB OTG
Boot ROM接收
        ↓
放入开发板DDR
        ↓
启动临时U-Boot/Fastboot
```

------

### 第二阶段：Fastboot烧写阶段

临时程序启动后，开发板可能重新显示为：

```text
Netchip USB download gadget
```

随后：

```text
Windows发送emmc.img
        ↓
临时U-Boot接收
        ↓
写入eMMC各分区
```

最后日志还显示写入或处理Bootloader：

```text
FB: flash bootloader .../files/u-boot-dtb.imx
```

并<span style="color:#FF0000; background:#00FF80;">设置eMMC相关启动配置。</span>

最终出现：

```text
Execution completed,
Please set to EMMC mode and restart the board.
```

说明烧写流程完成。

------

## 10. 为什么USB设备会出现两个名字？

视频中出现：

```text
Freescale SE Blank 6ULL
```

和：

```text
Netchip USB download gadget
```

这<span style="color:#FF0000; background:#00FF80;">不是同时连接了两块开发板，而是同一块板在两个阶段表现成不同设备。</span>

```text
阶段一：
IMX6ULL Boot ROM
→ Freescale SE Blank 6ULL

阶段二：
临时U-Boot/Fastboot
→ Netchip USB download gadget
```

<span style="color:#FF0000; background:#00FF80;">临时U-Boot启动后，USB控制程序发生变化，电脑会认为：</span>

```text
旧USB设备断开
新USB设备接入
```

这叫：

> **USB重新枚举。**

------

## 11. VMware为什么会导致烧写失败？

视频展示了错误：

```text
Failure open usb device
```

可能原因是<span style="color:#FF0000; background:#00FF80;">VMware抢走了开发板的USB设备。</span>

假设烧写开始时：

```text
Freescale SE Blank 6ULL
→ 连接到Windows主机
```

第一阶段可以正常工作。

但<span style="color:#FF0000; background:#00FF80;">临时U-Boot启动后设备重新枚举为</span>：

```text
Netchip USB download gadget
```

VMware可能弹出提示，并把新设备连接给Ubuntu虚拟机：

```text
Windows烧写工具
        ↓
突然失去设备
        ↓
Failure open usb device
```

因此<span style="color:#FF0000; background:#00FF80;">两个阶段出现的USB设备都应留给：</span>

```text
Windows宿主机
```

弹窗中应选择：

```text
连接到主机
勾选“记住我的选择，以后不再询问”
```

如果误接到虚拟机，可以从VMware菜单中断开：

```text
Player
→ Removable Devices
→ Freescale SE Blank 6ULL /
  Netchip USB download gadget
→ Disconnect from virtual machine
```

本节可以记成：

```text
Windows负责烧写
→ OTG USB设备必须归Windows

Ubuntu在这一步不参与烧写
→ 不要把下载设备交给Ubuntu虚拟机
```

------

## 12. USB串口是否必须连接？

文档明确说明：

```text
USB串口线可接可不接，
接上只是为了观察烧写和启动过程。
```

两条线职责不同：

```text
USB OTG
→ 传输烧写数据，烧写时必须使用

USB串口
→ 显示U-Boot/Linux日志，可选
```

结构是：

```text
Windows烧写工具
      ↓ USB OTG
开发板
      ↓ USB串口
MobaXterm串口窗口
```

<span style="color:#FF0000; background:#00FF80;">USB串口不能代替OTG传输完整系统镜像。</span>

<span style="color:#FF0000; background:#00FF80;">OTG也通常不能代替串口显示完整启动日志。</span>

------

## 13. 为什么烧写完成后必须切回eMMC？

烧写时拨码仍然设置为USB启动。

<span style="color:#FF0000; background:#00FF80;">如果不修改拨码，下一次重启时：</span>

```text
Boot ROM仍然等待USB下载
```

不会自动使用刚写入的eMMC系统。

正确顺序：

```text
烧写提示完成
        ↓
开发板断电
        ↓
拨码切换为eMMC启动
        ↓
重新上电
        ↓
Boot ROM从eMMC读取U-Boot
        ↓
启动新系统
```

对应拨码：

```text
eMMC：OFF OFF ON OFF
USB：  X   X  OFF ON
```

具体<span style="color:#FF0000; background:#00FF80;">拨动方向应以板上“ON”标记和手册图片为准。</span>

------

## 14. 结尾启动日志说明了什么？

### U-Boot找到eMMC

串口显示：

```text
mmc1(part 0) is current device
Normal Boot
```

说明<span style="color:#FF0000; background:#00FF80;">U-Boot正在访问MMC/eMMC设备。</span>

------

### U-Boot读取Linux内核

画面显示：

```text
7924872 bytes read
Booting from mmc ...
```

约7.9MB的文件对应视频中的 `zImage` 大小<span style="color:#FF0000; background:#00FF80;">，说明U-Boot已把内核从eMMC读取到DDR。</span>

------

### U-Boot读取设备树

画面显示：

```text
38370 bytes read
Flattened Device Tree blob at 83000000
```

38,370字节与视频中的：

```text
100ask_imx6ull-14x14.dtb
```

大小一致，说明设备树也被正确加载。

------

### 跳转到Linux内核

```text
Starting kernel ...
```

表示U-Boot完成任务，将CPU控制权交给Linux。

------

### Linux挂载eMMC根文件系统

后续日志显示：

```text
EXT4-fs (mmcblk1p2): mounted filesystem
VFS: Mounted root (ext4 filesystem) on device 179:2
```

可以拆解：

```text
mmcblk1
→ 一块MMC/eMMC存储设备

p2
→ 第2个分区

ext4
→ 根文件系统格式
```

也就是：

> Linux成功把eMMC的第二个分区挂载为根文件系统。

------

### 用户空间服务启动

画面随后出现：

```text
Starting syslogd: OK
Starting klogd: OK
Running sysctl: OK
Starting 100ask lvgl: OK
starting eudev-3.2.9
```

这说明：

```text
内核已经启动
根文件系统已经挂载
init启动脚本已经执行
用户空间服务正在运行
```

所以整套烧写和启动基本成功。

------

## 15. 日志中的两条异常是否表示烧写失败？

画面中出现：

```text
No ethernet found.
```

这是U-Boot阶段没有发现或没有初始化网卡，并不必然表示Linux中的网卡无法使用。

还出现：

```text
Error: "findtee" not defined
```

这可能是U-Boot环境变量或脚本中的某个命令未定义。

但后续仍然：

```text
读取zImage
读取DTB
Starting kernel
挂载rootfs
启动服务
```

所以在本视频中，它们没有阻止系统继续启动。

判断系统是否真正失败，不能只看到一行红色或Error，而要继续观察：

```text
有没有Starting kernel
有没有Kernel panic
有没有成功挂载rootfs
有没有进入用户空间
```

------

## 16. “烧写整个系统”和“更新部分系统”的区别

烧写工具提供：

```text
烧写整个系统
更新内核
更新设备树
更新Uboot
```

对应关系：

| 操作         | 适用情况                                   |
| ------------ | ------------------------------------------ |
| 烧写整个系统 | 系统首次安装、根文件系统变化、分区需要重建 |
| 更新内核     | 只修改了内核或内置驱动                     |
| 更新设备树   | 只修改了DTS/DTB                            |
| 更新U-Boot   | 只修改启动加载程序                         |

本视频使用：

```text
烧写整个系统
```

因为上一节Buildroot重新生成的是整套系统镜像。

烧写整个系统可能覆盖：

- 原有根文件系统；
- 用户自己存放的文件；
- 配置数据；
- 部分数据分区。

因此重要文件应提前备份。

------

## 17. 观点分析

| 观点内容                                               | 观点归属           | 明确/可能 | 依据                  |
| ------------------------------------------------------ | ------------------ | --------- | --------------------- |
| Buildroot完整镜像可以直接用于eMMC或SD卡烧写            | 讲师观点           | 明确      | 镜像文件说明          |
| Windows烧写工具通过固定文件名识别目标镜像              | 讲师观点           | 明确      | 要求改名为 `emmc.img` |
| 烧写时USB下载设备必须连接到Windows主机                 | 讲师观点           | 明确      | VMware故障讲解        |
| USB串口不是烧写必需接口                                | 讲师观点           | 明确      | 文档注明可接可不接    |
| 烧写成功必须以重启后正常加载内核和根文件系统为最终验证 | 作者可能暗含的观点 | 可能      | 结尾专门展示启动日志  |

**核心观点：**

```text
镜像生成完成不等于系统部署完成；
只有完成镜像传输、USB烧写、启动模式切换，
并确认内核和根文件系统正常启动，才形成完整闭环。
```

------

## 18. 人物分析

视频没有清晰出镜人物，主要是：

- Ubuntu终端；
- Word/PDF手册；
- FileZilla；
- Windows烧写工具；
- VMware；
- MobaXterm串口日志。

因此不适合分析人物的情绪、心理、性格或人格。

明确能看出讲解路径是：

```text
准备镜像
→ 实际烧写
→ 展示成功
→ 补充失败原因
→ 启动验证
```

------

## 19. Who / Whom / Whose / When / Where / What / Why / How

| 维度  | 视频中的答案                                             |
| ----- | -------------------------------------------------------- |
| Who   | Windows烧写工具、Ubuntu编译环境和IMX6ULL Pro开发板       |
| Whom  | 烧写操作指向开发板上的eMMC                               |
| Whose | 完整镜像由Ubuntu中的Buildroot生成，最终写入开发板        |
| When  | Buildroot完成整套系统构建之后                            |
| Where | Ubuntu `output/images`、Windows烧写工具目录、开发板eMMC  |
| What  | 将完整系统镜像写入eMMC并验证启动                         |
| Why   | 把自行编译的U-Boot、内核、设备树和根文件系统部署到开发板 |
| How   | FileZilla传输、USB启动、OTG连接、UUU烧写、eMMC重启       |

------

## 20. 重点片段精读

### 片段一：00:12-01:08

**表面发生了什么：**
找到完整镜像，改名并传入烧写工具目录。

**更深层含义：**
<span style="color:#FF0000; background:#00FF80;">烧写工具不是直接访问Ubuntu的Buildroot输出目录，必须把最终镜像放入它约定的Windows目录和文件名</span>。

**重要性：**
这是构建环境与部署环境之间的桥梁。

------

### 片段二：01:45-02:25

**表面发生了什么：**
选择EMMC并执行“烧写整个系统”。

**更深层含义：**
<span style="color:#FF0000; background:#00FF80;">USB OTG只是传输通道；真正写eMMC的是下载到开发板DDR中运行的临时U-Boot/Fastboot程序。</span>

**重要性：**
解释电脑为什么能操作开发板内部的eMMC。

------

### 片段三：02:25-03:45

**表面发生了什么：**
讲解VMware抢占USB设备导致烧写失败。

**更深层含义：**
<span style="color:#FF0000; background:#00FF80;">开发板从Boot ROM切换到临时U-Boot后会重新枚举，电脑会把它视为新的USB设备，因此两个阶段都必须保持归Windows管理。</span>

**重要性：**
这是烧写到一半突然失败的高频原因。

------

### 片段四：04:20-04:55

**表面发生了什么：**
切回eMMC启动，串口显示内核和根文件系统启动日志。

**更深层含义：**
<span style="color:#FF0000; background:#00FF80;">烧写工具提示“完成”只能证明传输流程结束；串口中成功读取内核、DTB并挂载 `mmcblk1p2`，才真正证明eMMC中的系统可启动。</span>

**重要性：**
完成部署结果的最终验证。

------

## 21. 我应该重点看什么？

这节课优先掌握五个对应关系：

```text
完整Buildroot镜像
→ output/images/*.img

烧写工具识别名称
→ emmc.img

烧写阶段启动模式
→ USB启动

烧写数据通道
→ USB OTG

烧写完成后的启动模式
→ eMMC启动
```

VMware中要记住：

```text
Freescale SE Blank 6ULL
Netchip USB download gadget
→ 都应连接到Windows主机
```

完整操作顺序：

```text
1. 找到完整系统.img
2. 改名为emmc.img
3. 放入烧写工具files目录
4. 开发板断电
5. 拨码切为USB启动
6. 不预先插TF卡
7. 连接OTG和电源
8. 目标选择EMMC
9. 点击烧写整个系统
10. 等待Execution completed
11. 断电并切回eMMC
12. 重新上电
13. 观察串口启动日志
```

------

## 22. 记忆版总结

```text
类型：
Buildroot完整系统镜像烧写教程

镜像来源：
Buildroot_2020.02.x/output/images

完整镜像：
100ask-imx6ull-pro-512d-systemv-v1.img

工具要求：
改名为emmc.img
放入Windows烧写工具files目录

烧写：
拨码切USB
OTG连接Windows
选择EMMC
点击“烧写整个系统”

USB两个阶段：
Freescale SE Blank 6ULL
→ Boot ROM下载阶段

Netchip USB download gadget
→ 临时U-Boot/Fastboot阶段

VMware注意：
两种USB设备都必须连接到Windows主机

完成后：
切回eMMC
重新上电

验证：
U-Boot读取zImage和DTB
Linux挂载mmcblk1p2根文件系统
系统服务显示OK

最重要时间段：
00:12-01:08 准备并传输镜像
01:45-02:25 完整系统烧写
02:25-03:45 VMware USB故障
04:20-04:55 eMMC启动验证

一句话记忆：
USB模式负责把系统写进eMMC，
eMMC模式负责把写进去的系统启动起来；
OTG负责传数据，串口负责看结果。
```
