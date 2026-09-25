# 视频分析：使用 ADB 替代 NFS，并运行第一个 ARM 应用

> 视频时长约 **14分16秒**。
> 本节包含一项 **2025 年勘误**：早期课程经常用 NFS 在 Ubuntu 和开发板之间共享文件，现在更推荐使用 **ADB＋USB OTG线**完成日常文件传输。

<span style="color:#FF0000; background:#00FF80;">补充：在技术领域，这两个缩写的标准全称如下：</span>

- **NFS**：**Network File System**（网络文件系统）
- **ADB**：**Android Debug Bridge**（安卓调试桥）

------

## 1. 视频类型判断

**主要类型：** 教程类、操作演示类
**次要类型：** 嵌入式Linux开发流程讲解、勘误说明类

**判断依据：**

1. 演示ADB环境检查、连接和命令使用。
2. 对比<span style="color:#FF0000; background:#00FF80;">ADB文件传输</span>与<span style="color:#FF0000; background:#00FF80;">传统NFS挂载</span>。
3. 编译并运行第一个C应用。
4. 专门演示“<span style="color:#FF0000; background:#00FF80;">普通GCC编译失败、ARM交叉编译成功</span>”的原因。

------

## 2. 一句话总结

这个视频主要讲的是：

> **Ubuntu通过USB OTG和开发板建立ADB连接，使用 `adb shell/push/pull` 操作开发板；随后通过第一个Hello程序说明，开发板程序必须使用ARM交叉编译器，而不能直接使用Ubuntu的普通GCC。**

主线可以记成：

```text
Ubuntu上的源码
      ↓ ARM交叉编译
生成ARM可执行文件
      ↓ adb push
传到开发板
      ↓ adb shell或串口
开发板执行程序
```

------

## 3. 时间段拆解

| 时间段      | 内容概括                                                     | 画面、命令与操作表现                                         | 作用                                                         |
| ----------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 00:00-00:40 | 介绍ADB连接方式                                              | 教材说明<span style="color:#FF0000; background:#00FF80;">使用开发板OTG口连接电脑</span>，并<span style="color:#FF0000; background:#00FF80;">把USB设备连接给Ubuntu虚拟机</span> | 建立ADB硬件连接                                              |
| 00:40-01:15 | 说明<span style="color:#FF0000;">开发板系统必须支持ADB</span> | 展示<span style="color:#FF0000; background:#00FF80;">支持ADB的系统镜像和开发板接线</span>；提示旧系统可能需要更新 | 说明ADB使用前提                                              |
| 01:15-02:15 | 检查开发板上的 `adbd`                                        | 开发板终端执行 `ps | grep adbd`，看到 `/usr/bin/adbd`进程    | 验证开发板端服务                                             |
| 02:15-03:15 | Ubuntu<span style="color:#FF0000;">安装并启动ADB</span>      | VMware中选择USB设备归属；提示可执行 `sudo apt install adb` 等命令 | 准备Ubuntu主机端工具                                         |
| 03:15-04:05 | 使用 <span style="color:#FF0000;">`adb shell`登录开发板</span> | <span style="color:#FF0000; background:#00FF80;">Ubuntu终端进入开发板Shell，查看开发板根目录</span> | 验证ADB连接                                                  |
| 04:05-05:25 | 传输单个文件                                                 | 创建文本文件，使用 `adb push`<span style="color:#FF0000; background:#00FF80;">上传</span>，再用 `adb pull`<span style="color:#FF0000; background:#00FF80;">下载</span> | 演示<span style="color:#FF0000; background:#00FF80;">双向文件传输</span> |
| 05:25-06:50 | 传输整个目录                                                 | 创建含多个文件的目录，使用<span style="color:#FF0000; background:#00FF80;">ADB递归上传、下载目录</span> | 扩展到<span style="color:#FF0000; background:#00FF80;">工程目录</span> |
| 06:50-08:20 | 介绍传统NFS连接结构                                          | 绘制<span style="color:#FF0000;">Ubuntu、路由器和开发板之间的网络关系</span> | 与ADB作对比                                                  |
| 08:20-09:15 | 配置并测试网络                                               | 开发板和Ubuntu查看IP，开发板 `ping 192.168.1.31`成功         | 准备NFS网络环境                                              |
| 09:15-10:40 | 挂载NFS目录                                                  | <span style="color:#FF0000; background:#00FF80;">开发板执行NFS挂载命令，将Ubuntu的 `nfs_rootfs`挂载到 `/mnt`</span> | 演示<span style="color:#FF0000; background:#00FF80;">共享目录机制</span> |
| 10:40-11:25 | 比较ADB和NFS传文件                                           | Ubuntu修改文件后通过ADB推送，开发板查看结果                  | 回到推荐的ADB流程                                            |
| 11:25-12:15 | 编写并普通编译Hello程序                                      | Ubuntu使用 `gcc -o hello hello.c`，在Ubuntu上成功输出Hello   | 生成x86程序                                                  |
| 12:15-12:45 | x86程序在开发板运行失败                                      | `adb push hello /root`后，开发板执行出现 `Exec format error` | <span style="color:#FF0000; background:#00FF80;">展示架构不匹配</span> |
| 12:45-13:35 | 使用ARM交叉编译器                                            | 执行 `arm-buildroot-linux-gnueabihf-gcc`，再用 `file hello`检查ARM格式 | <span style="color:#FF0000; background:#00FF80;">生成正确的开发板程序</span> |
| 13:35-14:00 | 开发板成功运行                                               | 重新推送ARM程序，开发板输出 `Hello, World!`                  | 验证完整开发流程                                             |
| 14:00-14:16 | 2025年勘误总结                                               | <span style="color:#FF0000; background:#00FF80;">提示早期课程常使用NFS，现在日常文件传输优先改用ADB</span> | 给出新推荐方案                                               |

------

## 4. 内容结构

1. **开头：** 建立ADB硬件和软件环境。
2. **发展：** 使用ADB进入开发板并传输文件。
3. **转折：** 回顾旧课程使用的NFS方式，并进行对比。
4. **结尾：** 编译第一个APP，通过错误演示说明为什么必须交叉编译。

整体表达路径：

```text
从【建立ADB连接】开始，
通过【shell、push、pull和NFS对比】展开，
最后落到【ARM交叉编译并在开发板成功运行程序】。
```

------

## 5. ADB的完整结构

<span style="color:#FF0000; background:#00FF80;">ADB不是只有一条命令，它涉及Ubuntu和开发板两端。</span>

```text
Ubuntu虚拟机
┌───────────────────────────┐
│ adb命令客户端              │
│ adb server，本机后台服务   │
└───────────┬───────────────┘
            │ USB OTG
            ▼
Linux开发板
┌───────────────────────────┐
│ adbd后台进程               │
│ Linux Shell和文件系统      │
└───────────────────────────┘
```

其中：

| 名称       | 运行位置     | 作用                                                         |
| ---------- | ------------ | ------------------------------------------------------------ |
| `adb`      | Ubuntu虚拟机 | <span style="color:#FF0000; background:#00FF80;">用户输入的ADB命令</span> |
| ADB server | Ubuntu虚拟机 | <span style="color:#FF0000; background:#00FF80;">管理USB连接，通常在主机后台运行</span> |
| `adbd`     | Linux开发板  | <span style="color:#FF0000; background:#00FF80;">接收主机命令，执行Shell和文件操作</span> |

视频中执行：

```bash
ps | grep adbd
```

看到类似：

```text
/usr/bin/adbd
```

说明<span style="color:#FF0000; background:#00FF80;">开发板端的ADB后台程序正在运行。</span>

------

### ADB使用的是哪条线？

ADB使用的是：

```text
开发板USB OTG口
      ↓
USB数据线
      ↓
电脑USB接口
      ↓
Ubuntu虚拟机
```

它<span style="color:#FF0000; background:#00FF80;">和上一节的USB串口不是同一个概念。</span>

| 对比                 | USB串口              | ADB                              |
| -------------------- | -------------------- | -------------------------------- |
| 开发板接口           | UART经CP2104转换     | USB OTG                          |
| Windows表现          | COM17等COM口         | USB设备                          |
| 常用软件             | MobaXterm Serial     | Ubuntu中的 `adb`                 |
| 是否需要 `adbd`      | 不需要               | 需要                             |
| 主要用途             | 看启动日志、登录终端 | 传文件、执行Shell命令            |
| 能否看到上电初期日志 | 可以                 | 通常不可以，需系统启动并运行adbd |

因此：

> <span style="color:#FF0000; background:#00FF80;">串口适合观察“开发板怎样启动”</span>；<span style="color:#FF0000; background:#00FF80;">ADB适合系统启动后高效操作和传文件。</span>

------

## 6. VMware中为什么要选择“连接到Ubuntu”？

<span style="color:#FF0000;">ADB命令</span>是<span style="color:#FF0000; background:#00FF80;">在Ubuntu虚拟机中执行</span>的，因此<span style="color:#FF0000;">USB OTG设备</span>必须<span style="color:#FF0000; background:#00FF80;">交给Ubuntu。</span>

关系是：

```text
开发板USB OTG设备
        ↓
VMware选择连接到虚拟机
        ↓
Ubuntu识别USB设备
        ↓
Ubuntu的adb访问开发板
```

如果该USB设备仍被Windows占用，Ubuntu中的ADB可能找不到设备。

这与上一节串口的设置不同：

```text
Windows版MobaXterm使用串口
→ CP2104连接到Windows宿主机

Ubuntu使用ADB
→ USB OTG设备连接到Ubuntu虚拟机
```

<span style="color:#FF0000; background:#00FF80;">两者是不同的USB接口，可以分别归属不同系统。</span>

------

## 7. 安装和检查ADB

视频提示，如果Ubuntu中没有 `adb` 命令，可以执行类似：

```bash
sudo apt install adb
```

随后可能执行：

```bash
sudo chmod a+x /usr/bin/adb
sudo chmod a+s /usr/bin/adb
sudo adb kill-server
```

核心安装命令是：

```bash
sudo apt install adb
```

安装后可以检查：

```bash
which adb
```

可能输出：

```text
/usr/bin/adb
```

查看连接设备通常使用：

```bash
adb devices
```

正常时可能显示：

```text
List of devices attached
某个设备编号    device
```

如果列表为空，优先检查：

1. OTG线是否连接正确；
2. 开发板系统是否启动；
3. 开发板是否运行 `adbd`；
4. VMware是否把USB设备连接给Ubuntu；
5. USB线是否支持数据传输；
6. 是否需要重启ADB服务。

可以尝试：

```bash
adb kill-server
adb start-server
adb devices
```

------

## 8. `adb shell`：进入开发板命令行

在Ubuntu执行：

```bash
adb shell
```

之后，提示符会变成开发板的提示符，例如：

```text
[root@100ask:/]#
```

此后输入：

```bash
ls
pwd
cd /root
```

都是由开发板执行的。

退出开发板Shell：

```bash
exit
```

退出后回到Ubuntu提示符：

```text
book@100ask:~$
```

因此要注意：

```text
在 book@100ask:~$ 输入 ls
→ 查看Ubuntu文件

进入 adb shell 后输入 ls
→ 查看开发板文件
```

------

## 9. `adb push`：Ubuntu传文件到开发板

<span style="color:#FF0000; border:1px solid #330000; background:#00FF80;">基本格式：</span>

```bash
adb push Ubuntu本地文件 开发板目标路径
```

例如：

```bash
adb push 1.txt /root
```

含义是：

```text
Ubuntu当前目录的1.txt
          ↓ USB
开发板的/root/1.txt
```

成功时可能显示：

```text
1 file pushed
```

<span style="color:#FF0000; background:#00FF80;">之后可以进入开发板查看：</span>

```bash
adb shell
cd /root
ls
cat 1.txt
```

------

### 上传并重命名

```bash
adb push 1.txt /root/new.txt
```

表示上传后在开发板上保存为：

```text
/root/new.txt
```

------

### 上传目录

```bash
adb push test_dir /root
```

表示把Ubuntu的整个 `test_dir`目录上传到开发板 `/root`。

目录中的多个文件也会一起传输。

------

## 10. `adb pull`：从开发板下载到Ubuntu

基本格式：

```bash
adb pull 开发板文件 Ubuntu本地位置
```

例如：

```bash
adb pull /root/2.txt
```

<span style="color:#FF0000; background:#00FF80;">会把开发板上的：</span>

```text
/root/2.txt
```

下载到<span style="color:#FF0000; background:#00FF80;">Ubuntu当前目录。</span>

指定本地目录：

```bash
adb pull /root/2.txt /home/book/Desktop/
```

下载整个目录：

```bash
adb pull /root/test_dir2
```

可以记成：

```text
push：从Ubuntu推向开发板
pull：从开发板拉回Ubuntu
```

方向如下：

```text
Ubuntu ──adb push──> 开发板
Ubuntu <──adb pull── 开发板
```

------

## 11. ADB与NFS到底是什么关系？

视频标题写“<span style="color:#FF0000; background:#00FF80;">使用ADB替代NFS</span>”，但<span style="color:#FF0000;">两者并不是完全相同的技术。</span>

### ADB：复制文件

执行：

```bash
adb push hello /root
```

会产生两份文件：

```text
Ubuntu有一份hello
开发板/root中又复制了一份hello
```

<span style="color:#FF0000; background:#00FF80;">修改Ubuntu中的文件后，开发板中的副本不会自动变化，需要重新推送。</span>

------

### NFS：挂载远程目录

视频中的命令类似：

```bash
mount -t nfs -o nolock,vers=3 \
192.168.1.31:/home/book/nfs_rootfs /mnt
```

拆解如下：

| 内容                    | 含义                                                         |
| ----------------------- | ------------------------------------------------------------ |
| `mount`                 | 挂载文件系统                                                 |
| `-t nfs`                | 类型为NFS                                                    |
| `-o nolock,vers=3`      | 使用NFSv3并关闭锁服务                                        |
| `192.168.1.31`          | <span style="color:#FF0000; background:#00FF80;">Ubuntu的IP地址</span> |
| `/home/book/nfs_rootfs` | <span style="color:#FF0000; background:#00FF80;">Ubuntu共享目录</span> |
| `/mnt`                  | <span style="color:#FF0000; background:#00FF80;">开发板上的访问入口</span> |

挂载后：

```text
开发板访问/mnt
        ↓ 网络
实际读取Ubuntu的/home/book/nfs_rootfs
```

<span style="color:#FF0000; background:#00FF80;">这不是复制，而是实时共享。</span>

Ubuntu修改：

```text
/home/book/nfs_rootfs/a.txt
```

开发板在：

```text
/mnt/a.txt
```

<span style="color:#FF0000; background:#00FF80;">会直接看到变化。</span>

------

### 两者对比

| 对比项           | ADB                | NFS                          |
| ---------------- | ------------------ | ---------------------------- |
| 连接方式         | USB OTG            | 以太网                       |
| 是否需要IP       | 不需要             | 需要                         |
| 是否需要网络配置 | 不需要             | 需要                         |
| 数据方式         | 复制               | 远程挂载、实时共享           |
| 操作难度         | 较低               | 较高                         |
| 修改是否自动同步 | 不会，需再次push   | 会，访问的是同一目录         |
| 常见用途         | 日常传输程序和文件 | 频繁编译测试、网络根文件系统 |

因此2025勘误更准确的意思是：

> <span style="color:#FF0000; background:#00FF80;">对于日常“把编译结果传到开发板”这一需求，用ADB比配置NFS简单；但ADB并没有完全取代NFS的远程挂载能力。</span>

------

## 12. 视频中的NFS流程

### 第一步：开发板和Ubuntu位于同一网络

开发板查看IP：

```bash
ifconfig
```

Ubuntu也查看IP。

视频中Ubuntu的地址类似：

```text
192.168.1.31
```

开发板测试：

```bash
ping 192.168.1.31
```

出现回复说明网络连通。

------

### 第二步：开发板挂载Ubuntu目录

```bash
mount -t nfs -o nolock,vers=3 \
192.168.1.31:/home/book/nfs_rootfs /mnt
```

------

### 第三步：开发板查看共享目录

```bash
ls /mnt
```

看到的其实是Ubuntu目录：

```text
/home/book/nfs_rootfs
```

中的文件。

例如：

```bash
cat /mnt/a.txt
```

<span style="color:#FF0000; background:#00FF80;">读取的是Ubuntu硬盘上的文件，只是通过网络传给开发板。</span>

------

## 13. 第一个APP：为什么普通GCC编译后会失败？

视频在Ubuntu执行：

```bash
gcc -o hello hello.c
```

生成：

```text
hello
```

然后在Ubuntu执行：

```bash
./hello
```

成功输出：

```text
Hello, World!
```

这说明源代码没有问题。

但是使用：

```bash
adb push hello /root
```

传到IMX6ULL开发板后执行：

```bash
./hello
```

出现：

```text
cannot execute binary file: Exec format error
```

------

### 错误的真正原因

Ubuntu虚拟机是：

```text
x86-64架构
```

普通：

```bash
gcc
```

默认生成：

```text
x86-64机器码
```

IMX6ULL是：

```text
ARM 32位架构
```

ARM处理器看不懂x86-64机器指令，因此内核拒绝运行。

这不是：

- C代码语法错误；
- 文件传输错误；
- ADB错误；
- 缺少执行权限。

而是：

> **可执行文件的CPU架构与开发板不匹配。**

------

## 14. 使用ARM交叉编译器

视频随后使用类似：

```bash
arm-buildroot-linux-gnueabihf-gcc -o hello hello.c
```

这一编译器的名称可以拆开：

| 部分        | 含义                      |
| ----------- | ------------------------- |
| `arm`       | 目标架构为32位ARM         |
| `buildroot` | 工具链与Buildroot环境相关 |
| `linux`     | 目标系统是Linux           |
| `gnueabihf` | GNU EABI硬浮点接口        |
| `gcc`       | C语言编译器               |

这个编译器：

```text
本身运行在x86 Ubuntu
但生成ARM机器码
```

------

### 使用 `file` 检查

视频执行：

```bash
file hello
```

输出大致包含：

```text
ELF 32-bit LSB executable
ARM
EABI5
dynamically linked
```

最关键的是：

```text
ARM
```

说明它已经是ARM开发板程序。

对比：

```bash
gcc -o hello hello.c
file hello
```

可能显示：

```text
ELF 64-bit LSB executable, x86-64
```

交叉编译后：

```text
ELF 32-bit LSB executable, ARM
```

------

## 15. 重新上传并运行

交叉编译完成后：

```bash
adb push hello /root
```

会覆盖开发板中原来的x86版本。

然后：

```bash
adb shell
cd /root
./hello
```

成功显示：

```text
Hello, World!
```

完整过程是：

```text
hello.c
   ↓ arm-buildroot-linux-gnueabihf-gcc
ARM版本hello
   ↓ adb push
/root/hello
   ↓ 开发板执行
Hello, World!
```

<span style="color:#FF0000; background:#00FF80;">如果出现权限问题，可以补充：</span>

```bash
chmod +x /root/hello
```

------

## 16. 为什么NFS中的程序也能运行？

视频还把ARM版 `hello`放入：

```text
Ubuntu：
/home/book/nfs_rootfs/hello
```

开发板已经将该目录挂载到：

```text
/mnt
```

因此开发板执行：

```bash
/mnt/hello
```

实际上是在：

```text
通过NFS读取Ubuntu中的ARM版hello
```

输出同样是：

```text
Hello, World!
```

这里仍然有一个前提：

> NFS只能解决“文件在哪里”，不能解决“架构是否正确”。

如果NFS目录中放的是x86程序，开发板仍会报：

```text
Exec format error
```

------

## 17. 2025年勘误应怎样理解？

视频结尾明确说明：

```text
早期课程：
经常使用NFS在Ubuntu和开发板之间传文件。

现在的建议：
日常传程序和文件时，优先使用ADB＋USB线。
```

后续看到旧课程命令：

```bash
cp hello /home/book/nfs_rootfs/
```

并在开发板执行：

```bash
/mnt/hello
```

可以按实际需求改成：

```bash
adb push hello /root
adb shell
cd /root
./hello
```

对应关系：

| 旧课程NFS操作       | ADB替代方式                |
| ------------------- | -------------------------- |
| 复制到 `nfs_rootfs` | `adb push 文件 /root`      |
| 开发板进入 `/mnt`   | `adb shell` 后进入 `/root` |
| 执行 `/mnt/hello`   | 执行 `/root/hello`         |
| 查看共享目录        | `adb shell ls /root`       |

不过后续如果课程真正讲的是：

- NFS根文件系统；
- 实时共享大型目录；
- 内核通过NFS挂载根文件系统；

就不能简单地用ADB完全替代。

------

## 18. 观点分析

| 观点内容                                                     | 观点归属           | 明确/可能 | 判断依据                                                     |
| ------------------------------------------------------------ | ------------------ | --------- | ------------------------------------------------------------ |
| 当前日常文件传输更推荐使用ADB                                | 视频创作者         | 明确      | 结尾2025勘误文字                                             |
| <span style="color:#FF0000; background:#00FF80;">开发板系统必须运行adbd才能使用ADB</span> | 讲师观点           | 明确      | 检查 `adbd`进程                                              |
| <span style="color:#FF0000; background:#00FF80;">ADB可以简化Ubuntu与开发板之间的文件传输</span> | 讲师观点           | 明确      | <span style="color:#FF0000;">演示push、pull和目录传输</span> |
| NFS配置比ADB复杂，但具有实时共享能力                         | 讲师观点及操作事实 | 明确      | 演示IP、ping和mount流程                                      |
| 程序能否在开发板运行，取决于目标架构                         | 讲师观点           | 明确      | x86失败、ARM成功的对比                                       |
| 出错演示比只展示成功流程更有助于理解交叉编译                 | 作者可能暗含的观点 | 可能      | 保留了 `Exec format error`过程                               |

**核心观点：**

```text
ADB解决“怎样方便地把文件传过去”；
交叉编译器解决“传过去的程序能不能被ARM CPU执行”。
```

------

## 19. 人物分析

视频没有清晰出镜人物，主要是教材、Ubuntu终端、开发板串口终端和网络示意图。

因此不能可靠分析人物情绪、心理、性格或人格。

明确可以看出，讲师采用了：

```text
先展示ADB
→ 再演示旧NFS方式
→ 制造x86程序运行错误
→ 使用ARM交叉编译器纠正
```

这种通过错误对比强化概念的教学方式。

------

## 20. Who / Whom / Whose / When / Where / What / Why / How

| 维度  | 视频中的答案                                       |
| ----- | -------------------------------------------------- |
| Who   | Ubuntu主机、IMX6ULL开发板和课程操作者              |
| Whom  | Ubuntu的ADB操作指向开发板上的adbd                  |
| Whose | 源代码和交叉编译器位于Ubuntu；最终程序运行在开发板 |
| When  | 开发板系统启动并运行adbd后                         |
| Where | Ubuntu虚拟机、USB OTG连接、IMX6ULL开发板           |
| What  | 建立ADB连接、传文件、NFS挂载、编译并运行Hello程序  |
| Why   | 简化文件传输，并验证ARM交叉编译开发流程            |
| How   | 使用 `adb shell/push/pull`以及ARM交叉编译器完成    |

------

## 21. 重点片段精读

### 片段一：01:15-03:15

**表面发生了什么：**
<span style="color:#FF0000;">开发板检查 `adbd`，Ubuntu安装ADB，并把USB OTG设备连接到虚拟机。</span>

**更深层含义：**
<span style="color:#FF0000; background:#00FF80;">ADB需要主机端和设备端共同工作。只有Ubuntu安装 `adb`，但开发板没有运行 `adbd`，仍然无法连接。</span>

**重要性：**
<span style="color:#FF0000;">解释ADB不是单纯安装一条命令即可。</span>

------

### 片段二：04:05-06:50

**表面发生了什么：**
使用 `adb push`和 `adb pull`传输文件及目录。

**更深层含义：**
<span style="color:#FF0000; background:#00FF80;">ADB文件传输不依赖开发板IP，也不需要先配置NFS服务器，适合初学阶段频繁上传小程序。</span>

**重要性：**
这是2025年推荐的新工作流。

------

### 片段三：09:15-10:40

**表面发生了什么：**
开发板挂载Ubuntu的NFS目录。

**更深层含义：**
NFS和ADB不是相同的数据模型。NFS让两个系统看到同一目录，ADB则产生文件副本。

**重要性：**
避免把“替代NFS”误解为两者功能完全一样。

------

### 片段四：11:25-13:35

**表面发生了什么：**
普通GCC生成的程序在开发板报错，ARM交叉编译版本成功运行。

**更深层含义：**
源码通常与CPU架构无关，但编译后的机器码有明确架构。文件成功传到开发板，不代表它一定能够运行。

**重要性：**
这是整节课程最关键的编译知识。

------

## 22. 我应该重点看什么？

优先掌握这三组内容。

### ADB命令

```bash
adb devices
adb shell
adb push 本地文件 开发板路径
adb pull 开发板文件 本地路径
```

### 判断当前系统

```text
book@100ask:~$
→ Ubuntu虚拟机

[root@100ask:/]#
→ Linux开发板
```

### 编译器选择

```bash
gcc -o hello hello.c
```

生成Ubuntu x86程序。

```bash
arm-buildroot-linux-gnueabihf-gcc -o hello hello.c
```

生成IMX6ULL ARM程序。

------

## 23. 记忆版总结

```text
类型：
ADB文件传输、NFS对比和第一个ARM应用教程

ADB结构：
Ubuntu的adb
→ USB OTG
→ 开发板的adbd

常用命令：
adb devices
adb shell
adb push hello /root
adb pull /root/hello

ADB与NFS：
ADB是复制文件，不需要IP；
NFS是挂载共享目录，需要网络和IP。

错误：
普通gcc生成x86程序，
放到ARM开发板出现Exec format error。

正确方式：
arm-buildroot-linux-gnueabihf-gcc -o hello hello.c
adb push hello /root
adb shell
/root/hello

最重要时间段：
01:15-04:05 ADB环境和连接
04:05-06:50 push、pull和目录传输
09:15-10:40 NFS挂载
11:25-13:35 x86失败与ARM交叉编译成功

一句话记忆：
ADB负责把文件送到板子，
交叉编译器负责让这个文件成为板子能执行的ARM程序。
```
