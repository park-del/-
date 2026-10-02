#include "smallwidget.h"
#include "ui_smallwidget.h"

Smallwidget::Smallwidget(QWidget *parent) : QWidget(parent),ui(new Ui::Smallwidget)
{
    ui->setupUi(this);
    //a1改变数字的话a2跟着移动（a1是ui界面中的QSpinbox控件，也即是a1是QSpinbox类的一个对象）
    void(QSpinBox::*spSignal)(int)=&QSpinBox::valueChanged;/*函数指针spSignal所指向的是
    valuechanged(int i)这个函数，故下面利用的信号和槽的机制a1所发送的信号是valuechanged(int i),
    即是a1每改变数字的时候就会发送一个整型的数给a2*/
    connect(ui->a1,spSignal,ui->a2,&QSlider::setValue); /*查帮助文档找QSpinbox类的signal，即
    是a1所能发送的信号。查帮助文档得到a1所能发送的信号就是valueChanged(),有两个重载的信号
    valueChanged()，其中一个参数为int i，另一个参数为QString &text，说明了当改变数字的时候可以发送
    整型的信号，也可以发送字符串类型的信号，这里用到的是发送整型的信号（即发送1、2、3给a2）。
    注意：由于a1所能发送的信号valuechanged()有两个，即valuechanged()进行了重载，故需要利用函数指针的
         方式来标识a1所发送的具体是哪一个信号。

     查看帮助文档找QSlider类的槽函数，即是a2接收到a1所发送的信号后a2所能够进行的操作，没有就去其父类中
     找。由于a1传送过来的信号是一个int类型的信号，故槽函数接收的信号也应该是int类型的，即槽函数的形参要
     是int类型的，所选择的槽函数是void setvalue(int i)。

     查看帮助文档的技巧：在索引框输入一个类时就能在最开始时候看到contens这个标题，这个标题下面的目录
                      有signals、public slots、public functions等，点击进去就能查看这个类有
                      哪些信号，有哪些槽函数，有哪些成员函数了。如果没有找到对应的信息，如没有找到
                      对应的信号，没有找到对应的槽函数，没有找到对应的成员函数的话就从它的父类中去找
                      父类中的信号、槽函数、成员函数该类所定义的对象也是可以调用的。
     */

    //a2移动的话a1改变数字
    connect(ui->a2,&QSlider::valueChanged,ui->a1,&QSpinBox::setValue);

}

//是对a1这个控件来进行设置数字或者获取数字的（a1这个对象是属于QspinBox的）
void Smallwidget::setNum(int num) //设置a1这个控价上所显示的数字
{
    ui->a1->setValue(num);   //调用这个函数时所设置的数字就会在a1这个控件上显示出来
}
int  Smallwidget::getNum()           //获取a1这个控件上所显示的数字
{
    return ui->a1->value();  //返回a1这个控件上所显示的数字
}

Smallwidget::~Smallwidget()
{
    delete ui;
}
