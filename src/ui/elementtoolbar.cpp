/**
 * @file elementtoolbar.cpp
 * @brief Element toolbar implementation
 */

#include "elementtoolbar.h"
#include <QHBoxLayout>
#include <QWidget>

namespace SmithTool {

ElementToolbar::ElementToolbar(QWidget* parent)
    : QToolBar(parent)
    , m_currentType(ComponentType::Inductor)
    , m_frequency(1e9)
{
    setupUI();
    setWindowTitle(tr("Elements"));
}

void ElementToolbar::setupUI()
{
    // Value input widget
    QWidget* valueWidget = new QWidget(this);
    QHBoxLayout* valueLayout = new QHBoxLayout(valueWidget);
    valueLayout->setContentsMargins(5, 0, 5, 0);
    
    m_valueSpinBox = new QDoubleSpinBox(this);
    m_valueSpinBox->setRange(0.001, 99999);
    m_valueSpinBox->setDecimals(3);
    m_valueSpinBox->setValue(10.0);
    m_valueSpinBox->setMaximumWidth(80);
    
    m_unitCombo = new QComboBox(this);
    m_unitCombo->addItems({"nH", "µH", "mH", "pF", "nF", "µF", "Ω", "kΩ"});
    m_unitCombo->setCurrentIndex(0);
    m_unitCombo->setMaximumWidth(60);
    
    valueLayout->addWidget(m_valueSpinBox);
    valueLayout->addWidget(m_unitCombo);
    
    addWidget(valueWidget);
    addSeparator();
    
    // Series element actions
    m_seriesLAction = new QAction(tr("串联L"), this);
    m_seriesLAction->setToolTip(tr("添加串联电感"));
    addAction(m_seriesLAction);
    
    m_seriesCAction = new QAction(tr("串联C"), this);
    m_seriesCAction->setToolTip(tr("添加串联电容"));
    addAction(m_seriesCAction);
    
    m_seriesRAction = new QAction(tr("串联R"), this);
    m_seriesRAction->setToolTip(tr("添加串联电阻"));
    addAction(m_seriesRAction);
    
    addSeparator();
    
    // Shunt element actions
    m_shuntLAction = new QAction(tr("并联L"), this);
    m_shuntLAction->setToolTip(tr("添加并联电感"));
    addAction(m_shuntLAction);
    
    m_shuntCAction = new QAction(tr("并联C"), this);
    m_shuntCAction->setToolTip(tr("添加并联电容"));
    addAction(m_shuntCAction);
    
    m_shuntRAction = new QAction(tr("并联R"), this);
    m_shuntRAction->setToolTip(tr("添加并联电阻"));
    addAction(m_shuntRAction);
    
    addSeparator();
    
    // Undo and clear
    m_undoAction = new QAction(tr("撤销"), this);
    m_undoAction->setShortcut(QKeySequence::Undo);
    addAction(m_undoAction);
    
    m_clearAction = new QAction(tr("清除"), this);
    addAction(m_clearAction);
    
    // Connections
    connect(m_seriesLAction, &QAction::triggered, this, &ElementToolbar::onAddSeriesL);
    connect(m_seriesCAction, &QAction::triggered, this, &ElementToolbar::onAddSeriesC);
    connect(m_seriesRAction, &QAction::triggered, this, &ElementToolbar::onAddSeriesR);
    connect(m_shuntLAction, &QAction::triggered, this, &ElementToolbar::onAddShuntL);
    connect(m_shuntCAction, &QAction::triggered, this, &ElementToolbar::onAddShuntC);
    connect(m_shuntRAction, &QAction::triggered, this, &ElementToolbar::onAddShuntR);
    connect(m_undoAction, &QAction::triggered, this, &ElementToolbar::undoLastElement);
    connect(m_clearAction, &QAction::triggered, this, &ElementToolbar::clearAllElements);
    connect(m_unitCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &ElementToolbar::onUnitChanged);
}

double ElementToolbar::currentValue() const
{
    return m_valueSpinBox->value();
}

void ElementToolbar::setCurrentValue(double value)
{
    m_valueSpinBox->setValue(value);
}

double ElementToolbar::frequency() const
{
    return m_frequency;
}

void ElementToolbar::setFrequency(double freq_hz)
{
    m_frequency = freq_hz;
}

double ElementToolbar::getScaledValue() const
{
    double value = m_valueSpinBox->value();
    int unit = m_unitCombo->currentIndex();
    
    switch (unit) {
        case 0: return value * 1e-9;  // nH
        case 1: return value * 1e-6;  // µH
        case 2: return value * 1e-3;  // mH
        case 3: return value * 1e-12; // pF
        case 4: return value * 1e-9;  // nF
        case 5: return value * 1e-6;  // µF
        case 6: return value;          // Ω
        case 7: return value * 1e3;   // kΩ
        default: return value;
    }
}

void ElementToolbar::onAddSeriesL()
{
    // Switch to inductor units if needed
    if (m_unitCombo->currentIndex() > 2) {
        m_unitCombo->setCurrentIndex(0);  // nH
    }
    emit addSeriesInductor(getScaledValue());
}

void ElementToolbar::onAddSeriesC()
{
    // Switch to capacitor units if needed
    if (m_unitCombo->currentIndex() < 3 || m_unitCombo->currentIndex() > 5) {
        m_unitCombo->setCurrentIndex(3);  // pF
    }
    emit addSeriesCapacitor(getScaledValue());
}

void ElementToolbar::onAddSeriesR()
{
    // Switch to resistor units if needed
    if (m_unitCombo->currentIndex() < 6) {
        m_unitCombo->setCurrentIndex(6);  // Ω
    }
    emit addSeriesResistor(getScaledValue());
}

void ElementToolbar::onAddShuntL()
{
    if (m_unitCombo->currentIndex() > 2) {
        m_unitCombo->setCurrentIndex(0);
    }
    emit addShuntInductor(getScaledValue());
}

void ElementToolbar::onAddShuntC()
{
    if (m_unitCombo->currentIndex() < 3 || m_unitCombo->currentIndex() > 5) {
        m_unitCombo->setCurrentIndex(3);
    }
    emit addShuntCapacitor(getScaledValue());
}

void ElementToolbar::onAddShuntR()
{
    if (m_unitCombo->currentIndex() < 6) {
        m_unitCombo->setCurrentIndex(6);
    }
    emit addShuntResistor(getScaledValue());
}

void ElementToolbar::onUnitChanged(int index)
{
    Q_UNUSED(index);
    // Update current type based on unit selection
    if (index <= 2) {
        m_currentType = ComponentType::Inductor;
    } else if (index <= 5) {
        m_currentType = ComponentType::Capacitor;
    } else {
        m_currentType = ComponentType::Resistor;
    }
}

} // namespace SmithTool
