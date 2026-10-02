#include "widget.h"
#include <QPushButton>

Widget::Widget(QWidget *parent): QWidget(parent)
{
    this->zt=new Teacher(this); //为this这个对象开辟一个老师zt和学生st
    this->st=new Student(this);

    QPushButton *btn=new QPushButton("下课",this);//设置一个按钮
    resize(600,400);  //并修改窗口的大小

    void (Teacher:: *teacherSignal)(void)=&Teacher::hungry; //将zt中的hungry(QString )信号
    void (Student:: *studentSlot)(void)=&Student::treat;    //与st中的treat(QString )信号进
    connect(zt,teacherSignal,st,studentSlot);               //行连接。


    connect(btn,&QPushButton::clicked,zt,teacherSignal);   //一个信号连接了两个槽函数，分别是
    connect(btn,&QPushButton::clicked,this,&QWidget::close);//hungry()和close(),实现了按下
    //下课按钮时既打印请老师吃饭的任务，同时也会关闭窗口。
/*
 信号槽机制扩展：
 1、信号可以连接信号
 2、一个信号可以连接多个槽函数    :如点击下课按钮会执行请老师吃饭，也会执行关闭窗口
 3、多个信号可以连接同一个槽函数  :如多个按钮都可以实现对窗口关闭的任务
 4、信号和槽函数的参数类型必须一一对应
    void (Teacher:: *teacherSignal)(void)=&Teacher::hungry;
    void (Student:: *studentSlot)(void)=&Student::treat;
    即是信号hungry()和槽函数treat()的参数按照从左到右的顺序每一个参数的类型必须一一匹配。
    不能一个是hungry(QString )，一个是treat(void)，这样的话会报错。
 5、信号和槽的参数的个数可以不一样，信号的参数的个数可以多余槽函数的参数个数，但是槽函数的参数个数不能多余信号
   参数个数，且按照从左到右的顺序，槽函数中每一个参数的类型必须与信号中每一个参数的类型一一匹配。
 如 void (Teacher:: *teacherSignal)(QString ，int )=&Teacher::hungry;
    void (Student:: *studentSlot)(QString )=&Student::treat;
    这样是可以的。

 如 void (Teacher:: *teacherSignal)(QString  )=&Teacher::hungry;
    void (Student:: *studentSlot)(QStrin g，int  )=&Student::treat;
    这样是不可以的，因为槽函数中参数的个数不能多余信号参数的个数。

 如 void (Teacher:: *teacherSignal)(QString ,int  )=&Teacher::hungry;
    void (Student:: *studentSlot)(int ,QString )=&Student::treat;
    这样是不可以的，因为槽函数中每一个参数的类型都要与信号中每一个参数的类型一一对应。

  解决疑惑：
    void (Teacher:: *teacherSignal)(void)=&Teacher::hungry;
    connect(btn,&QPushButton::clicked,zt,teacherSignal);
    这样是可以的，因为clicked()是有一个bool类型的参数的，当槽函数hungry为hungry(void)时属于信号的参数
    个数比槽函数的参数个数多，槽函数的参数按照从左到右的顺序与信号的参数的参数类型一一匹配，故可以连接成功。

    void (Teacher:: *teacherSignal)(QString)=&Teacher::hungry;
    connect(btn,&QPushButton::clicked,zt,teacherSignal);
    这样是不可以的，因为clicked()是有一个bool类型的参数的，当槽函数为hungry(QString)时属于信号参数的
    个数与槽函数的参数个数一样多的情况，槽函数的参数按照从左到右的顺序与信号的参数的参数类型不匹配，
    故不可以连接成功。
 */


}

void Widget::classisover() //classisover()执行会触发两个信号
{
    emit zt->hungry();            //关键字emit
    emit zt->hungry("宫保鸡丁");   //关键字emit
}
Widget::~Widget()
{
}



