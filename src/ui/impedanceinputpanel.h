/**
 * @file impedanceinputpanel.h
 * @brief Source and Load impedance input panel with complex number support
 */

#ifndef SMITHTOOL_IMPEDANCEINPUTPANEL_H
#define SMITHTOOL_IMPEDANCEINPUTPANEL_H

#include <QWidget>
#include <QLineEdit>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include <QGroupBox>
#include <complex>

namespace SmithTool {

/**
 * @brief Input format for complex impedance
 */
enum class ImpedanceFormat {
    Rectangular,    // R + jX
    Polar           // |Z| ∠θ
};

/**
 * @brief Panel for entering source and load impedances
 */
class ImpedanceInputPanel : public QWidget {
    Q_OBJECT

public:
    explicit ImpedanceInputPanel(QWidget* parent = nullptr);
    ~ImpedanceInputPanel() override = default;

    // Source impedance
    std::complex<double> sourceImpedance() const;
    void setSourceImpedance(const std::complex<double>& z);
    
    // Load impedance
    std::complex<double> loadImpedance() const;
    void setLoadImpedance(const std::complex<double>& z);
    
    // Reference impedance (Z0)
    double z0() const { return m_z0; }
    void setZ0(double z0);

signals:
    void sourceImpedanceChanged(std::complex<double> zs);
    void loadImpedanceChanged(std::complex<double> zl);
    void z0Changed(double z0);

private slots:
    void onSourceFormatChanged();
    void onLoadFormatChanged();
    void onSourceValueChanged();
    void onLoadValueChanged();
    void onZ0Changed();

private:
    void setupUI();
    void updateSourceDisplay();
    void updateLoadDisplay();
    
    std::complex<double> parseRectangular(const QString& real, const QString& imag) const;
    std::complex<double> parsePolar(const QString& mag, const QString& phase) const;
    
    // Data
    std::complex<double> m_sourceZ;
    std::complex<double> m_loadZ;
    double m_z0;
    
    // Source inputs
    QLineEdit* m_srcReal;
    QLineEdit* m_srcImag;
    QRadioButton* m_srcRectBtn;
    QRadioButton* m_srcPolarBtn;
    QLabel* m_srcLabel1;
    QLabel* m_srcLabel2;
    
    // Load inputs
    QLineEdit* m_loadReal;
    QLineEdit* m_loadImag;
    QRadioButton* m_loadRectBtn;
    QRadioButton* m_loadPolarBtn;
    QLabel* m_loadLabel1;
    QLabel* m_loadLabel2;
    
    // Z0 input
    QLineEdit* m_z0Edit;
};

} // namespace SmithTool

#endif // SMITHTOOL_IMPEDANCEINPUTPANEL_H
