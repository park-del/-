#include "widget.h"
#include "ui_widget.h"
#include <QTimer>  //定时器的类

Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    QTimer *timer1=new QTimer(this);
    timer1->start(1000);  //启动定时器（单位为毫秒，这里是每隔1s定时器time1就会发送一个信号）
    connect(timer1,&QTimer::timeout,[=](){  //每隔1s时定时器time1发送一个timeout的信号
     static int num=1;
     ui->label_1->setText(QString::number(num++));
    });

    QTimer *timer2=new QTimer(this);
    timer2->start(500);  //启动定时器（单位为毫秒，这里是每隔1s定时器time1就会发送一个信号）
    connect(timer2,&QTimer::timeout,[=](){  //每隔1s时定时器time1发送一个timeout的信号
     static int num=1;
     ui->label_2->setText(QString::number(num++));
    });

    connect(ui->btn1,&QPushButton::clicked,[=](){
    timer2->stop();       //实现按钮控制定时器timer2的暂停
    });

    connect(ui->btn2,&QPushButton::clicked,[=](){
    timer2->start(500);   //实现按钮控制定时器timer2的启动
    });


}

Widget::~Widget()
{
    delete ui;
}

