/**
 * @file smithchartwidget.h
 * @brief Interactive Smith Chart widget
 */

#ifndef SMITHTOOL_SMITHCHARTWIDGET_H
#define SMITHTOOL_SMITHCHARTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <complex>
#include <vector>

#include "../core/smithmath.h"
#include "../core/impedance.h"
#include "../core/component.h"
#include "../core/trace.h"
#include "../data/sparamdata.h"

namespace SmithTool {

/**
 * @brief Display mode for Smith Chart
 */
enum class ChartMode {
    Impedance,      // Z Smith Chart
    Admittance,     // Y Smith Chart
    Combined        // Both overlaid
};

/**
 * @brief Interaction mode for Smith Chart
 */
enum class InteractionMode {
    Normal,         // Default click to show info
    AddElement,     // Click to select target point for element
    DragEdit        // Drag trace endpoint to modify element
};

/**
 * @brief Interactive Smith Chart widget
 */
class SmithChartWidget : public QWidget {
    Q_OBJECT

public:
    explicit SmithChartWidget(QWidget* parent = nullptr);
    ~SmithChartWidget() override = default;

    // Chart settings
    void setZ0(double z0);
    double z0() const { return m_z0; }
    
    void setFrequency(double freq_hz);
    double frequency() const { return m_frequency; }
    
    void setChartMode(ChartMode mode);
    ChartMode chartMode() const { return m_chartMode; }
    
    // Data display
    void setSParamData(const SParamData& data);
    void clearSParamData();
    
    // Marker
    void setMarkerGamma(const Complex& gamma);
    void setMarkerImpedance(const Complex& z);
    Complex markerGamma() const { return m_markerGamma; }
    Complex markerImpedance() const;
    
    // Source and Load impedance markers
    void setSourceImpedance(const Complex& zs);
    void setLoadImpedance(const Complex& zl);
    void clearSourceImpedance();
    void clearLoadImpedance();
    Complex sourceImpedance() const { return m_sourceZ; }
    Complex loadImpedance() const { return m_loadZ; }
    
    // Display options
    void setShowAdmittanceGrid(bool show);
    void setShowVSWRCircles(bool show);
    void setShowLabels(bool show);
    void setShowQCircles(bool show);
    void setQValues(const std::vector<double>& qValues);
    
    // Matching trace
    void setMatchingTrace(const MatchingTrace& trace);
    void clearMatchingTrace();
    
    // VSWR circles
    void addVSWRCircle(double vswr);
    void clearVSWRCircles();
    
    // Interaction mode
    void setInteractionMode(InteractionMode mode);
    InteractionMode interactionMode() const { return m_interactionMode; }
    void setPendingElementType(ComponentType type, ConnectionType conn);
    void cancelPendingElement();

signals:
    /**
     * @brief Emitted when user clicks on the chart
     */
    void pointClicked(std::complex<double> gamma, std::complex<double> z);
    
    /**
     * @brief Emitted when mouse hovers over chart
     */
    void pointHovered(std::complex<double> gamma, std::complex<double> z);
    
    /**
     * @brief Emitted when component values are calculated
     */
    void componentValuesCalculated(double freq, double r, double l, double c);
    
    /**
     * @brief Emitted when target point is selected in AddElement mode
     */
    void targetPointSelected(std::complex<double> gamma, std::complex<double> z, 
                             ComponentType type, ConnectionType conn);
    
    /**
     * @brief Emitted when element value is modified by dragging
     */
    void elementValueDragged(int segmentIndex, double newValue);
    
    /**
     * @brief Emitted when drag editing starts
     */
    void dragEditStarted(int segmentIndex);
    
    /**
     * @brief Emitted when drag editing ends
     */
    void dragEditEnded();
    
    /**
     * @brief Emitted when reset view is requested
     */
    void resetViewRequested();
    
    /**
     * @brief Emitted when delete element is requested
     */
    void deleteElementRequested(int segmentIndex);
    
    /**
     * @brief Emitted when edit element is requested via context menu
     */
    void editElementRequested(int segmentIndex);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

private:
    // Chart geometry
    QPointF m_center;
    double m_radius;
    int m_margin;
    
    // Settings
    double m_z0;
    double m_frequency;
    ChartMode m_chartMode;
    
    // Display options
    bool m_showAdmittanceGrid;
    bool m_showVSWRCircles;
    bool m_showLabels;
    bool m_showQCircles;
    
    // Marker
    Complex m_markerGamma;
    bool m_markerVisible;
    
    // Source and Load impedance
    Complex m_sourceZ;
    Complex m_loadZ;
    bool m_sourceVisible;
    bool m_loadVisible;
    
    // Interaction mode
    InteractionMode m_interactionMode;
    ComponentType m_pendingType;
    ConnectionType m_pendingConnection;
    bool m_hasPendingElement;
    int m_dragSegmentIndex;
    int m_hoverSegmentIndex;
    bool m_isDragging;
    Complex m_dragStartGamma;
    Complex m_previewGamma;
    double m_originalValue;
    static constexpr double DRAG_HIT_RADIUS = 10.0; // Pixels
    
    // Zoom and pan
    double m_zoomLevel;
    QPointF m_panOffset;
    bool m_isPanning;
    QPointF m_panStartPos;
    static constexpr double MIN_ZOOM = 0.5;
    static constexpr double MAX_ZOOM = 5.0;
    
    // Data
    SParamData m_sparamData;
    std::vector<double> m_vswrCircles;
    std::vector<double> m_qValues;
    MatchingTrace m_matchingTrace;
    
    // Standard grid values
    static const std::vector<double> s_resistanceValues;
    static const std::vector<double> s_reactanceValues;
    
    // Drawing methods
    void drawBackground(QPainter& painter);
    void drawUnitCircle(QPainter& painter);
    void drawResistanceCircles(QPainter& painter);
    void drawReactanceArcs(QPainter& painter);
    void drawAdmittanceGrid(QPainter& painter);
    void drawVSWRCircles(QPainter& painter);
    void drawQCircles(QPainter& painter);
    void drawLabels(QPainter& painter);
    void drawSParamTrace(QPainter& painter);
    void drawMarker(QPainter& painter);
    void drawMatchingTrace(QPainter& painter);
    void drawDragHandles(QPainter& painter);
    void drawImpedanceMarkers(QPainter& painter);
    
    // Drag detection
    int hitTestTraceEndpoint(const QPointF& pos) const;
    double calculateNewValueFromDrag(int segmentIndex, const Complex& newGamma) const;
    
    // Coordinate conversion
    QPointF gammaToScreen(const Complex& gamma) const;
    Complex screenToGamma(const QPointF& point) const;
    
    // Geometry helpers
    void updateChartGeometry();
    QRectF circleRect(const QPointF& center, double radius) const;
    
    // Arc clipping (clip to unit circle)
    void drawClippedArc(QPainter& painter, const QPointF& center, 
                        double radius, double startAngle, double spanAngle);
};

} // namespace SmithTool

#endif // SMITHTOOL_SMITHCHARTWIDGET_H
