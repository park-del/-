#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void timerEvent(QTimerEvent *);  //重写定时器的事件
    int id_1; //存放定时器1的唯一标识符，可以认为就是id_1定时器
    int id_2; //存放定时器2的唯一标识符，可以认为就是id_2定时器
    int id_3; //存放定时器3的唯一标识符，可以认为就是id_3定时器
private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
