/**
 * @file trace.cpp
 * @brief Trace data structures implementation
 */

#include "trace.h"
#include "smithmath.h"
#include <cmath>

namespace SmithTool {

// Color palette for trace segments
const std::vector<QColor> MatchingTrace::s_colors = {
    QColor(0, 100, 200),    // Blue
    QColor(200, 50, 50),    // Red
    QColor(50, 150, 50),    // Green
    QColor(180, 100, 0),    // Orange
    QColor(128, 0, 128),    // Purple
    QColor(0, 150, 150),    // Cyan
    QColor(200, 150, 0),    // Yellow
    QColor(100, 100, 100)   // Gray
};

MatchingTrace::MatchingTrace()
    : m_sourceZ(50.0, 0.0)
    , m_loadZ(50.0, 0.0)
    , m_z0(50.0)
    , m_frequency(1e9)
{
}

void MatchingTrace::setSourceImpedance(const Complex& zs)
{
    m_sourceZ = zs;
}

void MatchingTrace::setLoadImpedance(const Complex& zl)
{
    m_loadZ = zl;
}

void MatchingTrace::setZ0(double z0)
{
    m_z0 = z0;
}

void MatchingTrace::setFrequency(double freq)
{
    m_frequency = freq;
}

void MatchingTrace::addSegment(const TraceSegment& segment)
{
    m_segments.push_back(segment);
}

void MatchingTrace::removeLastSegment()
{
    if (!m_segments.empty()) {
        m_segments.pop_back();
    }
}

void MatchingTrace::clear()
{
    m_segments.clear();
}

const TraceSegment& MatchingTrace::segment(int index) const
{
    static TraceSegment empty;
    if (index < 0 || index >= static_cast<int>(m_segments.size())) {
        return empty;
    }
    return m_segments[index];
}

TraceSegment& MatchingTrace::segmentRef(int index)
{
    static TraceSegment empty;
    if (index < 0 || index >= static_cast<int>(m_segments.size())) {
        return empty;
    }
    return m_segments[index];
}

void MatchingTrace::updateSegmentValue(int index, double newValue)
{
    if (index < 0 || index >= static_cast<int>(m_segments.size())) {
        return;
    }
    
    TraceSegment& seg = m_segments[index];
    seg.componentValue = newValue;
    
    // Get the starting impedance for this segment
    Complex startZ;
    if (index == 0) {
        startZ = m_loadZ;
    } else {
        startZ = m_segments[index - 1].endPoint().impedance;
    }
    
    // Recalculate the points for this segment
    if (seg.connectionType == ConnectionType::Series) {
        switch (seg.componentType) {
            case ComponentType::Inductor:
                {
                    double deltaX = 2.0 * SmithMath::PI * m_frequency * newValue;
                    seg.label = QString("L = %1").arg(newValue * 1e9, 0, 'f', 2) + " nH";
                    seg.points = generateConstantRArc(startZ, deltaX);
                }
                break;
            case ComponentType::Capacitor:
                {
                    double deltaX = 0.0;
                    if (newValue > 1e-18) {
                        deltaX = -1.0 / (2.0 * SmithMath::PI * m_frequency * newValue);
                    }
                    seg.label = QString("C = %1").arg(newValue * 1e12, 0, 'f', 2) + " pF";
                    seg.points = generateConstantRArc(startZ, deltaX);
                }
                break;
            case ComponentType::Resistor:
                {
                    double deltaR = newValue;
                    seg.label = QString("R = %1").arg(newValue, 0, 'f', 1) + " \u03a9";
                    seg.points = generateConstantXArc(startZ, deltaR);
                }
                break;
            default:
                break;
        }
    } else {
        Complex startY = Complex(1.0, 0.0) / startZ;
        switch (seg.componentType) {
            case ComponentType::Capacitor:
                {
                    double deltaB = 2.0 * SmithMath::PI * m_frequency * newValue;
                    seg.label = QString("C = %1").arg(newValue * 1e12, 0, 'f', 2) + " pF (shunt)";
                    seg.points = generateConstantGArc(startY, deltaB);
                }
                break;
            case ComponentType::Inductor:
                {
                    double deltaB = 0.0;
                    if (newValue > 1e-18) {
                        deltaB = -1.0 / (2.0 * SmithMath::PI * m_frequency * newValue);
                    }
                    seg.label = QString("L = %1").arg(newValue * 1e9, 0, 'f', 2) + " nH (shunt)";
                    seg.points = generateConstantGArc(startY, deltaB);
                }
                break;
            case ComponentType::Resistor:
                {
                    double deltaG = 1.0 / newValue;
                    seg.label = QString("R = %1").arg(newValue, 0, 'f', 1) + " \u03a9 (shunt)";
                    seg.points = generateConstantBArc(startY, deltaG);
                }
                break;
            default:
                break;
        }
    }
    
    // Recalculate all subsequent segments since their starting point changed
    for (int i = index + 1; i < static_cast<int>(m_segments.size()); ++i) {
        TraceSegment& nextSeg = m_segments[i];
        Complex nextStartZ = m_segments[i - 1].endPoint().impedance;
        
        if (nextSeg.connectionType == ConnectionType::Series) {
            switch (nextSeg.componentType) {
                case ComponentType::Inductor:
                    {
                        double deltaX = 2.0 * SmithMath::PI * m_frequency * nextSeg.componentValue;
                        nextSeg.points = generateConstantRArc(nextStartZ, deltaX);
                    }
                    break;
                case ComponentType::Capacitor:
                    {
                        double deltaX = 0.0;
                        if (nextSeg.componentValue > 1e-18) {
                            deltaX = -1.0 / (2.0 * SmithMath::PI * m_frequency * nextSeg.componentValue);
                        }
                        nextSeg.points = generateConstantRArc(nextStartZ, deltaX);
                    }
                    break;
                case ComponentType::Resistor:
                    {
                        double deltaR = nextSeg.componentValue;
                        nextSeg.points = generateConstantXArc(nextStartZ, deltaR);
                    }
                    break;
                default:
                    break;
            }
        } else {
            Complex nextStartY = Complex(1.0, 0.0) / nextStartZ;
            switch (nextSeg.componentType) {
                case ComponentType::Capacitor:
                    {
                        double deltaB = 2.0 * SmithMath::PI * m_frequency * nextSeg.componentValue;
                        nextSeg.points = generateConstantGArc(nextStartY, deltaB);
                    }
                    break;
                case ComponentType::Inductor:
                    {
                        double deltaB = 0.0;
                        if (nextSeg.componentValue > 1e-18) {
                            deltaB = -1.0 / (2.0 * SmithMath::PI * m_frequency * nextSeg.componentValue);
                        }
                        nextSeg.points = generateConstantGArc(nextStartY, deltaB);
                    }
                    break;
                case ComponentType::Resistor:
                    {
                        double deltaG = 1.0 / nextSeg.componentValue;
                        nextSeg.points = generateConstantBArc(nextStartY, deltaG);
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

Complex MatchingTrace::currentImpedance() const
{
    if (m_segments.empty()) {
        return m_loadZ;
    }
    return m_segments.back().endPoint().impedance;
}

Complex MatchingTrace::currentGamma() const
{
    return SmithMath::impedanceToGamma(currentImpedance(), m_z0);
}

QColor MatchingTrace::nextColor() const
{
    int idx = static_cast<int>(m_segments.size()) % s_colors.size();
    return s_colors[idx];
}

TraceSegment MatchingTrace::calculateSeriesElement(ComponentType type, double value) const
{
    TraceSegment seg;
    seg.componentType = type;
    seg.connectionType = ConnectionType::Series;
    seg.componentValue = value;
    seg.color = nextColor();
    
    Complex startZ = currentImpedance();
    
    switch (type) {
        case ComponentType::Inductor:
            {
                // X = 2πfL, series inductor moves along constant R circle
                seg.type = TraceType::ConstantR;
                double deltaX = 2.0 * SmithMath::PI * m_frequency * value;
                seg.label = QString("L = %1").arg(value * 1e9, 0, 'f', 2) + " nH";
                seg.points = generateConstantRArc(startZ, deltaX);
            }
            break;
        case ComponentType::Capacitor:
            {
                // X = -1/(2πfC), series capacitor moves along constant R circle
                seg.type = TraceType::ConstantR;
                double deltaX = 0.0;
                if (value > 1e-18) {
                    deltaX = -1.0 / (2.0 * SmithMath::PI * m_frequency * value);
                }
                seg.label = QString("C = %1").arg(value * 1e12, 0, 'f', 2) + " pF";
                seg.points = generateConstantRArc(startZ, deltaX);
            }
            break;
        case ComponentType::Resistor:
            {
                // Series resistor moves along constant X arc (changes R only)
                seg.type = TraceType::ConstantX;
                double deltaR = value;  // Add resistance
                seg.label = QString("R = %1").arg(value, 0, 'f', 1) + " \u03a9";
                seg.points = generateConstantXArc(startZ, deltaR);
            }
            break;
        default:
            break;
    }
    
    return seg;
}

TraceSegment MatchingTrace::calculateShuntElement(ComponentType type, double value) const
{
    TraceSegment seg;
    seg.componentType = type;
    seg.connectionType = ConnectionType::Shunt;
    seg.componentValue = value;
    seg.color = nextColor();
    
    Complex startZ = currentImpedance();
    Complex startY = Complex(1.0, 0.0) / startZ;
    
    switch (type) {
        case ComponentType::Capacitor:
            {
                // B = 2πfC, shunt capacitor moves along constant G circle
                seg.type = TraceType::ConstantG;
                double deltaB = 2.0 * SmithMath::PI * m_frequency * value;
                seg.label = QString("C = %1").arg(value * 1e12, 0, 'f', 2) + " pF (shunt)";
                seg.points = generateConstantGArc(startY, deltaB);
            }
            break;
        case ComponentType::Inductor:
            {
                // B = -1/(2πfL), shunt inductor moves along constant G circle
                seg.type = TraceType::ConstantG;
                double deltaB = 0.0;
                if (value > 1e-18) {
                    deltaB = -1.0 / (2.0 * SmithMath::PI * m_frequency * value);
                }
                seg.label = QString("L = %1").arg(value * 1e9, 0, 'f', 2) + " nH (shunt)";
                seg.points = generateConstantGArc(startY, deltaB);
            }
            break;
        case ComponentType::Resistor:
            {
                // Shunt resistor moves along constant B arc (changes G only)
                seg.type = TraceType::ConstantB;
                double deltaG = 1.0 / value;  // G = 1/R
                seg.label = QString("R = %1").arg(value, 0, 'f', 1) + " \u03a9 (shunt)";
                seg.points = generateConstantBArc(startY, deltaG);
            }
            break;
        default:
            break;
    }
    
    return seg;
}

std::vector<TracePoint> MatchingTrace::generateConstantRArc(
    const Complex& startZ, double deltaX, int numPoints) const
{
    std::vector<TracePoint> points;
    points.reserve(numPoints);
    
    double r = startZ.real();
    double startX = startZ.imag();
    
    for (int i = 0; i < numPoints; ++i) {
        double t = static_cast<double>(i) / (numPoints - 1);
        double x = startX + t * deltaX;
        Complex z(r, x);
        Complex gamma = SmithMath::impedanceToGamma(z, m_z0);
        points.emplace_back(gamma, z, m_frequency);
    }
    
    return points;
}

std::vector<TracePoint> MatchingTrace::generateConstantGArc(
    const Complex& startY, double deltaB, int numPoints) const
{
    std::vector<TracePoint> points;
    points.reserve(numPoints);
    
    double g = startY.real();
    double startB = startY.imag();
    
    for (int i = 0; i < numPoints; ++i) {
        double t = static_cast<double>(i) / (numPoints - 1);
        double b = startB + t * deltaB;
        Complex y(g, b);
        Complex z = Complex(1.0, 0.0) / y;
        Complex gamma = SmithMath::impedanceToGamma(z, m_z0);
        points.emplace_back(gamma, z, m_frequency);
    }
    
    return points;
}

std::vector<TracePoint> MatchingTrace::generateConstantXArc(
    const Complex& startZ, double deltaR, int numPoints) const
{
    std::vector<TracePoint> points;
    points.reserve(numPoints);
    
    double x = startZ.imag();  // Keep reactance constant
    double startR = startZ.real();
    
    for (int i = 0; i < numPoints; ++i) {
        double t = static_cast<double>(i) / (numPoints - 1);
        double r = startR + t * deltaR;
        if (r < 0.001) r = 0.001;  // Avoid zero or negative resistance
        Complex z(r, x);
        Complex gamma = SmithMath::impedanceToGamma(z, m_z0);
        points.emplace_back(gamma, z, m_frequency);
    }
    
    return points;
}

std::vector<TracePoint> MatchingTrace::generateConstantBArc(
    const Complex& startY, double deltaG, int numPoints) const
{
    std::vector<TracePoint> points;
    points.reserve(numPoints);
    
    double b = startY.imag();  // Keep susceptance constant
    double startG = startY.real();
    
    for (int i = 0; i < numPoints; ++i) {
        double t = static_cast<double>(i) / (numPoints - 1);
        double g = startG + t * deltaG;
        if (g < 0.001) g = 0.001;  // Avoid zero or negative conductance
        Complex y(g, b);
        Complex z = Complex(1.0, 0.0) / y;
        Complex gamma = SmithMath::impedanceToGamma(z, m_z0);
        points.emplace_back(gamma, z, m_frequency);
    }
    
    return points;
}

} // namespace SmithTool
