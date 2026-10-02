#include "mylabel.h"
#include <QDebug>
#include <QMouseEvent>

mylabel::mylabel(QWidget *parent) : QLabel(parent)
{
   setMouseTracking(true); //设置鼠标追踪状态,
}
void  mylabel::enterEvent(QEvent *event)  //鼠标进入事件
{
    qDebug()<<"鼠标进入了";
}
void  mylabel::leaveEvent(QEvent *event)  //鼠标离开事件
{
    qDebug()<<"鼠标离开了";
}

/*利用ui界面进行设计时的框默认是没有的(如widget的框、label的框），找到QFrame后点击framename选项就
  点击box就可以为这个框增加边界了，这样就能看到这个框了。

    void  enterEvent(QEvent *event);  //鼠标进入事件
    void  leaveEvent(QEvent *event);  //鼠标离开事件
    以上这两个事件是QT中自带的事件，其中enterEvent()是用来检测鼠标是否进入该函数当前
*/



void mylabel::mousePressEvent(QMouseEvent *ev) //鼠标按下事件
{
/*
   利用.arg(ev->x())可以将某一个函数x()的返回值传给%1。
   按照顺序依次传送，即 .arg(ev->x())传送的是%1
                    .arg(ev->y())传送的是%2
                    .arg(ev->globalX())传送的是%3
                    .arg(ev->globalY())传送的是%4

   利用ev->x(),ev->y()获取的坐标是基于自定义控件的容器的，利用ev->globalX()),arg(ev->globalY()
   获取的坐标是基于窗口的，即是外面这个大窗口。
*/

/*
 默认情况下是左右键按下时都会打印信息，中键按下不会打印信息(中键指的是鼠标滚轮按下时的键)
 可以利用if(ev->buttom()==Qt::LeftButton) { }函数来只打印出左键按下时的信息。
  Qt::LeftButton    鼠标左键按下时的枚举值
  Qt::RightButton   鼠标右键按下时的枚举值
  Qt::MidButton     鼠标中间按下时的枚举值
  ev->button()      鼠标当前那一个按键按下时的枚举值
*/
    if(ev->button()==Qt::LeftButton)
    {
    QString str=QString("鼠标按下了 x =%1 y=%2 globalX=%3 globalY=%4")
            .arg(ev->x()).arg(ev->y()).arg(ev->globalX()).arg(ev->globalY());
    qDebug()<<str;
    }
}

void mylabel::mouseReleaseEvent(QMouseEvent *ev) //鼠标释放事件
{
    if(ev->button()==Qt::LeftButton)
    {
    QString str=QString("鼠标释放了 x =%1 y=%2 globalX=%3 globalY=%4")
                      .arg(ev->x()).arg(ev->y()).arg(ev->globalX()).arg(ev->globalY());
    qDebug()<<str;
    }
}

void mylabel::mouseMoveEvent(QMouseEvent *ev)  //鼠标移动事件
{
  //  if(ev->buttons() & Qt::LeftButton) //设置了鼠标追踪状态时就不能够有这个if语句了，利用鼠标
  //  { 追踪状态可以不点击鼠标左键，只进行鼠标的移动就能打印出信息。
    QString str=QString("鼠标移动了 x =%1 y=%2 globalX=%3 globalY=%4")
                      .arg(ev->x()).arg(ev->y()).arg(ev->globalX()).arg(ev->globalY());
    qDebug()<<str;
  //  }
/*
注意：由于鼠标按下，释放，进入，离开都是一个瞬间的过程，故可以利用  if(ev->button()==Qt::LeftButton)
     { }的方式来区分开；但是鼠标移动却是一个持续的过程，并不是一个瞬间的过程，也就不能用
    if(ev->button()==Qt::LeftButton) { }的方式来区分开左右键，而应该用
    if(ev->buttons() & Qt::LeftButton) { }的方式来区分开左右键。

注意：鼠标移动时用的是“&”,而不是“==”,用“==”对于鼠标移动来说是无法区分鼠标左右键的。
     鼠标移动时是buttons，而不是button，用“button”时也是不行的。
*/

}
