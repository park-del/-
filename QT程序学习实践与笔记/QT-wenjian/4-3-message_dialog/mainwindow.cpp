#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //一定要加上这个connect语句，将相应的控件与某一个对话框所关联，来完成点击某一个控件来实现弹出对话框的功能
    connect(ui->actionnew,&QAction::triggered,[=]()  //点击新建按钮依次弹出下面的对话框
    {

    //消息对话框：提示的消息有四种，分别是问题消息、警告消息、提问消息、错误消息
    //是通过查帮助文档中的QMessageBox查找其静态成员函数来实现的，每一种消息都对应了有一个QMessage类的静态
    //成员函数。


    //错误对话框：静态成员函数为critical()
    QMessageBox::critical(this,"critical","错误",QMessageBox::Save|QMessageBox::Cancel);

    //信息对话框:静态成员函数为information()
    QMessageBox::information(this,"info","信息");

    //提问对话框：静态成员函数为question()
    QMessageBox::question(this,"question","提问",QMessageBox::Save | QMessageBox::Cancel,
                         QMessageBox::Cancel);

    //警告对话框：静态成员函数为warning()
    QMessageBox::warning(this,"warning","警告");

    });

    connect(ui->actionopen,&QAction::triggered,[=]()  //点击打开按钮弹出下面的警告对话框
    {
        QMessageBox::warning(this,"warning","警告");
    });

   /*
提问对话框的说明：
   提问对话框的第1个参数是为这个对话框指明其父亲的，第2个参数是指明这个对话框的标题，第3个参数是这个对话框所显
   示的内容，第4个参数指明这个提问对话框的选项，注意第4个参数是可以有多个的，当第4个参数有多个时该对话框就会有
   多个选项，该函数的返回值也将会有多个，第5个参数是设置回车所默认关联的选项，是第4个参数的某一个，即按下回车后
   执行的是那个选项。
   下面的提问对话框将会执行时将会显示四个选项：
   QMessageBox::question(this,"question","提问",QMessageBox::Save | QMessageBox::Cancel |
                          QMessageBox::Open | QMessageBox::Close,QMessageBox::Cancel);



   由于QMessage类的静态成员函数question的返回值为standarButton，为该静态成员函数中第3个参数的类型，
   即QMessageBox::Save 或者 QMessageBox::Cancel,用户点击这两个按钮中的哪一个该question函数()就
   返回哪个按钮，故用户可以根据if语句判断返回值来选择这两个按钮中的每一个所对应执行的功能。
   if(QMessageBox::Save==( QMessageBox::question(this,"question","提问",
                             QMessageBox::Save | QMessageBox::Cancel,
                             QMessageBox::Cancel))
   {
          qDebug()<<"选择是保存";
   }
   else
   {
          qDebug()<<"选择的是取消";
   }

错误对话框、信息对话框的说明、警告对话框的说明：
错误对话框与信息对话框和提问对话框一样，也是可以设置选项的个数的以及各个选项的名字的，且错误对话框和提问对话框
的返回值都是某一个选项，和提问对话框一样，它们也是可以根据通过if来判断返回的选项从而决定各个选项所执行的功能的。
错误对话框和信息对话框的参数个数以及参数类型是与提问对话框一样的。

四种对话框的所对应的函数的参数个数以及参数的类型都是一样的，这四种对话框的区别就是每一种对话框的图标不一样，
图标的作用是用来给用户提示信息的，故每一个对话框给用户提示的信息也不同，用户可以根据对话框提供的信息来选择
对不同的的对话框进行不同的操作。

注意:这四种对话框都是模态式的对话框，即当这四种对话框中的某一种弹出时是不能够对其他的窗口进行操作的。
*/

}

MainWindow::~MainWindow()
{

    delete ui;
}

