/**
 * @file mainwindow.cpp
 * @brief Main window implementation
 */

#include "mainwindow.h"
#include "componenteditdialog.h"
#include <QMessageBox>
#include <QApplication>
#include <QStyle>
#include <QScreen>
#include <QInputDialog>

namespace SmithTool {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_sourceZ(50.0, 0.0)
    , m_loadZ(50.0, 0.0)
{
    setupUI();
    setupMenus();
    setupToolbar();
    setupStatusBar();
    connectSignals();
    
    setWindowTitle(tr("SmithTool - Interactive Smith Chart"));
    resize(1200, 800);
    
    // Center on screen
    QScreen* screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();
        int x = (screenGeometry.width() - width()) / 2;
        int y = (screenGeometry.height() - height()) / 2;
        move(x, y);
    }
}

void MainWindow::setupUI()
{
    // Create splitter for Smith Chart and Circuit View
    m_splitter = new QSplitter(Qt::Vertical, this);
    
    // Smith Chart as main view
    m_smithChart = new SmithChartWidget(m_splitter);
    m_splitter->addWidget(m_smithChart);
    
    // Circuit diagram view below
    m_circuitView = new CircuitView(m_splitter);
    m_circuitView->setMinimumHeight(120);
    m_circuitView->setMaximumHeight(200);
    m_splitter->addWidget(m_circuitView);
    
    m_splitter->setStretchFactor(0, 4);  // Smith chart gets more space
    m_splitter->setStretchFactor(1, 1);
    
    setCentralWidget(m_splitter);
    
    // Component panel dock (right side)
    m_componentDock = new QDockWidget(tr("Properties"), this);
    m_componentPanel = new ComponentPanel(m_componentDock);
    m_componentDock->setWidget(m_componentPanel);
    m_componentDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, m_componentDock);
    
    // Impedance input panel dock (right side below component panel)
    m_impedanceDock = new QDockWidget(tr("Impedances"), this);
    m_impedancePanel = new ImpedanceInputPanel(m_impedanceDock);
    m_impedanceDock->setWidget(m_impedancePanel);
    m_impedanceDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, m_impedanceDock);
    
    // Element toolbar
    m_elementToolbar = new ElementToolbar(this);
    addToolBar(Qt::TopToolBarArea, m_elementToolbar);
}

void MainWindow::setupMenus()
{
    // File menu
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    
    m_openAction = new QAction(tr("&Open..."), this);
    m_openAction->setShortcut(QKeySequence::Open);
    m_openAction->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    fileMenu->addAction(m_openAction);
    
    m_saveAction = new QAction(tr("&Save As..."), this);
    m_saveAction->setShortcut(QKeySequence::SaveAs);
    m_saveAction->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    fileMenu->addAction(m_saveAction);
    
    fileMenu->addSeparator();
    
    m_exportAction = new QAction(tr("&Export Image..."), this);
    m_exportAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_E));
    fileMenu->addAction(m_exportAction);
    
    m_exportSpiceAction = new QAction(tr("Export &SPICE Netlist..."), this);
    m_exportSpiceAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    fileMenu->addAction(m_exportSpiceAction);
    
    fileMenu->addSeparator();
    
    m_exitAction = new QAction(tr("E&xit"), this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(m_exitAction);
    
    // View menu
    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
    
    m_admittanceAction = new QAction(tr("Show &Admittance Grid"), this);
    m_admittanceAction->setCheckable(true);
    m_admittanceAction->setChecked(false);
    viewMenu->addAction(m_admittanceAction);
    
    m_vswrAction = new QAction(tr("Show &VSWR Circles"), this);
    m_vswrAction->setCheckable(true);
    m_vswrAction->setChecked(true);
    viewMenu->addAction(m_vswrAction);
    
    m_labelsAction = new QAction(tr("Show &Labels"), this);
    m_labelsAction->setCheckable(true);
    m_labelsAction->setChecked(true);
    viewMenu->addAction(m_labelsAction);
    
    m_qCirclesAction = new QAction(tr("Show &Q Circles"), this);
    m_qCirclesAction->setCheckable(true);
    m_qCirclesAction->setChecked(false);
    viewMenu->addAction(m_qCirclesAction);
    
    m_configureQCirclesAction = new QAction(tr("Configure Q &Values..."), this);
    viewMenu->addAction(m_configureQCirclesAction);
    
    viewMenu->addSeparator();
    viewMenu->addAction(m_componentDock->toggleViewAction());
    viewMenu->addAction(m_impedanceDock->toggleViewAction());
    
    // Tools menu
    QMenu* toolsMenu = menuBar()->addMenu(tr("&Tools"));
    
    m_matchingWizardAction = new QAction(tr("&Matching Design..."), this);
    m_matchingWizardAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    toolsMenu->addAction(m_matchingWizardAction);
    
    // Help menu
    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    
    m_aboutAction = new QAction(tr("&About"), this);
    helpMenu->addAction(m_aboutAction);
}

void MainWindow::setupToolbar()
{
    QToolBar* toolbar = addToolBar(tr("Main"));
    toolbar->addAction(m_openAction);
    toolbar->addAction(m_saveAction);
    toolbar->addSeparator();
    toolbar->addAction(m_exportAction);
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::connectSignals()
{
    // File actions
    connect(m_openAction, &QAction::triggered, this, &MainWindow::onOpenFile);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::onSaveFile);
    connect(m_exportAction, &QAction::triggered, this, &MainWindow::onExportImage);
    connect(m_exportSpiceAction, &QAction::triggered, this, &MainWindow::onExportSpice);
    connect(m_exitAction, &QAction::triggered, qApp, &QApplication::quit);
    
    // View actions
    connect(m_admittanceAction, &QAction::toggled, this, &MainWindow::onToggleAdmittance);
    connect(m_vswrAction, &QAction::toggled, this, &MainWindow::onToggleVSWR);
    connect(m_labelsAction, &QAction::toggled, this, &MainWindow::onToggleLabels);
    connect(m_qCirclesAction, &QAction::toggled, this, &MainWindow::onToggleQCircles);
    connect(m_configureQCirclesAction, &QAction::triggered, this, &MainWindow::onConfigureQCircles);
    
    // Help actions
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
    
    // Smith chart signals
    connect(m_smithChart, &SmithChartWidget::pointClicked, 
            this, &MainWindow::onPointClicked);
    
    // Component panel signals
    connect(m_componentPanel, &ComponentPanel::frequencyChanged,
            this, &MainWindow::onFrequencyChanged);
    connect(m_componentPanel, &ComponentPanel::z0Changed,
            this, &MainWindow::onZ0Changed);
    
    // Element toolbar signals - Note: ElementToolbar handles value input internally
    // We connect to slots that add elements with user-input values
    connect(m_elementToolbar, &ElementToolbar::addSeriesResistor, 
            [this](double) { onAddSeriesR(); });
    connect(m_elementToolbar, &ElementToolbar::addSeriesInductor, 
            [this](double) { onAddSeriesL(); });
    connect(m_elementToolbar, &ElementToolbar::addSeriesCapacitor, 
            [this](double) { onAddSeriesC(); });
    connect(m_elementToolbar, &ElementToolbar::addShuntResistor, 
            [this](double) { onAddShuntR(); });
    connect(m_elementToolbar, &ElementToolbar::addShuntInductor, 
            [this](double) { onAddShuntL(); });
    connect(m_elementToolbar, &ElementToolbar::addShuntCapacitor, 
            [this](double) { onAddShuntC(); });
    connect(m_elementToolbar, &ElementToolbar::undoLastElement, 
            this, &MainWindow::onRemoveLastElement);
    connect(m_elementToolbar, &ElementToolbar::clearAllElements, 
            this, &MainWindow::onClearElements);
    
    // Impedance panel signals
    connect(m_impedancePanel, &ImpedanceInputPanel::sourceImpedanceChanged,
            this, &MainWindow::onSourceImpedanceChanged);
    connect(m_impedancePanel, &ImpedanceInputPanel::loadImpedanceChanged,
            this, &MainWindow::onLoadImpedanceChanged);
    
    // Matching wizard
    connect(m_matchingWizardAction, &QAction::triggered, 
            this, &MainWindow::onOpenMatchingWizard);
    
    // Target point selection for adding elements
    connect(m_smithChart, &SmithChartWidget::targetPointSelected,
            this, &MainWindow::onTargetPointSelected);
    
    // Select point mode from toolbar
    connect(m_elementToolbar, &ElementToolbar::selectPointForElement,
            [this](ComponentType type, ConnectionType conn) {
                m_smithChart->setPendingElementType(type, conn);
                statusBar()->showMessage(tr("Click on chart to select target impedance point..."), 5000);
            });
    
    // Drag editing signals
    connect(m_smithChart, &SmithChartWidget::elementValueDragged,
            this, &MainWindow::onElementValueDragged);
    connect(m_smithChart, &SmithChartWidget::dragEditStarted,
            this, &MainWindow::onDragEditStarted);
    connect(m_smithChart, &SmithChartWidget::dragEditEnded,
            this, &MainWindow::onDragEditEnded);
    
    // Circuit view double-click to edit element
    connect(m_circuitView, &CircuitView::elementDoubleClicked,
            this, &MainWindow::onCircuitElementDoubleClicked);
    
    // Context menu signals
    connect(m_smithChart, &SmithChartWidget::editElementRequested,
            this, &MainWindow::onCircuitElementDoubleClicked);
    connect(m_smithChart, &SmithChartWidget::deleteElementRequested,
            this, &MainWindow::onDeleteElement);
}

void MainWindow::onOpenFile()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Open Touchstone File"),
        QString(),
        tr("Touchstone Files (*.s1p *.s2p);;All Files (*)")
    );
    
    if (!filename.isEmpty()) {
        loadTouchstoneFile(filename);
    }
}

void MainWindow::onSaveFile()
{
    if (m_currentData.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), 
                            tr("No data to save."));
        return;
    }
    
    QString filename = QFileDialog::getSaveFileName(
        this,
        tr("Save Touchstone File"),
        QString(),
        tr("Touchstone 1-Port (*.s1p);;Touchstone 2-Port (*.s2p)")
    );
    
    if (!filename.isEmpty()) {
        TouchstoneWriter writer;
        if (!writer.write(filename, m_currentData)) {
            QMessageBox::critical(this, tr("Error"),
                                 tr("Failed to save file: %1").arg(writer.lastError()));
        }
    }
}

void MainWindow::onExportImage()
{
    QString filename = QFileDialog::getSaveFileName(
        this,
        tr("Export Image"),
        "smith_chart.png",
        tr("PNG Image (*.png);;JPEG Image (*.jpg);;SVG Image (*.svg)")
    );
    
    if (!filename.isEmpty()) {
        QPixmap pixmap(m_smithChart->size());
        m_smithChart->render(&pixmap);
        pixmap.save(filename);
        statusBar()->showMessage(tr("Image exported: %1").arg(filename), 3000);
    }
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, tr("About SmithTool"),
        tr("<h2>SmithTool</h2>"
           "<p>Version 0.1.0</p>"
           "<p>Interactive Smith Chart Tool for RF Engineering</p>"
           "<p>Features:</p>"
           "<ul>"
           "<li>Interactive impedance display</li>"
           "<li>Component value calculation</li>"
           "<li>Touchstone file support</li>"
           "<li>VSWR visualization</li>"
           "</ul>"));
}

void MainWindow::onPointClicked(std::complex<double> gamma, std::complex<double> z)
{
    Q_UNUSED(z);  // Impedance is recalculated in panel with current Z0
    m_componentPanel->setGamma(gamma);
    updateStatusBar();
}

void MainWindow::onFrequencyChanged(double freq)
{
    m_smithChart->setFrequency(freq);
    updateStatusBar();
}

void MainWindow::onZ0Changed(double z0)
{
    m_smithChart->setZ0(z0);
    updateStatusBar();
}

void MainWindow::onToggleAdmittance(bool show)
{
    m_smithChart->setShowAdmittanceGrid(show);
}

void MainWindow::onToggleVSWR(bool show)
{
    m_smithChart->setShowVSWRCircles(show);
}

void MainWindow::onToggleLabels(bool show)
{
    m_smithChart->setShowLabels(show);
}

void MainWindow::onToggleQCircles(bool show)
{
    m_smithChart->setShowQCircles(show);
}

void MainWindow::onConfigureQCircles()
{
    // Get current Q values as string
    std::vector<double> currentQ = {0.5, 1.0, 2.0, 5.0};  // Default values
    
    QString currentStr;
    for (size_t i = 0; i < currentQ.size(); ++i) {
        if (i > 0) currentStr += ", ";
        currentStr += QString::number(currentQ[i]);
    }
    
    bool ok;
    QString input = QInputDialog::getText(
        this,
        tr("Configure Q Circles"),
        tr("Enter Q values (comma-separated):"),
        QLineEdit::Normal,
        currentStr,
        &ok
    );
    
    if (!ok || input.isEmpty()) return;
    
    // Parse input
    std::vector<double> qValues;
    QStringList parts = input.split(',', Qt::SkipEmptyParts);
    
    for (const QString& part : parts) {
        bool valid;
        double val = part.trimmed().toDouble(&valid);
        if (valid && val > 0) {
            qValues.push_back(val);
        }
    }
    
    if (qValues.empty()) {
        QMessageBox::warning(this, tr("Invalid Input"),
            tr("No valid Q values entered. Please enter positive numbers separated by commas."));
        return;
    }
    
    // Apply Q values
    m_smithChart->setQValues(qValues);
    
    // Enable Q circles if not already enabled
    if (!m_qCirclesAction->isChecked()) {
        m_qCirclesAction->setChecked(true);
    }
    
    statusBar()->showMessage(tr("Q circles configured: %1 values").arg(qValues.size()), 3000);
}

void MainWindow::onAddSeriesR()
{
    addMatchingElement(ComponentType::Resistor, ConnectionType::Series);
}

void MainWindow::onAddSeriesL()
{
    addMatchingElement(ComponentType::Inductor, ConnectionType::Series);
}

void MainWindow::onAddSeriesC()
{
    addMatchingElement(ComponentType::Capacitor, ConnectionType::Series);
}

void MainWindow::onAddShuntR()
{
    addMatchingElement(ComponentType::Resistor, ConnectionType::Shunt);
}

void MainWindow::onAddShuntL()
{
    addMatchingElement(ComponentType::Inductor, ConnectionType::Shunt);
}

void MainWindow::onAddShuntC()
{
    addMatchingElement(ComponentType::Capacitor, ConnectionType::Shunt);
}

void MainWindow::onRemoveLastElement()
{
    if (m_matchingTrace.numSegments() > 0) {
        m_matchingTrace.removeLastSegment();
        m_circuitView->removeLastElement();
        updateTraces();
    }
}

void MainWindow::onClearElements()
{
    m_matchingTrace.clear();
    m_circuitView->clearElements();
    updateTraces();
}

void MainWindow::onSourceImpedanceChanged(std::complex<double> zs)
{
    m_sourceZ = zs;
    m_matchingTrace.setSourceImpedance(zs);
    m_circuitView->setSourceImpedance(zs);
    m_smithChart->setSourceImpedance(zs);
    updateTraces();
}

void MainWindow::onLoadImpedanceChanged(std::complex<double> zl)
{
    m_loadZ = zl;
    m_matchingTrace.setLoadImpedance(zl);
    m_circuitView->setLoadImpedance(zl);
    m_smithChart->setLoadImpedance(zl);
    updateTraces();
}

void MainWindow::addMatchingElement(ComponentType type, ConnectionType conn)
{
    double freq = m_componentPanel->frequency();
    double z0 = m_componentPanel->z0();
    
    m_matchingTrace.setFrequency(freq);
    m_matchingTrace.setZ0(z0);
    
    // Get value from user
    QString typeStr;
    QString unitStr;
    double defaultVal;
    
    switch (type) {
        case ComponentType::Resistor:
            typeStr = tr("Resistor");
            unitStr = "Ω";
            defaultVal = 50.0;
            break;
        case ComponentType::Inductor:
            typeStr = tr("Inductor");
            unitStr = "nH";
            defaultVal = 10.0;
            break;
        case ComponentType::Capacitor:
            typeStr = tr("Capacitor");
            unitStr = "pF";
            defaultVal = 1.0;
            break;
        default:
            return;
    }
    
    QString connStr = (conn == ConnectionType::Series) ? tr("Series") : tr("Shunt");
    
    bool ok;
    double value = QInputDialog::getDouble(
        this,
        tr("Add %1 %2").arg(connStr).arg(typeStr),
        tr("Value (%1):").arg(unitStr),
        defaultVal,
        0.001, 1e6, 3, &ok
    );
    
    if (!ok) return;
    
    // Convert to base units
    double baseValue;
    switch (type) {
        case ComponentType::Resistor:
            baseValue = value;  // Already in ohms
            break;
        case ComponentType::Inductor:
            baseValue = value * 1e-9;  // nH to H
            break;
        case ComponentType::Capacitor:
            baseValue = value * 1e-12;  // pF to F
            break;
        default:
            return;
    }
    
    // Calculate and add trace segment
    TraceSegment segment;
    if (conn == ConnectionType::Series) {
        segment = m_matchingTrace.calculateSeriesElement(type, baseValue);
    } else {
        segment = m_matchingTrace.calculateShuntElement(type, baseValue);
    }
    m_matchingTrace.addSegment(segment);
    
    // Add to circuit view
    m_circuitView->addElement(type, conn, baseValue);
    
    updateTraces();
}

void MainWindow::updateTraces()
{
    // Update Smith chart display
    m_smithChart->setMatchingTrace(m_matchingTrace);
    
    updateStatusBar();
}

void MainWindow::loadTouchstoneFile(const QString& filename)
{
    TouchstoneParser parser;
    
    if (parser.parse(filename)) {
        m_currentData = parser.data();
        m_currentFile = filename;
        m_smithChart->setSParamData(m_currentData);
        
        statusBar()->showMessage(
            tr("Loaded: %1 (%2 points)")
                .arg(filename)
                .arg(m_currentData.numPoints()),
            5000
        );
        
        setWindowTitle(tr("SmithTool - %1").arg(QFileInfo(filename).fileName()));
    } else {
        QMessageBox::critical(this, tr("Error"),
            tr("Failed to load file:\n%1").arg(parser.lastError()));
    }
}

void MainWindow::updateStatusBar()
{
    QString msg = tr("Z₀ = %1 Ω  |  f = %2")
        .arg(m_componentPanel->z0(), 0, 'f', 1)
        .arg(m_componentPanel->frequency() / 1e9, 0, 'f', 3);
    statusBar()->showMessage(msg);
}

void MainWindow::onOpenMatchingWizard()
{
    MatchingWizard wizard(this);
    wizard.setSourceImpedance(m_sourceZ);
    wizard.setLoadImpedance(m_loadZ);
    wizard.setFrequency(m_componentPanel->frequency());
    wizard.setZ0(m_componentPanel->z0());
    
    connect(&wizard, &MatchingWizard::solutionSelected,
            this, &MainWindow::onApplyMatchingSolution);
    
    wizard.exec();
}

void MainWindow::onApplyMatchingSolution(const MatchingSolution& solution)
{
    // Clear existing elements
    onClearElements();
    
    // Apply each element from the solution
    for (const auto& elem : solution.elements) {
        // Calculate and add trace segment
        m_matchingTrace.setFrequency(solution.frequency);
        m_matchingTrace.setZ0(m_componentPanel->z0());
        
        TraceSegment segment;
        if (elem.connection == ConnectionType::Series) {
            segment = m_matchingTrace.calculateSeriesElement(elem.type, elem.value);
        } else {
            segment = m_matchingTrace.calculateShuntElement(elem.type, elem.value);
        }
        m_matchingTrace.addSegment(segment);
        
        // Add to circuit view
        m_circuitView->addElement(elem.type, elem.connection, elem.value);
    }
    
    updateTraces();
    
    statusBar()->showMessage(
        tr("Applied matching: %1").arg(solution.toDescription()), 5000);
}

void MainWindow::onTargetPointSelected(std::complex<double> /* gamma */, std::complex<double> z,
                                       ComponentType type, ConnectionType conn)
{
    // Calculate element value to reach target impedance from current position
    Complex currentZ = m_matchingTrace.currentImpedance();
    double freq = m_componentPanel->frequency();
    double omega = 2.0 * M_PI * freq;
    
    double value = 0;
    bool validElement = true;
    
    if (conn == ConnectionType::Series) {
        // Series element: change reactance
        double targetX = z.imag();
        double currentX = currentZ.imag();
        double deltaX = targetX - currentX;
        
        switch (type) {
            case ComponentType::Inductor:
                if (deltaX > 0) {
                    value = deltaX / omega;  // L = X / omega
                } else {
                    validElement = false;
                }
                break;
            case ComponentType::Capacitor:
                if (deltaX < 0) {
                    value = -1.0 / (omega * deltaX);  // C = -1/(omega*X)
                } else {
                    validElement = false;
                }
                break;
            case ComponentType::Resistor:
                value = z.real() - currentZ.real();
                if (value < 0) validElement = false;
                break;
            default:
                validElement = false;
        }
    } else {
        // Shunt element: change susceptance (work in admittance)
        Complex currentY = 1.0 / currentZ;
        Complex targetY = 1.0 / z;
        double deltaB = targetY.imag() - currentY.imag();
        
        switch (type) {
            case ComponentType::Capacitor:
                if (deltaB > 0) {
                    value = deltaB / omega;  // C = B / omega
                } else {
                    validElement = false;
                }
                break;
            case ComponentType::Inductor:
                if (deltaB < 0) {
                    value = -1.0 / (omega * deltaB);  // L = -1/(omega*B)
                } else {
                    validElement = false;
                }
                break;
            case ComponentType::Resistor:
                value = 1.0 / (targetY.real() - currentY.real());
                if (value < 0) validElement = false;
                break;
            default:
                validElement = false;
        }
    }
    
    if (validElement && value > 0) {
        // Add the element
        addMatchingElement(type, conn);
        
        // Update the element value in the trace
        // The last segment was just added, update its value
        
        statusBar()->showMessage(
            tr("Added element to reach Z = %1 + j%2 \u03a9")
                .arg(z.real(), 0, 'f', 1)
                .arg(z.imag(), 0, 'f', 1), 5000);
    } else {
        statusBar()->showMessage(
            tr("Cannot add %1 element to reach target point")
                .arg(type == ComponentType::Inductor ? "inductor" :
                     type == ComponentType::Capacitor ? "capacitor" : "resistor"), 5000);
    }
}

void MainWindow::onElementValueDragged(int segmentIndex, double newValue)
{
    if (segmentIndex < 0 || segmentIndex >= m_matchingTrace.numSegments()) {
        return;
    }
    
    // Update the trace with the new value
    m_matchingTrace.updateSegmentValue(segmentIndex, newValue);
    
    // Update the circuit view
    m_circuitView->updateElementValue(segmentIndex, newValue);
    
    // Update the Smith chart
    m_smithChart->setMatchingTrace(m_matchingTrace);
    
    // Update status bar with current value
    const auto& seg = m_matchingTrace.segment(segmentIndex);
    QString typeStr;
    QString valueStr;
    
    switch (seg.componentType) {
        case ComponentType::Inductor:
            typeStr = "L";
            valueStr = QString("%1 nH").arg(newValue * 1e9, 0, 'f', 2);
            break;
        case ComponentType::Capacitor:
            typeStr = "C";
            valueStr = QString("%1 pF").arg(newValue * 1e12, 0, 'f', 2);
            break;
        case ComponentType::Resistor:
            typeStr = "R";
            valueStr = QString("%1 \u03a9").arg(newValue, 0, 'f', 1);
            break;
        default:
            break;
    }
    
    statusBar()->showMessage(tr("Dragging: %1 = %2").arg(typeStr).arg(valueStr));
}

void MainWindow::onDragEditStarted(int segmentIndex)
{
    if (segmentIndex >= 0 && segmentIndex < m_matchingTrace.numSegments()) {
        const auto& seg = m_matchingTrace.segment(segmentIndex);
        statusBar()->showMessage(
            tr("Drag to modify element value (segment %1)").arg(segmentIndex + 1));
        Q_UNUSED(seg);
    }
}

void MainWindow::onDragEditEnded()
{
    updateStatusBar();
}

void MainWindow::onCircuitElementDoubleClicked(int index)
{
    if (index < 0 || index >= m_matchingTrace.numSegments()) {
        return;
    }
    
    const auto& seg = m_matchingTrace.segment(index);
    
    ComponentEditDialog dialog(this);
    dialog.setComponent(seg.componentType, seg.connectionType, seg.componentValue);
    dialog.setComponentIndex(index);
    dialog.setFrequency(m_componentPanel->frequency());
    dialog.setWindowTitle(tr("Edit Element %1").arg(index + 1));
    
    // Connect preview signal
    connect(&dialog, &ComponentEditDialog::previewRequested,
            [this](int idx, double newValue) {
                m_matchingTrace.updateSegmentValue(idx, newValue);
                m_smithChart->setMatchingTrace(m_matchingTrace);
            });
    
    if (dialog.exec() == QDialog::Accepted) {
        double newValue = dialog.componentValue();
        m_matchingTrace.updateSegmentValue(index, newValue);
        m_circuitView->updateElementValue(index, newValue);
        m_smithChart->setMatchingTrace(m_matchingTrace);
        updateStatusBar();
    } else {
        // Restore original value if cancelled
        m_matchingTrace.updateSegmentValue(index, seg.componentValue);
        m_smithChart->setMatchingTrace(m_matchingTrace);
    }
}

void MainWindow::onDeleteElement(int index)
{
    if (index < 0 || index >= m_matchingTrace.numSegments()) {
        return;
    }
    
    // Confirm deletion
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("Delete Element"),
        tr("Delete element %1?").arg(index + 1),
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        // Remove element from trace
        // For now, we can only remove the last element easily
        // To remove from middle, we need to rebuild the trace
        if (index == m_matchingTrace.numSegments() - 1) {
            m_matchingTrace.removeLastSegment();
            m_circuitView->removeLastElement();
        } else {
            // Remove element from middle - need to rebuild
            // Store all segments after the deleted one
            std::vector<TraceSegment> remaining;
            for (int i = index + 1; i < m_matchingTrace.numSegments(); ++i) {
                remaining.push_back(m_matchingTrace.segment(i));
            }
            
            // Clear and rebuild up to deleted index
            while (m_matchingTrace.numSegments() > index) {
                m_matchingTrace.removeLastSegment();
            }
            m_circuitView->clearElements();
            
            // Re-add remaining elements from the remaining segments
            for (const auto& seg : remaining) {
                if (seg.connectionType == ConnectionType::Series) {
                    m_matchingTrace.addSegment(
                        m_matchingTrace.calculateSeriesElement(seg.componentType, seg.componentValue));
                } else {
                    m_matchingTrace.addSegment(
                        m_matchingTrace.calculateShuntElement(seg.componentType, seg.componentValue));
                }
                m_circuitView->addElement(seg.componentType, seg.connectionType, seg.componentValue);
            }
        }
        
        m_smithChart->setMatchingTrace(m_matchingTrace);
        updateStatusBar();
    }
}

void MainWindow::onExportSpice()
{
    if (m_matchingTrace.numSegments() == 0) {
        QMessageBox::information(this, tr("Export SPICE Netlist"),
            tr("No matching network to export. Please add some elements first."));
        return;
    }
    
    QString filename = QFileDialog::getSaveFileName(
        this,
        tr("Export SPICE Netlist"),
        QString(),
        tr("SPICE Netlist (*.sp *.cir);;LTspice (*.asc);;Ngspice (*.cir);;All Files (*)")
    );
    
    if (filename.isEmpty()) {
        return;
    }
    
    // Determine format from extension
    SpiceFormat format = SpiceFormat::Standard;
    if (filename.endsWith(".asc", Qt::CaseInsensitive)) {
        format = SpiceFormat::LTspice;
    } else if (filename.contains("ngspice", Qt::CaseInsensitive)) {
        format = SpiceFormat::Ngspice;
    }
    
    SpiceExporter exporter;
    exporter.setFormat(format);
    exporter.setFrequency(m_componentPanel->frequency());
    exporter.setZ0(m_componentPanel->z0());
    exporter.setSourceImpedance(m_sourceZ);
    exporter.setLoadImpedance(m_loadZ);
    exporter.setTitle("SmithTool Matching Network");
    
    // Set frequency range (decade around center frequency)
    double centerFreq = m_componentPanel->frequency();
    exporter.setFrequencyRange(centerFreq / 10.0, centerFreq * 10.0, 101);
    
    // Configure matching trace with proper parameters
    MatchingTrace exportTrace = m_matchingTrace;
    exportTrace.setSourceImpedance(m_sourceZ);
    exportTrace.setLoadImpedance(m_loadZ);
    exportTrace.setZ0(m_componentPanel->z0());
    exportTrace.setFrequency(centerFreq);
    
    if (exporter.exportToFile(filename, exportTrace)) {
        QMessageBox::information(this, tr("Export SPICE Netlist"),
            tr("Netlist exported successfully to:\n%1").arg(filename));
    } else {
        QMessageBox::warning(this, tr("Export Error"),
            tr("Failed to export netlist to:\n%1").arg(filename));
    }
}

} // namespace SmithTool
