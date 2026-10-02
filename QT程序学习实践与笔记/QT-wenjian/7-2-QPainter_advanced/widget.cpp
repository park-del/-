#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    /*
    QPainter painter(this);
    painter.drawEllipse(QPoint(100,50),50,50);

    painter.drawEllipse(QPoint(200,50),50,50);
    painter.drawEllipse(QPoint(100,100),50,50);
    在构造函数里面调用的这些绘图的成员函数无法执行，故有关绘图的成员函数一定要在绘图事件所对应的那一个函数
    里面写时才会有效。
   */


}
void Widget::paintEvent(QPaintEvent *) //注意：绘图一定要在绘图事件这个函数里面写
{
    QPainter painter(this);
    painter.drawEllipse(QPoint(100,50),50,50);
    painter.setRenderHint(QPainter::Antialiasing); //设置抗锯齿能力，画家画画的效率变低，就是让画
    //家好好画这个圆，是画家接下来所画的这个圆的毛边更少一些。
    painter.drawEllipse(QPoint(200,50),50,50);

    painter.setRenderHint(QPainter::HighQualityAntialiasing);//更高精度的抗锯齿能力，画家接下来
    //所画的这个圆将会更好。
    painter.drawEllipse(QPoint(300,50),50,50);

    /*
    painter.drawRect(QRect(20,20,50,50)); //画家从(0,0)处开始画一个矩形
    painter.translate(100,0);             //改变画家的位置，画家此时从(100,0)开始画下一个矩形
    painter.drawRect(QRect(20,20,50,50));
    painter.translate(100,0);             //改变画家的位置，画家此时从(200,0)开始画下一个矩形
    painter.drawRect(QRect(20,20,50,50));
    */

    painter.drawRect(QRect(20,20,50,50)); //画家从(0,0)处开始画一个矩形
    painter.translate(100,0);             //改变画家的位置，画家此时从(100,0)开始画下一个矩形
    painter.save();                       //保存画家此时的状态（即保存画家此时(100,0)的这一个位置）
    painter.drawRect(QRect(20,20,50,50));
    painter.translate(100,0);             //改变画家的位置，画家此时从(200,0)开始画下一个矩形
    painter.restore();                    //还原画家被保存的状态，画家此时从(100,0)开始画下一个矩形
    painter.drawRect(QRect(20,20,50,50));


}
Widget::~Widget()
{
    delete ui;
}

