#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <teacher.h>
#include <student.h>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void classisover();
private:
    Teacher *zt;  //定义一个老师和学生类的对象
    Student *st;
};
#endif // WIDGET_H
