/**
 * @file componenteditdialog.cpp
 * @brief Dialog for editing component values implementation
 */

#include "componenteditdialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <cmath>

namespace SmithTool {

ComponentEditDialog::ComponentEditDialog(QWidget* parent)
    : QDialog(parent)
    , m_type(ComponentType::Resistor)
    , m_connection(ConnectionType::Series)
    , m_frequency(1e9)
    , m_componentIndex(-1)
{
    setupUI();
    setWindowTitle(tr("Edit Component"));
    setMinimumWidth(300);
}

void ComponentEditDialog::setupUI()
{
    auto* mainLayout = new QVBoxLayout(this);
    
    // Component info group
    auto* infoGroup = new QGroupBox(tr("Component Information"), this);
    auto* infoLayout = new QFormLayout(infoGroup);
    
    m_typeLabel = new QLabel(this);
    m_connectionLabel = new QLabel(this);
    infoLayout->addRow(tr("Type:"), m_typeLabel);
    infoLayout->addRow(tr("Connection:"), m_connectionLabel);
    
    mainLayout->addWidget(infoGroup);
    
    // Value edit group
    auto* valueGroup = new QGroupBox(tr("Value"), this);
    auto* valueLayout = new QHBoxLayout(valueGroup);
    
    m_valueSpin = new QDoubleSpinBox(this);
    m_valueSpin->setRange(0.001, 999999.0);
    m_valueSpin->setDecimals(3);
    m_valueSpin->setValue(1.0);
    
    m_unitCombo = new QComboBox(this);
    // Units will be set based on component type
    
    valueLayout->addWidget(m_valueSpin);
    valueLayout->addWidget(m_unitCombo);
    
    mainLayout->addWidget(valueGroup);
    
    // Impedance display
    m_impedanceLabel = new QLabel(this);
    m_impedanceLabel->setStyleSheet("font-weight: bold; color: #0066cc;");
    mainLayout->addWidget(m_impedanceLabel);
    
    // Preview button
    m_previewBtn = new QPushButton(tr("Preview"), this);
    connect(m_previewBtn, &QPushButton::clicked, this, &ComponentEditDialog::onPreview);
    mainLayout->addWidget(m_previewBtn);
    
    // Dialog buttons
    auto* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);
    
    // Connect signals
    connect(m_valueSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ComponentEditDialog::onValueChanged);
    connect(m_unitCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ComponentEditDialog::onUnitChanged);
}

void ComponentEditDialog::setComponent(ComponentType type, ConnectionType connection, double value)
{
    m_type = type;
    m_connection = connection;
    
    // Set type label
    switch (type) {
        case ComponentType::Resistor:
            m_typeLabel->setText(tr("Resistor (R)"));
            break;
        case ComponentType::Inductor:
            m_typeLabel->setText(tr("Inductor (L)"));
            break;
        case ComponentType::Capacitor:
            m_typeLabel->setText(tr("Capacitor (C)"));
            break;
        default:
            m_typeLabel->setText(tr("Unknown"));
    }
    
    // Set connection label
    m_connectionLabel->setText(connection == ConnectionType::Series ? 
                               tr("Series") : tr("Shunt"));
    
    // Setup unit combo based on type
    m_unitCombo->clear();
    switch (type) {
        case ComponentType::Resistor:
            m_unitCombo->addItems({tr("Ω"), tr("kΩ"), tr("mΩ")});
            break;
        case ComponentType::Inductor:
            m_unitCombo->addItems({tr("H"), tr("mH"), tr("μH"), tr("nH"), tr("pH")});
            m_unitCombo->setCurrentIndex(3);  // Default nH
            break;
        case ComponentType::Capacitor:
            m_unitCombo->addItems({tr("F"), tr("mF"), tr("μF"), tr("nF"), tr("pF"), tr("fF")});
            m_unitCombo->setCurrentIndex(4);  // Default pF
            break;
        default:
            break;
    }
    
    // Set value with appropriate unit
    double displayValue = value;
    int unitIndex = 0;
    
    switch (type) {
        case ComponentType::Resistor:
            if (value >= 1000) {
                displayValue = value / 1000;
                unitIndex = 1;
            } else if (value < 1) {
                displayValue = value * 1000;
                unitIndex = 2;
            }
            break;
        case ComponentType::Inductor:
            if (value >= 1e-3) {
                displayValue = value * 1e3;
                unitIndex = 1;
            } else if (value >= 1e-6) {
                displayValue = value * 1e6;
                unitIndex = 2;
            } else if (value >= 1e-9) {
                displayValue = value * 1e9;
                unitIndex = 3;
            } else {
                displayValue = value * 1e12;
                unitIndex = 4;
            }
            break;
        case ComponentType::Capacitor:
            if (value >= 1e-3) {
                displayValue = value * 1e3;
                unitIndex = 1;
            } else if (value >= 1e-6) {
                displayValue = value * 1e6;
                unitIndex = 2;
            } else if (value >= 1e-9) {
                displayValue = value * 1e9;
                unitIndex = 3;
            } else if (value >= 1e-12) {
                displayValue = value * 1e12;
                unitIndex = 4;
            } else {
                displayValue = value * 1e15;
                unitIndex = 5;
            }
            break;
        default:
            break;
    }
    
    m_unitCombo->setCurrentIndex(unitIndex);
    m_valueSpin->setValue(displayValue);
    
    updateDisplay();
}

double ComponentEditDialog::componentValue() const
{
    return getScaledValue();
}

double ComponentEditDialog::getScaledValue() const
{
    double value = m_valueSpin->value();
    int unitIndex = m_unitCombo->currentIndex();
    
    switch (m_type) {
        case ComponentType::Resistor:
            switch (unitIndex) {
                case 0: return value;           // Ω
                case 1: return value * 1000;    // kΩ
                case 2: return value / 1000;    // mΩ
            }
            break;
        case ComponentType::Inductor:
            switch (unitIndex) {
                case 0: return value;           // H
                case 1: return value * 1e-3;    // mH
                case 2: return value * 1e-6;    // μH
                case 3: return value * 1e-9;    // nH
                case 4: return value * 1e-12;   // pH
            }
            break;
        case ComponentType::Capacitor:
            switch (unitIndex) {
                case 0: return value;           // F
                case 1: return value * 1e-3;    // mF
                case 2: return value * 1e-6;    // μF
                case 3: return value * 1e-9;    // nF
                case 4: return value * 1e-12;   // pF
                case 5: return value * 1e-15;   // fF
            }
            break;
        default:
            break;
    }
    return value;
}

void ComponentEditDialog::updateDisplay()
{
    double value = getScaledValue();
    double omega = 2.0 * M_PI * m_frequency;
    
    QString impedanceText;
    switch (m_type) {
        case ComponentType::Resistor:
            impedanceText = tr("Impedance: %1 Ω").arg(value, 0, 'f', 2);
            break;
        case ComponentType::Inductor: {
            double reactance = omega * value;
            impedanceText = tr("Reactance: j%1 Ω (at %2 GHz)")
                .arg(reactance, 0, 'f', 2)
                .arg(m_frequency / 1e9, 0, 'f', 3);
            break;
        }
        case ComponentType::Capacitor: {
            double reactance = -1.0 / (omega * value);
            impedanceText = tr("Reactance: j%1 Ω (at %2 GHz)")
                .arg(reactance, 0, 'f', 2)
                .arg(m_frequency / 1e9, 0, 'f', 3);
            break;
        }
        default:
            break;
    }
    
    m_impedanceLabel->setText(impedanceText);
}

void ComponentEditDialog::onValueChanged()
{
    updateDisplay();
}

void ComponentEditDialog::onUnitChanged(int)
{
    updateDisplay();
}

void ComponentEditDialog::onPreview()
{
    emit previewRequested(m_componentIndex, getScaledValue());
}

} // namespace SmithTool
