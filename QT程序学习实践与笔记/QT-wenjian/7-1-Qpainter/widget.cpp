#include "widget.h"
#include "ui_widget.h"
#include <QPainter>  //画家

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this); //定义了一个画家的对象并指明了这个画家是在当前这个widget窗口来进行画画

    painter.drawLine(QPoint(0,0),QPoint(100,100));//从(0,0)到(100,100)点画出一条线
    painter.drawEllipse(QPoint(100,100),50,50);/*在(100,100)处为圆心，rx为50（椭圆的长径），
    ry为50（椭圆的短径）画出一个椭圆（其实画出的就是一个圆）*/
    painter.drawRect(QRect(20,20,100,50));//在(x,y)为(20,20)处画出一个长为100，宽为50的矩形

    painter.drawText(QRect(10,200,200,50),"好好学习，天天向上");/*先在(x,y)为(10,200)处画一个长
    为100，宽为50的矩形框，然后在这个矩形框中写入文字“好好学习，天天向上”。画文字时要先画出一个矩形框，
    然后再在这个矩形框中指明要写的文字。
    调整宽度可以让文字显示在一行，在矩形框输入文字时是从矩形框的开头依次往右写的当走到矩形框的镜头还没有
    写完这个文字时就会换行，在这个矩形框中当前行的下一行来写入文字。*/


    QPen pen(QColor(255,0,0)); //定义了一个红色的画笔
    pen.setWidth(3);           //设置画笔的宽度（数越大就月宽），如果不设置的话1是默认值
    pen.setStyle(Qt::DotLine);//设置画笔的风格为点点点所组成的线
/*
 画笔的风格：    Qt::SolidLine:实线。
               Qt::DotLine:点点点组成的线。
               Qt::DashLine:实线段长度相等的虚线。
其他画笔的风格请查看帮助文档：搜画家类QPainter，搜setStyle。
*/
    painter.setPen(pen);       //让画家使用这个画笔
    painter.drawEllipse(QPoint(200,200),100,50); //画一个椭圆


    QBrush brush(Qt::cyan);//定义一个绿色的画刷（画刷是用来填充颜色的，这就要求了被填充的图形必须是
    //封闭的，如圆，矩形；像线这种不是封闭的图形就不能使用画刷来对其进行操作）
    /*
    在画家QPainter这个类中各种颜色是都有一个枚举值的。如果不想要用
    QColor(0,255,0)来指明颜色的话可以通过枚举值来指明颜色，即是
    QBrush brush(Qt::green);
    在帮助文档中输入Qt::green就能查到各种颜色的枚举值了
   */
    brush.setStyle(Qt::Dense7Pattern); //设置画刷的风格（在帮助文档中输入Qt::BrushStyle就能找到
    //各种画刷所对应的风格了）
    painter.setBrush(brush);//让画家使用这个画刷
    painter.drawEllipse(QPoint(500,500),100,50); //画一个椭圆
}

Widget::~Widget()
{
    delete ui;
}

