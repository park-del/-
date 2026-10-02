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
Containers（Containers就是容器的意思）中的一些控件：
<1>Scroll Area:对这个框中的东西设置滚动效果。
<2>Tool Box:设置QQ中类似于家人、朋友、黑名单这样的列表，一个列表展开后有多个项。
   对列表设置名字：双击对应的列表，在右下方的QToolBox中找到对应的currentltemText修改其名字就行了。
   在某一个列表的后面或者前面插入页：在右上方的列表中找到QtoolBox后右击就可以看到有插入页的选项了。
   在某一个列表中插入内容：点击某一个列表后该列表下面就会有空白的地方，将相应控件拖入到里面就可以在对应的
                       列表中插入控件了。

<3>Tab Widget:类似于网页的目录，浏览器的目录。（修改同<2>）。
<4>Stacked Widget:栈容器（起到切换页的功能，当为栈增加一页后栈容器的大小就会加1），可以单独设置几个按钮
                  来进行切换页的功能（每一个按钮都能切换到对应的那一页------利用到了信号和槽的机制，需
                  要利用到代码来进行实现，详细的代码见widget.cpp）。
<5>Widget:用来布局。


Input Widgets:
<1>Combo Box:下拉框。（可以利用代码往下拉框中添加内容，详细的代码见widget.cpp）
<2>Font Combo Box:字体的下拉框。
<3>Line Edit:单行输入框。（在QLineEdit找到echoMode后可以进行修改其模式，例如改为密码的模式）
<4>Text Edit:编辑多行文本（可以进行字体颜色，加粗，下划线，倾斜的设置）
<5>Plain Text Edit:编辑多行文本（只是一个纯文本，不能进行上面的字体颜色、加粗等的设置）
<6>Spain Box:数字加减（不带小数）
<7>Double Spin Box:数字加减（带小数）
<8>Time Edit:时间（显示时和分）
<9>Data Edit:日期（显示年月日）
<10>Data/Time Edit:同时显示时间和日期。
<11>Horizontal Scroll Bar与Horizontal Slider:两种不同形式的水平滑动条。
<12>Vertical Scroll Bar与Vertical Slider:两种不同形式的垂直滑动条。

Display Widget:
<1>Label:标签，既可以显示文字也可以显示图片（可以利用代码的方式在标签里显示图片，也可以显示动图，
         详细代码见widget.cpp）。


*/
