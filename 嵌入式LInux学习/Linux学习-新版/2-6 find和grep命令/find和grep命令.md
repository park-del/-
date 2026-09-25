# 视频分析总结

视频总长约 **7分19秒**。内容是一个 **Linux 基础命令教学视频**，主要讲 **查找/搜索命令：`find` 和 `grep`**，最后顺带进入下一节 **压缩/解压命令 `gzip`**。

## 一句话总结

这个视频主要在讲：
<span style="background:#00FF80;">**`find` 用来按文件名、路径、时间等条件查找文件；`grep` 用来在文件内容里查找字符串。**</span>

------

## 时间点总结

| 时间点            | 内容                                         | 重点                                                         |
| ----------------- | -------------------------------------------- | ------------------------------------------------------------ |
| **00:00 - 00:35** | 打开 PDF 课件，进入 **2.3.4 查找/搜索命令**  | 先讲 `find`。对比 Windows 搜索和 Linux 搜索：<span style="color:#FF0000;">都需要知道“在哪里找”和“找什么”</span>。 |
| **00:35 - 01:05** | 切到 Ubuntu 终端，输入 `find -name 2.txt`    | 演示<span style="color:#FF0000;">不指定路径时，`find` 默认在当前目录下查找</span>。中途用 `Ctrl + C` 停止查找。 |
| **01:05 - 01:30** | 查看当前目录内容，进入 `Desktop`，再执行查找 | 通过 `ls`、`cd Desktop/`、`find -name 2.txt` 演示：<span style="color:#FF0000; background:#00FF80;">当前目录不同，查找结果也不同。</span> |
| **01:30 - 02:10** | 使用 `find abc -name 2.txt`                  | <span style="color:#FF0000;">指定目录 `abc` 后，能找到 `abc/2.txt`。这里说明 `find 目录名 -name 文件名` 的基本格式。</span> |
| **02:10 - 03:30** | 在 `abc` 目录中复制文件并测试通配符          | 用 `cp 2.txt 12.txt`、`cp 2.txt 22.txt1` 制造测试文件，然后演示 `*2.txt` 和 `*2.txt*` 的区别。 |
| **03:30 - 04:05** | 回到课件总结 `find`                          | 讲解：如果不指定查找目录，默认当前路径；`.` 表示当前目录；还提到高级用法，如 `find /home/book -mtime -2` 查找最近两天变动的文件。 |
| **04:05 - 04:40** | 打开文本编辑器/文件，准备测试内容            | <span style="color:#FF0000;">为后面 `grep` 查找文件内容做准备，文件里包含 `abc`、`456abc` 等字符串。</span> |
| **04:40 - 05:15** | 课件进入 `grep` 命令                         | <span style="color:#FF0000; background:#00FF80;">`grep` 的作用是查找文件中符合条件的字符串。基本格式是：`grep [选项] [查找模式] [文件名]`。</span> |
| **05:15 - 05:55** | 终端演示 `grep "abc" *`                      | <span style="color:#FF0000;">在当前目录的多个文件中搜索 `abc`。结果中会显示文件名，并高亮匹配到的字符串。</span> |
| **05:55 - 06:20** | 演示 `grep "abc" * -n`                       | <span style="color:#FF0000;">加 `-n` 后显示匹配内容所在的行号</span>，例如第 1 行、第 2 行。 |
| **06:20 - 06:50** | 演示 `grep "abc" * -nw`                      | <span style="color:#FF0000;">加 `-w` 表示“全字匹配”。也就是只匹配独立的 `abc`，不会把 `456abc` 这种包含关系算作完整匹配。</span> |
| **06:50 - 07:05** | 演示递归/组合选项                            | 继续用 `grep` 的多个选项组合查找，例如 `-n`、`-w`、`-r` 等，用来控制行号、完整匹配、递归查找。 |
| **07:05 - 07:19** | 回到课件，进入下一节                         | 总结 `grep`，提到可以用管道 `                                |

------

## 核心知识点拆解

### 1. `find`：找“文件/目录本身”

`find` 主要用于<span style="color:#FF0000;">查找文件或目录</span>，比如：

```bash
find abc -name 2.txt
```

意思是：
在 `abc` 目录下，查找名字叫 `2.txt` 的文件。

视频里还演示了通配符：

```bash
find abc -name "*2.txt"
```

表示查找 **以 `2.txt` 结尾** 的文件，比如：

```bash
abc/2.txt
abc/12.txt
```

而：

```bash
find abc -name "*2.txt*"
```

表示只要名字里包含 `2.txt` 这一段就可以，所以能找到：

```bash
abc/2.txt
abc/12.txt
abc/22.txt1
```

这里的重点是：
**`\*` 放在哪里，匹配范围就在哪里。**

------

### 2. `grep`：找“文件内容”

`grep`<span style="color:#FF0000; background:#00FF80;"> 不是找文件名，而是找文件里面的文字内容。</span>

例如：

```bash
grep "abc" *
```

表示：
在<span style="color:#FF0000; background:#00FF80;">当前目录下的所有文件中，查找内容里包含 `abc` 的行。</span>

加上 `-n`：

```bash
grep "abc" * -n
```

表示显示行号。

加上 `-w`：

```bash
grep "abc" * -nw
```

表示完整匹配 `abc`，不会把 `456abc` 当成完整的 `abc`。

------

## `find` 和 `grep` 的区别

| 命令   | 查找对象                 | 例子                   | 含义                                   |
| ------ | ------------------------ | ---------------------- | -------------------------------------- |
| `find` | 文件名、目录名、文件属性 | `find abc -name 2.txt` | 在 `abc` 目录下找名字叫 `2.txt` 的文件 |
| `grep` | 文件内容                 | `grep "abc" *`         | 在当前目录文件内容中找 `abc`           |

可以这样记：

**`find` 是找“文件在哪里”；`grep` 是找“文件里面有没有这句话”。**

------

## 这个视频的教学重点

这段视频不是单纯讲概念，而是用 **课件 + 终端演示** 的方式讲 Linux 搜索命令。最重要的是让你区分：

**按文件名找：用 `find`。**
**按文件内容找：用 `grep`。**

另外还重点讲了几个容易混淆的地方：

`find` 不写目录时，默认从当前目录查找。
`*` 是通配符，位置不同，匹配结果不同。
`grep -n` 可以显示行号。
`grep -w` 可以完整匹配单词。
`grep` 可以和管道 `|` 结合，进一步筛选结果。



# grep查找内容区分

`grep` 查找内容时，**“行”是按文件里的换行符来区分的**。

也就是说，文本文件中每遇到一次换行，就算进入下一行。

例如文件 `a.txt` 内容是：

```bash
abc hello abc
456abc
hello
abc
```

那么它有 4 行：

```bash
第1行：abc hello abc
第2行：456abc
第3行：hello
第4行：abc
```

------

## 1. <span style="background:#00FF80;">普通 `grep` 是“按行输出”</span>

比如：

```bash
grep "abc" a.txt
```

结果会是：

```bash
abc hello abc
456abc
abc
```

注意：
**<span style="background:#00FF80;">只要这一行里面出现了 `abc`，这一整行就会被输出。</span>**

------

## 2. 如果一行里有多个匹配项，会不会全部列出来？

<span style="background:#00FF80;">默认情况下，**不会把同一行重复列多次**。</span>

例如第 1 行：

```bash
abc hello abc
```

里面有两个 `abc`，但是<span style="color:#FF0000; background:#00FF80;">普通 `grep` 只输出这一行一次：</span>

```bash
abc hello abc
```

<span style="color:#FF0000;">不过在终端里，两个 `abc` 可能都会被高亮显示。</span>

所以可以这样理解：

> 默认 `grep` 查的是“哪些行包含 abc”，不是“abc 一共出现了几次”。

------

## 3. 如果想显示行号，用 `-n`

```bash
grep -n "abc" a.txt
```

结果可能是：

```bash
1:abc hello abc
2:456abc
4:abc
```

这里的 `1:`、`2:`、`4:` 就是行号。

但是第 1 行虽然有两个 `abc`，也只显示一次：

```bash
1:abc hello abc
```

------

## 4. 如果想把每一个匹配项都单独列出来，用 `-o`

```bash
grep -o "abc" a.txt
```

结果是：

```bash
abc
abc
abc
abc
```

这里就会把每一个匹配到的 `abc` 都单独输出。

如果加上行号：

```bash
grep -n -o "abc" a.txt
```

结果可能是：

```bash
1:abc
1:abc
2:abc
4:abc
```

这就能看出：
第 1 行有两个 `abc`，第 2 行有一个，第 4 行有一个。

------

## 5. `-w` 会影响匹配范围

比如：

```bash
grep -n -w "abc" a.txt
```

它只匹配独立的 `abc`。

所以：

```bash
abc hello abc
```

会匹配。

但：

```bash
456abc
```

一般不会匹配，因为 `abc` 不是一个独立单词，而是粘在 `456` 后面。

------

## 记忆版

**普通 `grep`：找“包含关键词的行”。**
**`grep -n`：找“包含关键词的行 + 显示行号”。**
**`grep -o`：找“每一个匹配到的关键词”。**
**`grep -w`：只找“完整独立的关键词”。**







# find输出

<span style="background:#00FF80;">`find` 的输出结果一般是：**把找到的文件/目录路径，一行一行打印出来。**</span>

它默认等价于：

```bash
find 查找范围 查找条件 -print
```

也就是说，`find`<span style="color:#FF0000; background:#00FF80;"> 默认会把匹配到的路径输出出来。</span>

------

## 1. 基本输出样子

假设目录结构是：

```bash
Desktop/
└── abc/
    ├── 2.txt
    ├── 12.txt
    └── 22.txt1
```

执行：

```bash
find abc -name 2.txt
```

输出可能是：

```bash
abc/2.txt
```

意思是：
在 `abc` 目录下找到了一个叫 `2.txt` 的文件，它的位置是 `abc/2.txt`。

------

## 2. 如果找到多个，会一行一个

比如执行：

```bash
find abc -name "*2.txt"
```

可能输出：

```bash
abc/2.txt
abc/12.txt
```

因为：

```bash
2.txt
12.txt
```

都符合 `*2.txt`，也就是“以 `2.txt` 结尾”。

------

## 3. 如果写成 `*2.txt*`

```bash
find abc -name "*2.txt*"
```

可能输出：

```bash
abc/2.txt
abc/12.txt
abc/22.txt1
```

因为 `*2.txt*` 表示：
只要文件名里包含 `2.txt` 这一段就可以。

所以：

```bash
22.txt1
```

也能被找到，因为它里面有：

```bash
2.txt
```

------

## 4.<span style="background:#00FF80;"> 输出的是“路径”，不是文件内容</span>

这一点和 `grep` 不一样。

```bash
find abc -name 2.txt
```

输出：

```bash
abc/2.txt
```

<span style="background:#00FF80;">它告诉你：**文件在哪里。**</span>

<span style="color:#FF0000; background:#00FF80;">而不是输出 `2.txt` 文件里面写了什么。</span>

------

## 5.<span style="background:#00FF80;"> 路径形式取决于你怎么写查找范围</span>

<span style="color:#FF0000; background:#00FF80;">如果你写相对路径：</span>

```bash
find abc -name 2.txt
```

输出一般是：

```bash
abc/2.txt
```

<span style="color:#FF0000; background:#00FF80;">如果你写当前目录：</span>

```bash
find . -name 2.txt
```

输出可能是：

```bash
./abc/2.txt
```

<span style="color:#FF0000; background:#00FF80;">如果你写绝对路径：</span>

```bash
find /home/book/Desktop/abc -name 2.txt
```

输出可能是：

```bash
/home/book/Desktop/abc/2.txt
```

所以可以记成：

> <span style="color:#FF0000; background:#00FF80;">`find` 输出的路径，通常会跟你输入的查找范围保持同一种形式。</span>

------

## 6. <span style="background:#00FF80;">如果没找到，通常什么都不输出</span>

例如：

```bash
find abc -name 999.txt
```

<span style="color:#FF0000; background:#00FF80;">如果没有这个文件，结果可能就是空的：</span>

```bash

```

不是报错，只是没有匹配结果。

------

## 7. 如果没有权限，可能会输出错误信息

比如从根目录 `/` 查找：

```bash
find / -name 2.txt
```

可能看到：

```bash
find: ‘/root’: Permission denied
find: ‘/proc/1234’: Permission denied
/home/book/Desktop/abc/2.txt
```

这里：

```bash
/home/book/Desktop/abc/2.txt
```

是找到的结果。

```bash
Permission denied
```

<span style="color:#FF0000; background:#00FF80;">是没有权限访问某些目录的提示。</span>

------

## 记忆版

<span style="background:#00FF80;">**`find` 的输出 = 找到的文件/目录路径。**</span>

例如：

```bash
find abc -name 2.txt
```

可以理解成：

```bash
在 abc 里面找名字叫 2.txt 的东西
```

输出：

```bash
abc/2.txt
```

可以理解成：

```bash
找到了，它在 abc/2.txt
```





# 对Linux中的home目录与Desktop目录的理解

可以这样理解：

**`home` 是你的“个人家目录”，`Desktop` 是你家目录里面的“桌面文件夹”。**

它们不是同一层级的关系，而是 **包含关系**。

------

## 1. 目录关系是什么？

一般 Linux 里是这样：

```bash
/
├── home
│   └── book
│       ├── Desktop
│       ├── Documents
│       ├── Downloads
│       └── ...
├── usr
├── bin
├── etc
└── ...
```

如果用户名叫 `book`，那么：

```bash
/home/book
```

就是这个用户的 **home 目录**。

而桌面目录是：

```bash
/home/book/Desktop
```

所以关系是：

```bash
/home/book/Desktop
```

等于：

```bash
home目录 里面的 Desktop目录
```

------

## 2. home 目录是什么？

`home` 可以理解成：

> <span style="color:#FF0000; background:#00FF80;">每个用户自己的私人空间。</span>

比如系统里有三个用户：

```bash
/home/book
/home/tom
/home/jack
```

每个人都有自己的家目录。

`book` 用户自己的文件一般放在：

```bash
/home/book
```

里面，比如：

```bash
/home/book/Desktop
/home/book/Documents
/home/book/Downloads
```

也就是说，`home` 目录不是某一个具体用户的家，而是 **所有普通用户家目录的总文件夹**。

------

## 3. `/home` 和 `/home/book` 不一样

这个地方很容易混。

| 路径                 | 含义                          |
| -------------------- | ----------------------------- |
| `/home`              | 存放所有用户家目录的地方      |
| `/home/book`         | `book` 这个用户自己的家目录   |
| `/home/book/Desktop` | `book` 用户家目录里的桌面目录 |

可以具象化成：

```text
/home              = 小区
/home/book         = book 的家
/home/book/Desktop = book 家里的桌面
```

------

## 4. Desktop 是什么？

`Desktop` 就是图形界面里的“桌面”。

比如你在 Ubuntu 桌面上新建一个文件：

```text
1.txt
```

它实际可能就存放在：

```bash
/home/book/Desktop/1.txt
```

所以你在终端里进入桌面，可以这样写：

```bash
cd /home/book/Desktop
```

或者如果你当前就在自己的 home 目录下，可以写：

```bash
cd Desktop
```

------

## 5. `~` 是什么？

`~` 表示当前用户的 home 目录。

如果当前用户是 `book`：

```bash
~
```

就等于：

```bash
/home/book
```

所以：

```bash
cd ~
```

等于：

```bash
cd /home/book
```

而：

```bash
cd ~/Desktop
```

等于：

```bash
cd /home/book/Desktop
```

------

## 6. 为什么终端一打开通常就在 home 目录？

因为 home 目录是用户最常操作的地方。

比如你的文件、下载、桌面、文档，一般都在这里：

```bash
/home/book
```

所以打开终端后，默认通常在：

```bash
/home/book
```

提示符里可能显示：

```bash
book@ubuntu:~$
```

这里的 `~` 就表示你现在在自己的 home 目录。

------

## 7. `cd Desktop` 为什么有时成功，有时失败？

因为 `cd Desktop` 是相对路径。

它的意思是：

> 进入当前目录下面的 Desktop 文件夹。

如果你现在在：

```bash
/home/book
```

执行：

```bash
cd Desktop
```

就相当于进入：

```bash
/home/book/Desktop
```

所以成功。

但如果你现在在：

```bash
/usr
```

执行：

```bash
cd Desktop
```

系统会去找：

```bash
/usr/Desktop
```

一般没有这个目录，所以会失败。

更稳妥的写法是：

```bash
cd ~/Desktop
```

因为 `~` 永远表示你自己的 home 目录。

------

## 记忆版

**`/home`：所有用户的家放在这里。**
**`/home/book`：book 这个用户自己的家。**
**`/home/book/Desktop`：book 家里的桌面。**
**`~`：当前用户自己的家目录。**

最简单记法：

```text
/home/book 是我的家
Desktop 是我家里的桌面
~/Desktop 就是“我家的桌面”
```



# Linux命令 选项、目录名、文件名、查找内容，多个出现应该选择哪个

你这个问题<span style="color:#FF0000; background:#00FF80;">可以抓住一个核心：</span>

> Linux 命令一般是：
> **<span style="background:#00FF80;">命令名 + 选项 + 参数</span>**

其中：

```bash
命令名：你要做什么
选项：你要怎么做
参数：你要对谁做 / 在哪里做 / 找什么
```

------

## 一、先看总规则

一般格式是：

```bash
命令 [选项] [参数]
```

例如：

```bash
ls -l /home/book/Desktop
```

拆开看：

| 部分 | 例子                 | 含义           |
| ---- | -------------------- | -------------- |
| 命令 | `ls`                 | 列出文件       |
| 选项 | `-l`                 | 以详细格式显示 |
| 参数 | `/home/book/Desktop` | 要查看哪个目录 |

所以 **选项通常带 `-` 或 `--`**，比如：

```bash
-n
-w
-r
-name
-type
```

而目录名、文件名一般是不带 `-` 的，比如：

```bash
abc
2.txt
/home/book/Desktop
```

------

## 二、`find` 命令的顺序

<span style="color:#FF0000; background:#00FF80;">`find` 的标准格式可以记成：</span>

```bash
find 查找范围 查找条件
```

也可以写成：

```bash
find 目录名 选项/条件 文件名模式
```

例如：

```bash
find abc -name 2.txt
```

拆开看：

| 部分       | 内容    | 含义                    |
| ---------- | ------- | ----------------------- |
| 命令       | `find`  | 查找文件                |
| 查找范围   | `abc`   | 在 `abc` 目录里面找     |
| 查找条件   | `-name` | 按名字查找              |
| 文件名模式 | `2.txt` | 找名字叫 `2.txt` 的文件 |

所以这句话就是：

> 在 `abc` 目录下，查找名字叫 `2.txt` 的文件。

------

### `find` 里面的顺序重点

`find` 一般是：<span style="color:#FF0000; background:#00FF80;">（补充：这里注意下面中的 条件 和 条件的值）</span>

```bash
find 目录名 条件 条件的值
```

例如：

```bash
find abc -name "*.txt"
```

意思是：

```text
在 abc 目录下，查找名字以 .txt 结尾的文件
```

再比如：

```bash
find . -name "2.txt"
```

意思是：

```text
在当前目录下，查找名字叫 2.txt 的文件
```

这里的 `.` 表示当前目录。

------

### `find` 中谁是目录名？谁是文件名？

看这个例子：

```bash
find abc -name 2.txt
```

可以这样判断：

```text
abc      是目录名，因为它放在 find 后面，表示在哪里找
-name    是选项/条件，因为它带 -
2.txt    是文件名模式，因为它跟在 -name 后面，表示找什么名字
```

也就是说：

```bash
find abc -name 2.txt
```

不是在找 `abc`，而是：

```text
在 abc 里面找 2.txt
```

------

## 三、`grep` 命令的顺序

<span style="color:#FF0000; background:#00FF80;">`grep` 的标准格式可以记成：</span>

```bash
grep 选项 查找内容 文件名
```

或者：

```bash
grep [选项] "要查找的内容" 文件名
```

例如：

```bash
grep -n "abc" a.txt
```

拆开看：

| 部分     | 内容    | 含义                  |
| -------- | ------- | --------------------- |
| 命令     | `grep`  | 查找文件内容          |
| 选项     | `-n`    | 显示行号              |
| 查找内容 | `"abc"` | 查找内容里有没有 abc  |
| 文件名   | `a.txt` | 在 a.txt 文件里面查找 |

所以这句话就是：

> 在 `a.txt` 文件内容中查找 `abc`，并显示行号。

------

### `grep` 的顺序重点

`grep` 一般是：

```bash
grep 选项 查找内容 文件名
```

例如：

```bash
grep -n "abc" a.txt
```

<span style="color:#FF0000; background:#00FF80;">也可以没有选项：</span>

```bash
grep "abc" a.txt
```

意思是：

```text
在 a.txt 文件中查找 abc
```

如果查找多个文件：

```bash
grep "abc" *
```

意思是：

```text
在当前目录下的所有文件里查找 abc
```

这里的 `*` 表示多个文件。

------

## 四、`find` 和 `grep` 顺序对比

最重要的区别是：

| 命令   | 基本格式                   | 找什么     |
| ------ | -------------------------- | ---------- |
| `find` | `find 目录名 -name 文件名` | 找文件名   |
| `grep` | `grep 选项 "内容" 文件名`  | 找文件内容 |

对比一下：

```bash
find abc -name 2.txt
```

意思是：

```text
在 abc 目录下，找名字叫 2.txt 的文件
```

而：

```bash
grep "abc" 2.txt
```

意思是：

```text
在 2.txt 文件里面，找内容 abc
```

------

## 五、怎么区分“目录名、文件名、查找内容、选项”？

你可以用这个方法判断：

### 1. 带 `-` 的，多半是选项

例如：

```bash
-name
-n
-w
-r
-type
```

这些都是<span style="color:#FF0000; background:#00FF80;">选项或条件。</span>

比如：

```bash
grep -n "abc" a.txt
```

这里 `-n` 是选项，表示显示行号。

------

### 2. 表示“在哪里找”的，是目录名

例如：

```bash
find abc -name 2.txt
```

这里 `abc` 是目录名，因为它表示：

```text
在 abc 里面找
```

再比如：

```bash
find /home/book/Desktop -name 2.txt
```

这里：

```bash
/home/book/Desktop
```

就是查找范围。

------

### 3. 跟在 `-name` 后面的，是文件名模式

例如：

```bash
find abc -name 2.txt
```

这里：

```bash
2.txt
```

是文件名模式。

再比如：

```bash
find abc -name "*.txt"
```

这里：

```bash
"*.txt"
```

表示查找所有 `.txt` 文件。

------

### 4. <span style="background:#00FF80;">`grep` 里放在选项后面的字符串，是查找内容</span>

例如：

```bash
grep -n "abc" a.txt
```

这里：

```bash
"abc"
```

是查找内容。

```bash
a.txt
```

是被查找的文件。

------

## 六、最容易混的地方

### 1. `find` 的 `2.txt` 是“文件名”

```bash
find abc -name 2.txt
```

这里的 `2.txt` 是文件名，不是文件内容。

意思是：

```text
找名字叫 2.txt 的文件
```

------

### 2. `grep` 的 `"abc"` 是“文件内容”

```bash
grep "abc" 2.txt
```

这里的 `"abc"` 是要在文件里面查找的内容。

意思是：

```text
在 2.txt 文件里面找 abc 这段文字
```

------

## 七、为什么有时候选项放后面也能运行？

比如视频里可能出现过：

```bash
grep "abc" * -n
```

有些 Linux 系统里这样也能运行，因为 GNU 工具比较宽松，会自动识别 `-n` 是选项。

但是学习时建议你写成标准形式：

```bash
grep -n "abc" *
```

也就是：

```bash
命令 选项 查找内容 文件
```

这样最清楚，也最不容易错。

------

## 八、标准写法总结

### `find` 标准写法

```bash
find 目录名 -name 文件名
```

例子：

```bash
find abc -name 2.txt
```

理解成：

```text
在 abc 目录中，找名字叫 2.txt 的文件
```

------

### `grep` 标准写法

```bash
grep 选项 "查找内容" 文件名
```

例子：

```bash
grep -n "abc" 2.txt
```

理解成：

```text
在 2.txt 文件中，查找 abc，并显示行号
```

------

## 九、一句话记忆

你可以这样记：

```text
find：在哪里找 + 按什么文件名找
grep：找什么内容 + 在哪个文件里找
```

更具象一点：

```text
find abc -name 2.txt
= 去 abc 文件夹里，找名字叫 2.txt 的文件

grep -n "abc" 2.txt
= 打开 2.txt 文件，找里面有没有 abc，并显示行号
```

所以：

**`find` 找的是“文件的位置”。**
**`grep` 找的是“文件里面的内容”。**
