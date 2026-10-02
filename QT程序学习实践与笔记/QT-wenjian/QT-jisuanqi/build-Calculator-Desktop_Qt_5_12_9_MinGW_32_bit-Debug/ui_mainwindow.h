/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonDel;
    QPushButton *buttonClear;
    QWidget *widget;
    QGridLayout *gridLayout;
    QPushButton *buttonSqrt;
    QPushButton *buttonEqual;
    QPushButton *buttonPow;
    QPushButton *buttonAbs;
    QPushButton *buttonDigital3;
    QPushButton *buttonDigital7;
    QPushButton *buttonMin;
    QPushButton *buttonPI;
    QPushButton *buttonDigital5;
    QPushButton *buttonBIN;
    QPushButton *buttonDigital8;
    QPushButton *buttonDigital4;
    QPushButton *buttonAdd;
    QPushButton *buttonDigital6;
    QPushButton *buttonSin;
    QPushButton *buttonX;
    QPushButton *buttonMod;
    QPushButton *buttonMul;
    QPushButton *buttonCos;
    QPushButton *buttonDigital0;
    QPushButton *buttonNega;
    QPushButton *buttonPoint;
    QPushButton *buttonTan;
    QPushButton *buttonDigital1;
    QPushButton *buttonDigital2;
    QPushButton *buttonXY;
    QPushButton *buttonDigital9;
    QPushButton *buttonDiv;
    QLineEdit *display;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(405, 484);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        widget_2 = new QWidget(centralwidget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(0, 56, 403, 72));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        buttonDel = new QPushButton(widget_2);
        buttonDel->setObjectName(QString::fromUtf8("buttonDel"));
        buttonDel->setMinimumSize(QSize(0, 40));
        buttonDel->setMaximumSize(QSize(16777215, 40));
        buttonDel->setFlat(false);

        horizontalLayout->addWidget(buttonDel);

        buttonClear = new QPushButton(widget_2);
        buttonClear->setObjectName(QString::fromUtf8("buttonClear"));
        buttonClear->setMinimumSize(QSize(0, 40));
        buttonClear->setMaximumSize(QSize(16777215, 40));
        buttonClear->setFlat(false);

        horizontalLayout->addWidget(buttonClear);

        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 134, 403, 334));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        buttonSqrt = new QPushButton(widget);
        buttonSqrt->setObjectName(QString::fromUtf8("buttonSqrt"));
        buttonSqrt->setMinimumSize(QSize(0, 40));
        buttonSqrt->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonSqrt, 2, 0, 1, 1);

        buttonEqual = new QPushButton(widget);
        buttonEqual->setObjectName(QString::fromUtf8("buttonEqual"));
        buttonEqual->setMinimumSize(QSize(0, 40));
        buttonEqual->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonEqual, 6, 3, 1, 1);

        buttonPow = new QPushButton(widget);
        buttonPow->setObjectName(QString::fromUtf8("buttonPow"));
        buttonPow->setMinimumSize(QSize(0, 40));
        buttonPow->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonPow, 2, 1, 1, 1);

        buttonAbs = new QPushButton(widget);
        buttonAbs->setObjectName(QString::fromUtf8("buttonAbs"));
        buttonAbs->setMinimumSize(QSize(0, 40));
        buttonAbs->setMaximumSize(QSize(16777215, 40));
        buttonAbs->setFlat(false);

        gridLayout->addWidget(buttonAbs, 0, 1, 1, 1);

        buttonDigital3 = new QPushButton(widget);
        buttonDigital3->setObjectName(QString::fromUtf8("buttonDigital3"));
        buttonDigital3->setMinimumSize(QSize(0, 40));
        buttonDigital3->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonDigital3, 3, 2, 1, 1);

        buttonDigital7 = new QPushButton(widget);
        buttonDigital7->setObjectName(QString::fromUtf8("buttonDigital7"));
        buttonDigital7->setMinimumSize(QSize(0, 40));
        buttonDigital7->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonDigital7, 5, 0, 1, 1);

        buttonMin = new QPushButton(widget);
        buttonMin->setObjectName(QString::fromUtf8("buttonMin"));
        buttonMin->setMinimumSize(QSize(0, 40));
        buttonMin->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonMin, 3, 3, 1, 1);

        buttonPI = new QPushButton(widget);
        buttonPI->setObjectName(QString::fromUtf8("buttonPI"));
        buttonPI->setMinimumSize(QSize(0, 40));
        buttonPI->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonPI, 6, 2, 1, 1);

        buttonDigital5 = new QPushButton(widget);
        buttonDigital5->setObjectName(QString::fromUtf8("buttonDigital5"));
        buttonDigital5->setMinimumSize(QSize(0, 40));
        buttonDigital5->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonDigital5, 4, 1, 1, 1);

        buttonBIN = new QPushButton(widget);
        buttonBIN->setObjectName(QString::fromUtf8("buttonBIN"));
        buttonBIN->setMinimumSize(QSize(0, 40));
        buttonBIN->setMaximumSize(QSize(16777215, 40));
        buttonBIN->setFlat(false);

        gridLayout->addWidget(buttonBIN, 0, 3, 1, 1);

        buttonDigital8 = new QPushButton(widget);
        buttonDigital8->setObjectName(QString::fromUtf8("buttonDigital8"));
        buttonDigital8->setMinimumSize(QSize(0, 40));
        buttonDigital8->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonDigital8, 5, 1, 1, 1);

        buttonDigital4 = new QPushButton(widget);
        buttonDigital4->setObjectName(QString::fromUtf8("buttonDigital4"));
        buttonDigital4->setMinimumSize(QSize(0, 40));
        buttonDigital4->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonDigital4, 4, 0, 1, 1);

        buttonAdd = new QPushButton(widget);
        buttonAdd->setObjectName(QString::fromUtf8("buttonAdd"));
        buttonAdd->setMinimumSize(QSize(0, 40));
        buttonAdd->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonAdd, 2, 3, 1, 1);

        buttonDigital6 = new QPushButton(widget);
        buttonDigital6->setObjectName(QString::fromUtf8("buttonDigital6"));
        buttonDigital6->setMinimumSize(QSize(0, 40));
        buttonDigital6->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonDigital6, 4, 2, 1, 1);

        buttonSin = new QPushButton(widget);
        buttonSin->setObjectName(QString::fromUtf8("buttonSin"));
        buttonSin->setMinimumSize(QSize(0, 40));
        buttonSin->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonSin, 1, 0, 1, 1);

        buttonX = new QPushButton(widget);
        buttonX->setObjectName(QString::fromUtf8("buttonX"));
        buttonX->setMinimumSize(QSize(0, 40));
        buttonX->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonX, 2, 2, 1, 1);

        buttonMod = new QPushButton(widget);
        buttonMod->setObjectName(QString::fromUtf8("buttonMod"));
        buttonMod->setMinimumSize(QSize(0, 40));
        buttonMod->setMaximumSize(QSize(16777215, 40));
        buttonMod->setIconSize(QSize(16, 16));
        buttonMod->setFlat(false);

        gridLayout->addWidget(buttonMod, 0, 0, 1, 1);

        buttonMul = new QPushButton(widget);
        buttonMul->setObjectName(QString::fromUtf8("buttonMul"));
        buttonMul->setMinimumSize(QSize(0, 40));
        buttonMul->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonMul, 4, 3, 1, 1);

        buttonCos = new QPushButton(widget);
        buttonCos->setObjectName(QString::fromUtf8("buttonCos"));
        buttonCos->setMinimumSize(QSize(0, 40));
        buttonCos->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonCos, 1, 1, 1, 1);

        buttonDigital0 = new QPushButton(widget);
        buttonDigital0->setObjectName(QString::fromUtf8("buttonDigital0"));
        buttonDigital0->setMinimumSize(QSize(0, 40));
        buttonDigital0->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonDigital0, 6, 1, 1, 1);

        buttonNega = new QPushButton(widget);
        buttonNega->setObjectName(QString::fromUtf8("buttonNega"));
        buttonNega->setMinimumSize(QSize(0, 40));
        buttonNega->setMaximumSize(QSize(16777215, 40));
        buttonNega->setFlat(false);

        gridLayout->addWidget(buttonNega, 1, 3, 1, 1);

        buttonPoint = new QPushButton(widget);
        buttonPoint->setObjectName(QString::fromUtf8("buttonPoint"));
        buttonPoint->setMinimumSize(QSize(0, 40));
        buttonPoint->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonPoint, 6, 0, 1, 1);

        buttonTan = new QPushButton(widget);
        buttonTan->setObjectName(QString::fromUtf8("buttonTan"));
        buttonTan->setMinimumSize(QSize(0, 40));
        buttonTan->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonTan, 1, 2, 1, 1);

        buttonDigital1 = new QPushButton(widget);
        buttonDigital1->setObjectName(QString::fromUtf8("buttonDigital1"));
        buttonDigital1->setMinimumSize(QSize(0, 40));
        buttonDigital1->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonDigital1, 3, 0, 1, 1);

        buttonDigital2 = new QPushButton(widget);
        buttonDigital2->setObjectName(QString::fromUtf8("buttonDigital2"));
        buttonDigital2->setMinimumSize(QSize(0, 40));
        buttonDigital2->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonDigital2, 3, 1, 1, 1);

        buttonXY = new QPushButton(widget);
        buttonXY->setObjectName(QString::fromUtf8("buttonXY"));
        buttonXY->setMinimumSize(QSize(0, 40));
        buttonXY->setMaximumSize(QSize(16777215, 40));
        buttonXY->setFlat(false);

        gridLayout->addWidget(buttonXY, 0, 2, 1, 1);

        buttonDigital9 = new QPushButton(widget);
        buttonDigital9->setObjectName(QString::fromUtf8("buttonDigital9"));
        buttonDigital9->setMinimumSize(QSize(0, 40));
        buttonDigital9->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonDigital9, 5, 2, 1, 1);

        buttonDiv = new QPushButton(widget);
        buttonDiv->setObjectName(QString::fromUtf8("buttonDiv"));
        buttonDiv->setMinimumSize(QSize(0, 40));
        buttonDiv->setMaximumSize(QSize(16777215, 40));

        gridLayout->addWidget(buttonDiv, 5, 3, 1, 1);

        display = new QLineEdit(centralwidget);
        display->setObjectName(QString::fromUtf8("display"));
        display->setGeometry(QRect(0, 0, 403, 50));
        display->setMinimumSize(QSize(341, 50));
        display->setMaximumSize(QSize(16777215, 16777215));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        buttonDel->setDefault(true);
        buttonClear->setDefault(true);
        buttonSqrt->setDefault(true);
        buttonEqual->setDefault(true);
        buttonPow->setDefault(true);
        buttonAbs->setDefault(true);
        buttonDigital3->setDefault(true);
        buttonDigital7->setDefault(true);
        buttonMin->setDefault(true);
        buttonPI->setDefault(true);
        buttonDigital5->setDefault(true);
        buttonBIN->setDefault(true);
        buttonDigital8->setDefault(true);
        buttonDigital4->setDefault(true);
        buttonAdd->setDefault(true);
        buttonDigital6->setDefault(true);
        buttonSin->setDefault(true);
        buttonX->setDefault(true);
        buttonMod->setDefault(true);
        buttonMul->setDefault(true);
        buttonCos->setDefault(true);
        buttonDigital0->setDefault(true);
        buttonNega->setDefault(true);
        buttonPoint->setDefault(true);
        buttonTan->setDefault(true);
        buttonDigital1->setDefault(true);
        buttonDigital2->setDefault(true);
        buttonXY->setDefault(true);
        buttonDigital9->setDefault(true);
        buttonDiv->setDefault(true);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        buttonDel->setText(QApplication::translate("MainWindow", "Del", nullptr));
        buttonClear->setText(QApplication::translate("MainWindow", "Clear", nullptr));
        buttonSqrt->setText(QApplication::translate("MainWindow", "Sqrt", nullptr));
        buttonEqual->setText(QApplication::translate("MainWindow", "=", nullptr));
        buttonPow->setText(QApplication::translate("MainWindow", "G", nullptr));
        buttonAbs->setText(QApplication::translate("MainWindow", "|x|", nullptr));
        buttonDigital3->setText(QApplication::translate("MainWindow", "3", nullptr));
        buttonDigital7->setText(QApplication::translate("MainWindow", "7", nullptr));
        buttonMin->setText(QApplication::translate("MainWindow", "-", nullptr));
        buttonPI->setText(QApplication::translate("MainWindow", "PI", nullptr));
        buttonDigital5->setText(QApplication::translate("MainWindow", "5", nullptr));
        buttonBIN->setText(QApplication::translate("MainWindow", "BIN", nullptr));
        buttonDigital8->setText(QApplication::translate("MainWindow", "8", nullptr));
        buttonDigital4->setText(QApplication::translate("MainWindow", "4", nullptr));
        buttonAdd->setText(QApplication::translate("MainWindow", "+", nullptr));
        buttonDigital6->setText(QApplication::translate("MainWindow", "6", nullptr));
        buttonSin->setText(QApplication::translate("MainWindow", "sin", nullptr));
        buttonX->setText(QApplication::translate("MainWindow", "1/x", nullptr));
        buttonMod->setText(QApplication::translate("MainWindow", "mod", nullptr));
        buttonMul->setText(QApplication::translate("MainWindow", "*", nullptr));
        buttonCos->setText(QApplication::translate("MainWindow", "cos", nullptr));
        buttonDigital0->setText(QApplication::translate("MainWindow", "0", nullptr));
        buttonNega->setText(QApplication::translate("MainWindow", "+/-", nullptr));
        buttonPoint->setText(QApplication::translate("MainWindow", ".", nullptr));
        buttonTan->setText(QApplication::translate("MainWindow", "tan", nullptr));
        buttonDigital1->setText(QApplication::translate("MainWindow", "1", nullptr));
        buttonDigital2->setText(QApplication::translate("MainWindow", "2", nullptr));
        buttonXY->setText(QApplication::translate("MainWindow", "x^y", nullptr));
        buttonDigital9->setText(QApplication::translate("MainWindow", "9", nullptr));
        buttonDiv->setText(QApplication::translate("MainWindow", "/", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
