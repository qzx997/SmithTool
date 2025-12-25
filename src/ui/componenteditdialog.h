/**
 * @file componenteditdialog.h
 * @brief Dialog for editing component values
 */

#ifndef SMITHTOOL_COMPONENTEDITDIALOG_H
#define SMITHTOOL_COMPONENTEDITDIALOG_H

#include <QDialog>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include "../core/component.h"

namespace SmithTool {

/**
 * @brief Dialog for editing component values
 */
class ComponentEditDialog : public QDialog {
    Q_OBJECT

public:
    explicit ComponentEditDialog(QWidget* parent = nullptr);
    ~ComponentEditDialog() override = default;
    
    // Set component to edit
    void setComponent(ComponentType type, ConnectionType connection, double value);
    void setComponentIndex(int index) { m_componentIndex = index; }
    
    // Get edited values
    ComponentType componentType() const { return m_type; }
    ConnectionType connectionType() const { return m_connection; }
    double componentValue() const;
    int componentIndex() const { return m_componentIndex; }
    
    // Set frequency for reactance/susceptance calculation
    void setFrequency(double freq_hz) { m_frequency = freq_hz; }

signals:
    void valueChanged(int index, double newValue);
    void previewRequested(int index, double newValue);

private slots:
    void onValueChanged();
    void onUnitChanged(int index);
    void onPreview();

private:
    void setupUI();
    void updateDisplay();
    double getScaledValue() const;
    
    ComponentType m_type;
    ConnectionType m_connection;
    double m_frequency;
    int m_componentIndex;
    
    // UI elements
    QLabel* m_typeLabel;
    QLabel* m_connectionLabel;
    QDoubleSpinBox* m_valueSpin;
    QComboBox* m_unitCombo;
    QLabel* m_impedanceLabel;
    QPushButton* m_previewBtn;
    QPushButton* m_okBtn;
    QPushButton* m_cancelBtn;
};

} // namespace SmithTool

#endif // SMITHTOOL_COMPONENTEDITDIALOG_H
