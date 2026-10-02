#include "widget.h"
#include <QPushButton>

Widget::Widget(QWidget *parent): QWidget(parent)
{
    this->zt=new Teacher(this); //为this这个对象开辟一个老师zt和学生st
    this->st=new Student(this);

//先进行对象zt的hungry信号与对象st的treat处理进行连接
    void (Teacher:: *teacherSignal1)()=&Teacher::hungry;  //将zt中的hungry()信号与st中的treat()
    void (Student:: *studentSlot1)()=&Student::treat;     //信号进行连接。
    connect(zt,teacherSignal1,st,studentSlot1);

    void (Teacher:: *teacherSignal2)(QString)=&Teacher::hungry; //将zt中的hungry(QString )信号
    void (Student:: *studentSlot2)(QString)=&Student::treat;    //与st中的treat(QString )信号进
    connect(zt,teacherSignal2,st,studentSlot2);                  //行连接。


//<1>classisover();  同时触发两个信号对应的两件事
//<2>点击一个下课的按钮来触发触发下课，即是通过点击一个按钮来触发classisover();(下课这件事又会触发两个信号)
      QPushButton *btn=new QPushButton("下课",this);//设置一个按钮
      resize(600,400);  //并修改窗口的大小
      connect(btn,&QPushButton::clicked,this,&Widget::classisover);//点击按钮，触发classisover();
  /*  点击按钮，按钮将被被点击的
      信号发送给this，this接收到这个信号后就会触发槽函数classisover。（由于classiover()这个函数是属于
      this这个对象的，故被点击信号的接收者应该是this这个对象）
      注意：信号的发送方与信号的接收方均指的是某一个具体的对象，接收对象执行的槽函数其实就是该对象所具有的
      成员函数，该成员函数可以是自己定义的，也可以是从其父类继承过来的。
      接收对象执行的槽函数中如果有类似于emit zt->hungry();这样的语句时可能会触发另一个信号对应的那件事，此时
      这个信号的发送方会将这个信号发送给接收方，接收方接收到后会执行槽函数（接收对象的成员函数）去执行相应的
      功能。
      注意：接收对象执行槽函数时可能会触发一个信号对应的一件事，也可能会触发多个信号对应的多件事。
           信号槽中一般牵涉到了3个对象，即是触发信号a对应的那件事情的对象1，发送信号a的对象2，接收信号a
           的对象3。
  */

//<3>信号连接信号
/*
    QPushButton *btn=new QPushButton("下课",this);//设置一个按钮
    resize(600,400);  //并修改窗口的大小

    void (Teacher:: *teacherSignal)(void)=&Teacher::hungry; //将zt中的hungry(QString )信号
    void (Student:: *studentSlot)(void)=&Student::treat;    //与st中的treat(QString )信号进
    connect(zt,teacherSignal,st,studentSlot);               //行连接。
   //为什么当为有参的信号或者槽函数时不能够执行（见2-5中扩展的内容）

    connect(btn,&QPushButton::clicked,zt,teacherSignal);  //btn被点击的信号连接了zt饿了的信号，
    //当btn被点击时会将被点击的信号发送给zt，zt接收到后会执行teacherSignal所指向的信号，信号本身也是一
    //个函数的，zt执行teacherSignal函数指针所指向的函数hungry(QString )，zt执行hungry(QString )函
    //数这件事会触发hungry(QString )信号，zt发送hungry(QString )信号给st，st接收到后回去执行其对应的
    //槽函数，即就是函数指针studentSlot所指向的treat(QString )函数。
*/

//<1>中是通过执行某一个函数来触发信号
//<2>中是按键被点击的信号直接连接了槽函数，即是信号连接槽
//<3>中是按键被点击的信号直接连接了另一个信号，即是信号连接信号



//断开zt对象和st对象的连接：disconnect(zt,teacherSignal,st,studentSlot)
}

void Widget::classisover() //classisover()执行会触发两个信号
{
    emit zt->hungry();            //关键字emit
    emit zt->hungry("宫保鸡丁");   //关键字emit
}
Widget::~Widget()
{
}



