/**
 * @file circuitview.cpp
 * @brief Widget for displaying matching network schematic implementation
 */

#include "circuitview.h"
#include <QMouseEvent>
#include <QPainterPath>
#include <cmath>

namespace SmithTool {

CircuitView::CircuitView(QWidget* parent)
    : QWidget(parent)
    , m_sourceZ(50.0, 0.0)
    , m_loadZ(50.0, 0.0)
    , m_elementWidth(60)
    , m_elementHeight(40)
    , m_spacing(20)
    , m_wireY(0)
{
    setMinimumSize(300, 150);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setMouseTracking(true);
}

void CircuitView::setSourceImpedance(const std::complex<double>& zs)
{
    m_sourceZ = zs;
    update();
}

void CircuitView::setLoadImpedance(const std::complex<double>& zl)
{
    m_loadZ = zl;
    update();
}

void CircuitView::addElement(ComponentType type, ConnectionType conn, double value)
{
    CircuitElement elem;
    elem.type = type;
    elem.connection = conn;
    elem.value = value;
    elem.highlighted = false;
    m_elements.push_back(elem);
    updateLayout();
    update();
}

void CircuitView::removeLastElement()
{
    if (!m_elements.empty()) {
        m_elements.pop_back();
        updateLayout();
        update();
    }
}

void CircuitView::clearElements()
{
    m_elements.clear();
    updateLayout();
    update();
}

void CircuitView::updateElementValue(int index, double newValue)
{
    if (index >= 0 && index < static_cast<int>(m_elements.size())) {
        m_elements[index].value = newValue;
        m_elements[index].label = formatValue(m_elements[index].type, newValue);
        update();
    }
}

void CircuitView::updateLayout()
{
    if (m_elements.empty()) return;
    
    // Calculate layout based on widget size
    double availableWidth = width() - 120; // Reserve space for terminals
    double totalElements = static_cast<double>(m_elements.size());
    
    m_elementWidth = std::min(60.0, (availableWidth - (totalElements - 1) * m_spacing) / totalElements);
    m_wireY = height() / 2.0;
    
    double x = 60.0; // Start after source terminal
    
    for (auto& elem : m_elements) {
        if (elem.connection == ConnectionType::Series) {
            elem.bounds = QRectF(x, m_wireY - m_elementHeight / 2,
                                 m_elementWidth, m_elementHeight);
        } else {
            // Shunt element - positioned below wire
            elem.bounds = QRectF(x, m_wireY,
                                 m_elementWidth, m_elementHeight);
        }
        x += m_elementWidth + m_spacing;
    }
}

void CircuitView::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Background
    painter.fillRect(rect(), QColor(250, 250, 250));
    
    // Draw border
    painter.setPen(QPen(Qt::gray, 1));
    painter.drawRect(rect().adjusted(0, 0, -1, -1));
    
    m_wireY = height() / 2.0;
    
    // Draw source terminal
    QPointF sourcePos(30, m_wireY);
    drawTerminal(painter, sourcePos, formatImpedanceLabel(m_sourceZ, "Zs"));
    
    // Calculate load position
    double loadX = m_elements.empty() ? 150 : 
                   m_elements.back().bounds.right() + m_spacing + 30;
    loadX = std::max(loadX, static_cast<double>(width() - 60));
    QPointF loadPos(loadX, m_wireY);
    
    // Draw main wire
    painter.setPen(QPen(Qt::black, 2));
    
    if (m_elements.empty()) {
        // Direct connection
        drawWire(painter, sourcePos, loadPos);
    } else {
        // Wire from source to first element
        QPointF firstElemStart(m_elements.front().bounds.left(), m_wireY);
        drawWire(painter, sourcePos, firstElemStart);
        
        // Draw elements and connections
        for (size_t i = 0; i < m_elements.size(); ++i) {
            const auto& elem = m_elements[i];
            
            if (elem.connection == ConnectionType::Series) {
                // Draw series element
                drawSeriesElement(painter, elem);
                
                // Wire to next element or load
                QPointF elemEnd(elem.bounds.right(), m_wireY);
                QPointF nextStart;
                if (i + 1 < m_elements.size()) {
                    nextStart = QPointF(m_elements[i + 1].bounds.left(), m_wireY);
                } else {
                    nextStart = loadPos;
                }
                drawWire(painter, elemEnd, nextStart);
            } else {
                // Draw shunt element
                drawShuntElement(painter, elem);
                
                // Wire passes through
                QPointF wireStart(elem.bounds.left(), m_wireY);
                QPointF wireEnd(elem.bounds.right(), m_wireY);
                drawWire(painter, wireStart, wireEnd);
                
                // Wire to next element or load
                if (i + 1 < m_elements.size()) {
                    QPointF nextStart(m_elements[i + 1].bounds.left(), m_wireY);
                    drawWire(painter, wireEnd, nextStart);
                } else {
                    drawWire(painter, wireEnd, loadPos);
                }
            }
        }
    }
    
    // Draw load terminal
    drawTerminal(painter, loadPos, formatImpedanceLabel(m_loadZ, "ZL"));
}

void CircuitView::drawSeriesElement(QPainter& painter, const CircuitElement& elem)
{
    QRectF rect = elem.bounds;
    
    // Highlight if selected
    if (elem.highlighted) {
        painter.fillRect(rect.adjusted(-2, -2, 2, 2), QColor(255, 255, 200));
    }
    
    switch (elem.type) {
        case ComponentType::Resistor:
            drawResistor(painter, rect, false);
            break;
        case ComponentType::Capacitor:
            drawCapacitor(painter, rect, false);
            break;
        case ComponentType::Inductor:
            drawInductor(painter, rect, false);
            break;
        default:
            break;
    }
    
    // Draw value label
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 8));
    QString label = formatValue(elem.type, elem.value);
    painter.drawText(rect.adjusted(0, -15, 0, 0), Qt::AlignCenter, label);
}

void CircuitView::drawShuntElement(QPainter& painter, const CircuitElement& elem)
{
    QRectF rect = elem.bounds;
    
    // Highlight if selected
    if (elem.highlighted) {
        painter.fillRect(rect.adjusted(-2, -2, 2, 2), QColor(255, 255, 200));
    }
    
    // Draw vertical wire to element
    QPointF topWire(rect.center().x(), m_wireY);
    QPointF elemTop(rect.center().x(), rect.top());
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(topWire, elemTop);
    
    switch (elem.type) {
        case ComponentType::Resistor:
            drawResistor(painter, rect, true);
            break;
        case ComponentType::Capacitor:
            drawCapacitor(painter, rect, true);
            break;
        case ComponentType::Inductor:
            drawInductor(painter, rect, true);
            break;
        default:
            break;
    }
    
    // Draw ground
    QPointF groundPos(rect.center().x(), rect.bottom());
    drawGround(painter, groundPos);
    
    // Draw value label
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 8));
    QString label = formatValue(elem.type, elem.value);
    painter.drawText(QRectF(rect.left() - 20, rect.center().y(), 
                           rect.width() + 40, 15), Qt::AlignCenter, label);
}

void CircuitView::drawWire(QPainter& painter, const QPointF& from, const QPointF& to)
{
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(from, to);
}

void CircuitView::drawResistor(QPainter& painter, const QRectF& rect, bool vertical)
{
    painter.setPen(QPen(Qt::black, 1.5));
    painter.setBrush(Qt::NoBrush);
    
    if (vertical) {
        // Vertical resistor (zigzag pattern)
        double x = rect.center().x();
        double y = rect.top();
        double h = rect.height();
        double step = h / 6;
        double w = rect.width() / 3;
        
        QPainterPath path;
        path.moveTo(x, y);
        path.lineTo(x - w, y + step);
        path.lineTo(x + w, y + 2 * step);
        path.lineTo(x - w, y + 3 * step);
        path.lineTo(x + w, y + 4 * step);
        path.lineTo(x - w, y + 5 * step);
        path.lineTo(x, y + h);
        painter.drawPath(path);
    } else {
        // Horizontal resistor
        double y = rect.center().y();
        double x = rect.left();
        double w = rect.width();
        double step = w / 6;
        double h = rect.height() / 3;
        
        QPainterPath path;
        path.moveTo(x, y);
        path.lineTo(x + step, y - h);
        path.lineTo(x + 2 * step, y + h);
        path.lineTo(x + 3 * step, y - h);
        path.lineTo(x + 4 * step, y + h);
        path.lineTo(x + 5 * step, y - h);
        path.lineTo(x + w, y);
        painter.drawPath(path);
    }
}

void CircuitView::drawCapacitor(QPainter& painter, const QRectF& rect, bool vertical)
{
    painter.setPen(QPen(Qt::black, 2));
    
    double gap = 6;
    
    if (vertical) {
        double x = rect.center().x();
        double cy = rect.center().y();
        double plateW = rect.width() * 0.6;
        
        // Top wire
        painter.drawLine(QPointF(x, rect.top()), QPointF(x, cy - gap / 2));
        // Top plate
        painter.drawLine(QPointF(x - plateW / 2, cy - gap / 2), 
                        QPointF(x + plateW / 2, cy - gap / 2));
        // Bottom plate
        painter.drawLine(QPointF(x - plateW / 2, cy + gap / 2), 
                        QPointF(x + plateW / 2, cy + gap / 2));
        // Bottom wire
        painter.drawLine(QPointF(x, cy + gap / 2), QPointF(x, rect.bottom()));
    } else {
        double y = rect.center().y();
        double cx = rect.center().x();
        double plateH = rect.height() * 0.6;
        
        // Left wire
        painter.drawLine(QPointF(rect.left(), y), QPointF(cx - gap / 2, y));
        // Left plate
        painter.drawLine(QPointF(cx - gap / 2, y - plateH / 2), 
                        QPointF(cx - gap / 2, y + plateH / 2));
        // Right plate
        painter.drawLine(QPointF(cx + gap / 2, y - plateH / 2), 
                        QPointF(cx + gap / 2, y + plateH / 2));
        // Right wire
        painter.drawLine(QPointF(cx + gap / 2, y), QPointF(rect.right(), y));
    }
}

void CircuitView::drawInductor(QPainter& painter, const QRectF& rect, bool vertical)
{
    painter.setPen(QPen(Qt::black, 1.5));
    painter.setBrush(Qt::NoBrush);
    
    int coils = 4;
    
    if (vertical) {
        double x = rect.center().x();
        double y = rect.top();
        double h = rect.height();
        double coilH = h / coils;
        double coilR = rect.width() / 4;
        
        QPainterPath path;
        path.moveTo(x, y);
        for (int i = 0; i < coils; ++i) {
            path.arcTo(QRectF(x - coilR, y + i * coilH, 2 * coilR, coilH), 
                      90, -180);
        }
        path.lineTo(x, rect.bottom());
        painter.drawPath(path);
    } else {
        double y = rect.center().y();
        double x = rect.left();
        double w = rect.width();
        double coilW = w / coils;
        double coilR = rect.height() / 4;
        
        QPainterPath path;
        path.moveTo(x, y);
        for (int i = 0; i < coils; ++i) {
            path.arcTo(QRectF(x + i * coilW, y - coilR, coilW, 2 * coilR), 
                      180, -180);
        }
        path.lineTo(rect.right(), y);
        painter.drawPath(path);
    }
}

void CircuitView::drawGround(QPainter& painter, const QPointF& pos)
{
    painter.setPen(QPen(Qt::black, 1.5));
    
    double w = 12;
    double step = 4;
    
    // Three horizontal lines
    painter.drawLine(QPointF(pos.x() - w / 2, pos.y()), 
                    QPointF(pos.x() + w / 2, pos.y()));
    painter.drawLine(QPointF(pos.x() - w / 3, pos.y() + step), 
                    QPointF(pos.x() + w / 3, pos.y() + step));
    painter.drawLine(QPointF(pos.x() - w / 6, pos.y() + 2 * step), 
                    QPointF(pos.x() + w / 6, pos.y() + 2 * step));
}

void CircuitView::drawTerminal(QPainter& painter, const QPointF& pos, const QString& label)
{
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(Qt::white);
    
    // Draw circle
    painter.drawEllipse(pos, 5, 5);
    
    // Draw label
    painter.setFont(QFont("Arial", 9, QFont::Bold));
    QRectF labelRect(pos.x() - 30, pos.y() - 25, 60, 15);
    painter.drawText(labelRect, Qt::AlignCenter, label);
}

QString CircuitView::formatValue(ComponentType type, double value) const
{
    double absVal = std::abs(value);
    QString prefix;
    double scaled;
    
    switch (type) {
        case ComponentType::Resistor:
            if (absVal >= 1e6) { scaled = value / 1e6; prefix = "M"; }
            else if (absVal >= 1e3) { scaled = value / 1e3; prefix = "k"; }
            else { scaled = value; prefix = ""; }
            return QString("%1 %2Ω").arg(scaled, 0, 'g', 3).arg(prefix);
            
        case ComponentType::Inductor:
            if (absVal >= 1e-3) { scaled = value * 1e3; prefix = "m"; }
            else if (absVal >= 1e-6) { scaled = value * 1e6; prefix = "µ"; }
            else if (absVal >= 1e-9) { scaled = value * 1e9; prefix = "n"; }
            else { scaled = value * 1e12; prefix = "p"; }
            return QString("%1 %2H").arg(scaled, 0, 'g', 3).arg(prefix);
            
        case ComponentType::Capacitor:
            if (absVal >= 1e-6) { scaled = value * 1e6; prefix = "µ"; }
            else if (absVal >= 1e-9) { scaled = value * 1e9; prefix = "n"; }
            else if (absVal >= 1e-12) { scaled = value * 1e12; prefix = "p"; }
            else { scaled = value * 1e15; prefix = "f"; }
            return QString("%1 %2F").arg(scaled, 0, 'g', 3).arg(prefix);
            
        default:
            return QString::number(value);
    }
}

QString CircuitView::formatImpedanceLabel(const std::complex<double>& z, const QString& name) const
{
    QString sign = z.imag() >= 0 ? "+" : "";
    return QString("%1=%2%3j%4")
        .arg(name)
        .arg(z.real(), 0, 'f', 1)
        .arg(sign)
        .arg(z.imag(), 0, 'f', 1);
}

void CircuitView::mousePressEvent(QMouseEvent* event)
{
    int idx = elementAtPoint(event->pos());
    if (idx >= 0) {
        emit elementClicked(idx);
    }
}

void CircuitView::mouseDoubleClickEvent(QMouseEvent* event)
{
    int idx = elementAtPoint(event->pos());
    if (idx >= 0) {
        emit elementDoubleClicked(idx);
    }
}

void CircuitView::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    updateLayout();
}

int CircuitView::elementAtPoint(const QPointF& pos) const
{
    for (size_t i = 0; i < m_elements.size(); ++i) {
        if (m_elements[i].bounds.contains(pos)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

} // namespace SmithTool
