/****************************************************************************
** Meta object code from reading C++ file 'managementform.h'
**
** Created: Sun Oct 13 23:25:45 2024
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "managementform.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'managementform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ManagementForm[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,
      53,   15,   15,   15, 0x08,
      90,   15,   15,   15, 0x08,
     122,   15,   15,   15, 0x08,
     156,   15,   15,   15, 0x08,
     185,   15,   15,   15, 0x08,
     218,   15,   15,   15, 0x08,
     250,   15,   15,   15, 0x08,
     277,   15,   15,   15, 0x08,
     307,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ManagementForm[] = {
    "ManagementForm\0\0on_importRecordradioButton_clicked()\0"
    "on_exportRecordradioButton_clicked()\0"
    "on_storageradioButton_clicked()\0"
    "on_refreshStorageButton_clicked()\0"
    "on_querypushButton_clicked()\0"
    "on_increaseradioButton_clicked()\0"
    "on_descendradioButton_clicked()\0"
    "on_addpushButton_clicked()\0"
    "on_deletepushButton_clicked()\0"
    "on_modifypushButton_clicked()\0"
};

const QMetaObject ManagementForm::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ManagementForm,
      qt_meta_data_ManagementForm, 0 }
};

const QMetaObject *ManagementForm::metaObject() const
{
    return &staticMetaObject;
}

void *ManagementForm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ManagementForm))
	return static_cast<void*>(const_cast< ManagementForm*>(this));
    return QDialog::qt_metacast(_clname);
}

int ManagementForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_importRecordradioButton_clicked(); break;
        case 1: on_exportRecordradioButton_clicked(); break;
        case 2: on_storageradioButton_clicked(); break;
        case 3: on_refreshStorageButton_clicked(); break;
        case 4: on_querypushButton_clicked(); break;
        case 5: on_increaseradioButton_clicked(); break;
        case 6: on_descendradioButton_clicked(); break;
        case 7: on_addpushButton_clicked(); break;
        case 8: on_deletepushButton_clicked(); break;
        case 9: on_modifypushButton_clicked(); break;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
