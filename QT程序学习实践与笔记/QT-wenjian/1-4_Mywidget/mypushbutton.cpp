#include "mypushbutton.h"
#include <QDebug>  /*包含了这个头文件时就可以调用qDebug()这个函数利用"qDebug()<<"形式来在程序的编译框输出
自己想要的一些信息了，在这里是用来测试构造函数与析构函数的顺序的。*/
//在mypushbutton.cpp的源文件中写mypushbutton.h中声明的类的成员函数的具体实现
MyPushButton::MyPushButton(QWidget *parent) : QPushButton(parent)
{
  qDebug()<<"我的按钮类(是窗口类Widget的子类)构造调用"; //注意：这里会自动换行，故不用加上endl了。
}

MyPushButton::~MyPushButton()
{
    qDebug()<<"我的按钮类(是窗口类Widget的子类)析构调用";
}
