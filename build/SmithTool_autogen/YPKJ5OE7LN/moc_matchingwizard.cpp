/****************************************************************************
** Meta object code from reading C++ file 'matchingwizard.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/ui/matchingwizard.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'matchingwizard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SmithTool__MatchingWizard_t {
    const uint offsetsAndSize[24];
    char stringdata0[144];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_SmithTool__MatchingWizard_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_SmithTool__MatchingWizard_t qt_meta_stringdata_SmithTool__MatchingWizard = {
    {
QT_MOC_LITERAL(0, 25), // "SmithTool::MatchingWizard"
QT_MOC_LITERAL(26, 16), // "solutionSelected"
QT_MOC_LITERAL(43, 0), // ""
QT_MOC_LITERAL(44, 16), // "MatchingSolution"
QT_MOC_LITERAL(61, 8), // "solution"
QT_MOC_LITERAL(70, 11), // "onCalculate"
QT_MOC_LITERAL(82, 18), // "onSolutionSelected"
QT_MOC_LITERAL(101, 3), // "row"
QT_MOC_LITERAL(105, 6), // "column"
QT_MOC_LITERAL(112, 7), // "onApply"
QT_MOC_LITERAL(120, 17), // "onTopologyChanged"
QT_MOC_LITERAL(138, 5) // "index"

    },
    "SmithTool::MatchingWizard\0solutionSelected\0"
    "\0MatchingSolution\0solution\0onCalculate\0"
    "onSolutionSelected\0row\0column\0onApply\0"
    "onTopologyChanged\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SmithTool__MatchingWizard[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   44,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   47,    2, 0x08,    3 /* Private */,
       6,    2,   48,    2, 0x08,    4 /* Private */,
       9,    0,   53,    2, 0x08,    7 /* Private */,
      10,    1,   54,    2, 0x08,    8 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,

       0        // eod
};

void SmithTool::MatchingWizard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MatchingWizard *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->solutionSelected((*reinterpret_cast< std::add_pointer_t<MatchingSolution>>(_a[1]))); break;
        case 1: _t->onCalculate(); break;
        case 2: _t->onSolutionSelected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 3: _t->onApply(); break;
        case 4: _t->onTopologyChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MatchingWizard::*)(const MatchingSolution & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MatchingWizard::solutionSelected)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject SmithTool::MatchingWizard::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_SmithTool__MatchingWizard.offsetsAndSize,
    qt_meta_data_SmithTool__MatchingWizard,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_SmithTool__MatchingWizard_t
, QtPrivate::TypeAndForceComplete<MatchingWizard, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const MatchingSolution &, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>


>,
    nullptr
} };


const QMetaObject *SmithTool::MatchingWizard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SmithTool::MatchingWizard::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SmithTool__MatchingWizard.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SmithTool::MatchingWizard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SmithTool::MatchingWizard::solutionSelected(const MatchingSolution & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
