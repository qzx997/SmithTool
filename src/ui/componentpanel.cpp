/**
 * @file componentpanel.cpp
 * @brief Component value display panel implementation
 */

#include "componentpanel.h"
#include "../core/smithmath.h"
#include <QFormLayout>
#include <QDoubleValidator>
#include <cmath>

namespace SmithTool {

ComponentPanel::ComponentPanel(QWidget* parent)
    : QWidget(parent)
    , m_frequency(1e9)
    , m_z0(50.0)
    , m_gamma(0, 0)
    , m_impedance(50, 0)
{
    setupUI();
    updateDisplay();
}

void ComponentPanel::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Input section
    QGroupBox* inputGroup = new QGroupBox(tr("Settings"));
    QFormLayout* inputLayout = new QFormLayout(inputGroup);
    
    // Frequency input
    QHBoxLayout* freqLayout = new QHBoxLayout();
    m_freqEdit = new QLineEdit("1.0");
    m_freqEdit->setValidator(new QDoubleValidator(0.001, 1e6, 6, this));
    m_freqUnitCombo = new QComboBox();
    m_freqUnitCombo->addItems({"Hz", "kHz", "MHz", "GHz"});
    m_freqUnitCombo->setCurrentIndex(3); // GHz
    freqLayout->addWidget(m_freqEdit);
    freqLayout->addWidget(m_freqUnitCombo);
    inputLayout->addRow(tr("Frequency:"), freqLayout);
    
    // Z0 input
    m_z0Edit = new QLineEdit("50");
    m_z0Edit->setValidator(new QDoubleValidator(1, 1000, 2, this));
    inputLayout->addRow(tr("Z₀ (Ω):"), m_z0Edit);
    
    mainLayout->addWidget(inputGroup);
    
    // Gamma display
    QGroupBox* gammaGroup = new QGroupBox(tr("Reflection Coefficient Γ"));
    QFormLayout* gammaLayout = new QFormLayout(gammaGroup);
    
    m_gammaRectLabel = new QLabel("0.000 + j0.000");
    m_gammaPolarLabel = new QLabel("|Γ| = 0.000  ∠0.0°");
    gammaLayout->addRow(tr("Rectangular:"), m_gammaRectLabel);
    gammaLayout->addRow(tr("Polar:"), m_gammaPolarLabel);
    
    mainLayout->addWidget(gammaGroup);
    
    // Impedance display
    QGroupBox* zGroup = new QGroupBox(tr("Impedance Z"));
    QFormLayout* zLayout = new QFormLayout(zGroup);
    
    m_zNormLabel = new QLabel("1.000 + j0.000");
    m_zActualLabel = new QLabel("50.00 + j0.00 Ω");
    zLayout->addRow(tr("Normalized:"), m_zNormLabel);
    zLayout->addRow(tr("Actual:"), m_zActualLabel);
    
    mainLayout->addWidget(zGroup);
    
    // Component values
    QGroupBox* compGroup = new QGroupBox(tr("Component Values"));
    QFormLayout* compLayout = new QFormLayout(compGroup);
    
    m_resistanceLabel = new QLabel("R = 50.00 Ω");
    m_reactanceLabel = new QLabel("X = 0.00 Ω");
    m_componentLabel = new QLabel("—");
    compLayout->addRow(m_resistanceLabel);
    compLayout->addRow(m_reactanceLabel);
    compLayout->addRow(tr("Equivalent:"), m_componentLabel);
    
    mainLayout->addWidget(compGroup);
    
    // VSWR and Return Loss
    QGroupBox* matchGroup = new QGroupBox(tr("Match Quality"));
    QFormLayout* matchLayout = new QFormLayout(matchGroup);
    
    m_vswrLabel = new QLabel("VSWR = 1.00");
    m_returnLossLabel = new QLabel("RL = -∞ dB");
    matchLayout->addRow(m_vswrLabel);
    matchLayout->addRow(m_returnLossLabel);
    
    mainLayout->addWidget(matchGroup);
    mainLayout->addStretch();
    
    // Connections
    connect(m_freqEdit, &QLineEdit::editingFinished, this, &ComponentPanel::onFrequencyChanged);
    connect(m_freqUnitCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ComponentPanel::onFrequencyChanged);
    connect(m_z0Edit, &QLineEdit::editingFinished, this, &ComponentPanel::onZ0Changed);
    
    setMinimumWidth(250);
}

void ComponentPanel::setFrequency(double freq_hz)
{
    m_frequency = freq_hz;
    
    // Update UI
    double displayValue = freq_hz;
    int unitIndex = 0;
    
    if (freq_hz >= 1e9) {
        displayValue = freq_hz / 1e9;
        unitIndex = 3;
    } else if (freq_hz >= 1e6) {
        displayValue = freq_hz / 1e6;
        unitIndex = 2;
    } else if (freq_hz >= 1e3) {
        displayValue = freq_hz / 1e3;
        unitIndex = 1;
    }
    
    m_freqEdit->blockSignals(true);
    m_freqUnitCombo->blockSignals(true);
    m_freqEdit->setText(QString::number(displayValue, 'f', 3));
    m_freqUnitCombo->setCurrentIndex(unitIndex);
    m_freqEdit->blockSignals(false);
    m_freqUnitCombo->blockSignals(false);
    
    updateDisplay();
}

void ComponentPanel::setGamma(const std::complex<double>& gamma)
{
    m_gamma = gamma;
    m_impedance = SmithMath::gammaToImpedance(gamma, m_z0);
    updateDisplay();
}

void ComponentPanel::setImpedance(const std::complex<double>& z)
{
    m_impedance = z;
    m_gamma = SmithMath::impedanceToGamma(z, m_z0);
    updateDisplay();
}

void ComponentPanel::setZ0(double z0)
{
    m_z0 = z0;
    m_z0Edit->setText(QString::number(z0, 'f', 1));
    updateDisplay();
}

double ComponentPanel::frequency() const
{
    return m_frequency;
}

void ComponentPanel::onFrequencyChanged()
{
    double value = m_freqEdit->text().toDouble();
    double multiplier = 1.0;
    
    switch (m_freqUnitCombo->currentIndex()) {
        case 0: multiplier = 1.0; break;
        case 1: multiplier = 1e3; break;
        case 2: multiplier = 1e6; break;
        case 3: multiplier = 1e9; break;
    }
    
    m_frequency = value * multiplier;
    updateDisplay();
    emit frequencyChanged(m_frequency);
}

void ComponentPanel::onZ0Changed()
{
    m_z0 = m_z0Edit->text().toDouble();
    m_impedance = SmithMath::gammaToImpedance(m_gamma, m_z0);
    updateDisplay();
    emit z0Changed(m_z0);
}

void ComponentPanel::updateDisplay()
{
    // Gamma display
    QString sign = m_gamma.imag() >= 0 ? "+" : "-";
    m_gammaRectLabel->setText(QString("%1 %2 j%3")
        .arg(m_gamma.real(), 0, 'f', 4)
        .arg(sign)
        .arg(std::abs(m_gamma.imag()), 0, 'f', 4));
    
    double gammaMag = std::abs(m_gamma);
    double gammaPhase = SmithMath::gammaPhaseDegrees(m_gamma);
    m_gammaPolarLabel->setText(QString("|Γ| = %1  ∠%2°")
        .arg(gammaMag, 0, 'f', 4)
        .arg(gammaPhase, 0, 'f', 1));
    
    // Impedance display
    std::complex<double> zNorm = m_impedance / m_z0;
    sign = zNorm.imag() >= 0 ? "+" : "-";
    m_zNormLabel->setText(QString("%1 %2 j%3")
        .arg(zNorm.real(), 0, 'f', 3)
        .arg(sign)
        .arg(std::abs(zNorm.imag()), 0, 'f', 3));
    
    sign = m_impedance.imag() >= 0 ? "+" : "-";
    m_zActualLabel->setText(QString("%1 %2 j%3 Ω")
        .arg(m_impedance.real(), 0, 'f', 2)
        .arg(sign)
        .arg(std::abs(m_impedance.imag()), 0, 'f', 2));
    
    // Component values
    double r = m_impedance.real();
    double x = m_impedance.imag();
    
    m_resistanceLabel->setText(QString("R = %1").arg(formatValue(r, "Ω")));
    m_reactanceLabel->setText(QString("X = %1%2")
        .arg(x >= 0 ? "+" : "")
        .arg(formatValue(x, "Ω")));
    
    // Calculate equivalent component
    if (std::abs(x) < 0.01) {
        m_componentLabel->setText(tr("Pure resistance"));
    } else if (x > 0) {
        // Inductive
        double l = x / (2.0 * SmithMath::PI * m_frequency);
        ComponentValue cv(ComponentType::Inductor, l, m_frequency);
        m_componentLabel->setText(QString("L = %1 (series)").arg(cv.valueWithUnit()));
    } else {
        // Capacitive
        double c = -1.0 / (2.0 * SmithMath::PI * m_frequency * x);
        ComponentValue cv(ComponentType::Capacitor, c, m_frequency);
        m_componentLabel->setText(QString("C = %1 (series)").arg(cv.valueWithUnit()));
    }
    
    // VSWR and Return Loss
    double vswr = SmithMath::gammaToVSWR(gammaMag);
    double rl = SmithMath::gammaToReturnLoss(m_gamma);
    
    if (vswr > 1e5) {
        m_vswrLabel->setText("VSWR = ∞");
    } else {
        m_vswrLabel->setText(QString("VSWR = %1").arg(vswr, 0, 'f', 2));
    }
    
    if (rl < -100) {
        m_returnLossLabel->setText("RL = -∞ dB");
    } else {
        m_returnLossLabel->setText(QString("RL = %1 dB").arg(rl, 0, 'f', 2));
    }
}

QString ComponentPanel::formatFrequency(double freq) const
{
    if (freq >= 1e9) {
        return QString("%1 GHz").arg(freq / 1e9, 0, 'f', 3);
    } else if (freq >= 1e6) {
        return QString("%1 MHz").arg(freq / 1e6, 0, 'f', 3);
    } else if (freq >= 1e3) {
        return QString("%1 kHz").arg(freq / 1e3, 0, 'f', 3);
    }
    return QString("%1 Hz").arg(freq, 0, 'f', 1);
}

QString ComponentPanel::formatValue(double val, const QString& unit) const
{
    double absVal = std::abs(val);
    
    if (unit == "Ω") {
        if (absVal >= 1e6) return QString("%1 MΩ").arg(val / 1e6, 0, 'f', 2);
        if (absVal >= 1e3) return QString("%1 kΩ").arg(val / 1e3, 0, 'f', 2);
        if (absVal >= 1) return QString("%1 Ω").arg(val, 0, 'f', 2);
        return QString("%1 mΩ").arg(val * 1e3, 0, 'f', 2);
    }
    
    return QString("%1 %2").arg(val, 0, 'f', 3).arg(unit);
}

} // namespace SmithTool
