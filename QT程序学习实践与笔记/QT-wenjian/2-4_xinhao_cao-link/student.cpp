#include "student.h"
#include<QDebug>

Student::Student(QObject *parent) : QObject(parent)
{

}
void Student::treat() //槽函数treat()在这里实现
{
    qDebug()<<"请老师吃饭";
}

void Student::treat(QString foodname)  //槽函数treat()的重载
{
    qDebug()<<"请老师吃饭，老师要吃:"<<foodname.toUtf8().data();
/*注意：利用qDebug输出QString类型的字符串会带有引号“”；但是利用qDebug输出char *类型的字符串时却不会带有
       引号。
       故要想使输出的"宫保鸡丁"不带有引号，可以利用foodName.toUtf8().data()这种方式。
       foodName调用toUtf8()时会转化为QByteArray,再调用data()时才会转化为char *类型。
       QString转化为char *类型时必须先转化为QByteArray，然后才能转化为char *类型。
       因为char *类型的转化在QByteArray类中是有相应的成员函数的。
*/
}
