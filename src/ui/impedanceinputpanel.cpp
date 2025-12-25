/**
 * @file impedanceinputpanel.cpp
 * @brief Source and Load impedance input panel implementation
 */

#include "impedanceinputpanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDoubleValidator>
#include <cmath>

namespace SmithTool {

static constexpr double PI = 3.14159265358979323846;

ImpedanceInputPanel::ImpedanceInputPanel(QWidget* parent)
    : QWidget(parent)
    , m_sourceZ(50.0, 0.0)
    , m_loadZ(50.0, 0.0)
    , m_z0(50.0)
{
    setupUI();
}

void ImpedanceInputPanel::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Source impedance group
    QGroupBox* srcGroup = new QGroupBox(tr("源端阻抗 Z_S"));
    QVBoxLayout* srcLayout = new QVBoxLayout(srcGroup);
    
    // Source format selection
    QHBoxLayout* srcFormatLayout = new QHBoxLayout();
    m_srcRectBtn = new QRadioButton(tr("直角坐标"));
    m_srcPolarBtn = new QRadioButton(tr("极坐标"));
    m_srcRectBtn->setChecked(true);
    srcFormatLayout->addWidget(m_srcRectBtn);
    srcFormatLayout->addWidget(m_srcPolarBtn);
    srcFormatLayout->addStretch();
    srcLayout->addLayout(srcFormatLayout);
    
    // Source value inputs
    QHBoxLayout* srcValueLayout = new QHBoxLayout();
    m_srcReal = new QLineEdit("50.0");
    m_srcReal->setValidator(new QDoubleValidator(-1e6, 1e6, 4, this));
    m_srcReal->setMaximumWidth(80);
    m_srcLabel1 = new QLabel("+j");
    m_srcImag = new QLineEdit("0.0");
    m_srcImag->setValidator(new QDoubleValidator(-1e6, 1e6, 4, this));
    m_srcImag->setMaximumWidth(80);
    m_srcLabel2 = new QLabel("Ω");
    
    srcValueLayout->addWidget(m_srcReal);
    srcValueLayout->addWidget(m_srcLabel1);
    srcValueLayout->addWidget(m_srcImag);
    srcValueLayout->addWidget(m_srcLabel2);
    srcValueLayout->addStretch();
    srcLayout->addLayout(srcValueLayout);
    
    mainLayout->addWidget(srcGroup);
    
    // Load impedance group
    QGroupBox* loadGroup = new QGroupBox(tr("负载阻抗 Z_L"));
    QVBoxLayout* loadLayout = new QVBoxLayout(loadGroup);
    
    // Load format selection
    QHBoxLayout* loadFormatLayout = new QHBoxLayout();
    m_loadRectBtn = new QRadioButton(tr("直角坐标"));
    m_loadPolarBtn = new QRadioButton(tr("极坐标"));
    m_loadRectBtn->setChecked(true);
    loadFormatLayout->addWidget(m_loadRectBtn);
    loadFormatLayout->addWidget(m_loadPolarBtn);
    loadFormatLayout->addStretch();
    loadLayout->addLayout(loadFormatLayout);
    
    // Load value inputs
    QHBoxLayout* loadValueLayout = new QHBoxLayout();
    m_loadReal = new QLineEdit("50.0");
    m_loadReal->setValidator(new QDoubleValidator(-1e6, 1e6, 4, this));
    m_loadReal->setMaximumWidth(80);
    m_loadLabel1 = new QLabel("+j");
    m_loadImag = new QLineEdit("0.0");
    m_loadImag->setValidator(new QDoubleValidator(-1e6, 1e6, 4, this));
    m_loadImag->setMaximumWidth(80);
    m_loadLabel2 = new QLabel("Ω");
    
    loadValueLayout->addWidget(m_loadReal);
    loadValueLayout->addWidget(m_loadLabel1);
    loadValueLayout->addWidget(m_loadImag);
    loadValueLayout->addWidget(m_loadLabel2);
    loadValueLayout->addStretch();
    loadLayout->addLayout(loadValueLayout);
    
    mainLayout->addWidget(loadGroup);
    
    // Z0 group
    QGroupBox* z0Group = new QGroupBox(tr("归一化阻抗"));
    QHBoxLayout* z0Layout = new QHBoxLayout(z0Group);
    QLabel* z0Label = new QLabel("Z₀ =");
    m_z0Edit = new QLineEdit("50.0");
    m_z0Edit->setValidator(new QDoubleValidator(1, 1000, 2, this));
    m_z0Edit->setMaximumWidth(80);
    z0Layout->addWidget(z0Label);
    z0Layout->addWidget(m_z0Edit);
    z0Layout->addWidget(new QLabel("Ω"));
    z0Layout->addStretch();
    
    mainLayout->addWidget(z0Group);
    mainLayout->addStretch();
    
    // Connections
    connect(m_srcRectBtn, &QRadioButton::toggled, this, &ImpedanceInputPanel::onSourceFormatChanged);
    connect(m_loadRectBtn, &QRadioButton::toggled, this, &ImpedanceInputPanel::onLoadFormatChanged);
    
    connect(m_srcReal, &QLineEdit::editingFinished, this, &ImpedanceInputPanel::onSourceValueChanged);
    connect(m_srcImag, &QLineEdit::editingFinished, this, &ImpedanceInputPanel::onSourceValueChanged);
    connect(m_loadReal, &QLineEdit::editingFinished, this, &ImpedanceInputPanel::onLoadValueChanged);
    connect(m_loadImag, &QLineEdit::editingFinished, this, &ImpedanceInputPanel::onLoadValueChanged);
    connect(m_z0Edit, &QLineEdit::editingFinished, this, &ImpedanceInputPanel::onZ0Changed);
    
    setMinimumWidth(280);
}

std::complex<double> ImpedanceInputPanel::sourceImpedance() const
{
    return m_sourceZ;
}

void ImpedanceInputPanel::setSourceImpedance(const std::complex<double>& z)
{
    m_sourceZ = z;
    updateSourceDisplay();
    emit sourceImpedanceChanged(m_sourceZ);
}

std::complex<double> ImpedanceInputPanel::loadImpedance() const
{
    return m_loadZ;
}

void ImpedanceInputPanel::setLoadImpedance(const std::complex<double>& z)
{
    m_loadZ = z;
    updateLoadDisplay();
    emit loadImpedanceChanged(m_loadZ);
}

void ImpedanceInputPanel::setZ0(double z0)
{
    m_z0 = z0;
    m_z0Edit->setText(QString::number(z0, 'f', 1));
    emit z0Changed(m_z0);
}

void ImpedanceInputPanel::onSourceFormatChanged()
{
    if (m_srcRectBtn->isChecked()) {
        m_srcLabel1->setText("+j");
        m_srcLabel2->setText("Ω");
        // Convert current polar to rectangular display
        m_srcReal->setText(QString::number(m_sourceZ.real(), 'f', 2));
        m_srcImag->setText(QString::number(m_sourceZ.imag(), 'f', 2));
    } else {
        m_srcLabel1->setText("∠");
        m_srcLabel2->setText("° Ω");
        // Convert to polar display
        double mag = std::abs(m_sourceZ);
        double phase = std::arg(m_sourceZ) * 180.0 / PI;
        m_srcReal->setText(QString::number(mag, 'f', 2));
        m_srcImag->setText(QString::number(phase, 'f', 1));
    }
}

void ImpedanceInputPanel::onLoadFormatChanged()
{
    if (m_loadRectBtn->isChecked()) {
        m_loadLabel1->setText("+j");
        m_loadLabel2->setText("Ω");
        m_loadReal->setText(QString::number(m_loadZ.real(), 'f', 2));
        m_loadImag->setText(QString::number(m_loadZ.imag(), 'f', 2));
    } else {
        m_loadLabel1->setText("∠");
        m_loadLabel2->setText("° Ω");
        double mag = std::abs(m_loadZ);
        double phase = std::arg(m_loadZ) * 180.0 / PI;
        m_loadReal->setText(QString::number(mag, 'f', 2));
        m_loadImag->setText(QString::number(phase, 'f', 1));
    }
}

void ImpedanceInputPanel::onSourceValueChanged()
{
    if (m_srcRectBtn->isChecked()) {
        m_sourceZ = parseRectangular(m_srcReal->text(), m_srcImag->text());
    } else {
        m_sourceZ = parsePolar(m_srcReal->text(), m_srcImag->text());
    }
    emit sourceImpedanceChanged(m_sourceZ);
}

void ImpedanceInputPanel::onLoadValueChanged()
{
    if (m_loadRectBtn->isChecked()) {
        m_loadZ = parseRectangular(m_loadReal->text(), m_loadImag->text());
    } else {
        m_loadZ = parsePolar(m_loadReal->text(), m_loadImag->text());
    }
    emit loadImpedanceChanged(m_loadZ);
}

void ImpedanceInputPanel::onZ0Changed()
{
    m_z0 = m_z0Edit->text().toDouble();
    if (m_z0 < 1.0) m_z0 = 1.0;
    emit z0Changed(m_z0);
}

void ImpedanceInputPanel::updateSourceDisplay()
{
    if (m_srcRectBtn->isChecked()) {
        m_srcReal->setText(QString::number(m_sourceZ.real(), 'f', 2));
        m_srcImag->setText(QString::number(m_sourceZ.imag(), 'f', 2));
    } else {
        double mag = std::abs(m_sourceZ);
        double phase = std::arg(m_sourceZ) * 180.0 / PI;
        m_srcReal->setText(QString::number(mag, 'f', 2));
        m_srcImag->setText(QString::number(phase, 'f', 1));
    }
}

void ImpedanceInputPanel::updateLoadDisplay()
{
    if (m_loadRectBtn->isChecked()) {
        m_loadReal->setText(QString::number(m_loadZ.real(), 'f', 2));
        m_loadImag->setText(QString::number(m_loadZ.imag(), 'f', 2));
    } else {
        double mag = std::abs(m_loadZ);
        double phase = std::arg(m_loadZ) * 180.0 / PI;
        m_loadReal->setText(QString::number(mag, 'f', 2));
        m_loadImag->setText(QString::number(phase, 'f', 1));
    }
}

std::complex<double> ImpedanceInputPanel::parseRectangular(const QString& real, const QString& imag) const
{
    return std::complex<double>(real.toDouble(), imag.toDouble());
}

std::complex<double> ImpedanceInputPanel::parsePolar(const QString& mag, const QString& phase) const
{
    double m = mag.toDouble();
    double p = phase.toDouble() * PI / 180.0;
    return std::complex<double>(m * std::cos(p), m * std::sin(p));
}

} // namespace SmithTool
