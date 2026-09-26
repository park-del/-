#ifndef MANAGEMENT_H
#define MANAGEMENT_H
#include <QDialog>
#include "ui_management.h"
#include "sqlite3.h"
class ManagementForm : public QDialog
{
Q_OBJECT
public:
ManagementForm(QWidget *parent = 0);
public:
void sql_for_tableWidget(char *, char *);
int selected_radio;
int table_status;
private:
Ui::Form ui;
private slots:
void on_importRecordradioButton_clicked();
void on_exportRecordradioButton_clicked();
void on_storageradioButton_clicked();
void on_refreshStorageButton_clicked();
void on_querypushButton_clicked();
void on_increaseradioButton_clicked();
void on_descendradioButton_clicked();
void on_addpushButton_clicked();
void on_deletepushButton_clicked();
void on_modifypushButton_clicked();
};
#endif
