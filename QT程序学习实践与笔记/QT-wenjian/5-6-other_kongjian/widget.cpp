#include "widget.h"
#include "ui_widget.h"
#include <QMovie>

Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
/*ui设计界面中所起的名字：栈容器-stack、Scroll Area控件-QQ、Tool Box控件-roolling、
                       Tab Widget控件-scaning  */

//栈容器
     ui->stack->setCurrentIndex(1);//设置初始时默认的页面为栈容器索引号为1所对应的那一个页面

     connect(ui->QQ,&QPushButton::clicked,[=](){
     ui->stack->setCurrentIndex(2); /*栈容器中的每一页都对应了一个索引，点击ui界面的栈容器后在
     右下方的QStackedWidget中找到currentIndex就可查看当前也所在的索引。调用setCurrentIndex()
     后，函数实参为一个索引号，即调用setCurrentIndex(2)就能跳转到栈容器中索引号为2的那一页了。*/
     });

     connect(ui->roolling,&QPushButton::clicked,[=](){
     ui->stack->setCurrentIndex(0);
     });

     connect(ui->scaning,&QPushButton::clicked,[=](){
     ui->stack->setCurrentIndex(1);
     });

//下拉框
     ui->xiala_kuang->addItem("奔驰");//为名字为xiala_kuang的这个下拉框添加内容(默认是显示最开始添加
     ui->xiala_kuang->addItem("宝马");//的那一个)
     ui->xiala_kuang->addItem("拖拉机");

     //信号与槽在下拉框中的应用：点击按钮，选中下拉框中的某一个选项，如点击按钮，选中下拉框中的拖拉机选项
     connect(ui->btn,&QPushButton::clicked,[=](){
     ui->xiala_kuang->setCurrentIndex(2);  /*这里的索引号是按照编程时写入下拉框的名字的顺序决定的，
     即“奔驰”----0，“宝马”-----1，“拖拉机”-----2。
     注意：下拉框对应的索引号与栈容器所对应的索引号有些区别，栈容器是在ui界面就已经为其添加内容了，栈容器
         中的每一页对应了对应了一个索引号，依次为0、1、2、3、……，通过按钮可以通过这个索引号链接到某一页。
         下拉框并没有在ui设计界面为其添加内容，而是通过程序来为其添加的，此时下拉框中每一个选项所对应的
         索引号就是该下拉框调用addItem()函数时对应的顺序。
      */
      });

//标签
      //利用QLabel显示图片
      ui->label_p->setPixmap(QPixmap(":/photo/chicken.png"));/*利用setPixmap()函数来显示图片，
      QPixmap()函数的参数为某一个图片的路径。    */

      QMovie *movie=new QMovie(":/photo/tongtu.gif");
      ui->label_movie->setMovie(movie); //这个控件label_movie中放入一个movie动图

      movie->start(); //开始播放这个动图
}

Widget::~Widget()
{
    delete ui;
}

