/****************************************************************************
** Meta object code from reading C++ file 'componentpanel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/ui/componentpanel.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'componentpanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SmithTool__ComponentPanel_t {
    const uint offsetsAndSize[16];
    char stringdata0[96];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_SmithTool__ComponentPanel_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_SmithTool__ComponentPanel_t qt_meta_stringdata_SmithTool__ComponentPanel = {
    {
QT_MOC_LITERAL(0, 25), // "SmithTool::ComponentPanel"
QT_MOC_LITERAL(26, 16), // "frequencyChanged"
QT_MOC_LITERAL(43, 0), // ""
QT_MOC_LITERAL(44, 7), // "freq_hz"
QT_MOC_LITERAL(52, 9), // "z0Changed"
QT_MOC_LITERAL(62, 2), // "z0"
QT_MOC_LITERAL(65, 18), // "onFrequencyChanged"
QT_MOC_LITERAL(84, 11) // "onZ0Changed"

    },
    "SmithTool::ComponentPanel\0frequencyChanged\0"
    "\0freq_hz\0z0Changed\0z0\0onFrequencyChanged\0"
    "onZ0Changed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SmithTool__ComponentPanel[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x06,    1 /* Public */,
       4,    1,   41,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       6,    0,   44,    2, 0x08,    5 /* Private */,
       7,    0,   45,    2, 0x08,    6 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SmithTool::ComponentPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ComponentPanel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->frequencyChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 1: _t->z0Changed((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 2: _t->onFrequencyChanged(); break;
        case 3: _t->onZ0Changed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ComponentPanel::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ComponentPanel::frequencyChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ComponentPanel::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ComponentPanel::z0Changed)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject SmithTool::ComponentPanel::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_SmithTool__ComponentPanel.offsetsAndSize,
    qt_meta_data_SmithTool__ComponentPanel,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_SmithTool__ComponentPanel_t
, QtPrivate::TypeAndForceComplete<ComponentPanel, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *SmithTool::ComponentPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SmithTool::ComponentPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SmithTool__ComponentPanel.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SmithTool::ComponentPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SmithTool::ComponentPanel::frequencyChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SmithTool::ComponentPanel::z0Changed(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
