#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QLineEdit>
#include <QSystemTrayIcon>
#include <QPushButton>
#include <QGridLayout>
#include <QWidget>
namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    bool waittingForOperand;
    bool addOperator;                                //加法操作符
    bool minOperator;                                //减法操作符
    bool mulOperator;                                //乘法操作符
    bool divOperator;                                //除法操作符
    bool XYOperator;                                 //X的Y次方操作符
    bool modOperator;                                //余数操作符
    bool operatorFlag;                               //操作符标志
    bool calculate(double operand, const QString &operatorAct);
    bool calculate(double operand);
    void abortOperation();
    double value;
    double result;
private slots:  //自定义的一些槽函数
    void buttonDigitalClicked();                        //数字按键
    void buttonNegativeClicked();                       //正负号切换
    void buttonSinClicked();                            //三角函数sin
    void buttonCosClicked();                            //三角函数cos
    void buttonTanClicked();                            //三角函数tan
    void buttonBINClicked();                            //十进制转二进制
    void buttonAbsClicked();                            //绝对值
    void buttonSqrtClicked();                           //开方
    void buttonPowClicked();                            //平方
    void buttonXClicked();                              //倒数
    void buttonXYClicked();                             //X的Y次方
    void buttonModClicked();                            //取余
    void buttonEqualClicked();                          //等
    void buttonAddClicked();                            //加法
    void buttonMinClicked();                            //减法
    void buttonMulClicked();                            //乘法
    void buttonDivClicked();                            //除法
    void buttonPointClicked();                          //小数点
    void buttonPIClicked();                             //PI
    void buttonDelClicked();                            //回退
    void buttonClearClicked();                          //清空
};

#endif  //MAINWINDOW_H
