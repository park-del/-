#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cmath"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //设置当前这个窗口的一些属性
    this->setWindowTitle("calculator");             //设置当前这个窗口的标题为calculator
    this->setMaximumSize(421,486);                  //设置当前这个窗口的最大尺寸
    this->setMinimumSize(421,486);                  //设置当前这个窗口的最小尺寸
    this->setWindowIcon(QIcon(":/timg.jpg"));       //设置当前这个窗口的图标

    //对各个运算符刚开始置false，置true表示当前进行的是该运算
    waittingForOperand = true;                      /*显示窗口使能标志，为true时表示可以向窗口中
    输入数字（由于窗口中的默认数字是为0的，故为true时也可以认为是窗口数字为0的标志）；为false时表示不能向
    窗口中输入数字（由于窗口中的默认数字是为0的，故为false时也可以认为是窗口数字不为0的标志）*/
    result = 0.0;                                   //存放双目运算符的第1个操作数
    value  = 0.0;                                   //存放双目运算符的第2个操作数
    operatorFlag = false;                           /*操作符标标志，为true时不能够进行单目运算符
    的操作，可以进行保存双目运算符保存第1个操作数在result中的操作；为false时能够进行单目运算符的操作，也
    能够进行双目运算符保存第2个操作数在value中的操作。 */
    modOperator = false;                            //余数运算符
    XYOperator  = false;                             //X的Y次方运算符
    addOperator = false;                            //加法运算符
    minOperator = false;                            //减法运算符
    mulOperator = false;                            //乘法运算符
    divOperator = false;                            //除法运算符
/*上面的运算符标志都是双目运算符的标志，是用来区分最后再执行calculate()函数时执行哪一个if语句里面的内容的，
  即是进行哪种运算的。*/

    //点击计算器上的数字按键0、1、2、3、4、5、6、7、8、9会去触发槽函数buttonDigitalClicked()
    connect(ui->buttonDigital0, SIGNAL(clicked()), this, SLOT(buttonDigitalClicked()));
    connect(ui->buttonDigital1, SIGNAL(clicked()), this, SLOT(buttonDigitalClicked()));
    connect(ui->buttonDigital2, SIGNAL(clicked()), this, SLOT(buttonDigitalClicked()));
    connect(ui->buttonDigital3, SIGNAL(clicked()), this, SLOT(buttonDigitalClicked()));
    connect(ui->buttonDigital4, SIGNAL(clicked()), this, SLOT(buttonDigitalClicked()));
    connect(ui->buttonDigital5, SIGNAL(clicked()), this, SLOT(buttonDigitalClicked()));
    connect(ui->buttonDigital6, SIGNAL(clicked()), this, SLOT(buttonDigitalClicked()));
    connect(ui->buttonDigital7, SIGNAL(clicked()), this, SLOT(buttonDigitalClicked()));
    connect(ui->buttonDigital8, SIGNAL(clicked()), this, SLOT(buttonDigitalClicked()));
    connect(ui->buttonDigital9, SIGNAL(clicked()), this, SLOT(buttonDigitalClicked()));

    connect(ui->buttonNega, SIGNAL(clicked()), this, SLOT(buttonNegativeClicked()));//点击+/-

    connect(ui->buttonSin,SIGNAL(clicked()), this, SLOT(buttonSinClicked())); //点击sin
    connect(ui->buttonCos,SIGNAL(clicked()), this, SLOT(buttonCosClicked())); //点击cos
    connect(ui->buttonTan,SIGNAL(clicked()), this, SLOT(buttonTanClicked())); //点击tan
    connect(ui->buttonBIN, SIGNAL(clicked()), this, SLOT(buttonBINClicked()));//点击BIN
    connect(ui->buttonAbs,SIGNAL(clicked()), this, SLOT(buttonAbsClicked())); //点击|x|
    connect(ui->buttonSqrt, SIGNAL(clicked()), this, SLOT(buttonSqrtClicked()));//点击Sart
    connect(ui->buttonPow, SIGNAL(clicked()), this, SLOT(buttonPowClicked()));//点击G
    connect(ui->buttonX, SIGNAL(clicked()), this, SLOT(buttonXClicked()));//点击1/x
    connect(ui->buttonXY, SIGNAL(clicked()), this, SLOT(buttonXYClicked()));//点击x^y
    connect(ui->buttonMod, SIGNAL(clicked()), this, SLOT(buttonModClicked()));//点击mod
    connect(ui->buttonEqual, SIGNAL(clicked()), this, SLOT(buttonEqualClicked()));//点击=


    connect(ui->buttonAdd, SIGNAL(clicked()), this, SLOT(buttonAddClicked()));//点击+
    connect(ui->buttonMin, SIGNAL(clicked()), this, SLOT(buttonMinClicked()));//点击-
    connect(ui->buttonMul, SIGNAL(clicked()), this, SLOT(buttonMulClicked()));//点击*
    connect(ui->buttonDiv, SIGNAL(clicked()), this, SLOT(buttonDivClicked()));//点击/

    connect(ui->buttonPoint, SIGNAL(clicked()), this, SLOT(buttonPointClicked()));//点击.
    connect(ui->buttonPI, SIGNAL(clicked()), this, SLOT(buttonPIClicked()));//点击PI
    connect(ui->buttonDel, SIGNAL(clicked()), this, SLOT(buttonDelClicked()));//点击Del
    connect(ui->buttonClear, SIGNAL(clicked()), this, SLOT(buttonClearClicked()));//点击Clear





    //display是ui界面中的显示窗口的名字，对display所进行的操作就是对那个显示窗口所进行的操作
    ui->display->setAlignment(Qt::AlignRight);//设置显示框里的文字水平向右对齐
    ui->display->setReadOnly(true);//如果为true是可以在计算器的显示框中来进行编辑的，为false则是不能够
    ui-> display->setMinimumHeight(50);  //设置显示框的高度
    ui->display->setText("0");                              //这个显示窗口中显示一个0

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::buttonDigitalClicked() //点击数字时会触发的槽函数
{
 /*
  函数原型为QObject * obj = sender();
  sender()会返回发送信号的那一个对象，是用QObject类型的对象来进行接收的。signal发送者一般都是QObject
  的派生类所定义的一个对象，由于sender()返回的是发送者，不同的发送者类型可能不一样，又由于派生类的对象是
  可以向基类的对象来进行赋值的，故是可以统一用基类QObject所定义的一个对象来进行接收的。

  T qobject_cast ( QObject * object )
  本方法返回object向下的转型T，如果转型不成功则返回0，如果传入的object本身就是0则返回0。
  在下面的程序中T为QPushButton，故是将sender()返回的QObject类型的一个对象（该对象是发
  送者）转型为QPushButton类型的一个对象返回。返回的这个对象是用buttonClicked来进行接收的。
 */
    QPushButton *buttonClicked = qobject_cast<QPushButton *>(sender());/*返回的是消息的发送者,
    消息的发送者就是被点击的按键，此时被点击的那个按键就是buttonClicked。 */
    int buttonValue = buttonClicked->text().toInt(); /*注意：那几个数字按键分别已经在ui界面的text
    中写入了字符0-9，buttonClicked执行text().toInt()就会将buttonClicked这个按键的text中的字符型的数字
    转换为int类型的字符，text().toInt()函数返回的就是buttonClicked这个按键被转换为int类型的字符。故此时
    buttonValue就是被点击的那一个按键的数值。 */

    if(waittingForOperand)
    {
        ui->display->clear(); //显示框清屏
        waittingForOperand = false; /*每次输入一个数字之后waittingForOperand就会为false，只有
        waittingForOperand为true时才能够输入数字。*/
    }
    ui->display->setText(ui->display->text()+QString::number(buttonValue)); /*显示框的内容为
    显示框当前的文本内容拼接上buttonValue数字所对应的字符数字（显示框是一个文本框，文本框中是只能显示字符
    而不能显示数字的）。
    QString::number是将数字（整数、浮点数、有符号、无符号等）转换为QString类型。
    注意:ui->display->text()+是在display这个文本框中当前文字的后面进行拼接数字的，因此上面要有一个
        ui->display->clear();来将显示框中的0给清除掉，否则的话每输入就会在其前面出现1个0。*/
}
/*
waittingForOperand的作用就是用来对显示框进行清屏的工作并将新输入的数字加到ui->display->text()的后面，
即显示框当前已经显示的文字的后面的。每次执行完buttonDigitalClicked()函数里面的if语句后（if语句里面的内容
就是将新的一个数字加入到显示框的后面）都会将waittingForOperand置为false。
*/

///////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::buttonNegativeClicked()//点击“ +/- ”
{
    if(operatorFlag)
        return;
    else
    {
        double text = ui->display->text().toDouble();//将文本框中当前的字符型的数字转化int类型的数字
        text = -text;  //是对int类型的数字进行相应的操作完后得到一个新的int类型的数字
        ui->display->setText(QString::number(text));/*将所得到的新的int类型的数字转换成字符类型的数
        字后在放入显示框中的。注意：这里的显示不是拼接的，而是直接覆盖原来所显示的内容，此时的ui->display->
        text()就是显示框中的新内容。*/
        waittingForOperand = true;
    }
}

void MainWindow::buttonSinClicked()  //点击sin
{
    if(operatorFlag)
        return;
    else
    {
        double text = ui->display->text().toDouble();
        text = std::sin(text*3.141592654/180); //执行sin(text*π/180)
        ui->display->setText(QString::number(text));
    }
    waittingForOperand = true;
}

void MainWindow::buttonCosClicked() //点击cos
{
    if(operatorFlag)
        return;
    else
    {
        double text = ui->display->text().toDouble();
        text = std::cos(text*3.141592654/180);
        ui->display->setText(QString::number(text));
    }
    waittingForOperand = true;
}

void MainWindow::buttonTanClicked() //点击tan
{
    if(operatorFlag)
        return;
    else
    {
        double text = ui->display->text().toDouble();
        text = std::tan(text*3.141592654/180);
        ui->display->setText(QString::number(text));
    }
    waittingForOperand = true;
}

void MainWindow::buttonBINClicked() //点击BIN
{
    if(operatorFlag)
        return;
    else
    {
        int text = ui->display->text().toInt();
        QString str="";
        while (text!=0)
        {
            str = QString::number(text % 2) + str;/*注意这里的顺序，是QString::number(text % 2)+
str,即是在QString::number(text % 2)这个字符的后面拼接str的，str存放的是前面所获得字符，而QString::
number(text % 2)则是当前所获得的新转化的字符。反过来的话str+QString::number(text % 2)就是在str这个字符
串的后面拼接QString::number(text % 2)这个字符的，这不符合二进制的书写顺序，是不行的。*/
            text = text/ 2;
        }
       ui->display->setText(str);
    }
    waittingForOperand = true;
}

void MainWindow::buttonAbsClicked()//点击|x|
{
    if(operatorFlag)
        return;
    else
    {
        double text = ui->display->text().toDouble();
        text = std::fabs(text);
        ui->display->setText(QString::number(text));
    }
    waittingForOperand = true;
}

void MainWindow::buttonSqrtClicked()//点击Sqrt
{
    if(operatorFlag)
        return;
    else
    {
        double text = ui->display->text().toDouble();
        if(text < 0)
            abortOperation();//要是被开方的数小于0的话就调用abortOperation()函数输出ERROR的错误信息
        else
        {
            text = std::sqrt(text);
            ui->display->setText(QString::number(text));
        }
    }
    waittingForOperand = true;
}

void MainWindow::buttonPowClicked()//点击G
{
    if(operatorFlag)
        return;
    else
    {
        double text = ui->display->text().toDouble();
        text = std::pow(text,2);  //执行的是平方
        ui->display->setText(QString::number(text));
    }
    waittingForOperand = true;
}

void MainWindow::buttonXClicked()//点击1/x
{
    if(operatorFlag)
        return;
    else
    {
        double text = ui->display->text().toDouble();
        if(text == 0.0)//被求倒数的数为0的话就调用abortOperation()函数输出ERROR的错误信息
            abortOperation();
        else
        {
            text = 1/text;
            ui->display->setText(QString::number(text));
            waittingForOperand = true;
        }
    }
}

/*
 这里的都是单目运算符，单目运算符可以看做是对某一个数所进行的操作，所进行的处理。
*/
//////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::buttonXYClicked()//点击x^y
{
/*  第一次点击x^y时operatorFlag为false会执行if语句里面的内容，将当前显示框中的数字给保存到result中去。
    同时令operatorFlag和waittingForOperand为true，最后令XY0eraotr为true（最后执行calculate(value)
    计算的函数时就是根据XY0eraotr这个运算符标志来决定执行对应的那一个操作的if语句的）。
    此时点击任何单目运算符都是没有用的，因为在所有的单目运算符中都有一个 if(operatorFlag)  return;的语句。
    此时是可以点击任何的双目运算符的，

    第二次点击x^y(由于任何的双目运算符里面的else语句都是一样的，故第二次点击可以是点击任何的双目运算符)时
    operatorFlag为true会执行else语句中的内容，这个else语句里面的内容有一个value = ui->display->text()
    .toDouble();用来获取当前显示框中的数字（即是双目运算符的第2个操作数）。
    这个else语句里面还是有一个calculate(value)计算的函数，会调用该函数同时将计算的结果显示在窗口上。


    总：第1次点击双目运算符时会将第一个操作数保存在result，第2次点击双目运算符时会将第二个操作数保存在value
       中同时进行计算显示出结果（这里的第1次点击的双目运算符时不包括“=”的因为“=”的if语句里面没有result=
       的语句，也就没有将第1个操作数给保存起来，第2次点击的双目运算符是包括“=”的，因为第“=”的else语句里面
       也是有value=的语句来将第2个操作数来进行保存起来的）。
       第1次点击双目运算符和第2次点击双目运算符都会对waittingForOperand = true;  operatorFlag;进行
       相应的置位来限制某些操作。

*/
    if(!operatorFlag)
    {
        result = ui->display->text().toDouble(); /*result存放第一次点击时当前显示框中的数字（即是双
        目运算符的第1个操作数)*/
        operatorFlag = true;       //此时是不可以输入任何的一个单目运算符的
        waittingForOperand = true; //此时可以输入第2个操作数的数字
    }
    else
    {
        value = ui->display->text().toDouble(); /*value存放第二次点击时当前显示框中的数字（即是双目
        运算符的第2个操作数)*/
        if(calculate(value)==false)//
        {
            abortOperation();
        }
    }
    XYOperator = true;
}

void MainWindow::buttonModClicked()//点击mod
{
    if(!operatorFlag)
    {
        result = ui->display->text().toDouble();
        operatorFlag = true;
        waittingForOperand = true;
    }
    else
    {
        value = ui->display->text().toDouble();
        if(calculate(value)==false)
        {
            abortOperation();
        }
    }
    modOperator = true;
}

void MainWindow::buttonEqualClicked()//点击=
{
/*输入完第二个操作数后点击任何一个双目运算符都是可以调用calculate(value)函数计算出结果并将计算结果显示在窗口
  上的。 */
    if(!operatorFlag)
        return;
    else
    {
        value = ui->display->text().toDouble();
        if(calculate(value)==false)
        {
            abortOperation();
        }
        waittingForOperand = true; //点击了=之后是可以输入数字的
        operatorFlag = false;      //点击了=之后是可以输入单目运算符的，但是不能够输入双目运算符
    }
}

void MainWindow::buttonAddClicked()//点击+
{
    if(!operatorFlag)
    {
        result = ui->display->text().toDouble();
        operatorFlag = true;  //可以输入双目运算符
        waittingForOperand = true; //可以输入数字
    }
    else
    {
        value = ui->display->text().toDouble();
        if(calculate(value)==false)
        {
            abortOperation();
        }
    }
    addOperator = true;
}

void MainWindow::buttonMinClicked()//点击“-”
{
    if(!operatorFlag)
    {
        result = ui->display->text().toDouble();
        operatorFlag = true;
        waittingForOperand = true;
    }
    else
    {
        value = ui->display->text().toDouble();
        if(calculate(value)==false)
        {
            abortOperation();
        }
    }
    minOperator = true;
}

void MainWindow::buttonMulClicked()//点击“*”
{
    if(!operatorFlag)
    {
        result = ui->display->text().toDouble();
        operatorFlag = true;
        waittingForOperand = true;
    }
    else
    {
        value = ui->display->text().toDouble();
        if(calculate(value)==false)
        {
            abortOperation();
        }
    }
    mulOperator = true;
}

void MainWindow::buttonDivClicked()//点击“ / ”
{
    if(!operatorFlag)
    {
        result = ui->display->text().toDouble();
        operatorFlag = true;
        waittingForOperand = true;
    }
    else
    {
        value = ui->display->text().toDouble();
        if(calculate(value)==false)
        {
            abortOperation();
        }
    }
    divOperator = true;
}
/*  这里都是双目运算符，不同于单目运算符操作数只有一个，这里操作数是有两个的。    */
////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::buttonPointClicked()//点击“ . ”
{
    if(ui->display->text().contains('.'))//如果当前显示框中所显示的文字中已经包含了小数点的话
        return;
    else
        ui->display->setText(ui->display->text()+tr("."));
}

void MainWindow::buttonPIClicked()//点击“ PI ”
{
    if(waittingForOperand)
        ui->display->setText(QString::number(3.141592654));
    else
        return;
}

void MainWindow::buttonDelClicked()//点击“ Del ”
{
//
    if(waittingForOperand) //如当前的显示框的数字为0（即显示框是默认的状态）
        return;

    QString text = ui->display->text();
    text.chop(1);  //清楚显示窗口中的末尾的一个字符


    if(text.isEmpty()) //判断清楚显示窗口末尾的一个字符后该显示窗口为空就往显示窗口中输入一个字符0
    {
        text = "0";
        waittingForOperand = true;
    }
    ui->display->setText(text); /*将清楚后的text()再重新显示,这里就是对显示窗口的刷新操作，如果不进行
    这一步的话窗口还是显示原来的字符，是不会改变的。*/
}

void MainWindow::buttonClearClicked()//点击“ clear ”
{
//对相应的标志位恢复初始的状态
    waittingForOperand = true;
    result = 0.0;
    value = 0.0;
    operatorFlag = false;
    modOperator  = false;
    XYOperator   = false;
    addOperator  = false;
    minOperator  = false;
    mulOperator  = false;
    divOperator  = false;
    ui->display->setText("0"); //显示框中显示0
}

//计算的函数（函数的参数就是value，即是双目运算符的第2个操作数）
bool MainWindow::calculate(double operand)
{
    if(addOperator)
    {
        result += operand;
        addOperator = false;
    }
    else if(minOperator)
    {
        result -= operand;
        minOperator = false;
    }
    else if(mulOperator)
    {
        result *= operand;
        mulOperator = false;
    }
    else if(divOperator)
    {
        if(operand - 0 < 10e-6 )
            return false;
        result /= operand;
        divOperator = false;
    }
    else if(modOperator)
    {
        if(operand - 0 < 10e-6 )
            return false;
         result = (int)result%(int)operand;
         modOperator = false;
    }
    else if(XYOperator)
    {
        result = pow(result,operand);
        XYOperator = false;
    }
    waittingForOperand = true;
    ui->display->setText(QString::number(result));
    return true;
/*注意：是只有双目运算符才会调用这个calculate()函数来计算出结构并将结果显示在窗口上的，单目运算符是只在其所
  对应那个函数里面进行运算并打印出来的。 */
}

void MainWindow::abortOperation()//鍑虹幇閿欒
{
    buttonClearClicked();
    ui->display->setText(tr("ERROR!!!"));
}
