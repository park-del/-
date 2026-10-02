#include "widget.h"
#include "ui_widget.h"
#include  <QListWidgetItem>
#include <QList>

Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    //Table Widget控件:表格控件，每一个表格都是有一个水平的表头和一个垂直的表头的

    ui->table->setColumnCount(3);//设置列数（表格的列数）

    ui->table->setHorizontalHeaderLabels(QStringList()<<"姓名"<<"性别"<<"年龄");//设置水平的
    //表头


    ui->table->setRowCount(5); //设置行数（表格的行数）

  /*
  注意：表格控件里设置的行数和列数都是不包含水平表头和垂直表头的，即设置的3列5行只是表格中的内容总共
  是有3列5行的，如果包括水平表头和垂直的表头的话是有4列6行的。
  */

  /*设置正文内容
    ui->table->setItem(0,0,new QTableWidgetItem("亚瑟"));   设置第0行、第0列的内容为“亚瑟”，
    setItem()函数的第3个参数为一个Qstring类型的指针，使用new QTableWidgetItem()的方式来创建一个
    QString类型的指向某一个匿名对象的指针。  */

    QStringList nameList; //定义了名称的一个链表，用“<<”来区分开链表中的每一个结点
    nameList<<"亚瑟"<<"赵云"<<"关羽"<<"张飞"<<"花木兰";
    //该名称链表中的每一个结点依次是 "亚瑟"->"赵云"->"关羽"->"张飞"->"花木兰"

    QStringList sexList;  //定义了一个性别的链表，用“<<”来区分开链表中的每一个结点
    sexList<<"男"<<"男"<<"男"<<"男"<<"女";
    //该性别链表中的每一个结点依次是："男"->"男"->"男"->"男"->"女"

    for(int i=0;i<5;i++)
    {
     /*注意：nameList和sexList都为5个结点时才可以进行循环5次，当nameList和sexList中只要有一个链表
            少于5个结点或者有一个多于5个结点时就不能循环5次，必须两个链表都为5个结点时才能够进行循环。
     */
       int col=0;
       ui->table->setItem(i,col++,new QTableWidgetItem(nameList[i]));
       ui->table->setItem(i,col++,new QTableWidgetItem(sexList[i]));
       //int 转 QString
       ui->table->setItem(i,col++,new QTableWidgetItem(QString::number(i+18)));
   //int类型转为QString类型时调用的是QString::number()函数，即QString::number(18)就是将
   //整型的18转换为字符类型的18
    }


}



Widget::~Widget()
{
    delete ui;
}

