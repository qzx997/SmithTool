/**
 * @file matching.h
 * @brief Impedance matching network design algorithms
 */

#ifndef SMITHTOOL_MATCHING_H
#define SMITHTOOL_MATCHING_H

#include <complex>
#include <vector>
#include <QString>
#include "component.h"

namespace SmithTool {

using Complex = std::complex<double>;

/**
 * @brief Matching network topology types
 */
enum class MatchingTopology {
    LSection,           // L-network (2 elements)
    LSection_Reversed,  // L-network (reversed)
    PiNetwork,          // Pi-network (3 elements)
    TNetwork,           // T-network (3 elements)
    SingleStubOpen,     // Single open stub
    SingleStubShort,    // Single short stub
    QuarterWave         // Quarter-wave transformer
};

/**
 * @brief Element in a matching network
 */
struct MatchingElement {
    ComponentType type;
    ConnectionType connection;
    double value;           // In base units (ohms/henries/farads)
    QString label;
    
    MatchingElement() 
        : type(ComponentType::None)
        , connection(ConnectionType::Series)
        , value(0.0) {}
    
    MatchingElement(ComponentType t, ConnectionType c, double v)
        : type(t), connection(c), value(v) {}
    
    QString valueString() const;
};

/**
 * @brief A single matching solution
 */
struct MatchingSolution {
    MatchingTopology topology;
    std::vector<MatchingElement> elements;
    double frequency;
    Complex sourceZ;
    Complex loadZ;
    bool valid;
    QString description;
    
    MatchingSolution() : frequency(1e9), valid(false) {}
    
    // Calculate total Q of the network
    double networkQ() const;
    
    // Get netlist representation
    QString toNetlist() const;
    
    // Get human-readable description
    QString toDescription() const;
};

/**
 * @brief Matching network calculator
 */
class MatchingCalculator {
public:
    MatchingCalculator();
    ~MatchingCalculator() = default;
    
    // Set design parameters
    void setSourceImpedance(const Complex& zs);
    void setLoadImpedance(const Complex& zl);
    void setFrequency(double freq_hz);
    void setZ0(double z0);
    
    // Get parameters
    Complex sourceImpedance() const { return m_sourceZ; }
    Complex loadImpedance() const { return m_loadZ; }
    double frequency() const { return m_frequency; }
    double z0() const { return m_z0; }
    
    // Calculate matching networks
    std::vector<MatchingSolution> calculateLSection() const;
    std::vector<MatchingSolution> calculatePiNetwork(double targetQ = 2.0) const;
    std::vector<MatchingSolution> calculateTNetwork(double targetQ = 2.0) const;
    std::vector<MatchingSolution> calculateSingleStub() const;
    std::vector<MatchingSolution> calculateQuarterWave() const;
    
    // Calculate all possible solutions
    std::vector<MatchingSolution> calculateAll() const;
    
    // Utility functions
    static double reactanceToInductance(double x, double freq);
    static double reactanceToCapacitance(double x, double freq);
    static double susceptanceToCapacitance(double b, double freq);
    static double susceptanceToInductance(double b, double freq);
    
private:
    Complex m_sourceZ;
    Complex m_loadZ;
    double m_frequency;
    double m_z0;
    
    // Internal calculation helpers
    MatchingSolution createLSectionSolution(
        double x_series, double b_shunt, bool shuntFirst) const;
};

} // namespace SmithTool

#endif // SMITHTOOL_MATCHING_H
