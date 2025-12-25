/**
 * @file componentpanel.h
 * @brief Component value display panel
 */

#ifndef SMITHTOOL_COMPONENTPANEL_H
#define SMITHTOOL_COMPONENTPANEL_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <complex>

#include "../core/impedance.h"
#include "../core/component.h"

namespace SmithTool {

/**
 * @brief Panel displaying impedance and component values
 */
class ComponentPanel : public QWidget {
    Q_OBJECT

public:
    explicit ComponentPanel(QWidget* parent = nullptr);
    ~ComponentPanel() override = default;

    // Set values to display
    void setFrequency(double freq_hz);
    void setGamma(const std::complex<double>& gamma);
    void setImpedance(const std::complex<double>& z);
    void setZ0(double z0);
    
    // Get current frequency
    double frequency() const;
    double z0() const { return m_z0; }

signals:
    void frequencyChanged(double freq_hz);
    void z0Changed(double z0);

private slots:
    void onFrequencyChanged();
    void onZ0Changed();

private:
    void setupUI();
    void updateDisplay();
    QString formatFrequency(double freq) const;
    QString formatValue(double val, const QString& unit) const;
    
    // Data
    double m_frequency;
    double m_z0;
    std::complex<double> m_gamma;
    std::complex<double> m_impedance;
    
    // UI elements
    QLineEdit* m_freqEdit;
    QComboBox* m_freqUnitCombo;
    QLineEdit* m_z0Edit;
    
    // Display labels
    QLabel* m_gammaRectLabel;
    QLabel* m_gammaPolarLabel;
    QLabel* m_zNormLabel;
    QLabel* m_zActualLabel;
    QLabel* m_resistanceLabel;
    QLabel* m_reactanceLabel;
    QLabel* m_componentLabel;
    QLabel* m_vswrLabel;
    QLabel* m_returnLossLabel;
};

} // namespace SmithTool

#endif // SMITHTOOL_COMPONENTPANEL_H
