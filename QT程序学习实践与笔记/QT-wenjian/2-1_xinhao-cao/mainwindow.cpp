#include "mainwindow.h"
#include<QPushButton>
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
          /*利用信号和槽的机制完成对按钮功能的设置
            connect(信号的发送者，发送者发送的信号，信号的接收者，接收者执行功能（槽函数）)
            注意:发送者要想发送发送一个信号给接收者，接收者接收到该信号后去执行相应的槽函数(槽函数是
                接受者去执行的，是作用在这个接受者上面的)。
            注意：发送者要想发送某一个信号给接收者的话必须有一个触发事件，该触发事件就是该信号所指示的
                 事件，如信号clicked就是被点击，如果发生了被点击这个事件的话就说明了触发了信号发送的
                 事件，发送至就将这个clicked信号发送给接收者。


            如实现点击一个按钮关闭窗口的功能：程序中有两个实例化的对象，分别是按钮和窗口。
            这个两个对象中有一个是信号的发送者，一个是信号的接收者。
            按钮是信号的发送者，窗口是信号的接收者，发送者发送信号的触发事件为其是否被点击：按钮被点击的话就
            发送该被点击的信号给窗口，窗口接收到按钮发送的被点击信号后会执行槽函数（窗口执行函数就是对这个窗
            口的处理），这个槽函数所进行的功能就是关闭这个窗口。  */

            /*
            利用下面定义btn1按钮来实现点击这个按钮关闭窗口的功能：
            connect(btn1,signal,this,槽函数)

            找出signal的信号（利用帮助文档）
            在QPushbutton没有找到signal，但找到了inherited by QAbstractButton，说明信号signal是从
            其父类继承过来的，应该在其父类QAbstractButton中去找（会找到一个Signals大标题，下面的就是以
            些信号）。
            QAbstractButton中有4个signal（还是可以在其父类中找到其他的信号的）：
            <1>clicked(bool checked = false)    按钮点击
            <2>pressed()                        按下按钮
            <3>released()                       按下按钮的瞬间和释放按钮的瞬间都会触发这个信号
            <4>toggled(bool checked)            切换，按第1次按钮接收到的是一个信号，按第2次按钮接收
                                                到的是另一个信号，依次类推，可以触发两件事（上面的
                                                按钮点击按钮接收到的都是同一个信号，只能触发一件事）

             找出槽函数Slot（利用帮助文档）
             <1>void close()                    关闭窗口
             <2>void hide()                     隐藏窗口
             <3>void lower()

            connect的正确写法：connect(btn1,&QPushButton::clicked，this，)
            第1个参数:signal信号的检测者，signal信号的发送者
            第2个参数：某一个signal信号的地址，必须要指明这个信号是属于哪一个类的(这个类可是该信号所属的类，
                      也可以是该信号所属类的子类)
            第3个参数：signal信号的接收者，也即是执行下一个槽函数的对象，如果是窗口，则执行槽函数的就是窗口
            第4个参数：this对象的槽函数，同样，也必须要指明这个信号是属于哪一个类的(这个类可是该信号所属的类，
                     也可以是该信号所属类的子类)

            信号发送方（按钮）与信号接收方（窗口）是没有任何关联的，是通过connect()来将两者耦合在一起的。
          */
/*
  时刻记住：QWidget、QMainWindow、QDialog这三个类才是基类。像Widget、MainWindow都是用于自定义窗口类
  ，是那三个类中某一个的子类，不改用户自定义窗口类的名字的话自定义窗口类的名字默认就是Widget、MainWindow。

  由于基类Qwidget是另外两个基类QMainWindow、QDialog的父类，又是public继承的，故使用QMainWindow、
  QDialog作为自定义类的基类时在自定义类中是可以直接使用Qwidget中的成员函数或者signal信号或者槽函数的而
  不用包含Qwidget的头文件的，只需包含QMainWindow或者QDialog类的头文件就行了。
  而使用Qwidget作为基类时是在自定义类中很显然也是可以直接使用Qwidget中的成员函数或者signal信号或者
  槽函数的。故无论是三个基类中哪一个，都是可以直接使用基类Qwidget当中的一些东西，故将基类Qwidget当中
  的一些东西当做参考，找什么东西先从这个基类中去找，如找信号signal，找槽函数等。
*/
    QPushButton *btn1 = new QPushButton;  //创建一个按钮
    btn1->show();
    btn1->setParent(this);
    btn1->move(0,0);
    btn1->setText("第一个按钮");
    connect(btn1,&QPushButton::clicked,this,&QWidget::close);

    QPushButton *btn2 = new QPushButton("第二个按钮",this);
    btn2->move(100,100);
    connect(btn2,&QPushButton::clicked,this,&MainWindow::close);

    setWindowTitle("第一个窗口");   //设置窗口的标题，即是设置窗口左上角的文字
    setFixedSize(600,400);   //设置固定窗口的大小（用户不可以利用鼠标进行缩放）


}

MainWindow::~MainWindow()
{
}

