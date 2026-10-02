#ifndef TEACHER_H
#define TEACHER_H

#include <QObject>

class Teacher : public QObject
{
    Q_OBJECT
public:
    explicit Teacher(QObject *parent = nullptr);
    /*
     信号signal说明：
       自定义信号写到signals下；
       没有返回值，为void类型；
       信号是只需要进行声明，不需要实现，不需要实现的意思就是函数体为空；
       可以有参数（可以重载）
    */


signals:        //检测的信号需要写在这个signals下面，老师检测饿了的信号，故需要写一个信号void hugry()
    void hungry();//信号hungry()在这里进行声明

//public slots:  需要执行的槽函数写在这里，老师只是通知信号的发送者，并不需要槽函数，故不写
};

#endif // TEACHER_H
