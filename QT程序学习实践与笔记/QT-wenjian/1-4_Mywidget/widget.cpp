#include "widget.h"
#include<mypushbutton.h>  //包含了自定义按钮类的头文件
#include<QDebug>

Widget::Widget(QWidget *parent): QWidget(parent)
{
    qDebug()<<"mywidget构造调用";
    MyPushButton *myBtn = new MyPushButton;  //创建了自己的一个按钮
    myBtn->setText("我自己的按钮");
    myBtn->move(200,0);
    myBtn->setParent(this);  //指明这个mypushbutton类的父类为widget这个用户自定义的窗口类
    //已经在mypushbutton这个类中写了构造函数和析构函数调用时的标志文字
}

Widget::~Widget()  //在QT中打印的顺序是与析构的顺序相反的
{
    qDebug()<<"mywidget析构调用";
}

