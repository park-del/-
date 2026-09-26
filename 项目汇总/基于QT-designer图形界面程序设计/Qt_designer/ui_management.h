/********************************************************************************
** Form generated from reading ui file 'management.ui'
**
** Created: Sun Oct 13 23:25:45 2024
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MANAGEMENT_H
#define UI_MANAGEMENT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QTableWidget *tableWidget;
    QPushButton *refreshStorageButton;
    QGroupBox *tablegroupBox;
    QRadioButton *importRecordradioButton;
    QRadioButton *exportRecordradioButton;
    QRadioButton *storageradioButton;
    QGroupBox *filterquerygroupBox;
    QComboBox *columncomboBox;
    QLabel *orderlabel;
    QLabel *label;
    QLineEdit *pricelineEdit;
    QRadioButton *increaseradioButton;
    QComboBox *andorcomboBox;
    QComboBox *columncomboBox2;
    QLabel *biglabel;
    QLabel *littlelabel;
    QRadioButton *descendradioButton;
    QLineEdit *biglineEdit;
    QPushButton *querypushButton;
    QLineEdit *littlelineEdit;
    QLineEdit *textlineEdit;
    QPushButton *closepushButton;
    QGroupBox *operategroupBox;
    QPushButton *addpushButton;
    QPushButton *deletepushButton;
    QPushButton *modifypushButton;

    void setupUi(QWidget *Form)
    {
    if (Form->objectName().isEmpty())
        Form->setObjectName(QString::fromUtf8("Form"));
    Form->resize(630, 410);
    tableWidget = new QTableWidget(Form);
    if (tableWidget->columnCount() < 4)
        tableWidget->setColumnCount(4);
    QTableWidgetItem *__colItem = new QTableWidgetItem();
    tableWidget->setHorizontalHeaderItem(0, __colItem);
    QTableWidgetItem *__colItem1 = new QTableWidgetItem();
    tableWidget->setHorizontalHeaderItem(1, __colItem1);
    QTableWidgetItem *__colItem2 = new QTableWidgetItem();
    tableWidget->setHorizontalHeaderItem(2, __colItem2);
    QTableWidgetItem *__colItem3 = new QTableWidgetItem();
    tableWidget->setHorizontalHeaderItem(3, __colItem3);
    tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
    tableWidget->setGeometry(QRect(10, 10, 411, 271));
    refreshStorageButton = new QPushButton(Form);
    refreshStorageButton->setObjectName(QString::fromUtf8("refreshStorageButton"));
    refreshStorageButton->setGeometry(QRect(20, 380, 151, 27));
    tablegroupBox = new QGroupBox(Form);
    tablegroupBox->setObjectName(QString::fromUtf8("tablegroupBox"));
    tablegroupBox->setGeometry(QRect(10, 280, 201, 91));
    importRecordradioButton = new QRadioButton(tablegroupBox);
    importRecordradioButton->setObjectName(QString::fromUtf8("importRecordradioButton"));
    importRecordradioButton->setGeometry(QRect(10, 17, 131, 23));
    exportRecordradioButton = new QRadioButton(tablegroupBox);
    exportRecordradioButton->setObjectName(QString::fromUtf8("exportRecordradioButton"));
    exportRecordradioButton->setGeometry(QRect(10, 40, 151, 23));
    storageradioButton = new QRadioButton(tablegroupBox);
    storageradioButton->setObjectName(QString::fromUtf8("storageradioButton"));
    storageradioButton->setGeometry(QRect(10, 64, 151, 23));
    filterquerygroupBox = new QGroupBox(Form);
    filterquerygroupBox->setObjectName(QString::fromUtf8("filterquerygroupBox"));
    filterquerygroupBox->setGeometry(QRect(427, 20, 201, 301));
    columncomboBox = new QComboBox(filterquerygroupBox);
    columncomboBox->setObjectName(QString::fromUtf8("columncomboBox"));
    columncomboBox->setGeometry(QRect(26, 20, 101, 25));
    orderlabel = new QLabel(filterquerygroupBox);
    orderlabel->setObjectName(QString::fromUtf8("orderlabel"));
    orderlabel->setGeometry(QRect(26, 150, 61, 21));
    label = new QLabel(filterquerygroupBox);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(6, 50, 16, 16));
    pricelineEdit = new QLineEdit(filterquerygroupBox);
    pricelineEdit->setObjectName(QString::fromUtf8("pricelineEdit"));
    pricelineEdit->setGeometry(QRect(74, 120, 51, 21));
    increaseradioButton = new QRadioButton(filterquerygroupBox);
    increaseradioButton->setObjectName(QString::fromUtf8("increaseradioButton"));
    increaseradioButton->setEnabled(true);
    increaseradioButton->setGeometry(QRect(26, 207, 92, 23));
    andorcomboBox = new QComboBox(filterquerygroupBox);
    andorcomboBox->setObjectName(QString::fromUtf8("andorcomboBox"));
    andorcomboBox->setGeometry(QRect(26, 80, 101, 25));
    columncomboBox2 = new QComboBox(filterquerygroupBox);
    columncomboBox2->setObjectName(QString::fromUtf8("columncomboBox2"));
    columncomboBox2->setGeometry(QRect(26, 170, 101, 25));
    biglabel = new QLabel(filterquerygroupBox);
    biglabel->setObjectName(QString::fromUtf8("biglabel"));
    biglabel->setGeometry(QRect(129, 122, 16, 16));
    littlelabel = new QLabel(filterquerygroupBox);
    littlelabel->setObjectName(QString::fromUtf8("littlelabel"));
    littlelabel->setGeometry(QRect(62, 122, 16, 16));
    descendradioButton = new QRadioButton(filterquerygroupBox);
    descendradioButton->setObjectName(QString::fromUtf8("descendradioButton"));
    descendradioButton->setGeometry(QRect(26, 230, 92, 23));
    biglineEdit = new QLineEdit(filterquerygroupBox);
    biglineEdit->setObjectName(QString::fromUtf8("biglineEdit"));
    biglineEdit->setGeometry(QRect(140, 120, 51, 21));
    querypushButton = new QPushButton(filterquerygroupBox);
    querypushButton->setObjectName(QString::fromUtf8("querypushButton"));
    querypushButton->setGeometry(QRect(40, 260, 75, 27));
    littlelineEdit = new QLineEdit(filterquerygroupBox);
    littlelineEdit->setObjectName(QString::fromUtf8("littlelineEdit"));
    littlelineEdit->setGeometry(QRect(6, 120, 51, 21));
    textlineEdit = new QLineEdit(filterquerygroupBox);
    textlineEdit->setObjectName(QString::fromUtf8("textlineEdit"));
    textlineEdit->setGeometry(QRect(26, 50, 101, 23));
    closepushButton = new QPushButton(Form);
    closepushButton->setObjectName(QString::fromUtf8("closepushButton"));
    closepushButton->setGeometry(QRect(440, 340, 151, 27));
    operategroupBox = new QGroupBox(Form);
    operategroupBox->setObjectName(QString::fromUtf8("operategroupBox"));
    operategroupBox->setGeometry(QRect(250, 280, 141, 121));
    addpushButton = new QPushButton(operategroupBox);
    addpushButton->setObjectName(QString::fromUtf8("addpushButton"));
    addpushButton->setGeometry(QRect(10, 18, 121, 27));
    deletepushButton = new QPushButton(operategroupBox);
    deletepushButton->setObjectName(QString::fromUtf8("deletepushButton"));
    deletepushButton->setGeometry(QRect(10, 52, 121, 27));
    modifypushButton = new QPushButton(operategroupBox);
    modifypushButton->setObjectName(QString::fromUtf8("modifypushButton"));
    modifypushButton->setGeometry(QRect(10, 90, 121, 27));
    QWidget::setTabOrder(refreshStorageButton, tableWidget);

    retranslateUi(Form);
    QObject::connect(closepushButton, SIGNAL(clicked()), Form, SLOT(close()));

    QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
    Form->setWindowTitle(QApplication::translate("Form", "Database Management System", 0, QApplication::UnicodeUTF8));
    tableWidget->horizontalHeaderItem(0)->setText(QApplication::translate("Form", "ID", 0, QApplication::UnicodeUTF8));
    tableWidget->horizontalHeaderItem(1)->setText(QApplication::translate("Form", "name", 0, QApplication::UnicodeUTF8));
    tableWidget->horizontalHeaderItem(2)->setText(QApplication::translate("Form", "producer", 0, QApplication::UnicodeUTF8));
    tableWidget->horizontalHeaderItem(3)->setText(QApplication::translate("Form", "price", 0, QApplication::UnicodeUTF8));
    refreshStorageButton->setText(QApplication::translate("Form", "refresh storage", 0, QApplication::UnicodeUTF8));
    tablegroupBox->setTitle(QApplication::translate("Form", "table", 0, QApplication::UnicodeUTF8));
    importRecordradioButton->setText(QApplication::translate("Form", "import records", 0, QApplication::UnicodeUTF8));
    exportRecordradioButton->setText(QApplication::translate("Form", "export records", 0, QApplication::UnicodeUTF8));
    storageradioButton->setText(QApplication::translate("Form", "storage", 0, QApplication::UnicodeUTF8));
    filterquerygroupBox->setTitle(QApplication::translate("Form", "filter query", 0, QApplication::UnicodeUTF8));
    columncomboBox->insertItems(0, QStringList()
     << QApplication::translate("Form", "ID", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Form", "name", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Form", "producer", 0, QApplication::UnicodeUTF8)
    );
    orderlabel->setText(QApplication::translate("Form", "order by:", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Form", "=", 0, QApplication::UnicodeUTF8));
    pricelineEdit->setText(QApplication::translate("Form", "price", 0, QApplication::UnicodeUTF8));
    increaseradioButton->setText(QApplication::translate("Form", "increase", 0, QApplication::UnicodeUTF8));
    andorcomboBox->insertItems(0, QStringList()
     << QApplication::translate("Form", "and", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Form", "or", 0, QApplication::UnicodeUTF8)
    );
    columncomboBox2->insertItems(0, QStringList()
     << QApplication::translate("Form", "ID", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Form", "name", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Form", "producer", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Form", "price", 0, QApplication::UnicodeUTF8)
    );
    biglabel->setText(QApplication::translate("Form", "<", 0, QApplication::UnicodeUTF8));
    littlelabel->setText(QApplication::translate("Form", "<", 0, QApplication::UnicodeUTF8));
    descendradioButton->setText(QApplication::translate("Form", "descend", 0, QApplication::UnicodeUTF8));
    querypushButton->setText(QApplication::translate("Form", "query", 0, QApplication::UnicodeUTF8));
    closepushButton->setText(QApplication::translate("Form", "close", 0, QApplication::UnicodeUTF8));
    operategroupBox->setTitle(QApplication::translate("Form", "operate", 0, QApplication::UnicodeUTF8));
    addpushButton->setText(QApplication::translate("Form", "add", 0, QApplication::UnicodeUTF8));
    deletepushButton->setText(QApplication::translate("Form", "delete", 0, QApplication::UnicodeUTF8));
    modifypushButton->setText(QApplication::translate("Form", "modify", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Form);
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MANAGEMENT_H
