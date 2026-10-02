#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>

class mylabel : public QLabel /*自定义控件需要一个类来对其进行描述，同时该类也需要继承一个基类，自定
义控件是在哪个容器里面写的则所其继承的基类就是哪一个，当然容器里面也可以没有控件，即是容器里面为空控件。
本例就是自定义了一个空控件，这个空控件是放在一个标签容器里面的，即是ui界面的label里面的，故这个空控件
继承的基类就是QLabel。
*/
{
    Q_OBJECT
public:
    explicit mylabel(QWidget *parent = nullptr);
    void  enterEvent(QEvent *event);  //鼠标进入事件
    void  leaveEvent(QEvent *event);  //鼠标离开事件

//虚函数的功能只是可以根据基类的指针通过指向不同派生类的对象来调用不同派生类中被声明为虚函数的同名的函数
//在这里是可加可不加的。

    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseReleaseEvent(QMouseEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);

/*
自定义控件中的成员函数是这个自定义控件所提供的对外接口（所谓对外接口就是当触发相应的成员函数时会使该自定义
控件执行相应的功能，即用户可以调用自定义控件的成员函数来完成对自定义控件的一些操作），该对外接口有两种触发
方式，一种是代码的方式，即在程序中通过调用自定义控件的成员函数来操作该控件；另一种是事件的方式，例如鼠标进
入这一个事件(即当鼠标进入到该自定义控件的容器这一事件发生时就会调用相应的成员函数，从而完成相应的功能的，
鼠标进入这一事件发生时调用的成员函数是固定的，即是enterEvent(),用户可以在该函数中写入当该事件触发时需要
完成的操作)，类似的还有鼠标离开事件、鼠标按下事件、鼠标移动事件、鼠标释放事件。

注意：鼠标事件的触发都是针对与一个容器来说的，即是鼠标进入该容器，鼠标离开该容器，鼠标在该容器中按下，鼠标
在该容器中移动，鼠标在该容器中释放。
*/
signals:

};

#endif // MYLABEL_H
