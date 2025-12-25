/**
 * @file matchingwizard.cpp
 * @brief Matching network design wizard implementation
 */

#include "matchingwizard.h"
#include <QHeaderView>
#include <QDoubleValidator>
#include <QMessageBox>

namespace SmithTool {

MatchingWizard::MatchingWizard(QWidget* parent)
    : QDialog(parent)
{
    setupUI();
    setWindowTitle(tr("Impedance Matching Design"));
    resize(600, 500);
}

void MatchingWizard::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Input section
    QGroupBox* inputGroup = new QGroupBox(tr("Design Parameters"));
    QGridLayout* inputLayout = new QGridLayout(inputGroup);
    
    // Source impedance
    inputLayout->addWidget(new QLabel(tr("Source Z (Ω):")), 0, 0);
    m_sourceREdit = new QLineEdit("50");
    m_sourceREdit->setValidator(new QDoubleValidator(0.001, 1e6, 3, this));
    inputLayout->addWidget(m_sourceREdit, 0, 1);
    inputLayout->addWidget(new QLabel(tr("+ j")), 0, 2);
    m_sourceXEdit = new QLineEdit("0");
    m_sourceXEdit->setValidator(new QDoubleValidator(-1e6, 1e6, 3, this));
    inputLayout->addWidget(m_sourceXEdit, 0, 3);
    
    // Load impedance
    inputLayout->addWidget(new QLabel(tr("Load Z (Ω):")), 1, 0);
    m_loadREdit = new QLineEdit("100");
    m_loadREdit->setValidator(new QDoubleValidator(0.001, 1e6, 3, this));
    inputLayout->addWidget(m_loadREdit, 1, 1);
    inputLayout->addWidget(new QLabel(tr("+ j")), 1, 2);
    m_loadXEdit = new QLineEdit("50");
    m_loadXEdit->setValidator(new QDoubleValidator(-1e6, 1e6, 3, this));
    inputLayout->addWidget(m_loadXEdit, 1, 3);
    
    // Frequency
    inputLayout->addWidget(new QLabel(tr("Frequency:")), 2, 0);
    m_freqEdit = new QLineEdit("1.0");
    m_freqEdit->setValidator(new QDoubleValidator(0.001, 1e6, 6, this));
    inputLayout->addWidget(m_freqEdit, 2, 1);
    m_freqUnitCombo = new QComboBox();
    m_freqUnitCombo->addItems({"MHz", "GHz", "kHz"});
    m_freqUnitCombo->setCurrentIndex(1);  // Default GHz
    inputLayout->addWidget(m_freqUnitCombo, 2, 2, 1, 2);
    
    // Z0
    inputLayout->addWidget(new QLabel(tr("Z₀ (Ω):")), 3, 0);
    m_z0Edit = new QLineEdit("50");
    m_z0Edit->setValidator(new QDoubleValidator(1, 1000, 3, this));
    inputLayout->addWidget(m_z0Edit, 3, 1);
    
    // Topology selection
    inputLayout->addWidget(new QLabel(tr("Topology:")), 4, 0);
    m_topologyCombo = new QComboBox();
    m_topologyCombo->addItems({
        tr("All"),
        tr("L-Section"),
        tr("Pi-Network"),
        tr("T-Network")
    });
    inputLayout->addWidget(m_topologyCombo, 4, 1, 1, 3);
    
    mainLayout->addWidget(inputGroup);
    
    // Calculate button
    m_calculateBtn = new QPushButton(tr("Calculate Matching Networks"));
    connect(m_calculateBtn, &QPushButton::clicked, this, &MatchingWizard::onCalculate);
    mainLayout->addWidget(m_calculateBtn);
    
    // Results table
    QGroupBox* resultsGroup = new QGroupBox(tr("Solutions"));
    QVBoxLayout* resultsLayout = new QVBoxLayout(resultsGroup);
    
    m_resultsTable = new QTableWidget();
    m_resultsTable->setColumnCount(4);
    m_resultsTable->setHorizontalHeaderLabels({
        tr("Topology"), tr("Elements"), tr("Q"), tr("Description")
    });
    m_resultsTable->horizontalHeader()->setStretchLastSection(true);
    m_resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_resultsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_resultsTable, &QTableWidget::cellClicked, 
            this, &MatchingWizard::onSolutionSelected);
    resultsLayout->addWidget(m_resultsTable);
    
    m_statusLabel = new QLabel();
    resultsLayout->addWidget(m_statusLabel);
    
    mainLayout->addWidget(resultsGroup);
    
    // Dialog buttons
    m_buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Apply | QDialogButtonBox::Close);
    m_applyBtn = m_buttonBox->button(QDialogButtonBox::Apply);
    m_applyBtn->setEnabled(false);
    connect(m_applyBtn, &QPushButton::clicked, this, &MatchingWizard::onApply);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(m_buttonBox);
}

void MatchingWizard::setSourceImpedance(const std::complex<double>& zs)
{
    m_sourceREdit->setText(QString::number(zs.real(), 'f', 2));
    m_sourceXEdit->setText(QString::number(zs.imag(), 'f', 2));
}

void MatchingWizard::setLoadImpedance(const std::complex<double>& zl)
{
    m_loadREdit->setText(QString::number(zl.real(), 'f', 2));
    m_loadXEdit->setText(QString::number(zl.imag(), 'f', 2));
}

void MatchingWizard::setFrequency(double freq_hz)
{
    // Convert to appropriate unit
    if (freq_hz >= 1e9) {
        m_freqEdit->setText(QString::number(freq_hz / 1e9, 'f', 3));
        m_freqUnitCombo->setCurrentIndex(1);  // GHz
    } else if (freq_hz >= 1e6) {
        m_freqEdit->setText(QString::number(freq_hz / 1e6, 'f', 3));
        m_freqUnitCombo->setCurrentIndex(0);  // MHz
    } else {
        m_freqEdit->setText(QString::number(freq_hz / 1e3, 'f', 3));
        m_freqUnitCombo->setCurrentIndex(2);  // kHz
    }
}

void MatchingWizard::setZ0(double z0)
{
    m_z0Edit->setText(QString::number(z0, 'f', 1));
}

void MatchingWizard::onCalculate()
{
    // Parse input values
    double sourceR = m_sourceREdit->text().toDouble();
    double sourceX = m_sourceXEdit->text().toDouble();
    double loadR = m_loadREdit->text().toDouble();
    double loadX = m_loadXEdit->text().toDouble();
    double freqVal = m_freqEdit->text().toDouble();
    double z0 = m_z0Edit->text().toDouble();
    
    // Convert frequency to Hz
    double freqMult = 1e9;  // Default GHz
    switch (m_freqUnitCombo->currentIndex()) {
        case 0: freqMult = 1e6; break;  // MHz
        case 1: freqMult = 1e9; break;  // GHz
        case 2: freqMult = 1e3; break;  // kHz
    }
    double freq = freqVal * freqMult;
    
    // Set calculator parameters
    m_calculator.setSourceImpedance(std::complex<double>(sourceR, sourceX));
    m_calculator.setLoadImpedance(std::complex<double>(loadR, loadX));
    m_calculator.setFrequency(freq);
    m_calculator.setZ0(z0);
    
    // Calculate solutions based on selected topology
    m_solutions.clear();
    int topoIndex = m_topologyCombo->currentIndex();
    
    if (topoIndex == 0 || topoIndex == 1) {
        auto lsec = m_calculator.calculateLSection();
        m_solutions.insert(m_solutions.end(), lsec.begin(), lsec.end());
    }
    if (topoIndex == 0 || topoIndex == 2) {
        auto pi = m_calculator.calculatePiNetwork();
        m_solutions.insert(m_solutions.end(), pi.begin(), pi.end());
    }
    if (topoIndex == 0 || topoIndex == 3) {
        auto t = m_calculator.calculateTNetwork();
        m_solutions.insert(m_solutions.end(), t.begin(), t.end());
    }
    
    updateResults();
}

void MatchingWizard::updateResults()
{
    m_resultsTable->setRowCount(static_cast<int>(m_solutions.size()));
    
    for (size_t i = 0; i < m_solutions.size(); ++i) {
        const auto& sol = m_solutions[i];
        int row = static_cast<int>(i);
        
        // Topology
        m_resultsTable->setItem(row, 0, 
            new QTableWidgetItem(topologyName(sol.topology)));
        
        // Elements count
        m_resultsTable->setItem(row, 1, 
            new QTableWidgetItem(QString::number(sol.elements.size())));
        
        // Q value
        m_resultsTable->setItem(row, 2, 
            new QTableWidgetItem(QString::number(sol.networkQ(), 'f', 2)));
        
        // Description
        m_resultsTable->setItem(row, 3, 
            new QTableWidgetItem(sol.toDescription()));
    }
    
    m_resultsTable->resizeColumnsToContents();
    
    if (m_solutions.empty()) {
        m_statusLabel->setText(tr("No matching solutions found."));
    } else {
        m_statusLabel->setText(tr("Found %1 solution(s).").arg(m_solutions.size()));
    }
    
    m_selectedSolution = MatchingSolution();
    m_applyBtn->setEnabled(false);
}

void MatchingWizard::onSolutionSelected(int row, int column)
{
    Q_UNUSED(column);
    
    if (row >= 0 && row < static_cast<int>(m_solutions.size())) {
        m_selectedSolution = m_solutions[row];
        m_applyBtn->setEnabled(true);
    }
}

void MatchingWizard::onApply()
{
    if (m_selectedSolution.valid) {
        emit solutionSelected(m_selectedSolution);
        accept();
    }
}

void MatchingWizard::onTopologyChanged(int index)
{
    Q_UNUSED(index);
    // Could trigger recalculation here
}

QString MatchingWizard::topologyName(MatchingTopology topo) const
{
    switch (topo) {
        case MatchingTopology::LSection: return tr("L-Section");
        case MatchingTopology::LSection_Reversed: return tr("L-Section (Rev)");
        case MatchingTopology::PiNetwork: return tr("Pi-Network");
        case MatchingTopology::TNetwork: return tr("T-Network");
        case MatchingTopology::SingleStubOpen: return tr("Single Stub (Open)");
        case MatchingTopology::SingleStubShort: return tr("Single Stub (Short)");
        case MatchingTopology::QuarterWave: return tr("λ/4 Transformer");
        default: return tr("Unknown");
    }
}

} // namespace SmithTool
