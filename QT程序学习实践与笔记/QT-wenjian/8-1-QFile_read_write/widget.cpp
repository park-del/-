#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QFile>      //读写文件时需要用到的头文件
#include <QFileInfo>  //查询某一个文件的信息时需要用到的类
#include <QDebug>
#include <QDateTime>

Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    //在c语言中有一种对文件的读写操作，在C++中也有一种对文件的读写操作，在QT中也有一种对文件的读写操作

    //点击选取文件按钮，弹出一个文件对话框
    connect(ui->pushButton,&QPushButton::clicked,[=](){
    QString path=QFileDialog::getOpenFileName(this,"打开文件","D:\\QT-文件读写的文件");
    ui->lineEdit->setText(path);  //将所打开文件的路径放到lineEdit中

    //将所打开的文件的内容放到textEdit中
    QFile file(path); /*用QFile定义了一个文件，接下来就是对file所指向的那个文件来进行读或者写的，
    file(a)中的参数a为一个文件的路径，此时file文件就指向了a这个文件，对file文件的操作也就是对a文件的
    操作。*/
    file.open( QIODevice::ReadOnly ); //对file这个文件设置其打开方式，这里设置的是以只读的方式打开file
                                      //这个文件。
    /*
    在帮助文档中搜索file.open可以查到文件打开方式的枚举值
    如   QIODevice::ReadOnly   以只读的方式打开这个文件（此时是只能读取文件中的内容，而不能向文件中
                               写入东西的）
        QIODevice::WriteOnly   以只写的方式打开这个文件
        QIODevice::ReadWrite   以读写的方式打开这个文件
    */

   /*  一下子读取file文件中全部的内容并将读取到的内容放到textEdit中。
    QByteArray array=file.readAll(); //file.readAll()返回的是一串字符，这一串字符是用QByteArray
    //类所定义的一个对象来进行接收的。
    ui->textEdit->setText(array); //将读取到的数据(即array)放入到textEdit中
  */

    /* 只读取file文件中第1行的内容
    QByteArray array =file.readLine();
    ui->textEdit->setText(array);
    */

    //每次只读取一行，利用一个while循环来读取文件中全部的内容
    QByteArray array;
    while(!file.atEnd())     //当file文件到达末尾的时候file.atEnd()为真
    {
    array +=file.readLine(); //每次读取到一行就追加到array的后面
    ui->textEdit->setText(array);
    }

    file.close();  //关闭这个文件(进行文件打开方式的转换时要先把这个文件关闭才能转换为另一种文件的打开
    //方式)
    file.open(QIODevice::Append); /*以追加的方式来向这个文件中写入东西,注意：如果是以
    QIODevice::WriteOnle,即是以只写的方式打开这个文件的话，则是从这个文件的开头开始进行
    写的，此时会把长恨歌的内容给覆盖掉；而以QIODevice::Append，即追加的方式打开这个文件的
    话是从这个文件的末尾开始进行写的，此时并不会把长恨歌的内容给覆盖掉，而是在其末尾追加了所写
    的内容。*/
    file.write("啊啊啊啊啊");
    file.close();





    //QFileInfo:获取文件中的一些信息的类
    QFileInfo info(path);
    qDebug()<<"大小:"<<info.size()<<"后缀名:"<<info.suffix()<<"文件名称:"<<info.fileName()
            <<"文件路径"<<info.filePath();
    qDebug()<<info.created();
    qDebug()<<info.created().toString("yyyy/MM/dd hh:mm:ss");//对输出的时间进行格式的设置
/*
created()函数的返回值的类型为QDateTime。
  d:1-31
  dd:01-31
  ddd:Mod-Sun
  dddd:Monday-Sunday（全拼的）
 （月份m的格式设置与d的一样，分为了m、mm、mmm、mmmm，但是年份y的格式设置只有两种，分别为yy、yyyy
  yy：00-99     yyyy：2018  ）

 其他的时分秒的格式设置键帮助文档（在帮助文档中搜QDateTime即可）
*/
    });

}

Widget::~Widget()
{
    delete ui;
}

