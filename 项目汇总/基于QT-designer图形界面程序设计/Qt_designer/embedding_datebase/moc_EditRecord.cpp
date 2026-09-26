/****************************************************************************
** Meta object code from reading C++ file 'EditRecord.h'
**
** Created: Wed May 11 18:28:45 2011
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "EditRecord.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EditRecord.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EditRecordDlg[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      27,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_EditRecordDlg[] = {
    "EditRecordDlg\0\0okClicked()\0cancleClicked()\0"
};

const QMetaObject EditRecordDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_EditRecordDlg,
      qt_meta_data_EditRecordDlg, 0 }
};

const QMetaObject *EditRecordDlg::metaObject() const
{
    return &staticMetaObject;
}

void *EditRecordDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EditRecordDlg))
	return static_cast<void*>(const_cast< EditRecordDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int EditRecordDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: okClicked(); break;
        case 1: cancleClicked(); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
