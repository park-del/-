#include "widget.h"
Widget::Widget(QWidget *parent): QWidget(parent)
{
    this->zt=new Teacher(this);
    this->st=new Student(this);

//    connect(zt,&Teacher::hungry,st,&Student::treat) 注释掉的这段程序时无法执行的
//    classisover();

    void (Teacher:: *teacherSignal0)()=&Teacher::hungry;
    void (Student:: *studentSlot0)()=&Student::treat;
    connect(zt,teacherSignal0,st,studentSlot0);
    classisover();


    //连接带参数的 信号和槽
    //指针->地址
    //函数指针->函数地址
/*当牵涉到信号或者槽函数的重载时，由于信号与槽都是一个函数，故可以利用一个函数指针来指明发送者发送的是哪一个
  信号，用一个函数指针来指明接收者执行的是哪个槽函数。

 void (Teacher:: *teacherSignal)(QString)=&Teacher::hungry;
 定义了一个指向hungry(QString )的信号函数指针teacherSignal，由于该函数指针在定义时已经指明了其所属类为
 Teacher，故在利用connect进行连接时不需要再指明其属于哪一个类了。
 其中(QString)代表了对应形参为(QString )的函数。
 其中&Teacher::hungry表示该指针所指向的函数的函数名为hungry。

 void (Student:: *studentSlot)(QString)=&Student::treat;
*/
    void (Teacher:: *teacherSignal)(QString)=&Teacher::hungry;
    void (Student:: *studentSlot)(QString)=&Student::treat;
    connect(zt,teacherSignal,st,studentSlot);
    classisover();
}

void Widget::classisover() //classisover()执行会触发两个信号
{
    emit zt->hungry();            //关键字emit
    emit zt->hungry("宫保鸡丁");   //关键字emit
}
Widget::~Widget()
{
}



