#ifndef STUDENT_H
#define STUDENT_H

#include <QObject>

class Student : public QObject
{
    Q_OBJECT
public:
    explicit Student(QObject *parent = nullptr);

    /*
       槽函数说明：
       没有返回值，即类型是void；
       需要声明，也需要实现；
       可以有参数（）可以发生重载；
       注意:信号和槽函数都是一个函数，其中信号只需要声明，不需要实现，而槽函数既需要声明也需要事先。
       声明是在类体中进行的，即是在某一个类的.h头文件中完成的；而实现是在类体外进行的，即是在某一个类的.cpp源文件
       中进行的。
    */

    //学生是通知信号的接收者，并不需要发送通知信号或者检测signal信号，故对于学生只写槽函数，signal信号不写。


signals:

public slots:
    void treat();

};

#endif // STUDENT_H
