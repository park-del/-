# 题目2 实现LRU结构

<span style="color:#FF0000; background:#00FF80; border:1px solid #FF00FF;">测试链接：https://leetcode.cn/problems/insert-delete-getrandom-o1/</span>

这份 Java 代码手动实现了<span style="color:#FF0000; background:#00FFFF; border:1px solid #FF00FF;">双向链表和哈希表</span>的结合，构建了一个标准的 LRU（最近最少使用）缓存。

下面是对应的 C++ 实现。为了保持与原 Java 代码逻辑的一致性，我保留了<span style="color:#FF0000; background:#00FF80;">手动实现双向链表（`DoubleList`）的结构</span>，但在内存管理和语法细节上做了 C++ 特有的调整。

### 1. C++ 代码实现

C++

```C++
#include <iostream>
#include <unordered_map>

using namespace std;

class LRUCache {
private:
    // 定义双向链表节点
    struct DoubleNode {
        int key;
        int val;
        DoubleNode* last; // 对应 Java 的 last (prev)
        DoubleNode* next;

        DoubleNode(int k, int v) : key(k), val(v), last(nullptr), next(nullptr) {}
    };

    // 定义双向链表管理类
    class DoubleList {
    private:
        DoubleNode* head;
        DoubleNode* tail;

    public:
        DoubleList() : head(nullptr), tail(nullptr) {}

        // 在尾部添加节点
        void addNode(DoubleNode* newNode) {
            if (newNode == nullptr) {
                return;
            }
            if (head == nullptr) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                newNode->last = tail;
                tail = newNode;
            }
        }

        // 将已有节点移动到尾部（代表最近使用）
        void moveNodeToTail(DoubleNode* node) {
            if (tail == node) {
                return;
            }
            if (head == node) {
                head = node->next;
                if (head) head->last = nullptr; // C++ 需要判空
            } else {
                node->last->next = node->next;
                node->next->last = node->last;
            }
            node->last = tail;
            node->next = nullptr;
            tail->next = node;
            tail = node;
        }

        // 移除头部节点（最久未使用），并返回该节点以便从 map 中删除
        DoubleNode* removeHead() {
            if (head == nullptr) {
                return nullptr;
            }
            DoubleNode* ans = head;
            if (head == tail) {
                head = nullptr;
                tail = nullptr;
            } else {
                head = ans->next;
                ans->next = nullptr; // 断开连接
                head->last = nullptr;
            }
            return ans;
        }
    };

private:
    unordered_map<int, DoubleNode*> keyNodeMap; // 哈希表
    DoubleList nodeList; // 双向链表
    int capacity; // 容量

public:
    LRUCache(int cap) : capacity(cap) {}

    // 析构函数：释放内存（Java 有 GC，C++ 需要手动管理）
    ~LRUCache() {
        // 这里需要遍历链表释放所有 DoubleNode 的内存，防止内存泄漏
        // 简单起见，实际工程中通常使用智能指针或 std::list
        // 这一步在算法题中常被省略，但在工程中必不可少
    }

    int get(int key) {
        if (keyNodeMap.find(key) != keyNodeMap.end()) {
            DoubleNode* ans = keyNodeMap[key];
            nodeList.moveNodeToTail(ans);
            return ans->val;
        }
        return -1;
    }

    void put(int key, int value) {
        if (keyNodeMap.find(key) != keyNodeMap.end()) {
            // Key 存在，更新值并移到末尾
            DoubleNode* node = keyNodeMap[key];
            node->val = value;
            nodeList.moveNodeToTail(node);
        } else {
            // Key 不存在
            if (keyNodeMap.size() == capacity) {
                // 容量满了，淘汰头部
                DoubleNode* removedNode = nodeList.removeHead();
                keyNodeMap.erase(removedNode->key);
                delete removedNode; // 【关键】C++ 需要手动释放内存
            }
            // 创建新节点
            DoubleNode* newNode = new DoubleNode(key, value);
            keyNodeMap[key] = newNode;
            nodeList.addNode(newNode);
        }
    }
};
```

------

### 2. C++ 知识扩展与核心差异

针对这份代码，从 Java 转到 C++ 时，有几个核心概念必须掌握：

#### ① 内存管理 (Memory Management)

- **Java:** 只要你把 `keyNodeMap.remove(...)` 做了，那个 `Node` 对象就没有引用指向它了，垃圾回收器（GC）会自动回收它的内存。
- **C++:** 没有 GC。当你从链表和 Map 中移除了 `Node`，该对象依然存在于堆内存（Heap）中。如果你不写 `delete removedNode`，这块内存永远不会被释放，这就叫 **内存泄漏 (Memory Leak)**。
  - *代码体现：* 在 `put` 方法的淘汰逻辑中，我增加了 `delete removedNode;`。

#### ② 结构体 vs 类 (`struct` vs `class`)

- **Java:** 只有 `class`。内部类通常用来做节点。
- **C++:** 既有 `struct` 也有 `class`。
  - `struct` 默认成员是 `public` 的，`class` 默认是 `private` 的。
  - 在定义数据结构节点（如 `DoubleNode`）时，C++ 程序员习惯使用 `struct`，因为它是纯数据集合，不需要复杂的封装，代码更简洁。

#### ③ 容器的选择 (`unordered_map` vs `map`)

- **Java:** `HashMap` 是基于哈希表的，$O(1)$ 操作。`TreeMap` 是基于红黑树的，$O(\log N)$ 操作。
- **C++:** * `std::map`: 底层是 **红黑树**，Key 是有序的，查找复杂度 $O(\log N)$。
  - `std::unordered_map`: 底层是 **哈希表**，Key 是无序的，查找复杂度平均 $O(1)$。
  - *本题选择：* LRU 要求 $O(1)$ 的时间复杂度，所以必须使用 `unordered_map`。

#### ④ 指针操作 (`->` vs `.`)

- **Java:** 所有的对象变量其实都是引用（类似指针），访问成员统一用点号 `.`。
- **C++:**
  - 如果变量是对象本身：`obj.member`
  - 如果变量是指针（如 `DoubleNode* node`）：`node->member`
  - `node->next` 等价于 `(*node).next`。

#### ⑤ 进阶：C++ STL 的 "骚操作" (splice)

在 C++ 的实际面试或工程中，如果手写 LRU，往往不需要自己造轮子写 `DoubleList`。我们可以结合 `std::list`（标准双向链表）和 `std::unordered_map`。

`std::list` 有一个神奇的函数叫 **`splice`**，它可以将一个节点从链表的一个位置直接“剪切”并“粘贴”到另一个位置（或者另一个链表），过程中**不需要申请或释放内存**，仅修改指针，效率极高。

**使用 STL 实现 LRU 的简略版：**

C++

```C++
#include <list>
#include <unordered_map>

class LRUCacheSTL {
    int capacity;
    // 链表存 pair<key, value>
    std::list<std::pair<int, int>> lruList; 
    // Map 存 Key -> 链表迭代器 (迭代器类似指针)
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> keyMap;

public:
    int get(int key) {
        if (keyMap.find(key) == keyMap.end()) return -1;
        // 使用 splice 将当前节点直接“瞬移”到链表头部
        lruList.splice(lruList.begin(), lruList, keyMap[key]);
        return keyMap[key]->second;
    }

    void put(int key, int value) {
        if (keyMap.find(key) != keyMap.end()) {
            lruList.splice(lruList.begin(), lruList, keyMap[key]);
            keyMap[key]->second = value;
            return;
        }
        if (keyMap.size() == capacity) {
            int keyToRemove = lruList.back().first;
            lruList.pop_back(); // 移除链表尾部
            keyMap.erase(keyToRemove);
        }
        lruList.push_front({key, value}); // 插入头部
        keyMap[key] = lruList.begin();
    }
};
```

这个版本利用了 C++ STL 的强大特性，代码量只有原来的 1/3，且内存管理由 `std::list` 自动处理。这是 C++ 程序员的高阶写法。

### 3. 下一步建议

**Would you like me to explain exactly how `std::list::splice` works with a diagram, or help you convert another data structure (like LFU) to C++?**



# 写代码困难点（函数接口设计 与 变量命名设计）

# 提问1（函数接口设计）

<span style="color:#FF0000; background:#00FF80; border:1px solid #FF00FF;">我发现有时候写代码不知道从哪里下手不是不会写，而是不知道要定义哪些函数，以及这些函数的接口怎么定义（参数类型和参数个数），这种方式要怎么解决呢，你能够给我举些例子吗。</span>

这是一个非常经典且高阶的问题！这标志着你正在从**“代码实现者”**向**“软件设计者”**转变。

不知道“定义哪些函数”和“接口怎么写”，通常是因为<span style="color:#FF0000; border:1px solid #FF00FF;">**思维方式还停留在“怎么写代码（How）”，而不是“要做什么（What）”**。</span>

解决这个问题的核心心法叫：**“自顶向下，逐步求精”**，或者叫**“老板思维”**。

我为你总结了<span style="color:#FF0000;">三个实用的思维模型</span>，并配合具体的例子带你走一遍。

------

### 方法一：<span style="color:#FF0000;">“老板思维” (Caller-First) —— 先写调用，再写定义</span>

这是最有效的方法。假设你是老板（`main` 函数），你很懒，你不想亲自干活，你只想指挥手下干活。

**操作步骤：**

1. 先写 `main` 函数（或者你的核心入口函数）。
2. <span style="color:#FF0000;">用**自然语言**把流程写在注释里。</span>
3. 把注释翻译成函数调用（这时候你被迫要决定传什么参数、要什么返回值）。
4. 最后再去定义这些函数。

#### 举例：我们要写一个“电商订单处理系统”

**第一步：写注释（讲故事）**

C++

```C++
void processOrder(int orderId) {
    // 1. 根据ID去数据库里查出订单详情
    // 2. 检查库存够不够
    // 3. 如果够，扣减库存
    // 4. 计算打折后的总价
    // 5. 更新订单状态为“已支付”
}
```

第二步：把注释变成函数调用（被迫定义接口）

这时候你就会发现：

- 查订单：我得给它 `orderId`，它得给我返回一个 `Order` 对象。 -> `Order getOrder(int id)`
- 查库存：我得给它商品列表，它告诉我够不够（bool）。 -> `bool checkStock(vector<Item> items)`

C++

```C++
// 此时不需要关心函数内部怎么实现，只关心我要什么
void processOrder(int orderId) {
    // 1. 获取订单
    Order order = getOrderById(orderId); 
    
    // 2. 检查库存
    // 我手里有 order，显然要把 order 传进去
    bool isStockEnough = checkInventory(order); 
    
    if (!isStockEnough) {
        return; // 或者抛出异常
    }
    
    // 3. 扣库存
    decreaseInventory(order);
    
    // 4. 算钱
    // 我需要给它订单，它得还给我一个数字(double)
    double finalPrice = calculateTotalPrice(order);
    
    // 5. 存库
    // 既要更新状态，又要记录价格
    updateOrderStatus(orderId, "PAID", finalPrice);
}
```

第三步：根据上面的调用，写出函数声明

现在，函数名、参数类型、返回值类型，全都被**“逼”**出来了：

C++

```C++
Order getOrderById(int id);
bool checkInventory(const Order& order); // 为了效率，改用引用
void decreaseInventory(const Order& order);
double calculateTotalPrice(const Order& order);
void updateOrderStatus(int id, string status, double price);
```

------

### 方法二：<span style="color:#FF0000;">“输入-处理-输出”模型 (IPO Model)</span>

如果你面对的是<span style="color:#FF0000;">数据处理类的问题（比如算法题、日志分析）</span>，<span style="color:#FF0000; background:#00FF80; border:1px solid #FF00FF;">用这个模型。</span>

核心思想：

<span style="color:#FF0000;">任何函数本质上都是一个转换器</span>。你问自己三个问题：

1. **Input:** 我手里有什么原材料？（参数）
2. **Output:** 我想要得到什么成品？（返回值）
3. **Process:** 中间需要经过什么步骤？（函数名）

#### 举例：统计一篇英文文章中出现频率最高的单词

1. **宏观思考：**
   - 原材料：一个巨大的字符串 `string content`。
   - 成品：一个单词 `string word`。
2. **拆解步骤（定义中间层接口）：**
   - *步骤 A：* 原始字符串里有标点、大小写，没法统计。
     - 定义函数：清洗数据。
     - Input: `string` -> Output: `vector<string>` (一堆干净的单词)。
     - 接口：`vector<string> splitAndClean(const string& text);`
   - *步骤 B：* 拿着一堆单词，要数数。
     - 定义函数：统计频率。
     - Input: `vector<string>` -> Output: `map<string, int>` (单词对次数)。
     - 接口：`unordered_map<string, int> countFrequencies(const vector<string>& words);`
   - *步骤 C：* 拿着 map，找最大的 value。
     - 定义函数：找最大。
     - Input: `map<string, int>` -> Output: `string`。
     - 接口：`string findMostFrequent(const unordered_map<string, int>& counts);`

------

### 方法三：<span style="color:#FF0000;">“单一职责”原则 (Keep It Simple)</span>

当你发现一个函数不知道怎么定义参数时，往往是因为**你想让这个函数干太多事了**。

**判断标准：** 试着给你的函数起名字。

- 如果你的函数名需要用 `And` (比如 `CreateUserAndSendEmailAndLog`)，说明设计错了。
- 把它拆成三个函数：`CreateUser`、`SendEmail`、`Log`。

#### 举例：LRU 缓存（回顾刚才的代码）

如果你一开始想写一个巨大的 `put` 函数：

C++

```C++
// 错误的思维：试图在一个函数里搞定所有逻辑
void put(int key, int value) {
    // 1. 查哈希表有没有
    // 2. 如果有，改值，然后把链表节点断开，移到尾部，重新连接前后
    // 3. 如果没有，判断满没满
    // 4. 如果满了，删头节点，删哈希表
    // 5. 新建节点，挂到尾部，加哈希表
}
```

你会发现逻辑极其混乱，参数怎么传都觉得别扭。

正确的拆解（辅助函数思维）：

你<span style="color:#FF0000;">会发现 put 和 get 都要做一件共同的事：“把节点移到尾部”。</span>

于是你<span style="color:#FF0000;">提取出一个私有函数（Private Helper）：</span>

- **功能：** 仅仅负责链表指针操作，不关心哈希表。
- **参数：** <span style="background:#00FF80; color:#FF0000;">既然是操作节点，就传节点指针。</span>
- **接口：** `void moveNodeToTail(Node* node)`

你会发现 put 如果满了要删头。

于是提取出：

- **接口：** `Node* removeHead()` (返回被删的节点，因为外层可能需要用它的 key 去删哈希表)。

**总结：** 当你觉得逻辑复杂写不下去时，**提取公共步骤为独立的小函数**。

------

### 练习建议：如何从 0 开始？

下次你写代码时，尝试按照这个模板填空：

**场景：** 假设你要写一个“贪吃蛇”游戏。

\1. 定义数据结构 (Data)：

蛇是什么？(deque<Point>)

食物是什么？(Point)

地图多大？(int width, int height)

**2. 伪代码 (The Story)：**

C++

```C++
void gameLoop() {
    while (!gameOver) {
        // 1.以此决定下一步方向
        Direction dir = getInput(); 
        // 2.蛇移动一步
        moveSnake(snake, dir);
        // 3.判断有没有撞墙或撞自己
        if (checkCollision(snake)) break;
        // 4.判断有没有吃到食物
        if (checkEat(snake, food)) {
            // 变长，生成新食物
            growSnake(snake);
            food = generateFood();
        }
        // 5.画出来
        render(snake, food);
    }
}
```

**3. 只有写完上面的伪代码，才去写真正的函数定义：**

- `Direction getInput()`
- `void moveSnake(deque<Point>& snake, Direction dir)`
- `bool checkCollision(const deque<Point>& snake)`
- `...`

一句话总结：

不要还没想好剧本就开始搭舞台（写具体的代码实现）。先当编剧（写调用流程），再当演员（写函数实现）。



# 提问2（变量名设计）

<span style="color:#FF0000; background:#00FF80; border:1px solid #FF00FF;">有时候函数内部写逻辑的时候也是，不是不会写循环，而是不知道该定义哪些变量，以及不知道该如何给这些变量取名称，你可以给我几个常见的变量名字吗，比较通用的哪些，比如函数得到的结果ans，定义的辅助数组名称，定义的遍历指针，各种临时记录下一个的指针或变量，等等。</span>

这又是一个非常经典的痛点！在编程界有一句名言：**“计算机科学中最难的两件事是：缓存失效和给变量命名。”**

你会写循环，但不知道“定义哪些变量”，说明你的**“思维缓存”**不够用了。大脑同时在想算法逻辑和记忆当前状态，内存不够了。

<span style="background:#00FF80; color:#FF0000;">**定义变量的本质是：给内存里的某个状态贴个标签，防止你忘了它。**</span>

我为你总结了一套**算法题/工程代码中最通用的变量命名速查表**，按功能分类。下次卡住时，直接从这里选！

------

### 一、 最终结果 / 返回值 (The Goal)

这是<span style="color:#FF0000;">函数一开始就该定义的，或者最后要返回的</span>。

- **`ans`** (Answer): <span style="background:#00FF80;">最通用，算法题最爱</span>。
  - *场景：* `int ans = 0; ... return ans;`
- **`res`** (Result): <span style="background:#00FF80;">稍微正式一点，工程中常用</span>。
  - *场景：* `vector<int> res;`
- **`ret`** (Return): <span style="background:#00FF80;">通常用于表示状态码或立即要返回的值</span>。
- **`dummy`** (Dummy Node): **链表神器**。
  - *场景：* 虚拟头节点，`ListNode* dummy = new ListNode(-1);`

------

### 二、 遍历与索引 (The Loopers)

控制循环进度的变量。

- **`i`, `j`, `k`**: 多层循环的标准标号。不要用 `a, b, c`。
- **`idx`** (Index): <span style="color:#FF0000;">当 `i` 已经被占用，或者你想强调这是一个“下标”时</span>。
  - *场景：* `int curIdx = 0;`
- **`cnt`** (Count): **计数器**。<span style="color:#FF0000;">统计个数用</span>。
  - *场景：* `int cnt = 0; if (valid) cnt++;`
- **`len` / `n` / `m`**: <span style="color:#FF0000;">长度或大小</span>。
  - *场景：* `int n = nums.size();`
- **`sz`** (Size): <span style="color:#FF0000;">`n` 的替代品，通常指容器大小</span>。

------

### 三、 指针与位置 (The Pointers)

这是最容易乱的，特别是双指针或链表。

#### 1. 数组/双指针

- **`l`, `r`** (Left, Right): <span style="color:#FF0000;">左右边界</span>。
  - *场景：* 二分查找，滑动窗口。
- **`low`, `high`**: <span style="color:#FF0000;">同上，二分查找常用</span>。
- **`start`, `end`**: <span style="color:#FF0000;">区间操作常用</span>。

#### 2. 链表/遍历

- **`cur`** / **`curr`** (Current): <span style="color:#FF0000;">**当前指针**。最常用的名字</span>！
  - *场景：* `ListNode* cur = head;`
- **`prev`** / **`pre`** (Previous): **<span style="color:#FF0000;">前驱节点</span>**。链表反转必用。
- **`next`** / **`nxt`** (Next): <span style="color:#FF0000;">**后继节点**</span>。
  - *技巧：* 在 C++ 中 `next` 有时会和标准库冲突，建议用 `nxt` 或 `tempNext`。
- **`fast`, `slow`**: <span style="color:#FF0000;">快慢指针</span>。判圈算法必用。

------

### 四、 状态与辅助 (Helpers)

记录过程中的临时状态。

- **`tmp`** / **`temp`**: **<span style="color:#FF0000;">临时变量</span>**。交换两个数时必用。
  - *场景：* `int temp = a; a = b; b = temp;`
- **`flag`**: **<span style="color:#FF0000;">标志位</span>** (bool)。
  - *场景：* `bool flag = false;` (找到了改为 true)。
- **`vis`** (Visited): **访问数组**。DFS/BFS 必用。
  - *场景：* `vector<bool> vis(n, false);`
- **`dp`** / **`memo`**: <span style="color:#FF0000;">动态规划或记忆化搜索的数组</span>。
- **`buf`** (Buffer): 缓冲区。网络编程或处理字符串常用。

------

### 五、 极值与比较 (The Extremes)

- **`mx`** / **`maxVal`**: 记录<span style="color:#FF0000;">当前最大值</span>。
- **`mn`** / **`minVal`**: 记录<span style="color:#FF0000;">当前最小值</span>。
- **`target`** / **`key`**: 你<span style="color:#FF0000;">要查找的目标值</span>。
- **`sum`**: 求和。
- **`diff`**: <span style="color:#FF0000;">差值 (Difference)</span>。

------

### 💡 实战演示：如何把这些词串起来？

假设我们要写一个 “反转链表” 的函数。

如果你不知道该定义啥，脑子里先过一遍上面的表：

1. **我要干嘛？** -> 遍历链表 -> 需要 **`cur`** (当前节点)。
2. **反转需要啥？** -> 把箭头指回头 -> 需要 **`prev`** (前一个节点)。
3. **断链了咋办？** -> 需要记一下后面那个 -> 需要 **`nxt`** (临时存下一个)。

于是代码自然就流出来了：

C++

```C++
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr; // 前驱
    ListNode* cur = head;     // 当前
    
    while (cur != nullptr) {
        ListNode* nxt = cur->next; // 【临时】记下后面是谁
        
        cur->next = prev;     // 【反转】当前指向前一个
        
        prev = cur;           // 【推进】prev 往前走
        cur = nxt;            // 【推进】cur 往前走
    }
    return prev; // 返回新的头
}
```

### 🧠 心法总结

当你写逻辑卡住时，按照这个顺序问自己：

1. **我正在处理谁？** $\rightarrow$ 定义 `cur` 或 `i`。
2. **我要在这个位置记录什么？** $\rightarrow$ 定义 `ans`, `sum`, `cnt`。
3. **如果是链表/图，我下一步去哪？** $\rightarrow$ 定义 `nxt`。
4. **我是不是需要回头看？** $\rightarrow$ 定义 `prev`。
5. **我是不是需要两个东西做比较？** $\rightarrow$ 定义 `mx`, `mn`。

把这些常用词背下来，就像打游戏记住了快捷键，写代码时手速会快很多！



# 代码结构分析（重点）

<span style="color:#FF0000; background:#00FF80; border:1px solid #FF00FF; font-size:1.2em;">我的这个C++代码的结构好像是这种的，就是数据结构 LRUCache（本质上一个类），这个类里面好像用private又单独声明了一个结构体 struct DoubleNode，单独声明了一个类 class DoubleList，紧接着又用一个private 定义了一个 unordered_map keyNodeMap 和 DoubleList nodeList，这种好像是声明和定义都是在一个类中实现的呢，这种实现方式对我有没有什么启发呢？</span>



你观察得非常敏锐！你发现的这种结构在 C++（以及 Java）设计模式中非常经典，被称为 <span style="color:#FF0000; border:1px solid #FF00FF;">**“嵌套类” (Nested Class)** 或 **“内部类” (Inner Class)**。</span>

这种“<span style="color:#FF0000;">在类里面定义类/结构体</span>”的写法，绝对不是随意的，它蕴含了 **软件工程中极具价值的三个设计哲学**。这对你以后写复杂的系统代码有极大的启发。

以下为你深度拆解这种写法的“三大启发”：

### 启发一：<span style="color:#FF0000; background:#00FF80;">极致的封装 —— “对外隐藏实现细节”</span>

这是最核心的原因。

* **场景：** 作为一个 `LRUCache` 的使用者（比如 `main` 函数里的调用者），我只关心两件事：
1. `put(key, value)`：存数据。
2. `get(key)`：取数据。


* **并不关心：** 我<span style="color:#FF0000;">完全不关心你内部是用双向链表、单向链表，还是数组实现的。我也不想知道 `DoubleNode` 长什么样。</span>
* **启发：**
* <span style="color:#FF0000;">将 `DoubleNode` 和 `DoubleList` 定义在 `LRUCache` 的 `private` 区域，就是告诉外界：**“这是我的私有零件，你们谁都别碰，也别想看见。”**</span>
* 这样做非常**安全**。<span style="color:#FF0000;">外部代码无法创建一个 `DoubleNode`，也无法依赖你的底层实现。以后如果你想把双向链表换成别的结构，外部代码一行都不用改。</span>

### 启发二：<span style="background:#00FF80; color:#FF0000;">防止命名污染 —— “重名的艺术”</span>

* **问题：** `Node` 是一个非常烂大街的名字。
* 如果你的项目很大，有一个 `Graph`（图）类，它需要 `Node`。
* 有一个 `LinkedList`（链表）类，它也需要 `Node`。
* 有一个 `Tree`（树）类，它还需要 `Node`。
* <span style="color:#FF0000;">如果你把它们都定义在全局（类外面），编译器直接报错：`redefinition of 'Node'`（重定义）。</span>


* **解决：**
* <span style="color:#FF0000; background:#00FF80;">把 `Node` 定义在 `LRUCache` 内部，它的全名实际上变成了 **`LRUCache::DoubleNode`**。</span>
* 这<span style="color:#FF0000; background:#00FF80;">就有了 **“命名空间 (Namespace)”** 的隔离保护。你可以在 `Graph` 里再定义一个 `Graph::Node`，两者互不干扰。</span>


* **启发：**
* **“谁用的东西，谁自己拿着。”** <span style="border:1px solid #FF00FF; color:#FF0000;">如果一个结构体只为某个类服务，就把它塞到这个类肚子里面去，不要扔在外面污染环境。</span>



### 启发三：<span style="color:#FF0000; background:#00FF80;">模块化思维 —— “分而治之”</span>

这是代码逻辑层面的启发。

* **观察：** 你看 `LRUCache` 的 `put` 方法，逻辑其实挺复杂的（涉及哈希表操作 + 链表操作）。
* **做法：** 作者<span style="color:#FF0000;">没有把所有链表操作（断开指针、重连指针）都堆在 `put` 函数里，而是提取了一个 `DoubleList` 类，专门负责“脏活累活”（指针操作）。</span>
* `DoubleList` 负责：<span style="background:#00FF80;">物理层面的指针移动 (`addNode`, `removeHead`)。</span>
* `LRUCache` 负责：<span style="background:#00FF80;">业务层面的逻辑 (`get`, `put`，何时淘汰，何时更新)。</span>


* **启发：**
* 哪怕是在一个类内部，也要学会 **“层级划分”**。<span style="color:#FF0000;">不要让核心业务逻辑充斥着底层的</span> `node->next = prev` 这种细节。
* **Helper Class（辅助类）：** `DoubleList` 就是 `LRUCache` 的专属小秘书。



---

### 对比总结：两种写法的区别

为了让你看清区别，我画个简单的对比表：

| 特性 | **写法 A：定义在外面 (Global)** | **写法 B：定义在里面 (Nested) —— 你的代码** |
| ---- | ------------------------------- | ------------------------------------------- |
|      | **代码结构**                    | `struct Node {};`<br>                       |

<br>`class LRU { Node* n; };` | `class LRU {`<br>

<br>`  struct Node {};`<br>

<br>`  Node* n;`<br>

<br>`};` |
| **可见性** | **全局可见**。谁都能用 `Node`，不安全。 | **私有可见**。只有 `LRU` 能用，外界看不见。 |
| **命名冲突** | **高风险**。容易和其他 `Node` 冲突。 | **零风险**。自带 `LRU::` 前缀。 |
| **适用场景** | 当 `Node` 需要被多个不同的类共享时。 | 当 `Node` **仅仅** 为 `LRU` 服务时（绝大多数情况）。 |

### 针对 C++ 的一个特殊补充 (和 Java 的不同)

既然你学过 Java，这点**必须**注意：

* **Java 的内部类：** 默认持有外部类对象的引用（也就是内部类能直接访问外部类的 `capacity` 或 `keyNodeMap`）。
* **C++ 的嵌套类：** **完全独立！**
* C++ 中，`DoubleList` 虽然定义在 `LRUCache` 里面，但它只是“寄宿”在这里。
* `DoubleList` 的函数 **不能直接访问** `LRUCache` 的成员变量（除非你把 `LRUCache` 的指针传给它）。
* C++ 的嵌套主要起到了 **“作用域控制” (Scope Control)** 的作用，而不是逻辑绑定的作用。



**总结你的启发：**
下次当你写一个复杂的类（比如 `MyClass`），发现需要用到一个辅助结构（比如 `Item` 或 `Record`），先问自己：**“这个辅助结构别的类会用到吗？”**
如果不会，**请毫不犹豫地把它定义在 `MyClass` 的 `private` 区域里！** 这就是专业程序员的“代码洁癖”。



# 自己书写代码

```C++
class LRUCache {
private:
    struct DoubleNode{
        int val;
        DoubleNode* last;
        DoubleNode* next;
        DoubleNode(int x):val(x),last(nullptr),next(nullptr);
    };

    class DoubleList{

        private:
            DoubleNode* head;
            DoubelNode* tail;
        poublic:
            void moveNodeToTail(DoubleNode* node){
                if(node == tail)
                    return;
                
                if((node == head)){
                    head = head->next;
                    head->last = nullptr;

                    node->last = tail;
                    node->next = nullptr;
                    tail = node; 
                } else {

                    node->last->next = node->next;
                    node->next->last = node->last;

                    node->last = tail;
                    tail->next = node;
                    node->next = nullptr;
                    
                    tail = node;
                }
            }

            void removeHead(DoubleNode* node){
                head = head->next;
                head->last = nullptr;

                node->last = tail;
                node->next = nullptr;
                tail=node;
            }
    }

private:
    unordered_map<int,DoubleNode*> nodeKeyMap;
    int capacity;

public:
    LRUCache(int capacity) {
        this->capacity = capacity;
    }
    
    int get(int key) {
        //nodekeyMap[key] = new;
        if(nodeKeyMap.find(key)==nodeKeyMap.end())
            return -1;

        DoubleNode *node = nodeKeyMap[key];
        moveNodeToTail(node);
        
        return node->val;
    }
    
    void put(int key, int value) {

        if(nodeKeyMap.count()==capacity){
            nodekeyMap[key] = new DoubleNode(val);
            removeHead(nodeKeyMap[key]);
        }

        nodekeyMap[key] = new DoubleNode(val);
        moveNodeToTail(nodeKeyMap[key]); 
    }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */
```

# 代码错误分析

这份代码有几个**核心逻辑错误**、**语法错误**以及**内存管理问题**。它目前无法通过编译，逻辑也是混乱的。

别担心，从 Java 转 C++ 写链表操作确实容易出错。我来帮你详细拆解问题，并提供修正后的代码。

### ❌ 主要错误分析

#### 1. 结构体定义与语法错误

- **构造函数写法不对：** `DoubleNode(int x):val(x),last(nullptr),next(nullptr);` 这里不能加分号，而且没有函数体 `{}`。
- **缺少 `key`：** `DoubleNode` 必须同时存 `key` 和 `val`。
  - *为什么？* 当缓存满了需要删除链表头部节点（最久未使用的）时，你需要通过这个节点的 `key` 去哈希表 `nodeKeyMap` 里把对应的记录也删掉。如果节点里没有 `key`，你就不知道该删哈希表里的谁。
- **拼写错误：** `DoubelNode` -> `DoubleNode`, `poublic` -> `public`。

#### 2. `DoubleList` 的逻辑混乱

- **并没有实例化：** 你在 `LRUCache` 里定义了 `class DoubleList`，但是**没有在 `LRUCache` 的成员变量里创建一个 `DoubleList` 的对象**（比如 `DoubleList cacheList;`）。
  - *后果：* 你在 `get` 和 `put` 里直接调用 `moveNodeToTail`，编译器会报错，因为这些是 `DoubleList` 的成员函数，不是 `LRUCache` 的。
- **`moveNodeToTail` 指针操作有误：** 你的逻辑里，头部处理完了，但是没有把旧的 tail 和新移过来的 node 连接起来（缺少 `tail->next = node`）。
- **`removeHead` 逻辑完全错了：**
  - 你的 `removeHead` 接收了一个 `DoubleNode* node` 参数。实际上，移除头部不需要参数，它应该直接移除 `head` 指向的节点。
  - 你的代码里写的是把 node 移到尾部的逻辑，而不是删除头部。

#### 3. `LRUCache` 的逻辑漏洞

- **`count()` 用法错误：** `nodeKeyMap.count()` 需要传入参数。你想获取当前大小，应该用 `nodeKeyMap.size()`。
- **`put` 方法逻辑错误：**
  - **更新 vs 插入：** 你没有判断 key 是否已经存在。如果 key 存在，应该更新 value 并移动到尾部；如果不存在，才判断容量。
  - **淘汰逻辑：** `removeHead(nodeKeyMap[key])` 写法是错的。应该是先从链表删掉头节点，拿到被删节点的 key，再去 map 里删，最后释放内存。
- **内存泄漏：** C++ 没有垃圾回收。`new` 出来的节点，在删除时必须 `delete`，否则会内存泄漏。

------

### ✅ 修正后的 C++ 代码

我尽量保留了你的类结构，修复了上述所有问题，并加上了注释。

C++

```C++
#include <unordered_map>

using namespace std;

class LRUCache {
private:
    // 1. 节点结构体：必须存 key，方便反向查 map 删除
    struct DoubleNode {
        int key;
        int val;
        DoubleNode* last;
        DoubleNode* next;
        // 构造函数修复
        DoubleNode(int k, int v) : key(k), val(v), last(nullptr), next(nullptr) {}
    };

    // 2. 双向链表类
    class DoubleList {
    private:
        DoubleNode* head;
        DoubleNode* tail;

    public:
        DoubleList() : head(nullptr), tail(nullptr) {}

        // 新增节点到尾部
        void addNode(DoubleNode* node) {
            if (node == nullptr) return;
            if (head == nullptr) {
                head = node;
                tail = node;
            } else {
                tail->next = node;
                node->last = tail;
                tail = node;
            }
        }

        // 移动已有节点到尾部
        void moveNodeToTail(DoubleNode* node) {
            if (tail == node) return; // 已经是尾部，不用动

            if (head == node) {
                // 如果是头节点
                head = head->next;
                head->last = nullptr;
            } else {
                // 如果是中间节点，断开前后
                node->last->next = node->next;
                node->next->last = node->last;
            }

            // 放到尾部
            node->last = tail;
            node->next = nullptr;
            tail->next = node;
            tail = node;
        }

        // 删除头节点（最久未使用），并返回它以便外部 delete
        DoubleNode* removeHead() {
            if (head == nullptr) return nullptr;
            
            DoubleNode* res = head;
            if (head == tail) {
                head = nullptr;
                tail = nullptr;
            } else {
                head = head->next;
                head->last = nullptr;
                res->next = nullptr;
            }
            return res;
        }
    };

private:
    unordered_map<int, DoubleNode*> nodeKeyMap;
    // 【重要】这里必须实例化一个链表对象
    DoubleList cacheList; 
    int capacity;

public:
    LRUCache(int capacity) {
        this->capacity = capacity;
    }
    
    int get(int key) {
        if (nodeKeyMap.find(key) == nodeKeyMap.end()) {
            return -1;
        }

        DoubleNode* node = nodeKeyMap[key];
        // 调用 list 对象的成员函数
        cacheList.moveNodeToTail(node);
        return node->val;
    }
    
    void put(int key, int value) {
        // 情况1：Key 已经存在 -> 更新 value，移到尾部
        if (nodeKeyMap.find(key) != nodeKeyMap.end()) {
            DoubleNode* node = nodeKeyMap[key];
            node->val = value;
            cacheList.moveNodeToTail(node);
        } 
        // 情况2：Key 不存在 -> 新增
        else {
            // 如果满了，先淘汰
            if (nodeKeyMap.size() == capacity) {
                // 1. 从链表移除头
                DoubleNode* oldHead = cacheList.removeHead();
                // 2. 从 Map 移除 (这就为什么 Node 里要有 key)
                nodeKeyMap.erase(oldHead->key);
                // 3. 【C++特有】释放内存
                delete oldHead;
            }

            // 创建新节点
            DoubleNode* newNode = new DoubleNode(key, value);
            // 加入链表和 Map
            cacheList.addNode(newNode);
            nodeKeyMap[key] = newNode;
        }
    }
};
```

### 🧠 关键点总结 (C++ vs Java)

1. **对象实例化：** Java 里 `private DoubleList list;` 只是一个引用，必须在构造函数里 `list = new DoubleList()`。C++ 里 `DoubleList list;` 在类初始化时就直接创建了对象，可以直接用 `list.func()`。
2. **Size 的获取：** Map 的大小是 `.size()`，不是 `.count()`（count 是查某个 key 存不存在）。
3. **内存管理：**
   - Java: `map.remove(key)` 后，GC 会自动回收那个 Node。
   - C++: `map.erase(key)` 只是从 map 里删除了指针。**Node 占用的堆内存还在**。你必须显式调用 `delete oldNode`，否则程序跑久了内存就爆了。

你可以把修正后的代码复制去跑一下，结合注释理解一下 C++ 指针操作的逻辑。





