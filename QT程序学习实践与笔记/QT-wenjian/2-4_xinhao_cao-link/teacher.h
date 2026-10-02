#ifndef TEACHER_H
#define TEACHER_H

#include <QObject>

class Teacher : public QObject
{
    Q_OBJECT
public:
    explicit Teacher(QObject *parent = nullptr);



signals:
    void hungry();//信号hungry()在这里进行声明
    void hungry(QString foodname);  //对信号hungry()的重载


};

#endif // TEACHER_H
