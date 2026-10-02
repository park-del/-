#ifndef SMALLWIDGET_H
#define SMALLWIDGET_H

#include <QWidget>

namespace Ui {
class Smallwidget;
}

class Smallwidget : public QWidget
{
    Q_OBJECT

public:
    explicit Smallwidget(QWidget *parent = nullptr);

/*为自定义控件提供封装的接口，这些接口其实就是成员函数，使得用户可以通过调动这些成员函数来
  对控件实现相应的功能。    */
    void setNum(int num); //设置a1这一个控件上所显示数字
    int  getNum();           //获取a1这一个控件上所显示的数字

    ~Smallwidget();
private:
    Ui::Smallwidget *ui;
};

#endif // SMALLWIDGET_H
