#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
/*
 QEvent是QT中的事件。
 QEnterEvent：鼠标进入的事件（鼠标进入之后会捕获到的一些事件）

 void QWidget::enterEvent(QEvent *event) //鼠标进入的事件
 void QWidget::leaveEvent()              //鼠标离开的事件
 void QWidget::mouseMoveEvent()          //鼠标移动的事件


  //对控件做一些构造、析构、捕捉时需要用一些自定义的控件。

 */
