#include "mainwindow.h"
#include "ui_mainwindow.h"
#include  <QDialog>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);  //ui这个对象的可以认为就是那个设计界面，进行ui->设计界面中某个东西的名字->相应的成
    //员函数这样的操作就是对设计界面中某个东西所进行的操作。

    /*点击文件下的新建，弹出一个对话框（此时消息的发送者是ui下的新建，即是ui->actionnew;此时消息的接收者是
      所对应的那个对话框，故需要用到信号和槽的机制） */
    connect(ui->actionnew,&QAction::triggered,
    [=](){ QDialog dlg1(this); dlg1.resize(200,100); dlg1.exec();} );
    /*当消息的发送者是ui设计界面的某一个东西时，此时被点击的信号函数为&QAction::triggered()，当消息的发送者
      不是ui设计界面时，此时被点击的信号为&QWidget::clicked()。  */

    /*lambel表达式的说明：lambel表达式其实就是一个匿名的函数，[]内部为"="号时表示该匿名函数的参数为该匿名函
      数所在函数所定义的变量中的任何一个。（lambel表达式？？？？？）
      也可以在该匿名函数中重新定义一个变量从而再进行相关的操作的。
     （这里所说的变量一般都是某一个类的对象）
    */

    /*对话框的说明：对话框分为了模态对话框和非模态对话框
      模态对话框： 当该对话框打开后不可以对其它的窗口进行操作（利用到了阻塞的机制，即是把这个对话框打开后不可以
                 再对其他窗口进行操作，只能对本窗口进行操作）
      非模态对话框：当该对话框打开后可以对其他的窗口进行操作 */

    connect(ui->actionopen,&QAction::triggered,
    [=]()
    {
     //  QDialog dlg2(this); resize(200,100); dlg2.show(); 无法创建
         QDialog *dlg2=new QDialog(this); dlg2->resize(200,100); dlg2->show();
         dlg2->setAttribute(Qt::WA_DeleteOnClose); /*由于dlg2是进行动态开辟的，故在程序执行时系统会
         一直开辟存储空间，为了防止因为所开辟的存储空间过多而出现内存泄漏的情况，故要对dlg2设置一个属性。
         对dlg2设置的一个属性为QT::WA_DeleteonClose（这是55号属性）,表示在关闭的时候撤销对象。*/
    });
/*
区别模态对话框与非模态对话框：
    connect(ui->actionnew,&QAction::triggered,
    [=](){
    QDialog dlg1(this);   //由于模态对话框当程序执行到exc时会被阻塞，故模态对话框是可以为函数内部
    dlg1.exec();          //（在这里是匿名函数的内部）的一个局部变量的。
    });

    connect(ui->actionnew,&QAction::triggered,
    [=](){
    QDialog dlg2(this);  //由于非模态对话框当程序执行到dlg2.show()时会显示窗口，但是由于dlg2为函数内部
    dlg2.show();         //的一个局部变量，当函数执行结束时，该对话框也会被撤销，故当程序执行时可能会看不
    //到非模态对话框或者非模态对话框一闪而过，解决方法是将该对象放在堆区，而不是栈区。堆区是由用户自己开辟的，
    如果用户不自己释放的话，该片区域的存储空间是一直存在的。

 });
*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

