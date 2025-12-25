/**
 * @file sparamdata.h
 * @brief S-parameter data structures
 */

#ifndef SMITHTOOL_SPARAMDATA_H
#define SMITHTOOL_SPARAMDATA_H

#include <complex>
#include <vector>
#include <QString>
#include <QVector>

namespace SmithTool {

using Complex = std::complex<double>;

/**
 * @brief Single frequency S-parameter data point
 */
struct SParamPoint {
    double frequency;       // Frequency in Hz
    Complex s11;            // S11 (input reflection)
    Complex s21;            // S21 (forward transmission)
    Complex s12;            // S12 (reverse transmission)
    Complex s22;            // S22 (output reflection)
    
    SParamPoint() : frequency(0), s11(0,0), s21(0,0), s12(0,0), s22(0,0) {}
    
    SParamPoint(double f, const Complex& s11Val)
        : frequency(f), s11(s11Val), s21(0,0), s12(0,0), s22(0,0) {}
    
    SParamPoint(double f, const Complex& s11Val, const Complex& s21Val,
                const Complex& s12Val, const Complex& s22Val)
        : frequency(f), s11(s11Val), s21(s21Val), s12(s12Val), s22(s22Val) {}
};

/**
 * @brief S-parameter data format types
 */
enum class SParamFormat {
    RI,     // Real/Imaginary
    MA,     // Magnitude/Angle (degrees)
    DB      // dB/Angle (degrees)
};

/**
 * @brief Port count for S-parameter data
 */
enum class PortCount {
    OnePort,    // .s1p
    TwoPort,    // .s2p
    NPort       // .snp
};

/**
 * @brief S-parameter data container
 */
class SParamData {
public:
    SParamData();
    ~SParamData() = default;
    
    // Data access
    int numPoints() const { return m_points.size(); }
    bool isEmpty() const { return m_points.empty(); }
    
    const SParamPoint& point(int index) const;
    SParamPoint& point(int index);
    
    // Frequency range
    double minFrequency() const;
    double maxFrequency() const;
    
    // Get S-parameter at specific frequency (interpolated if needed)
    Complex s11At(double freq) const;
    Complex s21At(double freq) const;
    Complex s12At(double freq) const;
    Complex s22At(double freq) const;
    
    // Find index of closest frequency
    int closestIndex(double freq) const;
    
    // Data manipulation
    void addPoint(const SParamPoint& point);
    void clear();
    void sortByFrequency();
    
    // Properties
    double referenceImpedance() const { return m_z0; }
    void setReferenceImpedance(double z0) { m_z0 = z0; }
    
    PortCount portCount() const { return m_portCount; }
    void setPortCount(PortCount count) { m_portCount = count; }
    
    QString filename() const { return m_filename; }
    void setFilename(const QString& name) { m_filename = name; }
    
    // All points access
    const QVector<SParamPoint>& points() const { return m_points; }
    
    // Get frequency list
    QVector<double> frequencies() const;
    
    // Get S11 as vector
    QVector<Complex> s11Vector() const;
    
private:
    QVector<SParamPoint> m_points;
    double m_z0;
    PortCount m_portCount;
    QString m_filename;
    
    // Linear interpolation helper
    Complex interpolate(double freq, int idx1, int idx2, 
                        const Complex& v1, const Complex& v2) const;
};

} // namespace SmithTool

#endif // SMITHTOOL_SPARAMDATA_H
