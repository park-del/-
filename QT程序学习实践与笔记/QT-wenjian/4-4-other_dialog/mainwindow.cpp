#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>
#include <QFileDialog>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
   ui->setupUi(this);

//其他标准对话框
   connect(ui->actionopen,&QAction::triggered,[=]()  //点击新建控件后会弹出一个其他标准类型的对话框
   {

    QColor color = QColorDialog::getColor(QColor(255,0,0));  //颜色对话框
    qDebug()<<"r="<<color.red()<<"g="<<color.green()<<"b="<<color.blue(); //打印出返回的颜色信息
/*
颜色对话框：静态成员函数为getColor(),调用该函数会创建一个颜色对话框。
   颜色对话框的第1个参数为QColor类型的一个对象，这里是采用C++中临时对象的方式（即是调用QColor的构造函数）。
   getcolor()函数是用来打开一个颜色对话框的，函数的第1个参数为设置打开这个对话框后这个对话框默认的调色。
   函数的第2个参数是用来设置父亲的。
   函数的第3个参数是用来设置这个颜色对话框的标题的。
   函数的第4个参数是用来设置这个颜色对话框的选项的，默认是有两个选项的。

   当在这个对话框中选择某一个颜色点击ok后会返回一个颜色，这个颜色也就是这个函数getColor的返回值，故定义了一
   个颜色类型QColor的变量color来保存这个颜色。

   某一个颜色是由3个参数来决定的，即分别为r（红色）、g（绿色）、b（蓝色），这是光的3基色，我们生活中所见到的
   任何一种颜色都是由这3中颜色按照不同的配比构成的，即确定了r、g和b后就能够确定某一种颜色了。
*/



    QString str=QFileDialog::getOpenFileName(this,"打开文件","C:\\Users\\86185\\Desktop",
                                            "(*.txt)");
    qDebug()<<str;
    /*
文件对话框:是通过静态成员函数getOpenFileName()来创建一个文件对话框的。
     函数第1个参数：为其指明父亲。
     函数第2个参数：设置这个文件对话框的标题。
     函数第3个参数：要打开的这个文件夹的路径，如果是某一个文件，则会打开这个文件所在文件夹中的所有文件
                 （即点击相应的控件后会打开指定的文件夹，要想打开文件还必须自己去点击才能够打开该指定
                   的文件）。
     函数第4个参数：对一些文件进行过滤，如(*.txt)就是过滤掉其他所有类型的文件，只保留.txt文件，即是当打开
                  某一个文件对话框时,在打开的文件夹中是只能看到后缀名为.txt的文件的，其他所有的文件都会被
                  过滤掉，但是要注意文件夹是不会被过滤掉的。
     注意：文件对话框所打开的是一个文件夹，而不是一个文件。

     当在所打开的文件对话框的窗口中选中某一个文件点击打开后该函数会返回一个QString类型的变量str，
     str保存了要打开的的这个文件的路径。
     */

});



}

MainWindow::~MainWindow()
{
    delete ui;
}

