#include "widget.h"
#include "ui_widget.h"
#include  <QDebug>

Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    //点击“获取当前值”这个按钮，获取a1这个控件当前所显示的值
    connect(ui->btn_get,&QPushButton::clicked,[=](){
    qDebug()<<ui->widget1->getNum();  //调用自定义控件所提供的接口getNum(),widget1是包含这个
    //自定义控件的一个容器的名字。
    });

    //点击“设置到一半”这个按钮，a1这个控件的值显示为最大值的一半
    connect(ui->btn_set,&QPushButton::clicked,[=](){
     ui->widget1->setNum(50);
    });


}

Widget::~Widget()
{
    delete ui;


}

