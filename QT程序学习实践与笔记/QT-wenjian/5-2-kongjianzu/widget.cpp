#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QDebug>
//注意：在修改ui界面后一定要对其进行编译，否则的话是之前没有修改过的ui界面。
Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);  //对ui所进行的操作一定要放在这个下面。
    ui->boy->setChecked(true); /*设计界面中的每一个控件都对应有一个名字，即是Qobjectname，可以
    通过ui->名字->成员函数来完成对ui设计界面中相应名字的控件的设置，注意是可以修改ui界面的相应控件的名字的。
    这里的操作是对用来选中boy这个单选按钮的。
    */

//单选按钮：实现选中不同的选项触发不同的功能，利用到了信号和槽的机制，如选中女后，打印信息，同时弹出一个对话框
    connect(ui->girl,&QRadioButton::clicked,[=]() //点击girl这个单选按钮触发相应的功能，也即是触发
    {                                             //相应的槽函数。
        QMessageBox::information(this,"info","信息"); //弹出一个信息对话框
        qDebug()<<"选中女了";  //输出选中女了的信息
    });

//多选按钮：信号与槽机制（既有clicked信号，也有stateChanged信号）
    connect(ui->B4,&QCheckBox::stateChanged,[=](int state)//在多选按钮中有一种特殊的按钮点击信号，那
    {//就是stateChanged，该信号，也即是该函数有一个int类型的参数state用来表示当前该多选按钮的状态，可以利用
     //lambel表达式获取该int类型的参数，这个程序是用来测试state的值，当B4被选中时state为2，当B4没有被选中时
     //state为0，每一个多选按钮都可以进行这样，这样当多选按钮只有4个时就可以根据所对应的不同状态的数字的组合
     //（四个数字）来决定要执行什么功能。（由于lambel只是一个函数，可以利用4个connect分别与4个多选按钮进行
     //连接，然后通lambel表达式获得的四个值分别赋值给connect外部的四个变量，这样就可以在connect函数外部
     //利用if语句四个值来决定四个选项的不同组合所对应执行的功能）
        qDebug()<<state;

    });



}

Widget::~Widget()
{
    delete ui;
}

