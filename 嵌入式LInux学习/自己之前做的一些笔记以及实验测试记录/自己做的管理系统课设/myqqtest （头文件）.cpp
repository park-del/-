#ifndef MYQTTEST_H
#define MYQTTEST_H

#include <QtGui/QDialog>
#include "ui_myqttest.h"

class myQtTest : public QDialog
{
    Q_OBJECT

public:
    myQtTest(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~myQtTest();

private:
    Ui::Form ui;

};

#endif // MYQTTEST_H
