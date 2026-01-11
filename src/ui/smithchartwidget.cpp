/**
 * @file smithchartwidget.cpp
 * @brief Interactive Smith Chart widget implementation
 */

#include "smithchartwidget.h"
#include <QPainterPath>
#include <QToolTip>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <cmath>

namespace SmithTool {

// Standard grid values
const std::vector<double> SmithChartWidget::s_resistanceValues = {0, 0.2, 0.5, 1.0, 2.0, 5.0};
const std::vector<double> SmithChartWidget::s_reactanceValues = {0.2, 0.5, 1.0, 2.0, 5.0};

SmithChartWidget::SmithChartWidget(QWidget* parent)
    : QWidget(parent)
    , m_margin(40)
    , m_z0(50.0)
    , m_frequency(1e9)
    , m_chartMode(ChartMode::Impedance)
    , m_showAdmittanceGrid(false)
    , m_showVSWRCircles(true)
    , m_showLabels(true)
    , m_showQCircles(false)
    , m_markerGamma(0, 0)
    , m_markerVisible(false)
    , m_sourceZ(50, 0)
    , m_loadZ(50, 0)
    , m_sourceVisible(false)
    , m_loadVisible(false)
    , m_interactionMode(InteractionMode::Normal)
    , m_pendingType(ComponentType::None)
    , m_pendingConnection(ConnectionType::Series)
    , m_hasPendingElement(false)
    , m_dragSegmentIndex(-1)
    , m_hoverSegmentIndex(-1)
    , m_isDragging(false)
    , m_dragStartGamma(0, 0)
    , m_previewGamma(0, 0)
    , m_originalValue(0.0)
    , m_zoomLevel(1.0)
    , m_panOffset(0, 0)
    , m_isPanning(false)
    , m_panStartPos(0, 0)
{
    setMinimumSize(400, 400);
    setMouseTracking(true);
    updateChartGeometry();
    
    // Default Q values
    m_qValues = {0.5, 1.0, 2.0, 5.0};
}

void SmithChartWidget::setZ0(double z0)
{
    m_z0 = z0;
    update();
}

void SmithChartWidget::setFrequency(double freq_hz)
{
    m_frequency = freq_hz;
    update();
}

void SmithChartWidget::setChartMode(ChartMode mode)
{
    m_chartMode = mode;
    update();
}

void SmithChartWidget::setSParamData(const SParamData& data)
{
    m_sparamData = data;
    update();
}

void SmithChartWidget::clearSParamData()
{
    m_sparamData.clear();
    update();
}

void SmithChartWidget::setMarkerGamma(const Complex& gamma)
{
    m_markerGamma = gamma;
    m_markerVisible = true;
    update();
}

void SmithChartWidget::setMarkerImpedance(const Complex& z)
{
    m_markerGamma = SmithMath::impedanceToGamma(z, m_z0);
    m_markerVisible = true;
    update();
}

Complex SmithChartWidget::markerImpedance() const
{
    return SmithMath::gammaToImpedance(m_markerGamma, m_z0);
}

void SmithChartWidget::setSourceImpedance(const Complex& zs)
{
    m_sourceZ = zs;
    m_sourceVisible = true;
    update();
}

void SmithChartWidget::setLoadImpedance(const Complex& zl)
{
    m_loadZ = zl;
    m_loadVisible = true;
    update();
}

void SmithChartWidget::clearSourceImpedance()
{
    m_sourceVisible = false;
    update();
}

void SmithChartWidget::clearLoadImpedance()
{
    m_loadVisible = false;
    update();
}

void SmithChartWidget::setShowAdmittanceGrid(bool show)
{
    m_showAdmittanceGrid = show;
    update();
}

void SmithChartWidget::setShowVSWRCircles(bool show)
{
    m_showVSWRCircles = show;
    update();
}

void SmithChartWidget::setShowLabels(bool show)
{
    m_showLabels = show;
    update();
}

void SmithChartWidget::setShowQCircles(bool show)
{
    m_showQCircles = show;
    update();
}

void SmithChartWidget::setQValues(const std::vector<double>& qValues)
{
    m_qValues = qValues;
    update();
}

void SmithChartWidget::setMatchingTrace(const MatchingTrace& trace)
{
    m_matchingTrace = trace;
    update();
}

void SmithChartWidget::clearMatchingTrace()
{
    m_matchingTrace.clear();
    update();
}

void SmithChartWidget::addVSWRCircle(double vswr)
{
    m_vswrCircles.push_back(vswr);
    update();
}

void SmithChartWidget::clearVSWRCircles()
{
    m_vswrCircles.clear();
    update();
}

void SmithChartWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    drawBackground(painter);
    drawResistanceCircles(painter);
    drawReactanceArcs(painter);
    
    if (m_showAdmittanceGrid || m_chartMode == ChartMode::Admittance || 
        m_chartMode == ChartMode::Combined) {
        drawAdmittanceGrid(painter);
    }
    
    drawUnitCircle(painter);
    
    if (m_showVSWRCircles) {
        drawVSWRCircles(painter);
    }
    
    if (m_showQCircles) {
        drawQCircles(painter);
    }
    
    if (m_showLabels) {
        drawLabels(painter);
    }
    
    drawSParamTrace(painter);
    drawMatchingTrace(painter);
    drawDragHandles(painter);
    drawImpedanceMarkers(painter);
    drawMarker(painter);
}

void SmithChartWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        // First check if clicking on a drag handle
        int hitSegment = hitTestTraceEndpoint(event->pos());
        if (hitSegment >= 0 && m_matchingTrace.numSegments() > 0) {
            // Start dragging
            m_isDragging = true;
            m_dragSegmentIndex = hitSegment;
            m_dragStartGamma = screenToGamma(event->pos());
            const auto& seg = m_matchingTrace.segment(hitSegment);
            m_originalValue = seg.componentValue;
            setCursor(Qt::ClosedHandCursor);
            emit dragEditStarted(hitSegment);
            return;
        }
        
        Complex gamma = screenToGamma(event->pos());
        
        if (SmithMath::isInsideUnitCircle(gamma)) {
            Complex z = SmithMath::gammaToImpedance(gamma, m_z0);
            
            // Handle based on interaction mode
            if (m_interactionMode == InteractionMode::AddElement && m_hasPendingElement) {
                // In AddElement mode, emit target point selection
                emit targetPointSelected(gamma, z, m_pendingType, m_pendingConnection);
                cancelPendingElement();
            } else {
                // Normal mode - show marker and calculate values
                m_markerGamma = gamma;
                m_markerVisible = true;
                
                emit pointClicked(gamma, z);
                
                // Calculate component values
                double r = z.real();
                double x = z.imag();
                double l = 0, c = 0;
                
                if (x > 0) {
                    l = x / (2.0 * SmithMath::PI * m_frequency);
                } else if (x < 0) {
                    c = -1.0 / (2.0 * SmithMath::PI * m_frequency * x);
                }
                
                emit componentValuesCalculated(m_frequency, r, l, c);
            }
            update();
        }
    } else if (event->button() == Qt::MiddleButton) {
        // Start panning
        m_isPanning = true;
        m_panStartPos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    } else if (event->button() == Qt::RightButton) {
        // Cancel pending element or drag on right click
        if (m_isDragging) {
            m_isDragging = false;
            m_dragSegmentIndex = -1;
            setCursor(Qt::ArrowCursor);
            emit dragEditEnded();
            update();
        } else if (m_hasPendingElement) {
            cancelPendingElement();
        }
    }
}

void SmithChartWidget::mouseMoveEvent(QMouseEvent* event)
{
    // Handle panning
    if (m_isPanning) {
        QPointF delta = event->pos() - m_panStartPos;
        m_panOffset += delta;
        m_panStartPos = event->pos();
        updateChartGeometry();
        update();
        return;
    }
    
    Complex gamma = screenToGamma(event->pos());
    
    // Handle dragging
    if (m_isDragging && m_dragSegmentIndex >= 0) {
        if (SmithMath::isInsideUnitCircle(gamma)) {
            m_previewGamma = gamma;
            double newValue = calculateNewValueFromDrag(m_dragSegmentIndex, gamma);
            if (newValue > 0) {
                emit elementValueDragged(m_dragSegmentIndex, newValue);
            }
            update();
        }
        return;
    }
    
    // Check for hover over drag handles
    int hitSegment = hitTestTraceEndpoint(event->pos());
    if (hitSegment != m_hoverSegmentIndex) {
        m_hoverSegmentIndex = hitSegment;
        if (hitSegment >= 0) {
            setCursor(Qt::OpenHandCursor);
        } else if (!m_hasPendingElement) {
            setCursor(Qt::ArrowCursor);
        }
        update();
    }
    
    if (SmithMath::isInsideUnitCircle(gamma)) {
        Complex z = SmithMath::gammaToImpedance(gamma, m_z0);
        emit pointHovered(gamma, z);
        
        // Show tooltip
        QString tip = QString("Z = %1 %2 j%3 Ω\n|Γ| = %4  ∠%5°")
            .arg(z.real(), 0, 'f', 1)
            .arg(z.imag() >= 0 ? "+" : "-")
            .arg(std::abs(z.imag()), 0, 'f', 1)
            .arg(std::abs(gamma), 0, 'f', 3)
            .arg(SmithMath::gammaPhaseDegrees(gamma), 0, 'f', 1);
        QToolTip::showText(event->globalPosition().toPoint(), tip, this);
    } else {
        QToolTip::hideText();
    }
}

void SmithChartWidget::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    updateChartGeometry();
}

void SmithChartWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_isDragging) {
        m_isDragging = false;
        m_dragSegmentIndex = -1;
        setCursor(Qt::ArrowCursor);
        emit dragEditEnded();
        update();
    } else if (event->button() == Qt::MiddleButton && m_isPanning) {
        m_isPanning = false;
        setCursor(Qt::ArrowCursor);
    }
}

void SmithChartWidget::wheelEvent(QWheelEvent* event)
{
    // Zoom with mouse wheel
    double delta = event->angleDelta().y() / 120.0;
    double zoomFactor = 1.0 + delta * 0.1;
    
    double newZoom = m_zoomLevel * zoomFactor;
    newZoom = qBound(MIN_ZOOM, newZoom, MAX_ZOOM);
    
    if (newZoom != m_zoomLevel) {
        // Zoom towards mouse position
        QPointF mousePos = event->position();
        QPointF centerToMouse = mousePos - m_center;
        
        // Adjust pan offset to zoom towards mouse position
        double ratio = newZoom / m_zoomLevel - 1.0;
        m_panOffset -= centerToMouse * ratio * 0.5;
        
        m_zoomLevel = newZoom;
        updateChartGeometry();
        update();
    }
    
    event->accept();
}

int SmithChartWidget::hitTestTraceEndpoint(const QPointF& pos) const
{
    if (m_matchingTrace.numSegments() == 0) return -1;
    
    const auto& segments = m_matchingTrace.segments();
    
    // Test each segment's endpoint (we allow dragging the last point of each segment)
    for (size_t i = 0; i < segments.size(); ++i) {
        const TraceSegment& seg = segments[i];
        if (seg.isEmpty()) continue;
        
        // Check end point of this segment
        QPointF endPt = gammaToScreen(seg.endPoint().gamma);
        double dist = std::hypot(pos.x() - endPt.x(), pos.y() - endPt.y());
        
        if (dist <= DRAG_HIT_RADIUS) {
            return static_cast<int>(i);
        }
    }
    
    return -1;
}

double SmithChartWidget::calculateNewValueFromDrag(int segmentIndex, const Complex& newGamma) const
{
    if (segmentIndex < 0 || segmentIndex >= m_matchingTrace.numSegments()) {
        return 0;
    }
    
    const TraceSegment& seg = m_matchingTrace.segment(segmentIndex);
    Complex startZ = seg.startPoint().impedance;
    Complex newZ = SmithMath::gammaToImpedance(newGamma, m_z0);
    double omega = 2.0 * SmithMath::PI * m_frequency;
    
    double newValue = 0;
    
    if (seg.connectionType == ConnectionType::Series) {
        // Series element changes reactance
        double deltaX = newZ.imag() - startZ.imag();
        
        switch (seg.componentType) {
            case ComponentType::Inductor:
                if (deltaX > 0) {
                    newValue = deltaX / omega;  // L = X / omega
                }
                break;
            case ComponentType::Capacitor:
                if (deltaX < 0) {
                    newValue = -1.0 / (omega * deltaX);  // C = -1/(omega*X)
                }
                break;
            case ComponentType::Resistor:
                newValue = newZ.real() - startZ.real();
                break;
            default:
                break;
        }
    } else {
        // Shunt element changes susceptance (work in admittance)
        Complex startY = 1.0 / startZ;
        Complex newY = 1.0 / newZ;
        double deltaB = newY.imag() - startY.imag();
        
        switch (seg.componentType) {
            case ComponentType::Capacitor:
                if (deltaB > 0) {
                    newValue = deltaB / omega;  // C = B / omega
                }
                break;
            case ComponentType::Inductor:
                if (deltaB < 0) {
                    newValue = -1.0 / (omega * deltaB);  // L = -1/(omega*B)
                }
                break;
            case ComponentType::Resistor:
                {
                    double deltaG = newY.real() - startY.real();
                    if (deltaG > 0) {
                        newValue = 1.0 / deltaG;
                    }
                }
                break;
            default:
                break;
        }
    }
    
    return newValue;
}

void SmithChartWidget::updateChartGeometry()
{
    int size = qMin(width(), height()) - 2 * m_margin;
    m_radius = (size / 2.0) * m_zoomLevel;
    m_center = QPointF(width() / 2.0, height() / 2.0) + m_panOffset;
}

QPointF SmithChartWidget::gammaToScreen(const Complex& gamma) const
{
    return SmithMath::gammaToScreen(gamma, m_center, m_radius);
}

Complex SmithChartWidget::screenToGamma(const QPointF& point) const
{
    return SmithMath::screenToGamma(point, m_center, m_radius);
}

QRectF SmithChartWidget::circleRect(const QPointF& center, double radius) const
{
    return QRectF(center.x() - radius, center.y() - radius, 
                  2 * radius, 2 * radius);
}

void SmithChartWidget::drawBackground(QPainter& painter)
{
    painter.fillRect(rect(), QColor(255, 255, 255));
}

void SmithChartWidget::drawUnitCircle(QPainter& painter)
{
    QPen pen(Qt::black, 2);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(circleRect(m_center, m_radius));
}

void SmithChartWidget::drawResistanceCircles(QPainter& painter)
{
    QPen pen(QColor(100, 100, 100), 1);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    
    for (double r : s_resistanceValues) {
        Complex center = SmithMath::constantRCircleCenter(r);
        double radius = SmithMath::constantRCircleRadius(r);
        
        QPointF screenCenter = gammaToScreen(center);
        double screenRadius = radius * m_radius;
        
        // Clip to unit circle using path intersection
        QPainterPath circlePath;
        circlePath.addEllipse(circleRect(screenCenter, screenRadius));
        
        QPainterPath unitPath;
        unitPath.addEllipse(circleRect(m_center, m_radius));
        
        painter.drawPath(circlePath.intersected(unitPath));
    }
}

void SmithChartWidget::drawReactanceArcs(QPainter& painter)
{
    QPen pen(QColor(100, 100, 100), 1);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    
    // Draw horizontal axis (x=0 line)
    painter.drawLine(QPointF(m_center.x() - m_radius, m_center.y()),
                     QPointF(m_center.x() + m_radius, m_center.y()));
    
    for (double x : s_reactanceValues) {
        // Positive reactance (upper half)
        Complex arcCenter = SmithMath::constantXArcCenter(x);
        double arcRadius = SmithMath::constantXArcRadius(x);
        QPointF screenCenter = gammaToScreen(arcCenter);
        double screenRadius = arcRadius * m_radius;
        
        QPainterPath arcPath;
        arcPath.addEllipse(circleRect(screenCenter, screenRadius));
        QPainterPath unitPath;
        unitPath.addEllipse(circleRect(m_center, m_radius));
        painter.drawPath(arcPath.intersected(unitPath));
        
        // Negative reactance (lower half)
        arcCenter = SmithMath::constantXArcCenter(-x);
        screenCenter = gammaToScreen(arcCenter);
        
        QPainterPath arcPath2;
        arcPath2.addEllipse(circleRect(screenCenter, screenRadius));
        painter.drawPath(arcPath2.intersected(unitPath));
    }
}

void SmithChartWidget::drawClippedArc(QPainter& painter, const QPointF& center,
                                       double radius, double startAngle, double spanAngle)
{
    QPainterPath arcPath;
    arcPath.arcMoveTo(circleRect(center, radius), startAngle);
    arcPath.arcTo(circleRect(center, radius), startAngle, spanAngle);
    
    QPainterPath unitPath;
    unitPath.addEllipse(circleRect(m_center, m_radius));
    
    painter.drawPath(arcPath.intersected(unitPath));
}

void SmithChartWidget::drawAdmittanceGrid(QPainter& painter)
{
    QPen pen(QColor(150, 150, 200), 1, Qt::DashLine);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    
    // Admittance circles are rotated 180° from impedance
    for (double g : s_resistanceValues) {
        Complex center = SmithMath::constantRCircleCenter(g);
        center = -center;  // Rotate 180°
        double radius = SmithMath::constantRCircleRadius(g);
        
        QPointF screenCenter = gammaToScreen(center);
        double screenRadius = radius * m_radius;
        
        QPainterPath circlePath;
        circlePath.addEllipse(circleRect(screenCenter, screenRadius));
        
        QPainterPath unitPath;
        unitPath.addEllipse(circleRect(m_center, m_radius));
        
        painter.drawPath(circlePath.intersected(unitPath));
    }
}

void SmithChartWidget::drawVSWRCircles(QPainter& painter)
{
    QPen pen(QColor(200, 100, 100), 1, Qt::DotLine);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    
    // Default VSWR circles: 1.5, 2.0, 3.0
    std::vector<double> vswrs = m_vswrCircles;
    if (vswrs.empty()) {
        vswrs = {1.5, 2.0, 3.0};
    }
    
    for (double vswr : vswrs) {
        double gammaMag = SmithMath::vswrToGamma(vswr);
        double screenRadius = gammaMag * m_radius;
        painter.drawEllipse(circleRect(m_center, screenRadius));
    }
}

void SmithChartWidget::drawQCircles(QPainter& painter)
{
    QPen pen(QColor(0, 150, 100), 1, Qt::DashDotLine);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    
    QFont font = painter.font();
    font.setPointSize(7);
    painter.setFont(font);
    
    for (double q : m_qValues) {
        QCircle qCircle(q);
        
        // Upper Q-circle (positive reactance region)
        QPointF screenCenterUpper = gammaToScreen(qCircle.centerUpper);
        double screenRadius = qCircle.radius * m_radius;
        
        QPainterPath upperPath;
        upperPath.addEllipse(circleRect(screenCenterUpper, screenRadius));
        
        QPainterPath unitPath;
        unitPath.addEllipse(circleRect(m_center, m_radius));
        
        painter.drawPath(upperPath.intersected(unitPath));
        
        // Lower Q-circle (negative reactance region)
        QPointF screenCenterLower = gammaToScreen(qCircle.centerLower);
        
        QPainterPath lowerPath;
        lowerPath.addEllipse(circleRect(screenCenterLower, screenRadius));
        
        painter.drawPath(lowerPath.intersected(unitPath));
        
        // Draw label on upper circle
        painter.setPen(QColor(0, 150, 100));
        // Label near the intersection with real axis
        Complex labelGamma = Complex(0.5, 0.5 / q);  // Approximate intersection point
        if (SmithMath::isInsideUnitCircle(labelGamma)) {
            QPointF labelPos = gammaToScreen(labelGamma);
            painter.drawText(labelPos + QPointF(5, -2), QString("Q=%1").arg(q, 0, 'g', 2));
        }
        
        painter.setPen(pen);  // Reset pen for next circle
    }
}

void SmithChartWidget::drawLabels(QPainter& painter)
{
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);
    painter.setPen(Qt::black);
    
    // Resistance labels on real axis
    for (double r : s_resistanceValues) {
        Complex gamma = SmithMath::normalizedZToGamma(Complex(r, 0));
        QPointF pos = gammaToScreen(gamma);
        pos.setY(pos.y() + 12);
        
        QString label = (r == 0) ? "0" : QString::number(r);
        painter.drawText(pos, label);
    }
    
    // Reactance labels
    for (double x : s_reactanceValues) {
        // Positive reactance
        Complex gamma = SmithMath::normalizedZToGamma(Complex(0, x));
        if (SmithMath::isInsideUnitCircle(gamma)) {
            QPointF pos = gammaToScreen(gamma);
            painter.drawText(pos + QPointF(5, -5), QString("+j%1").arg(x));
        }
        
        // Negative reactance
        gamma = SmithMath::normalizedZToGamma(Complex(0, -x));
        if (SmithMath::isInsideUnitCircle(gamma)) {
            QPointF pos = gammaToScreen(gamma);
            painter.drawText(pos + QPointF(5, 12), QString("-j%1").arg(x));
        }
    }
}

void SmithChartWidget::drawSParamTrace(QPainter& painter)
{
    if (m_sparamData.isEmpty()) return;
    
    QPen pen(Qt::blue, 2);
    painter.setPen(pen);
    
    QPainterPath path;
    bool first = true;
    
    for (const auto& point : m_sparamData.points()) {
        Complex gamma = point.s11;
        if (SmithMath::isInsideUnitCircle(gamma)) {
            QPointF screenPoint = gammaToScreen(gamma);
            if (first) {
                path.moveTo(screenPoint);
                first = false;
            } else {
                path.lineTo(screenPoint);
            }
        }
    }
    
    painter.drawPath(path);
    
    // Draw frequency markers
    painter.setBrush(Qt::blue);
    for (int i = 0; i < m_sparamData.numPoints(); i += m_sparamData.numPoints() / 10 + 1) {
        const auto& point = m_sparamData.point(i);
        QPointF screenPoint = gammaToScreen(point.s11);
        painter.drawEllipse(screenPoint, 4, 4);
    }
}

void SmithChartWidget::drawMarker(QPainter& painter)
{
    if (!m_markerVisible) return;
    
    QPointF pos = gammaToScreen(m_markerGamma);
    
    // Draw crosshair
    QPen pen(Qt::red, 2);
    painter.setPen(pen);
    painter.drawLine(pos + QPointF(-8, 0), pos + QPointF(8, 0));
    painter.drawLine(pos + QPointF(0, -8), pos + QPointF(0, 8));
    
    // Draw circle
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(pos, 6, 6);
}

void SmithChartWidget::drawMatchingTrace(QPainter& painter)
{
    if (m_matchingTrace.numSegments() == 0) return;
    
    const auto& segments = m_matchingTrace.segments();
    
    for (size_t i = 0; i < segments.size(); ++i) {
        const TraceSegment& seg = segments[i];
        if (seg.isEmpty()) continue;
        
        // Set color for this segment
        QPen pen(seg.color, 2);
        painter.setPen(pen);
        
        // Draw trace path
        QPainterPath path;
        bool first = true;
        
        for (const auto& pt : seg.points) {
            if (SmithMath::isInsideUnitCircle(pt.gamma)) {
                QPointF screenPoint = gammaToScreen(pt.gamma);
                if (first) {
                    path.moveTo(screenPoint);
                    first = false;
                } else {
                    path.lineTo(screenPoint);
                }
            }
        }
        
        painter.drawPath(path);
        
        // Draw start point marker
        if (!seg.points.empty()) {
            QPointF startPt = gammaToScreen(seg.startPoint().gamma);
            painter.setBrush(seg.color);
            painter.drawEllipse(startPt, 4, 4);
        }
        
        // Draw label
        if (!seg.label.isEmpty() && !seg.points.empty()) {
            QPointF midPoint = gammaToScreen(seg.points[seg.points.size() / 2].gamma);
            painter.setFont(QFont("Arial", 8));
            painter.drawText(midPoint + QPointF(5, -5), seg.label);
        }
    }
}

void SmithChartWidget::drawDragHandles(QPainter& painter)
{
    if (m_matchingTrace.numSegments() == 0) return;
    
    const auto& segments = m_matchingTrace.segments();
    
    for (size_t i = 0; i < segments.size(); ++i) {
        const TraceSegment& seg = segments[i];
        if (seg.isEmpty()) continue;
        
        QPointF endPt = gammaToScreen(seg.endPoint().gamma);
        
        // Determine if this handle is being hovered or dragged
        bool isHovered = (static_cast<int>(i) == m_hoverSegmentIndex);
        bool isDragged = (static_cast<int>(i) == m_dragSegmentIndex && m_isDragging);
        
        // Draw drag handle
        if (isDragged) {
            // Dragging: show preview position
            QPen pen(Qt::red, 2);
            painter.setPen(pen);
            painter.setBrush(QColor(255, 100, 100, 180));
            QPointF previewPt = gammaToScreen(m_previewGamma);
            painter.drawEllipse(previewPt, 8, 8);
            
            // Draw line from original to preview
            painter.setPen(QPen(Qt::red, 1, Qt::DashLine));
            painter.drawLine(endPt, previewPt);
            
            // Original position
            painter.setPen(QPen(seg.color, 1));
            painter.setBrush(Qt::white);
            painter.drawEllipse(endPt, 5, 5);
        } else if (isHovered) {
            // Hovering: highlight the handle
            QPen pen(seg.color, 2);
            painter.setPen(pen);
            painter.setBrush(QColor(255, 255, 200, 200));
            painter.drawEllipse(endPt, 8, 8);
            
            // Draw inner circle
            painter.setBrush(seg.color);
            painter.drawEllipse(endPt, 4, 4);
        } else {
            // Normal: simple endpoint marker
            QPen pen(seg.color, 1);
            painter.setPen(pen);
            painter.setBrush(Qt::white);
            painter.drawEllipse(endPt, 5, 5);
        }
    }
}

void SmithChartWidget::drawImpedanceMarkers(QPainter& painter)
{
    // Draw source impedance marker (Zs) - Green square
    if (m_sourceVisible) {
        Complex gammaS = SmithMath::impedanceToGamma(m_sourceZ, m_z0);
        if (SmithMath::isInsideUnitCircle(gammaS)) {
            QPointF pos = gammaToScreen(gammaS);
            
            QPen pen(QColor(0, 150, 0), 2);
            painter.setPen(pen);
            painter.setBrush(QColor(0, 200, 0, 150));
            
            // Draw square marker
            QRectF rect(pos.x() - 6, pos.y() - 6, 12, 12);
            painter.drawRect(rect);
            
            // Draw label "Zs"
            painter.setFont(QFont("Arial", 9, QFont::Bold));
            painter.drawText(pos + QPointF(10, -5), "Zs");
        }
    }
    
    // Draw load impedance marker (Zl) - Magenta triangle
    if (m_loadVisible) {
        Complex gammaL = SmithMath::impedanceToGamma(m_loadZ, m_z0);
        if (SmithMath::isInsideUnitCircle(gammaL)) {
            QPointF pos = gammaToScreen(gammaL);
            
            QPen pen(QColor(180, 0, 180), 2);
            painter.setPen(pen);
            painter.setBrush(QColor(220, 0, 220, 150));
            
            // Draw triangle marker
            QPolygonF triangle;
            triangle << QPointF(pos.x(), pos.y() - 8)
                     << QPointF(pos.x() - 7, pos.y() + 5)
                     << QPointF(pos.x() + 7, pos.y() + 5);
            painter.drawPolygon(triangle);
            
            // Draw label "Zl"
            painter.setFont(QFont("Arial", 9, QFont::Bold));
            painter.drawText(pos + QPointF(10, -5), "Zl");
        }
    }
}

void SmithChartWidget::setInteractionMode(InteractionMode mode)
{
    m_interactionMode = mode;
    
    if (mode == InteractionMode::AddElement) {
        setCursor(Qt::CrossCursor);
    } else if (mode == InteractionMode::DragEdit) {
        setCursor(Qt::OpenHandCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
    
    update();
}

void SmithChartWidget::setPendingElementType(ComponentType type, ConnectionType conn)
{
    m_pendingType = type;
    m_pendingConnection = conn;
    m_hasPendingElement = true;
    setInteractionMode(InteractionMode::AddElement);
}

void SmithChartWidget::cancelPendingElement()
{
    m_hasPendingElement = false;
    m_pendingType = ComponentType::None;
    setInteractionMode(InteractionMode::Normal);
}

void SmithChartWidget::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    
    // Check if clicking on a trace endpoint
    int hitSegment = hitTestTraceEndpoint(event->pos());
    
    if (hitSegment >= 0 && m_matchingTrace.numSegments() > 0) {
        const auto& seg = m_matchingTrace.segment(hitSegment);
        
        QString componentName;
        switch (seg.componentType) {
            case ComponentType::Inductor:
                componentName = "Inductor";
                break;
            case ComponentType::Capacitor:
                componentName = "Capacitor";
                break;
            case ComponentType::Resistor:
                componentName = "Resistor";
                break;
            default:
                componentName = "Component";
                break;
        }
        
        QString connType = (seg.connectionType == ConnectionType::Series) ? "Series" : "Shunt";
        
        QAction* editAction = menu.addAction(QString("Edit %1 (%2) %3").arg(componentName).arg(connType).arg(seg.label));
        connect(editAction, &QAction::triggered, [this, hitSegment]() {
            emit editElementRequested(hitSegment);
        });
        
        QAction* deleteAction = menu.addAction(QString("Delete %1").arg(componentName));
        deleteAction->setIcon(QIcon::fromTheme("edit-delete"));
        connect(deleteAction, &QAction::triggered, [this, hitSegment]() {
            emit deleteElementRequested(hitSegment);
        });
        
        menu.addSeparator();
    }
    
    // View options
    QAction* resetAction = menu.addAction("Reset View");
    connect(resetAction, &QAction::triggered, [this]() {
        m_zoomLevel = 1.0;
        m_panOffset = QPointF(0, 0);
        updateChartGeometry();
        update();
        emit resetViewRequested();
    });
    
    QAction* zoomInAction = menu.addAction("Zoom In");
    connect(zoomInAction, &QAction::triggered, [this]() {
        double newZoom = qBound(MIN_ZOOM, m_zoomLevel * 1.2, MAX_ZOOM);
        m_zoomLevel = newZoom;
        updateChartGeometry();
        update();
    });
    
    QAction* zoomOutAction = menu.addAction("Zoom Out");
    connect(zoomOutAction, &QAction::triggered, [this]() {
        double newZoom = qBound(MIN_ZOOM, m_zoomLevel / 1.2, MAX_ZOOM);
        m_zoomLevel = newZoom;
        updateChartGeometry();
        update();
    });
    
    menu.addSeparator();
    
    // Grid options
    QAction* showAdmittanceAction = menu.addAction("Show Admittance Grid");
    showAdmittanceAction->setCheckable(true);
    showAdmittanceAction->setChecked(m_showAdmittanceGrid);
    connect(showAdmittanceAction, &QAction::triggered, [this](bool checked) {
        setShowAdmittanceGrid(checked);
    });
    
    QAction* showQAction = menu.addAction("Show Q Circles");
    showQAction->setCheckable(true);
    showQAction->setChecked(m_showQCircles);
    connect(showQAction, &QAction::triggered, [this](bool checked) {
        setShowQCircles(checked);
    });
    
    QAction* showVSWRAction = menu.addAction("Show VSWR Circles");
    showVSWRAction->setCheckable(true);
    showVSWRAction->setChecked(m_showVSWRCircles);
    connect(showVSWRAction, &QAction::triggered, [this](bool checked) {
        setShowVSWRCircles(checked);
    });
    
    menu.exec(event->globalPos());
}

} // namespace SmithTool
