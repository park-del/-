#include "student.h"
#include<QDebug>

Student::Student(QObject *parent) : QObject(parent)
{

}
void Student::treat() //槽函数treat()在这里实现
{
    qDebug()<<"请老师吃饭";
}
