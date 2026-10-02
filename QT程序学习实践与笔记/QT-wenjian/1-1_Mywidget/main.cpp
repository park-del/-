#include "widget.h"
#include <QApplication> //应用程序类的头文件

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); //用QApplication定义的一个对象就是应用程序的对象，在QT中有且仅有1个

    Widget w; //定义了一个自定义的窗口
    w.show(); //调用类widget类中的成员函数show()是显示窗口的
    return a.exec();  //让应用程序对象a进入消息循环，类似于嵌入式当中的while(1)循环不断扫描是否有消息，
    //有的话就利用if语句来完成相应的功能，如点击窗口中的×执行的是if  break;的功能。
    //a.exec（）就是进入一个消息循环，类似于system("pause")，让程序阻塞在这里，也因此窗口才不会一闪而过，
    //而是一直停留在那里。
}
