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
 在设计界面的一些东西：
  Layouts:用来设置布局的一些选项（如水平布局，垂直布局，栅格布局），也是可以不采用这种方法，而直接采用
          在Containers菜单中选择QWidget菜单项，然后将要布局的控件拖到该QWidget栏中，点击上方的布局
          按钮就可进行相应的布局了。
  Spacers:用来选择一些弹簧控件。
  Buttons：按钮控件，在这里可以选择添加各种各样的按钮。
       <1> Push Button：Push Button添加的按钮可以在上面添加文字，也可以添加图片，但一般不在Push Button
                        按钮上添加图片。
                        添加图片的方式：在QAbstractButton类中找到icon，点击它选择“选择资源”就可以在相应
                        按钮上添加图片了。（这种方式添加的图片适合于按钮中同时有文字和图片的情况）。
       <2> Tool Button:可以添加文字也可以添加图片，但常用Push Button来显示文字，用Tool Button来显示图标。

   利用Tool Button可以搭建类似于QQ好友的界面：
   可以在Tool Button的某一个按钮上显示文字，可以设置文字显示的位置，如在QAbstractButton类中找到icon选项就
   可以往这个按钮上添加某个图片；在QAbstractButton类中找到text选项就可以在这个按钮上显示自己所需要的文字，
   此时按钮还不会显示文字，要想显示此时在QToolButton类中找到toolButtonStyle选项，改变其风格为ToolButton
   Besideicon就可以在按钮中图片的旁边显示自己所写的文字了。选中autoRaise就可以将按钮设置为透明，这样就起到
   类似于QQ好友界面的效果了。

       <3>Radio Button:单选框，单选按钮。
                       当由多个类别，每个类别都有一些单选按钮来供你选择时，此时要将某一个类别的多个单选按钮
                       放在一个组中，此时需要借助Containers中的一个控件GrouBox来进行，将一个类别的多个单
                       选按钮放在一起就可以进行多种类别的选择而不影响其他类别的选择了。
                       将同一类别的单选按钮放在一个Group Box中可以为这个类别起一个名字，如性别、年龄。
                       将同一类别的单选按钮放在一个Group Box中后选择水平布局或则垂直布局可以对这些按钮进行
                       对齐处理。
                       可以利用信号和槽的机制来完成对点击一个按钮触发相应功能的机制。
       <4>Check Box:多选框，多选按钮。（类似于单选按钮，也是要对其设置分组）



 */
