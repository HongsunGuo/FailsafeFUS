/****************************************************************************
** Meta object code from reading C++ file 'qDOWriteThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../qDOWriteThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qDOWriteThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_qDOWriteThread_t {
    QByteArrayData data[9];
    char stringdata0[70];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_qDOWriteThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_qDOWriteThread_t qt_meta_stringdata_qDOWriteThread = {
    {
QT_MOC_LITERAL(0, 0, 14), // "qDOWriteThread"
QT_MOC_LITERAL(1, 15, 8), // "response"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 1), // "s"
QT_MOC_LITERAL(4, 27, 8), // "errorMsg"
QT_MOC_LITERAL(5, 36, 9), // "relayOpen"
QT_MOC_LITERAL(6, 46, 1), // "v"
QT_MOC_LITERAL(7, 48, 10), // "breakRelay"
QT_MOC_LITERAL(8, 59, 10) // "resetRelay"

    },
    "qDOWriteThread\0response\0\0s\0errorMsg\0"
    "relayOpen\0v\0breakRelay\0resetRelay"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_qDOWriteThread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       4,    1,   42,    2, 0x06 /* Public */,
       5,    1,   45,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   48,    2, 0x0a /* Public */,
       8,    0,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Bool,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void qDOWriteThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<qDOWriteThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->response((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->errorMsg((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->relayOpen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->breakRelay(); break;
        case 4: _t->resetRelay(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (qDOWriteThread::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&qDOWriteThread::response)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (qDOWriteThread::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&qDOWriteThread::errorMsg)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (qDOWriteThread::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&qDOWriteThread::relayOpen)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject qDOWriteThread::staticMetaObject = { {
    &QThread::staticMetaObject,
    qt_meta_stringdata_qDOWriteThread.data,
    qt_meta_data_qDOWriteThread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *qDOWriteThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *qDOWriteThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_qDOWriteThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int qDOWriteThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void qDOWriteThread::response(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void qDOWriteThread::errorMsg(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void qDOWriteThread::relayOpen(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
