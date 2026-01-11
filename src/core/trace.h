/**
 * @file trace.h
 * @brief Trace data structures for impedance trajectories
 */

#ifndef SMITHTOOL_TRACE_H
#define SMITHTOOL_TRACE_H

#include <complex>
#include <vector>
#include <QString>
#include <QColor>
#include "../core/component.h"

namespace SmithTool {

using Complex = std::complex<double>;

/**
 * @brief A single point on a trace
 */
struct TracePoint {
    Complex gamma;          // Reflection coefficient
    Complex impedance;      // Impedance at this point
    double frequency;       // Frequency (Hz)
    
    TracePoint() : gamma(0,0), impedance(50,0), frequency(1e9) {}
    TracePoint(const Complex& g, const Complex& z, double f = 1e9)
        : gamma(g), impedance(z), frequency(f) {}
};

/**
 * @brief Type of trace segment
 */
enum class TraceType {
    ConstantR,      // Along constant resistance circle
    ConstantX,      // Along constant reactance arc
    ConstantG,      // Along constant conductance circle (Y chart)
    ConstantB,      // Along constant susceptance arc (Y chart)
    SParam,         // S-parameter trace from file
    Custom          // Arbitrary points
};

/**
 * @brief A single trace segment representing one component effect
 */
struct TraceSegment {
    std::vector<TracePoint> points;
    TraceType type;
    QColor color;
    QString label;
    
    // Associated component (if any)
    ComponentType componentType;
    ConnectionType connectionType;
    double componentValue;
    
    TraceSegment()
        : type(TraceType::Custom)
        , color(Qt::blue)
        , componentType(ComponentType::None)
        , connectionType(ConnectionType::Series)
        , componentValue(0.0)
    {}
    
    bool isEmpty() const { return points.empty(); }
    
    TracePoint startPoint() const {
        return points.empty() ? TracePoint() : points.front();
    }
    
    TracePoint endPoint() const {
        return points.empty() ? TracePoint() : points.back();
    }
};

/**
 * @brief Complete matching network trace from source to load
 */
class MatchingTrace {
public:
    MatchingTrace();
    ~MatchingTrace() = default;
    
    // Source and load
    void setSourceImpedance(const Complex& zs);
    void setLoadImpedance(const Complex& zl);
    Complex sourceImpedance() const { return m_sourceZ; }
    Complex loadImpedance() const { return m_loadZ; }
    
    // Reference
    void setZ0(double z0);
    void setFrequency(double freq);
    double z0() const { return m_z0; }
    double frequency() const { return m_frequency; }
    
    // Segment management
    void addSegment(const TraceSegment& segment);
    void removeLastSegment();
    void clear();
    void updateSegmentValue(int index, double newValue);
    
    int numSegments() const { return static_cast<int>(m_segments.size()); }
    const TraceSegment& segment(int index) const;
    TraceSegment& segmentRef(int index);
    
    // Current impedance (after all segments)
    Complex currentImpedance() const;
    Complex currentGamma() const;
    
    // Calculate trace for adding a series element
    TraceSegment calculateSeriesElement(ComponentType type, double value) const;
    
    // Calculate trace for adding a shunt element
    TraceSegment calculateShuntElement(ComponentType type, double value) const;
    
    // Get all segments
    const std::vector<TraceSegment>& segments() const { return m_segments; }
    
private:
    Complex m_sourceZ;
    Complex m_loadZ;
    double m_z0;
    double m_frequency;
    std::vector<TraceSegment> m_segments;
    
    // Color palette for segments
    static const std::vector<QColor> s_colors;
    QColor nextColor() const;
    
    // Generate arc points
    std::vector<TracePoint> generateConstantRArc(
        const Complex& startZ, double deltaX, int numPoints = 50) const;
    std::vector<TracePoint> generateConstantXArc(
        const Complex& startZ, double deltaR, int numPoints = 50) const;
    std::vector<TracePoint> generateConstantGArc(
        const Complex& startY, double deltaB, int numPoints = 50) const;
    std::vector<TracePoint> generateConstantBArc(
        const Complex& startY, double deltaG, int numPoints = 50) const;
};

/**
 * @brief Q-circle parameters
 * 
 * Constant-Q circles on Smith Chart: Q = |X|/R
 * In Γ-plane, there are two circles (upper and lower):
 * - Center: (0, ±1/Q)  
 * - Radius: sqrt(1 + 1/Q²)
 */
struct QCircle {
    double q;           // Quality factor
    Complex centerUpper;  // Center of upper circle in Gamma plane
    Complex centerLower;  // Center of lower circle in Gamma plane
    double radius;      // Radius in Gamma plane (same for both)
    QColor color;
    bool visible;
    
    QCircle(double qVal = 1.0)
        : q(qVal)
        , color(Qt::darkGreen)
        , visible(true)
    {
        // Correct Q-circle formula for Smith Chart:
        // Center: (0, ±1/Q), Radius: sqrt(1 + 1/Q²)
        double invQ = 1.0 / q;
        centerUpper = Complex(0.0, invQ);
        centerLower = Complex(0.0, -invQ);
        radius = std::sqrt(1.0 + invQ * invQ);
    }
    
    // Backwards compatibility
    Complex center;  // Legacy: set to upper for single-circle drawing
};

} // namespace SmithTool

#endif // SMITHTOOL_TRACE_H
