/****************************************************************************
** Meta object code from reading C++ file 'impedanceinputpanel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/ui/impedanceinputpanel.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'impedanceinputpanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SmithTool__ImpedanceInputPanel_t {
    const uint offsetsAndSize[28];
    char stringdata0[210];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_SmithTool__ImpedanceInputPanel_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_SmithTool__ImpedanceInputPanel_t qt_meta_stringdata_SmithTool__ImpedanceInputPanel = {
    {
QT_MOC_LITERAL(0, 30), // "SmithTool::ImpedanceInputPanel"
QT_MOC_LITERAL(31, 22), // "sourceImpedanceChanged"
QT_MOC_LITERAL(54, 0), // ""
QT_MOC_LITERAL(55, 20), // "std::complex<double>"
QT_MOC_LITERAL(76, 2), // "zs"
QT_MOC_LITERAL(79, 20), // "loadImpedanceChanged"
QT_MOC_LITERAL(100, 2), // "zl"
QT_MOC_LITERAL(103, 9), // "z0Changed"
QT_MOC_LITERAL(113, 2), // "z0"
QT_MOC_LITERAL(116, 21), // "onSourceFormatChanged"
QT_MOC_LITERAL(138, 19), // "onLoadFormatChanged"
QT_MOC_LITERAL(158, 20), // "onSourceValueChanged"
QT_MOC_LITERAL(179, 18), // "onLoadValueChanged"
QT_MOC_LITERAL(198, 11) // "onZ0Changed"

    },
    "SmithTool::ImpedanceInputPanel\0"
    "sourceImpedanceChanged\0\0std::complex<double>\0"
    "zs\0loadImpedanceChanged\0zl\0z0Changed\0"
    "z0\0onSourceFormatChanged\0onLoadFormatChanged\0"
    "onSourceValueChanged\0onLoadValueChanged\0"
    "onZ0Changed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SmithTool__ImpedanceInputPanel[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   62,    2, 0x06,    1 /* Public */,
       5,    1,   65,    2, 0x06,    3 /* Public */,
       7,    1,   68,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       9,    0,   71,    2, 0x08,    7 /* Private */,
      10,    0,   72,    2, 0x08,    8 /* Private */,
      11,    0,   73,    2, 0x08,    9 /* Private */,
      12,    0,   74,    2, 0x08,   10 /* Private */,
      13,    0,   75,    2, 0x08,   11 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    6,
    QMetaType::Void, QMetaType::Double,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SmithTool::ImpedanceInputPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ImpedanceInputPanel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sourceImpedanceChanged((*reinterpret_cast< std::add_pointer_t<std::complex<double>>>(_a[1]))); break;
        case 1: _t->loadImpedanceChanged((*reinterpret_cast< std::add_pointer_t<std::complex<double>>>(_a[1]))); break;
        case 2: _t->z0Changed((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 3: _t->onSourceFormatChanged(); break;
        case 4: _t->onLoadFormatChanged(); break;
        case 5: _t->onSourceValueChanged(); break;
        case 6: _t->onLoadValueChanged(); break;
        case 7: _t->onZ0Changed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ImpedanceInputPanel::*)(std::complex<double> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImpedanceInputPanel::sourceImpedanceChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ImpedanceInputPanel::*)(std::complex<double> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImpedanceInputPanel::loadImpedanceChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ImpedanceInputPanel::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImpedanceInputPanel::z0Changed)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject SmithTool::ImpedanceInputPanel::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_SmithTool__ImpedanceInputPanel.offsetsAndSize,
    qt_meta_data_SmithTool__ImpedanceInputPanel,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_SmithTool__ImpedanceInputPanel_t
, QtPrivate::TypeAndForceComplete<ImpedanceInputPanel, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<std::complex<double>, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<std::complex<double>, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *SmithTool::ImpedanceInputPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SmithTool::ImpedanceInputPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SmithTool__ImpedanceInputPanel.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SmithTool::ImpedanceInputPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void SmithTool::ImpedanceInputPanel::sourceImpedanceChanged(std::complex<double> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SmithTool::ImpedanceInputPanel::loadImpedanceChanged(std::complex<double> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SmithTool::ImpedanceInputPanel::z0Changed(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
