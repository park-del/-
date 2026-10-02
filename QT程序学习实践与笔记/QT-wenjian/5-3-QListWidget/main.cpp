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
 Item Widgets菜单中的控件:
 <1>List Widget:List Widget相当于是一个文本框，可以在里面写入文本，既可以通过鼠标双击设计界面中的文本框直接
                在里面写入一些文本；也可以通过在程序中利用ui->文本框名字->成员函数的方式来在里面写入一些文本。



 */
