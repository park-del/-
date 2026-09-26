#ifndef MANAGEMENT_H
#define MANAGEMENT_H
#include <QDialog>

#include "ui_management.h"    //利用到了ui设计界面，即是在本类中的一个数据成员是为Ui::Form类的成员
#include "sqlite3.h"          //利用到了sqlite数据库

class ManagementForm : public QDialog
{
Q_OBJECT
public:
ManagementForm(QWidget *parent = 0);  //构造函数

public:                               //类中的数据成员和成员函数
void sql_for_tableWidget(char *, char *);  //这个成员函数是在下面的槽函数中被调用的
int selected_radio;          //升序还是降序的一个标志位
int table_status;            //表的状态

private:                              //ui界面
Ui::Form ui;

private slots:                        //槽函数

//是有3张表的，其中入库单就是为一张表的、出库单就是为一张表的、库存也是为一张表的
void on_importRecordradioButton_clicked();      //入库单
void on_exportRecordradioButton_clicked();      //出库单
void on_storageradioButton_clicked();           //库存

void on_refreshStorageButton_clicked();         //更新库存
void on_querypushButton_clicked();              //查询

void on_increaseradioButton_clicked();          //增加
void on_descendradioButton_clicked();           //删除

void on_addpushButton_clicked();                //增加记录
void on_deletepushButton_clicked();             //删除记录
void on_modifypushButton_clicked();             //修改记录
};
#endif
