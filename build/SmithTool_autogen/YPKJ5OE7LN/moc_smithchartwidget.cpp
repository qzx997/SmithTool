/****************************************************************************
** Meta object code from reading C++ file 'smithchartwidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/ui/smithchartwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'smithchartwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SmithTool__SmithChartWidget_t {
    const uint offsetsAndSize[50];
    char stringdata0[315];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_SmithTool__SmithChartWidget_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_SmithTool__SmithChartWidget_t qt_meta_stringdata_SmithTool__SmithChartWidget = {
    {
QT_MOC_LITERAL(0, 27), // "SmithTool::SmithChartWidget"
QT_MOC_LITERAL(28, 12), // "pointClicked"
QT_MOC_LITERAL(41, 0), // ""
QT_MOC_LITERAL(42, 20), // "std::complex<double>"
QT_MOC_LITERAL(63, 5), // "gamma"
QT_MOC_LITERAL(69, 1), // "z"
QT_MOC_LITERAL(71, 12), // "pointHovered"
QT_MOC_LITERAL(84, 25), // "componentValuesCalculated"
QT_MOC_LITERAL(110, 4), // "freq"
QT_MOC_LITERAL(115, 1), // "r"
QT_MOC_LITERAL(117, 1), // "l"
QT_MOC_LITERAL(119, 1), // "c"
QT_MOC_LITERAL(121, 19), // "targetPointSelected"
QT_MOC_LITERAL(141, 13), // "ComponentType"
QT_MOC_LITERAL(155, 4), // "type"
QT_MOC_LITERAL(160, 14), // "ConnectionType"
QT_MOC_LITERAL(175, 4), // "conn"
QT_MOC_LITERAL(180, 19), // "elementValueDragged"
QT_MOC_LITERAL(200, 12), // "segmentIndex"
QT_MOC_LITERAL(213, 8), // "newValue"
QT_MOC_LITERAL(222, 15), // "dragEditStarted"
QT_MOC_LITERAL(238, 13), // "dragEditEnded"
QT_MOC_LITERAL(252, 18), // "resetViewRequested"
QT_MOC_LITERAL(271, 22), // "deleteElementRequested"
QT_MOC_LITERAL(294, 20) // "editElementRequested"

    },
    "SmithTool::SmithChartWidget\0pointClicked\0"
    "\0std::complex<double>\0gamma\0z\0"
    "pointHovered\0componentValuesCalculated\0"
    "freq\0r\0l\0c\0targetPointSelected\0"
    "ComponentType\0type\0ConnectionType\0"
    "conn\0elementValueDragged\0segmentIndex\0"
    "newValue\0dragEditStarted\0dragEditEnded\0"
    "resetViewRequested\0deleteElementRequested\0"
    "editElementRequested"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SmithTool__SmithChartWidget[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   74,    2, 0x06,    1 /* Public */,
       6,    2,   79,    2, 0x06,    4 /* Public */,
       7,    4,   84,    2, 0x06,    7 /* Public */,
      12,    4,   93,    2, 0x06,   12 /* Public */,
      17,    2,  102,    2, 0x06,   17 /* Public */,
      20,    1,  107,    2, 0x06,   20 /* Public */,
      21,    0,  110,    2, 0x06,   22 /* Public */,
      22,    0,  111,    2, 0x06,   23 /* Public */,
      23,    1,  112,    2, 0x06,   24 /* Public */,
      24,    1,  115,    2, 0x06,   26 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    5,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    5,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double,    8,    9,   10,   11,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 13, 0x80000000 | 15,    4,    5,   14,   16,
    QMetaType::Void, QMetaType::Int, QMetaType::Double,   18,   19,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Int,   18,

       0        // eod
};

void SmithTool::SmithChartWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SmithChartWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->pointClicked((*reinterpret_cast< std::add_pointer_t<std::complex<double>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::complex<double>>>(_a[2]))); break;
        case 1: _t->pointHovered((*reinterpret_cast< std::add_pointer_t<std::complex<double>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::complex<double>>>(_a[2]))); break;
        case 2: _t->componentValuesCalculated((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4]))); break;
        case 3: _t->targetPointSelected((*reinterpret_cast< std::add_pointer_t<std::complex<double>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::complex<double>>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<ComponentType>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<ConnectionType>>(_a[4]))); break;
        case 4: _t->elementValueDragged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        case 5: _t->dragEditStarted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->dragEditEnded(); break;
        case 7: _t->resetViewRequested(); break;
        case 8: _t->deleteElementRequested((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->editElementRequested((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SmithChartWidget::*)(std::complex<double> , std::complex<double> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmithChartWidget::pointClicked)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SmithChartWidget::*)(std::complex<double> , std::complex<double> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmithChartWidget::pointHovered)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SmithChartWidget::*)(double , double , double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmithChartWidget::componentValuesCalculated)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SmithChartWidget::*)(std::complex<double> , std::complex<double> , ComponentType , ConnectionType );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmithChartWidget::targetPointSelected)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SmithChartWidget::*)(int , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmithChartWidget::elementValueDragged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (SmithChartWidget::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmithChartWidget::dragEditStarted)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (SmithChartWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmithChartWidget::dragEditEnded)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (SmithChartWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmithChartWidget::resetViewRequested)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (SmithChartWidget::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmithChartWidget::deleteElementRequested)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (SmithChartWidget::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmithChartWidget::editElementRequested)) {
                *result = 9;
                return;
            }
        }
    }
}

const QMetaObject SmithTool::SmithChartWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_SmithTool__SmithChartWidget.offsetsAndSize,
    qt_meta_data_SmithTool__SmithChartWidget,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_SmithTool__SmithChartWidget_t
, QtPrivate::TypeAndForceComplete<SmithChartWidget, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<std::complex<double>, std::false_type>, QtPrivate::TypeAndForceComplete<std::complex<double>, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<std::complex<double>, std::false_type>, QtPrivate::TypeAndForceComplete<std::complex<double>, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<std::complex<double>, std::false_type>, QtPrivate::TypeAndForceComplete<std::complex<double>, std::false_type>, QtPrivate::TypeAndForceComplete<ComponentType, std::false_type>, QtPrivate::TypeAndForceComplete<ConnectionType, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>



>,
    nullptr
} };


const QMetaObject *SmithTool::SmithChartWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SmithTool::SmithChartWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SmithTool__SmithChartWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SmithTool::SmithChartWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void SmithTool::SmithChartWidget::pointClicked(std::complex<double> _t1, std::complex<double> _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SmithTool::SmithChartWidget::pointHovered(std::complex<double> _t1, std::complex<double> _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SmithTool::SmithChartWidget::componentValuesCalculated(double _t1, double _t2, double _t3, double _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SmithTool::SmithChartWidget::targetPointSelected(std::complex<double> _t1, std::complex<double> _t2, ComponentType _t3, ConnectionType _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SmithTool::SmithChartWidget::elementValueDragged(int _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void SmithTool::SmithChartWidget::dragEditStarted(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void SmithTool::SmithChartWidget::dragEditEnded()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void SmithTool::SmithChartWidget::resetViewRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void SmithTool::SmithChartWidget::deleteElementRequested(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void SmithTool::SmithChartWidget::editElementRequested(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
