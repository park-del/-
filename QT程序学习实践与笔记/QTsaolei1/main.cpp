#include "mineGame.h"
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
/*

QPixmap:用该类所定义的一个对象就是一个图片，是通过调用这个对象的构造函数来指明其表示的是哪一个图像的，这个对象
        可以指向一个静态图片，也是可以指向一个动态图片的。

在程序完全运行前可以显示一个启动画面。在QT中是使用QSplashScreen类来实现的，该类中定义了一些对程序启动前的界面
的一些设置。
QSplashScreen：用该类定义的一个对象就是程序完全运行前启动的界面，对该对象的操作就是对这个程序运行前启动启动界
               面的操作。（当定义了多个对象时就会有多个启动界面，可以设置启动界面的时间的，一般情况下程序运行
               到主窗口的显示时就要关掉启动界面的显示了，可以在主窗口前增加延时来控制启动界面所显示的时间的）
提示：程序的启动界面可以是一个静态的图片，也可以是一个动态的图片的。（也是可以为程序的启动界面搭配上一段音乐的）
*/


int main(int argc , char** argv)
{
    QApplication app(argc,argv);

    QPixmap pixmap(":images/start2.jpg");   //创建QPixmap对象，设置启动图片
    QSplashScreen splash(pixmap); //创建一个启动界面，即splash，同时为这个启动界面设置pixmap图片

    splash.show ();                         //显示此启动界面

    app.processEvents ();                   /*使程序在显示启动动画的同时能响应鼠标等其他事件,即是在
    程序启动的过程中能执行一些事件（事件也是成员函数，为了区分事件与普通的成员函数，常将事件设置为protected
    ，将普通的成员函数设置为private的）。 */

    mineGame B;
    B.show();
    splash.finish (&B); /*上面是显示B主窗口，此时应该把启动界面的显示给关闭掉了，可以调用splash.finish()
    函数来关闭splash这个启动界面，函数的参数为定义的某个对象的地址，意思就是该启动界面的关闭是根据B这个对象来进
    行的。       */

    return app.exec();
}
