/**
 * @file spiceexporter.h
 * @brief SPICE netlist exporter for matching networks
 */

#ifndef SMITHTOOL_SPICEEXPORTER_H
#define SMITHTOOL_SPICEEXPORTER_H

#include <QString>
#include <QTextStream>
#include <complex>
#include <vector>
#include "../core/trace.h"

namespace SmithTool {

/**
 * @brief SPICE netlist format options
 */
enum class SpiceFormat {
    Standard,       // Standard SPICE format
    LTspice,        // LTspice compatible
    Ngspice,        // Ngspice compatible
    QucsS           // Qucs-S compatible
};

/**
 * @brief Analysis type for SPICE simulation
 */
enum class AnalysisType {
    AC,             // AC analysis
    SP,             // S-parameter analysis
    Transient       // Transient analysis
};

/**
 * @brief SPICE netlist exporter class
 */
class SpiceExporter {
public:
    SpiceExporter();
    
    // Settings
    void setFormat(SpiceFormat format) { m_format = format; }
    SpiceFormat format() const { return m_format; }
    
    void setFrequency(double freq) { m_frequency = freq; }
    double frequency() const { return m_frequency; }
    
    void setZ0(double z0) { m_z0 = z0; }
    double z0() const { return m_z0; }
    
    void setSourceImpedance(const std::complex<double>& zs) { m_sourceZ = zs; }
    void setLoadImpedance(const std::complex<double>& zl) { m_loadZ = zl; }
    
    void setAnalysisType(AnalysisType type) { m_analysisType = type; }
    AnalysisType analysisType() const { return m_analysisType; }
    
    void setFrequencyRange(double startFreq, double stopFreq, int numPoints);
    
    void setTitle(const QString& title) { m_title = title; }
    QString title() const { return m_title; }
    
    /**
     * @brief Generate SPICE netlist from matching trace
     * @param trace The matching trace containing circuit elements
     * @return SPICE netlist as string
     */
    QString generateNetlist(const MatchingTrace& trace) const;
    
    /**
     * @brief Export netlist to file
     * @param filename Output file path
     * @param trace The matching trace
     * @return true if successful
     */
    bool exportToFile(const QString& filename, const MatchingTrace& trace) const;
    
    /**
     * @brief Generate standalone subcircuit
     */
    QString generateSubcircuit(const MatchingTrace& trace, const QString& name) const;
    
private:
    SpiceFormat m_format;
    AnalysisType m_analysisType;
    double m_frequency;
    double m_z0;
    std::complex<double> m_sourceZ;
    std::complex<double> m_loadZ;
    QString m_title;
    
    // Frequency sweep settings
    double m_startFreq;
    double m_stopFreq;
    int m_numPoints;
    
    // Helper methods
    QString formatHeader() const;
    QString formatSourceImpedance(int& nodeNum) const;
    QString formatLoadImpedance(int nodeNum) const;
    QString formatElement(const TraceSegment& seg, int index, int& nodeNum) const;
    QString formatAnalysis() const;
    QString formatEnding() const;
    
    QString formatResistor(const QString& name, int node1, int node2, double value) const;
    QString formatCapacitor(const QString& name, int node1, int node2, double value) const;
    QString formatInductor(const QString& name, int node1, int node2, double value) const;
    
    QString formatValue(double value, const QString& unit) const;
    QString engineeringNotation(double value) const;
};

} // namespace SmithTool

#endif // SMITHTOOL_SPICEEXPORTER_H
