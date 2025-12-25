/**
 * @file mainwindow.h
 * @brief Main window for SmithTool application
 */

#ifndef SMITHTOOL_MAINWINDOW_H
#define SMITHTOOL_MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QFileDialog>
#include <QSplitter>

#include "smithchartwidget.h"
#include "componentpanel.h"
#include "impedanceinputpanel.h"
#include "elementtoolbar.h"
#include "circuitview.h"
#include "matchingwizard.h"
#include "../data/touchstone.h"
#include "../core/trace.h"
#include "../core/matching.h"
#include "../data/spiceexporter.h"

namespace SmithTool {

/**
 * @brief Main application window
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void onOpenFile();
    void onSaveFile();
    void onExportImage();
    void onAbout();
    
    void onPointClicked(std::complex<double> gamma, std::complex<double> z);
    void onFrequencyChanged(double freq);
    void onZ0Changed(double z0);
    
    void onToggleAdmittance(bool show);
    void onToggleVSWR(bool show);
    void onToggleLabels(bool show);
    void onToggleQCircles(bool show);
    
    // Element toolbar slots
    void onAddSeriesR();
    void onAddSeriesL();
    void onAddSeriesC();
    void onAddShuntR();
    void onAddShuntL();
    void onAddShuntC();
    void onRemoveLastElement();
    void onClearElements();
    
    // Impedance input slots
    void onSourceImpedanceChanged(std::complex<double> zs);
    void onLoadImpedanceChanged(std::complex<double> zl);
    
    // Matching wizard
    void onOpenMatchingWizard();
    void onApplyMatchingSolution(const MatchingSolution& solution);
    
    // Target point selection
    void onTargetPointSelected(std::complex<double> gamma, std::complex<double> z,
                               ComponentType type, ConnectionType conn);
    
    // Drag editing
    void onElementValueDragged(int segmentIndex, double newValue);
    void onDragEditStarted(int segmentIndex);
    void onDragEditEnded();
    
    // Circuit view element editing
    void onCircuitElementDoubleClicked(int index);
    void onDeleteElement(int index);
    
    // Export functions
    void onExportSpice();

private:
    void setupUI();
    void setupMenus();
    void setupToolbar();
    void setupStatusBar();
    void connectSignals();
    
    void loadTouchstoneFile(const QString& filename);
    void updateStatusBar();
    void addMatchingElement(ComponentType type, ConnectionType conn);
    void updateTraces();
    
    // Central widget with splitter
    QSplitter* m_splitter;
    SmithChartWidget* m_smithChart;
    CircuitView* m_circuitView;
    
    // Dock widgets
    QDockWidget* m_componentDock;
    ComponentPanel* m_componentPanel;
    QDockWidget* m_impedanceDock;
    ImpedanceInputPanel* m_impedancePanel;
    
    // Toolbars
    ElementToolbar* m_elementToolbar;
    
    // Matching network
    MatchingTrace m_matchingTrace;
    std::complex<double> m_sourceZ;
    std::complex<double> m_loadZ;
    
    // Current data
    SParamData m_currentData;
    QString m_currentFile;
    
    // Actions
    QAction* m_openAction;
    QAction* m_saveAction;
    QAction* m_exportAction;
    QAction* m_exitAction;
    QAction* m_admittanceAction;
    QAction* m_vswrAction;
    QAction* m_labelsAction;
    QAction* m_qCirclesAction;
    QAction* m_matchingWizardAction;
    QAction* m_aboutAction;
    QAction* m_exportSpiceAction;
};

} // namespace SmithTool

#endif // SMITHTOOL_MAINWINDOW_H
