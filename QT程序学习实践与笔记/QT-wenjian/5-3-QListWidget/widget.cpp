#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    //链表控件就相当于是一个链表
    QListWidgetItem *item=new QListWidgetItem("锄禾日当午");
    ui->list1->addItem(item); //在这个链表list1的后面插入一个结点item（在这个链表控件list1中插入一行
    //item）。
    item->setTextAlignment(Qt::AlignHCenter); //将这个链表控件中的每一行文字居中对齐。

    QStringList list; //用QStringList定义的一个对象就是一个链表，注意是一个链表，而不是链表中的一个结点。
    list<<"床前明月光"<<"疑是地上霜"<<"举头望明月"<<"低头思故乡";
    /*用QStringList定义的对象保留了左移"<<"运算符的操作，每一个左移运算符都对应了链表中的一个结点，第1个左移
      运算符输入的是该链表list中的第一个结点的数据，第2个左移运算符输入的是该链表list中第二个结点的数据。*/
    ui->list1->addItems(list); //在这个链表list1的后面再插入一个链表list
}

/*
  Item Widgets选项：
  List Widget：链表控件，链表控件就相当于是一个链表，链表控件中的某一行就是链表中的某一个结点。
  用QListWidgetItem定义的一个对象就是链表中的一个结点，也对应了链表控件中的某一行，为这个结点
  输入相应的文字就相当于是为这个链表控件中的某一行输入了相应的文字。
  用QListWidgetItem定义的一个对象就是定义了链表中的一个结点。

  链表控件（链表控件对应有一个ui界面的名字，本题中为item），即item执行相应的成员函数就是就是对这个链表
  所进行的操作，如ui->list1->addItem(item);就是为这个链表中插入一个结点item；如假设已经利用
  QStringList list定义了一个链表，ui->list1->addItem(list)就是在这个链表list1的后面再插入一个链表list。

*/

Widget::~Widget()
{
    delete ui;
}

