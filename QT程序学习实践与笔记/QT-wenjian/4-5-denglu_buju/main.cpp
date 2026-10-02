#include "mainwindow.h"
#include <QApplication>

//界面布局

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
//可以在任何两个控件之间加上弹簧，也可以在控件与容器的边界之间加上弹簧，加双弹簧后就可以修改弹簧的尺寸
//从而完成对两个控件之间距离的修改或者一个控价与一个边界之间距离的修改。
