/**
 * @file matchingwizard.h
 * @brief Matching network design wizard dialog
 */

#ifndef SMITHTOOL_MATCHINGWIZARD_H
#define SMITHTOOL_MATCHINGWIZARD_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QDialogButtonBox>
#include <complex>

#include "../core/matching.h"

namespace SmithTool {

/**
 * @brief Dialog for impedance matching network design
 */
class MatchingWizard : public QDialog {
    Q_OBJECT

public:
    explicit MatchingWizard(QWidget* parent = nullptr);
    ~MatchingWizard() override = default;

    // Set initial values
    void setSourceImpedance(const std::complex<double>& zs);
    void setLoadImpedance(const std::complex<double>& zl);
    void setFrequency(double freq_hz);
    void setZ0(double z0);
    
    // Get selected solution
    MatchingSolution selectedSolution() const { return m_selectedSolution; }
    bool hasValidSelection() const { return m_selectedSolution.valid; }

signals:
    void solutionSelected(const MatchingSolution& solution);

private slots:
    void onCalculate();
    void onSolutionSelected(int row, int column);
    void onApply();
    void onTopologyChanged(int index);

private:
    void setupUI();
    void updateResults();
    QString topologyName(MatchingTopology topo) const;
    
    // Input widgets
    QLineEdit* m_sourceREdit;
    QLineEdit* m_sourceXEdit;
    QLineEdit* m_loadREdit;
    QLineEdit* m_loadXEdit;
    QLineEdit* m_freqEdit;
    QComboBox* m_freqUnitCombo;
    QLineEdit* m_z0Edit;
    QComboBox* m_topologyCombo;
    
    // Results
    QTableWidget* m_resultsTable;
    QLabel* m_statusLabel;
    
    // Buttons
    QPushButton* m_calculateBtn;
    QPushButton* m_applyBtn;
    QDialogButtonBox* m_buttonBox;
    
    // Calculator and results
    MatchingCalculator m_calculator;
    std::vector<MatchingSolution> m_solutions;
    MatchingSolution m_selectedSolution;
};

} // namespace SmithTool

#endif // SMITHTOOL_MATCHINGWIZARD_H
