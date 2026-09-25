# 视频分析

> 视频时长约 **10分40秒**。以下以画面、终端命令和讲义中能明确辨认的内容为主；个别口头讲解没有逐字转录，因此时间点可能有数秒误差。

## 1. 视频类型判断

**主要类型：** 教程类、讲解类
**次要类型：** 操作演示类、Linux基础教学类

**判断依据：**

1. 围绕<span style="color:#FF0000; background:#00FF80;"> VI 编辑器的新建、保存、退出和浏览</span>展开。
2. 先解释 VI 的模式结构，再进入 Ubuntu 终端实际操作。
3. 使用 `1.txt`、`2.txt`、`3.txt` 等文件验证操作结果，并展示错误提示。

------

## 2. 一句话总结

这个视频主要讲的是：

> <span style="color:#FF0000; background:#00FF80;">**如何在 Linux 终端中使用 VI 新建文件、输入内容、保存或放弃修改，以及浏览较长的文本文件。**</span>

最核心的知识不是某一个命令，而是：

> **先弄清自己当前处于“<span style="color:#FF0000; background:#00FF80;">一般模式、编辑模式还是命令行模式</span>”。**

------

## 3. 时间段拆解

| 时间段      | 内容概括                   | 画面/声音/人物表现                                           | 作用                                                         |
| ----------- | -------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 00:00-00:28 | 介绍 VI 编辑器             | 讲义列出<span style="color:#FF0000; background:#00FF80;">打开、新建、保存、移动、编辑、复制、删除、查找替换</span>等功能，随后进入 Ubuntu 桌面 | 交代课程主题                                                 |
| 00:28-00:53 | 使用图形编辑器新建文件     | 在<span style="color:#FF0000; background:#00FF80;">终端输入类似 `gedit 1.txt`，打开图形化文本编辑器</span> | <span style="color:#FF0000; background:#00FF80;">与 VI 形成对比</span> |
| 00:53-02:31 | 演示普通文件与系统文件权限 | 查看 `/etc/environment`，直接使用 gedit 打开，关闭时出现保存提示；随后使用 `sudo gedit /etc/environment` 并输入密码 | 说明编辑系统文件往往涉及权限，终端编辑器更常用               |
| 02:31-04:18 | 讲解 VI 的模式结构         | 展示“<span style="color:#FF0000; background:#00FF80;">一般模式—编辑模式—命令行模式</span>”<span style="color:#FF0000;">关系图</span>，并<span style="background:#00FF80;">手写 `vi 1.txt` 等示例</span> | 建立 VI 的核心概念                                           |
| 04:18-05:22 | 使用 VI 新建文件           | 输入 `vi 2.txt`，底部出现 `"2.txt" [New File]`；退出后用 `ls -l` 查看文件 | 演示“文件不存在时新建”                                       |
| 05:22-06:18 | 输入内容并保存             | 按 `i` 进入插入模式，底部出现 `-- INSERT --`，输入网址和姓名等内容 | 演示<span style="color:#FF0000; background:#00FF80;">编辑模式及保存</span> |
| 06:18-06:45 | 验证保存结果               | 使用 `cat 2.txt`，终端显示刚才输入的三行内容                 | <span style="color:#FF0000; background:#00FF80;">证明内容已经写入磁盘</span> |
| 06:45-07:42 | 修改后尝试直接退出         | 再次打开文件并继续修改，退出时出现 `E37: No write since last change` | 说明存在未保存修改时，普通退出会被阻止                       |
| 07:42-08:20 | 总结移动命令并准备长文件   | 讲义列出上下左右、翻页、跳转等命令；将 `/etc/sensors3.conf` 复制为 `3.txt` | 为浏览长文件做准备                                           |
| 08:20-09:04 | 浏览长文本                 | 打开包含523行的 `3.txt`，在文件不同位置之间移动              | 演示一般模式下的浏览                                         |
| 09:04-09:40 | 文件尾部和行号显示         | 使用 `G` 到文件末尾，执行 `:set number` 显示行号，再执行 `:set nonumber` 关闭行号 | 演示快速定位和显示设置                                       |
| 09:40-10:37 | 跳转指定行、行首与行尾     | 画面在第250行、第90行等位置间变化，结合讲义说明输入行号跳转，以及 `0`、`$` 的作用 | 总结浏览文件的主要操作                                       |

------

## 4. 内容结构

1. **开头：** 介绍 VI 的功能，并用图形编辑器 gedit 作为对照。
2. **发展：** 讲解 VI 的三种模式和进入文件的方法。
3. **转折：** 从概念图切换到实际终端，演示新建、保存和退出。
4. **结尾：** 使用<span style="color:#FF0000; background:#00FF80;">长文件演示翻页、行号、跳转和首尾定位。</span>

整体表达路径：

> <span style="color:#FF0000; background:#00FF80;">从【为什么需要终端编辑器】开始，通过【三种模式与实际操作】展开，最后落到【如何快速浏览和定位长文件】。</span>

------

## 5. 核心知识讲透

### 5.1 终端命令行和 VI 命令行不是一回事

这是视频里最容易混淆的地方。

#### 终端命令行

看到类似下面的提示符时：

```bash
book@100ask:~$
```

说明你还在 Linux Shell 中。

此时输入：

```bash
vi 2.txt
```

意思是：“启动 VI，并打开 `2.txt`。”

#### VI 命令行模式

<span style="color:#FF0000; background:#00FF80;">进入 VI 后，先按 `Esc` 回到一般模式，再输入冒号：</span>

```text
:
```

屏幕底部<span style="color:#FF0000; background:#00FF80;">出现冒号后，才能输入：</span>

```vim
:w
:q
:wq
:q!
:set number
```

因此：

```bash
vi 2.txt
```

是<span style="color:#FF0000; background:#00FF80;">终端命令。</span>

```vim
:wq
```

是 <span style="color:#FF0000; background:#00FF80;">VI 内部命令。</span>

------

### 5.2 VI 的三种核心模式

| 模式       | 怎么进入                                                     | 能做什么                                                     |
| ---------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 一般模式   | <span style="color:#FF0000; background:#00FF80;">打开文件后默认进入；其他模式按 `Esc` 返回</span> | <span style="color:#FF0000;">移动、删除、复制、跳转</span>   |
| 编辑模式   | <span style="color:#FF0000; background:#00FF80;">一般模式按 `i` 等键</span> | <span style="color:#FF0000;">输入普通文字</span>             |
| 命令行模式 | <span style="color:#FF0000; background:#00FF80;">一般模式按 `:`</span> | <span style="color:#FF0000;">保存、退出、设置行号、跳转行</span> |

#### 为什么模式特别重要？

假设你想输入字母 `i`：

- 在<span style="color:#FF0000;">编辑模式中，`i` 就是普通文字。</span>
- 在<span style="color:#FF0000;">一般模式中，`i` 表示“进入编辑模式”。</span>

因此，<span style="background:#00FF80; color:#FF0000;">当键盘输入结果不符合预期时，第一件事应该检查：</span>

> <span style="background:#00FF80;">**我现在到底处于什么模式？**</span>

------

### 5.3 新建文件

在终端输入：

```bash
vi notes.txt
```

如果 `notes.txt` 不存在，底部一般会出现：

```text
"notes.txt" [New File]
```

但需要注意：

> <span style="color:#FF0000; background:#00FF80;">打开一个不存在的文件，不等于它已经真正保存到磁盘。</span>

<span style="color:#FF0000; background:#00FF80;">通常要执行一次 `:w` 或 `:wq`，文件才会真正建立。</span>

#### 完整示例

```text
vi notes.txt
```

进入 VI 后：

1. 按 `i`；
2. 输入内容；
3. 按 `Esc`；
4. 输入 `:wq`；
5. 按回车。

------

### 5.4 保存与退出

| 命令  | 作用               | 是否退出               |
| ----- | ------------------ | ---------------------- |
| `:w`  | 保存当前文件       | 不退出                 |
| `:q`  | 退出               | 有未保存修改时不能退出 |
| `:wq` | 保存并退出         | 是                     |
| `:x`  | 保存修改并退出     | 是                     |
| `:q!` | 放弃修改，强制退出 | 是                     |

#### 视频中的错误提示

约在 **07:30**，画面底部出现：

```text
E37: No write since last change
```

它的意思是：

> 文件已经被修改，但修改还没有保存，VI 为了防止内容丢失，拒绝执行普通退出。

这时有两种选择。

保存修改：

```vim
:wq
```

不要这些修改：

```vim
:q!
```

这里的<span style="color:#FF0000; border:1px solid #330000; background:#00FF80;">感叹号可以理解为：</span>

> “<span style="color:#FF0000; background:#00FF80;">我确定要强制执行。</span>”

------

### 5.5 浏览长文件

这些命令<span style="background:#00FF80;">多数需要在**一般模式**下使用。</span>

| 操作                   | 命令                          |
| ---------------------- | ----------------------------- |
| 光标上、下、左、右移动 | 方向键，或 `k`、`j`、`h`、`l` |
| 向后翻一页             | `Ctrl+f`                      |
| 向前翻一页             | `Ctrl+b`                      |
| 跳到文件末尾           | `G`                           |
| 跳到文件开头           | `gg`，这是补充命令            |
| 跳到当前行行首         | `0`                           |
| 跳到当前行行尾         | `$`                           |
| 显示行号               | `:set number`                 |
| 隐藏行号               | `:set nonumber`               |
| 跳到第250行            | `:250` 后回车                 |

#### 三个特别容易按错的地方

1. `G` 是大写字母，需要按 `Shift+g`。
2. <span style="color:#FF0000; background:#00FF80;">`0` 是数字零，不是大写字母 `O`。</span>
3. `$` 通常需要按 `Shift+4`。

------

### 5.6 指定打开位置

更稳妥、常见的写法是：

```bash
vi +100 文件名
```

例如：

```bash
vi +100 3.txt
```

表示：

> <span style="color:#FF0000; background:#00FF80;">打开 `3.txt`，并把光标定位到第100行附近。</span>

<span style="color:#FF0000;">打开后直接到文件末尾，可以使用：</span>

```bash
vi + 3.txt
```

------

### 5.7 一套完整操作示例

#### <span style="background:#00FF80;">新建并保存</span>

```bash
vi test.txt
```

进入后依次操作：

```text
i
Hello Linux
按 Esc
:wq
按回车
```

#### <span style="background:#00FF80;">修改后放弃</span>

```bash
vi test.txt
```

进入后：

```text
i
输入一些不想保留的内容
按 Esc
:q!
按回车
```

#### <span style="background:#00FF80;">浏览长文件</span>

```bash
vi /etc/sensors3.conf
```

进入后可以执行：

```text
Ctrl+f          向后翻页
Ctrl+b          向前翻页
:set number     显示行号
:250            跳到第250行
G               跳到最后一行
0               跳到当前行开头
$               跳到当前行结尾
:q              退出
```

------

## 6. 观点分析

本视频是<span style="color:#FF0000;">教程类视频，没有明显争议性立场，主要是知识判断。</span>

| 观点                                                         | 观点归属                    | 明确/可能  | 依据                                                         |
| ------------------------------------------------------------ | --------------------------- | ---------- | ------------------------------------------------------------ |
| VI 可以在终端中完成文件的新建、编辑、保存和浏览              | 讲师/视频创作者             | 明确       | 开头功能列表和后续操作演示                                   |
| <span style="color:#FF0000;">掌握 VI 的关键是理解模式切换</span> | 视频创作者                  | 可能但很强 | 三模式关系图被反复讲解，多个操作都以模式为前提               |
| <span style="color:#FF0000;">未保存修改时不能随意退出，是为了防止内容丢失</span> | VI 程序机制，同时被讲师强调 | 明确       | <span style="background:#00FF80; color:#FF0000;">`E37` 错误演示</span> |
| <span style="color:#FF0000;">长文件应使用翻页、行号和跳转命令，而不是只按方向键</span> | 视频创作者                  | 可能       | 后半段集中展示这些快捷操作                                   |

**核心观点：**

> VI 的命令并不难，真正需要理解的是“<span style="color:#FF0000; background:#00FF80;">当前模式决定按键含义</span>”。

**无人认领但可能属于作者的观点：**

> 对 Linux 学习者而言，VI 是比单纯依赖图形编辑器更基础、更通用的工具。

------

## 7. 人物分析

视频中没有清晰出镜人物，主要呈现桌面录屏、鼠标操作、终端和讲义。

因此：

- 无法可靠分析人物表情、情绪或心理。
- 不能依据操作速度判断讲师性格或人格。
- 能明确判断的是，讲授方式采用了“<span style="color:#FF0000; background:#00FF80;">对比展示 → 模式讲解 → 实际演示 → 错误演示 → 总结</span>”的结构。

------

## 8. Who / Whom / Whose / When / Where / What / Why / How

| 维度  | 视频中的答案                                                 |
| ----- | ------------------------------------------------------------ |
| Who   | Linux课程讲师或终端操作者                                    |
| Whom  | 面向刚开始学习 Linux、VI 的学习者                            |
| Whose | 文件属于当前用户或系统账户；系统文件可能属于 root            |
| When  | 在学习 Linux 文件编辑基础操作时                              |
| Where | Ubuntu 18.04 虚拟机、终端、VI 编辑器内部                     |
| What  | 新建、输入、保存、退出、放弃修改和浏览文本文件               |
| Why   | Linux服务器、嵌入式设备等环境可能没有图形化编辑器，并且系统文件常需终端权限操作 |
| How   | 通过终端启动 VI，在一般、编辑、命令行三种模式之间切换完成操作 |

------

## 9. 重点片段精读

### 片段一：02:31-04:18

**表面发生了什么：**
讲师展示 VI 的三种模式关系图。

**更深层可能意味着：**
后面的保存、退出和移动命令都建立在模式切换上。如果这里没理解，后续容易出现“为什么输入不了文字”“为什么按键变成命令”等问题。

**重要性：**
这是全视频的理论核心。

------

### 片段二：05:22-06:45

**表面发生了什么：**
进入插入模式，输入三行文字，保存后用 `cat 2.txt` 查看。

**更深层可能意味着：**
视频完成了“<span style="color:#FF0000; background:#00FF80;">内存中的编辑内容 → 保存到磁盘 → 用外部命令验证</span>”的完整闭环。

**重要性：**
这是最适合跟着练习的一段。

------

### 片段三：06:45-07:42

**表面发生了什么：**
修改文件后直接退出，出现 `E37`。

**更深层可能意味着：**
<span style="color:#FF0000; background:#00FF80;">VI 不会在存在未保存修改时默默退出。用户必须明确选择“保存”或“放弃”。</span>

**重要性：**
这是<span style="color:#FF0000; background:#00FF80;">初学者最常遇到的报错之一</span>。

------

### 片段四：09:04-10:37

**表面发生了什么：**
讲师使用长文件演示 `G`、行号显示、指定行跳转、行首和行尾。

**更深层可能意味着：**
<span style="color:#FF0000; background:#00FF80;">VI 的优势不只是输入文字，还包括不依赖鼠标快速定位大量文本。</span>

**重要性：**
决定了实际使用 VI 时的效率。

------

## 10. 我应该重点看什么？

观看这段教程时，优先掌握以下四点：

1. **怎么判断当前模式：** <span style="color:#FF0000; background:#00FF80;">是否显示 `-- INSERT --`，底部是否出现冒号。</span>
2. **怎么安全退出：**<span style="color:#FF0000; background:#00FF80;"> `:wq`、`:q`、`:q!` 的区别。</span>
3. **怎么处理 `E37`：** <span style="color:#FF0000; background:#00FF80;">保存就用 `:wq`，放弃就用 `:q!`。</span>
4. **怎么浏览长文件：**<span style="color:#FF0000; background:#00FF80;"> `Ctrl+f`、`Ctrl+b`、`G`、`:行号`、`0`、`$`。</span>

------

## 11. 记忆版总结

```text
类型：Linux基础教程、VI操作演示

主线：
从图形编辑器对比开始，讲解VI三种模式，
然后演示新建、保存、退出和浏览。

核心观点：
使用VI前，必须先判断自己处于什么模式。

关键操作：
i       进入编辑模式
Esc     返回一般模式
:w      保存
:wq     保存退出
:q      普通退出
:q!     放弃修改并退出
G       文件末尾
0       行首
$       行尾
:250    跳到第250行

最重要的时间段：
02:31-04:18 三种模式
05:22-06:45 输入与保存
06:45-07:42 E37错误
09:04-10:37 文件浏览

一句话记忆：
终端里用vi打开文件，按i输入，按Esc停止输入，
输入:wq保存退出，输入:q!放弃退出。
```







# 问题1

<span style="color:#FF0000; background:#00FF80;">我有点搞混乱，这里在ubuntu终端中输入的命令是linux开发板执行的吗，如果是这样的话那么开发板执行gedit，vi命令是吗，Linux开发版不是没有图形化的编辑界面吗，这里怎么能执行gedit命令呢？</span>



你这里最关键的误区是：

> **“在 Ubuntu 终端里输入命令”不等于“命令一定由 Linux 开发板执行”。**

<span style="color:#FF0000; background:#00FF80;">命令由谁执行，取决于你当前这个终端连接的是谁。</span>

------

## 一、视频中的命令大概率不是开发板执行的

从视频画面来看，讲师是在带图形桌面的 **Ubuntu 虚拟机或 Ubuntu 电脑** 中打开终端，然后输入：

```bash
gedit 1.txt
vi 2.txt
```

因此，<span style="color:#FF0000; background:#00FF80;">这两个命令都是由当前 Ubuntu 系统执行的，而不是开发板执行的</span>。

整体关系大致是：

```text
Windows电脑
   │
   └── Ubuntu虚拟机
          │
          ├── Ubuntu终端
          ├── gedit图形编辑器
          └── vi终端编辑器
```

视频这一节主要是在 Ubuntu 环境中教你学习 VI 命令，并不表示已经登录了开发板。

------

## 二、终端只是一个“命令输入窗口”

你可以把终端理解成电话。

打开电话不代表你正在和开发板通话。关键要看电话接通了谁。

### 情况1：直接打开 Ubuntu 终端

例如终端提示符是：

```bash
book@100ask:~$
```

这表示：

```text
当前用户：book
当前主机：100ask
```

此时命令由名为 `100ask` 的 Ubuntu 系统执行。

输入：

```bash
gedit 1.txt
```

就是<span style="color:#FF0000;">让 Ubuntu 电脑或虚拟机启动 gedit。</span>

输入：

```bash
vi 1.txt
```

也是<span style="color:#FF0000;">让 Ubuntu 电脑或虚拟机启动 vi。</span>

------

### 情况2：<span style="background:#00FF80;">先通过 SSH 登录开发板</span>

例如先执行：

```bash
ssh root@192.168.1.100
```

登录成功后，提示符可能变成：

```bash
root@imx6ull:~#
```

这时再输入：

```bash
vi 1.txt
```

<span style="color:#FF0000;">才是让 Linux 开发板执行 `vi`。</span>

关系是：

```text
Ubuntu电脑上的终端
        │
        │ SSH网络连接
        ▼
Linux开发板
        │
        └── 执行 vi 1.txt
```

虽然终端窗口显示在电脑屏幕上，但命令实际由开发板执行。

------

## 三、为什么视频里能运行 gedit？

因为视频中的 Ubuntu 系统带有图形桌面。

`gedit` 是一个图形化文本编辑器，运行时需要：

1. 系统安装了 `gedit`；
2. 系统有图形桌面；
3. 有图形显示服务；
4. 当前程序能够访问显示器。

例如在 Ubuntu 桌面系统中输入：

```bash
gedit 1.txt
```

Ubuntu 会弹出一个图形窗口。

这和你在 Windows 中打开记事本类似。

------

## 四、Linux开发板一定没有图形界面吗？

不一定。

“Linux开发板没有图形界面”只是一种常见情况，不是绝对规则。

### 常见的嵌入式开发板

<span style="color:#FF0000; background:#00FF80;">很多开发板使用精简的 Linux 文件系统</span>，只保留：

```text
Linux内核
命令行Shell
驱动程序
BusyBox工具
应用程序
```

通常没有：

```text
Ubuntu桌面
gedit
浏览器
窗口管理器
完整图形环境
```

这类开发板一般使用：

```bash
vi 文件名
```

或者：

```bash
vim 文件名
```

进行文本编辑。

运行 `gedit` 时可能出现：

```text
gedit: command not found
```

或者：

```text
Unable to init server
Cannot open display
```

------

### 配置较完整的开发板

有些<span style="color:#FF0000; background:#00FF80;">性能较强的开发板可以安装 Ubuntu Desktop</span>，例如部分：

- Raspberry Pi；
- NVIDIA Jetson；
- RK3588开发板；
- 带显示屏的工控板。

这些开发板可能真的拥有桌面，也可能运行：

```bash
gedit 1.txt
```

所以不能简单地说“开发板一定没有图形界面”。

更准确的说法是：

> 嵌入式开发板通常<span style="color:#FF0000; background:#00FF80;">为了节省存储和内存，只安装命令行环境，因此一般不使用 gedit；但硬件和系统配置允许时，也可以安装图形桌面。</span>

------

## 五、为什么课程<span style="color:#FF0000; background:#00FF80;">先在 Ubuntu 中演示 VI？</span>

因为 `vi` 不只属于开发板，它是 Linux 系统中非常通用的编辑器。

它<span style="color:#FF0000; background:#00FF80;">可以运行在：</span>

```text
Ubuntu电脑
Ubuntu虚拟机
Linux服务器
嵌入式Linux开发板
云服务器
串口终端
SSH远程终端
```

所以<span style="color:#FF0000;">课程通常先在 Ubuntu 虚拟机里练习：</span>

```bash
vi test.txt
```

等你<span style="color:#FF0000; background:#00FF80;">掌握以后，再登录开发板执行同样的命令：</span>

```bash
ssh root@开发板IP
vi /etc/配置文件
```

VI 的操作方法基本相同：

```text
i       进入编辑模式
Esc     返回一般模式
:wq     保存退出
:q!     放弃修改退出
dd      删除一行
u       撤销
```

因此，视频中的 `gedit` 主要是用于对比：

```text
gedit：依赖图形桌面的编辑器
vi：可以直接在终端中使用的编辑器
```

------

## 六、怎么判断命令当前由谁执行？

看终端提示符。

一般形式是：

```text
用户名@主机名:当前目录$
```

例如：

```bash
book@ubuntu:~$
```

表示你<span style="color:#FF0000; background:#00FF80;">现在操作的是主机 `ubuntu`。</span>

如果登录开发板后变成：

```bash
root@imx6ull:~#
```

说明<span style="color:#FF0000; background:#00FF80;">现在操作的是开发板 `imx6ull`。</span>

也可以输入：

```bash
hostname
```

查看当前主机名。

例如返回：

```text
ubuntu
```

说明命令在 Ubuntu 系统执行。

如果返回：

```text
imx6ull
```

说明命令在开发板执行。

还可以输入：

```bash
uname -a
```

查看当前系统的内核和CPU架构。

电脑或虚拟机可能显示：

```text
x86_64
```

ARM开发板通常可能显示：

```text
armv7l
```

或者：

```text
aarch64
```

------

## 七、一个完整例子

假设你的电脑上运行 Ubuntu 虚拟机，同时有一块 Linux 开发板。

### 在 Ubuntu 虚拟机中

提示符：

```bash
book@ubuntu:~$
```

执行：

```bash
gedit test.txt
```

结果：

> Ubuntu 虚拟机<span style="color:#FF0000;">弹出图形编辑窗口。</span>

执行：

```bash
vi test.txt
```

结果：

> Ubuntu 虚拟机<span style="color:#FF0000;">在终端中打开文件。</span>

------

### 登录开发板以后

执行：

```bash
ssh root@192.168.1.100
```

提示符变为：

```bash
root@board:~#
```

执行：

```bash
vi test.txt
```

结果：

> 开发板执行 VI，只是操作画面通过 SSH显示在你的电脑终端里。

执行：

```bash
gedit test.txt
```

开发板很可能返回：

```text
gedit: command not found
```

因为开发板通常没有安装 gedit，也没有图形桌面。

------

## 八、最简单的记忆方式

```text
终端窗口在哪里显示，不决定谁执行命令。

没有登录开发板：
命令由Ubuntu电脑或虚拟机执行。

通过SSH或串口进入开发板后：
命令由开发板执行。

gedit：
需要图形界面，通常在Ubuntu电脑上使用。

vi：
不需要图形界面，Ubuntu和Linux开发板都可以使用。
```

所以结合这个视频，最准确的理解是：

> **讲师是在 Ubuntu 图形系统中演示 `gedit` 和 `vi`。这时命令由 Ubuntu 虚拟机或电脑执行，并非开发板。<span style="color:#FF0000; background:#00FF80;">学习 VI 后，再通过 SSH或串口登录开发板，就可以在开发板上使用相同的 VI 命令。</span>**





