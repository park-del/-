#include "widget.h"
#include "ui_widget.h"
#include  <QPainter>
Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    //点击移动按钮，实现图片的向右移动
    connect(ui->btn,&QPushButton::clicked,[=](){
    posx+=20;  //对posx加的操作不要放在绘图事件里面，移动的时候会出现一点问题，放在这里就不会出现问题
    update();  // 利用update()函数，意思就是调用绘图事件，同时将新的图像显示在窗口上(原来的那个是被
    //删除了)
    });

}
void Widget::paintEvent(QPaintEvent *)
{

    if(posx>this->width()) posx=0; //如果超出屏幕的话，从0开始
//   if(posx>=100) posx=0;
    QPainter painter(this);
    painter.drawPixmap(posx,100,QPixmap(":/photo/pony.png")); //在(20,10)的位置处画一个路径名
    //为:/photo/pony.png的图片。
}


Widget::~Widget()
{
    delete ui;
}

