#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
//首先在界面中找到Item Widgets中找到QTreeWidgetS树控件，点击界面进行水平布局或者垂直布局可以使树控件与
//我们的界面一样大。

    //treeWidget树控件使用

    ui->tree->setHeaderLabels(QStringList()<<"英雄"<<"英雄介绍");//设置水平头
/*
树控件刚开始时是有一个水平头的，就是树控件最顶端的那一个框框。
在程序中通过ui对树控件的名字所进行的操作就是对树控件所进行的操作。

如ui->tree->setHeaderLabels()就是用来设置树控件的水平头的，函数setHeaderLabels()的形参为
QStringList类型的一个对象，可以利用C++中运算符里面的临时对象，即调用QStringList的构造函数就相当于
一个对象了，如QStringList()就相当于是一个对象，对象所能够进行的任何操作其也是可以运行的。

QListWidgetItem所定义的对象对应了链表中的一个结点（链表控件），用QListWidgetItem定义了多少对象对应的链表
控件中就会有多少个结点，链表控件中的每一个结点（链表控件就相当于是一个链表，每一个结点就是链表控件中的一行），
即QListWidget定义的一个对象就是链表控件中的一行。
QTreeWidgetItem所定义的对象对应了树中的一个结点（树控件），用QTreeWidgetItem定义了多少对象该树控件tree中
就会有多少个结点，树控件中的每一个结点（树控件就相当于是一个森林，包含有根结点和普通的分支结点），即
QTreeWidgetItem定义的一个对象就是树控件中的一行。
可以在用QTreeWidgetItem定义一个对象时（定义树中的一个结点时）通过调用相应的构造函数来为这个结点初始化，
QTreeWidgetItem构造函数的形参是为一个QStringList类型的对象的，可以采用C++中临时对象的方式。
注意:用QTreeWidgetItem定义的一个对象是树中的一个普通结点，调用addTopLevelItem()函数是为这个树控件中插入
    一个根结点的;假设已经利用QTreeWidgetIrem定义了树中的一个结点l1，根结点->addChild(l1),即是根结点调用
     addChild(l1)函数是将l1这个结点成为到根结点的孩子结点的。

     树中的根结点和分支结点都是QTreeWidgetItem定义的对象，用QTreeWidgetItem定义的对象是树控件中的根结点
     还是分支结点取决于所调用的函数，这个树控件是插入根结点的，故是树控件(这个树控件所对应的在ui界面中的名字)
     调用相应的成员函数来插入根结点的；是根结点来插入其所需要的分支结点的，故是根结点调用相应的成员函数来插入
     其所需要的分支结点的（一个分支结点调用相应的成员函数也是可以接着再往下插入结点的，依次类推，如此就能构成
     一种分层的结构了）。
*/
    QTreeWidgetItem *liItem=new QTreeWidgetItem(QStringList()<<"力量"); //定义了树中的一个普通结点
    //为这个临时对象赋好了值后才调用构造函数的。
    QTreeWidgetItem *minItem=new QTreeWidgetItem(QStringList()<<"敏捷");//定义了树中的一个普通结点
    QTreeWidgetItem *zhiItem=new QTreeWidgetItem(QStringList()<<"智力");//定义了树中的一个普通结点

    //加载顶层的结点（顶层的结点就是根结点，力量、敏捷、智力都是一个根结点，根结点下面有支点）
    ui->tree->addTopLevelItem(liItem); //为树控件tree添加一个根结点liItem
    ui->tree->addTopLevelItem(minItem);//为树控件tree添加一个根结点minItem
    ui->tree->addTopLevelItem(zhiItem);//为树控件tree添加一个根结点zhiItem

    //追加子节点（子结点也就是树中的分支结点）
    QStringList heroL1; //定义了一个分支结点
    heroL1<<"刚被猪"<<"前排坦克，能在吸收伤害的同时造成可观的范围输出"; //为这个分支结点赋值
    QTreeWidgetItem *l1=new QTreeWidgetItem(heroL1); //将这个分支结点变成树中的一个结点
    liItem->addChild(l1);  //将这个结点成为根结点liItem的孩子结点

    QStringList heroL2;
    heroL2<<"船长"<<"前排坦克，能肉能输出能控场的全能英雄";
    QTreeWidgetItem *l2=new QTreeWidgetItem(heroL2);
    liItem->addChild(l2);  //将这个结点成为根结点liItem的孩子结点

    QStringList heroL3;
    heroL3<<"月骑"<<"中排物理输出，可以使用分裂利刃攻击多个目标";
    QTreeWidgetItem *l3=new QTreeWidgetItem(heroL3);
    minItem->addChild(l3);

    QStringList heroL4;
    heroL4<<"小鱼人"<<"前排战士"<<"擅长偷取敌人的属性来增强自身战力";
    QTreeWidgetItem *l4=new QTreeWidgetItem(heroL4);
    minItem->addChild(l4);

    QStringList heroL5;
    heroL5<<"死灵法师"<<"前排法师坦克，魔抗性较高，拥有治疗技能";
    QTreeWidgetItem *l5=new QTreeWidgetItem(heroL5);
    zhiItem->addChild(l5);

    QStringList heroL6;
    heroL6<<"巫医"<<"后排辅助法师"<<"可以使用奇特的巫术诅咒敌人与治疗队友";
    QTreeWidgetItem *l6=new QTreeWidgetItem(heroL6);
    zhiItem->addChild(l6);


    //为子节点再插入一个子节点
    QStringList heroL7;   //为l6这个分支结点再插入一个分支结点了l7
    heroL7<<"后裔"<<"后排输出射手"<<"可以使用弓箭来对敌方英雄造成伤害";
    QTreeWidgetItem *l7=new QTreeWidgetItem(heroL7);
    l6->addChild(l7);
}

Widget::~Widget()
{
    delete ui;
}

