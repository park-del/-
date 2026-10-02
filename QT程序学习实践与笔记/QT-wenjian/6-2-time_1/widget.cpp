#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    id_1= startTimer(1000);       //启动定时器id_1
    id_2=startTimer(2000);        //启动定时器id_2
    id_3=startTimer(3000);
 /*
  startTimer()函数的参数：每隔多少时间会执行一下timerEvent()函数，单位是毫秒，故1000是1s。

  每一个定时器都有一个唯一的id，当启动一个定时器时，即调用startTimer()时就会返回所启动的这个定时器的id，
  可以通过QTimerEvent类所定义的一个对象指针，即*ev来指示当前执行timerEvent()这个函数的定时器，
  ev->timerId()会返回当前执行这个函数的那一个定时器的ID，由此在timerEvent()函数中就可以通过
  if(ev->timeId()==??)来区分不同的定时器了。
  */
}

void Widget::timerEvent(QTimerEvent *ev)
{
    if(ev->timerId()==id_1)
    {
    static int num=1;
    ui->label_1->setText(QString::number(num++));//将num这个数字int转为QString后放入label_1
    //这个标签中。
    }

    if(ev->timerId()==id_2)
    {
    static int num=1;
    ui->label_2->setText(QString::number(num++));//将num这个数字int转为QString后放入label_2
    //这个标签中。
    }

    if(ev->timerId()==id_3)
    {
    static int num=1;
    ui->label_3->setText(QString::number(num++));//将num这个数字int转为QString后放入label_3
    //这个标签中。
    }

}
Widget::~Widget()
{
    delete ui;
}

