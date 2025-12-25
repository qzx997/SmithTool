/**
 * @file circuitview.h
 * @brief Widget for displaying matching network schematic
 */

#ifndef SMITHTOOL_CIRCUITVIEW_H
#define SMITHTOOL_CIRCUITVIEW_H

#include <QWidget>
#include <QPainter>
#include <vector>
#include "../core/component.h"
#include "../core/trace.h"

namespace SmithTool {

/**
 * @brief A circuit element for display
 */
struct CircuitElement {
    ComponentType type;
    ConnectionType connection;
    double value;
    QString label;
    QRectF bounds;
    bool highlighted;
    
    CircuitElement()
        : type(ComponentType::None)
        , connection(ConnectionType::Series)
        , value(0.0)
        , highlighted(false)
    {}
};

/**
 * @brief Widget displaying the matching network schematic
 */
class CircuitView : public QWidget {
    Q_OBJECT

public:
    explicit CircuitView(QWidget* parent = nullptr);
    ~CircuitView() override = default;

    // Set source and load impedances for display
    void setSourceImpedance(const std::complex<double>& zs);
    void setLoadImpedance(const std::complex<double>& zl);
    
    // Add/remove elements
    void addElement(ComponentType type, ConnectionType conn, double value);
    void removeLastElement();
    void clearElements();
    void updateElementValue(int index, double newValue);
    
    // Get element list
    const std::vector<CircuitElement>& elements() const { return m_elements; }
    int numElements() const { return static_cast<int>(m_elements.size()); }

signals:
    void elementClicked(int index);
    void elementDoubleClicked(int index);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void updateLayout();
    void drawWire(QPainter& painter, const QPointF& from, const QPointF& to);
    void drawResistor(QPainter& painter, const QRectF& rect, bool vertical);
    void drawCapacitor(QPainter& painter, const QRectF& rect, bool vertical);
    void drawInductor(QPainter& painter, const QRectF& rect, bool vertical);
    void drawGround(QPainter& painter, const QPointF& pos);
    void drawTerminal(QPainter& painter, const QPointF& pos, const QString& label);
    void drawSeriesElement(QPainter& painter, const CircuitElement& elem);
    void drawShuntElement(QPainter& painter, const CircuitElement& elem);
    
    QString formatValue(ComponentType type, double value) const;
    QString formatImpedanceLabel(const std::complex<double>& z, const QString& name) const;
    int elementAtPoint(const QPointF& pos) const;
    
    std::vector<CircuitElement> m_elements;
    std::complex<double> m_sourceZ;
    std::complex<double> m_loadZ;
    
    // Layout parameters
    double m_elementWidth;
    double m_elementHeight;
    double m_spacing;
    double m_wireY;
};

} // namespace SmithTool

#endif // SMITHTOOL_CIRCUITVIEW_H
