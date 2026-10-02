#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>  //修改头文件，同时将自己添加的这一个类文件（类文件就相当于是一个类，只要你创建的
//是一个类文件的话下面类的声明就会给你写好了）所表示的类继承QPushButton这个类。

class MyPushButton : public QPushButton
{
public:
    explicit MyPushButton(QWidget *parent = nullptr);  //自定义类的构造函数
    ~ MyPushButton();   //添加了一个自定义类的析构函数
signals:

};

#endif // MYPUSHBUTTON_H
