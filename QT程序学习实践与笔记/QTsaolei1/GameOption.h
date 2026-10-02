#ifndef GAMEOPTION_H_
#define GAMEOPTION_H_
#include <QDialog>
#include <QPushButton>
#include <QGroupBox>
#include <QRadioButton>
#include <QCloseEvent>

class GameOption:public QDialog
{
    Q_OBJECT
private:
    int levels;       //难度等级，1为10个雷，2为20个，3为30个
    QPushButton  *ok_button;     //开始按键
    QRadioButton *easy_button;   //容易按键
    QRadioButton *mid_button;    //中等按键
    QRadioButton *hard_button;   //困难按键
    QGroupBox    *group;         //
public:
    GameOption(QWidget* parent = 0);  //该类的构造函数
protected:
    void closeEvent(QCloseEvent*);   /*QCloseEvent事件是指，当你鼠标点击窗口右上角的关闭按钮时，所触发
                                       的函数*/
private slots:
    void chose_level();              //
signals:
    void new_level(int);             //信号

};
#endif
