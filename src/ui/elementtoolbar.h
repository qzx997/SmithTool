/**
 * @file elementtoolbar.h
 * @brief Toolbar for adding series/shunt circuit elements
 */

#ifndef SMITHTOOL_ELEMENTTOOLBAR_H
#define SMITHTOOL_ELEMENTTOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QComboBox>
#include "../core/component.h"

namespace SmithTool {

/**
 * @brief Toolbar with buttons for adding circuit elements
 */
class ElementToolbar : public QToolBar {
    Q_OBJECT

public:
    explicit ElementToolbar(QWidget* parent = nullptr);
    ~ElementToolbar() override = default;

    // Get current element value from spinbox
    double currentValue() const;
    void setCurrentValue(double value);
    
    // Get current frequency
    double frequency() const;
    void setFrequency(double freq_hz);

signals:
    // Emitted when user clicks to add an element
    void addSeriesInductor(double value_henry);
    void addSeriesCapacitor(double value_farad);
    void addSeriesResistor(double value_ohm);
    void addShuntInductor(double value_henry);
    void addShuntCapacitor(double value_farad);
    void addShuntResistor(double value_ohm);
    
    // Emitted when user wants to select target point on chart
    void selectPointForElement(ComponentType type, ConnectionType connection);
    
    // Clear and undo
    void clearAllElements();
    void undoLastElement();

private slots:
    void onAddSeriesL();
    void onAddSeriesC();
    void onAddSeriesR();
    void onAddShuntL();
    void onAddShuntC();
    void onAddShuntR();
    void onUnitChanged(int index);

private:
    void setupUI();
    double getScaledValue() const;
    
    // Value input
    QDoubleSpinBox* m_valueSpinBox;
    QComboBox* m_unitCombo;
    
    // Actions
    QAction* m_seriesLAction;
    QAction* m_seriesCAction;
    QAction* m_seriesRAction;
    QAction* m_shuntLAction;
    QAction* m_shuntCAction;
    QAction* m_shuntRAction;
    QAction* m_undoAction;
    QAction* m_clearAction;
    
    // Current component type for unit selection
    ComponentType m_currentType;
    double m_frequency;
};

} // namespace SmithTool

#endif // SMITHTOOL_ELEMENTTOOLBAR_H
