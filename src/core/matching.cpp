/**
 * @file matching.cpp
 * @brief Impedance matching network design algorithms implementation
 */

#include "matching.h"
#include "smithmath.h"
#include <cmath>

namespace SmithTool {

// Helper function to format values with SI prefix
static QString formatValue(double value, const QString& unit)
{
    double absVal = std::abs(value);
    QString prefix;
    double scaled;
    
    if (absVal >= 1.0) {
        scaled = value;
        prefix = "";
    } else if (absVal >= 1e-3) {
        scaled = value * 1e3;
        prefix = "m";
    } else if (absVal >= 1e-6) {
        scaled = value * 1e6;
        prefix = "μ";
    } else if (absVal >= 1e-9) {
        scaled = value * 1e9;
        prefix = "n";
    } else if (absVal >= 1e-12) {
        scaled = value * 1e12;
        prefix = "p";
    } else {
        scaled = value * 1e15;
        prefix = "f";
    }
    
    return QString("%1 %2%3").arg(scaled, 0, 'f', 2).arg(prefix).arg(unit);
}

// MatchingElement methods
QString MatchingElement::valueString() const
{
    double absVal = std::abs(value);
    QString prefix;
    double scaled;
    
    switch (type) {
        case ComponentType::Resistor:
            if (absVal >= 1e6) { scaled = value / 1e6; prefix = "M"; }
            else if (absVal >= 1e3) { scaled = value / 1e3; prefix = "k"; }
            else { scaled = value; prefix = ""; }
            return QString("%1 %2Ω").arg(scaled, 0, 'f', 2).arg(prefix);
            
        case ComponentType::Inductor:
            if (absVal >= 1e-3) { scaled = value * 1e3; prefix = "m"; }
            else if (absVal >= 1e-6) { scaled = value * 1e6; prefix = "µ"; }
            else if (absVal >= 1e-9) { scaled = value * 1e9; prefix = "n"; }
            else { scaled = value * 1e12; prefix = "p"; }
            return QString("%1 %2H").arg(scaled, 0, 'f', 2).arg(prefix);
            
        case ComponentType::Capacitor:
            if (absVal >= 1e-6) { scaled = value * 1e6; prefix = "µ"; }
            else if (absVal >= 1e-9) { scaled = value * 1e9; prefix = "n"; }
            else if (absVal >= 1e-12) { scaled = value * 1e12; prefix = "p"; }
            else { scaled = value * 1e15; prefix = "f"; }
            return QString("%1 %2F").arg(scaled, 0, 'f', 2).arg(prefix);
            
        default:
            return QString::number(value);
    }
}

// MatchingSolution methods
double MatchingSolution::networkQ() const
{
    // Calculate the Q based on impedance transformation ratio
    double rs = sourceZ.real();
    double rl = loadZ.real();
    if (rs <= 0 || rl <= 0) return 0;
    
    double ratio = std::max(rs, rl) / std::min(rs, rl);
    return std::sqrt(ratio - 1.0);
}

QString MatchingSolution::toNetlist() const
{
    QString netlist;
    int nodeNum = 1;
    
    for (const auto& elem : elements) {
        QString compType;
        switch (elem.type) {
            case ComponentType::Resistor: compType = "R"; break;
            case ComponentType::Inductor: compType = "L"; break;
            case ComponentType::Capacitor: compType = "C"; break;
            default: continue;
        }
        
        if (elem.connection == ConnectionType::Series) {
            netlist += QString("%1%2 %3 %4 %5\n")
                .arg(compType)
                .arg(nodeNum)
                .arg(nodeNum)
                .arg(nodeNum + 1)
                .arg(elem.value);
            nodeNum++;
        } else {
            netlist += QString("%1%2 %3 0 %4\n")
                .arg(compType)
                .arg(nodeNum)
                .arg(nodeNum)
                .arg(elem.value);
        }
    }
    return netlist;
}

QString MatchingSolution::toDescription() const
{
    if (!valid) return "Invalid solution";
    
    QString desc;
    switch (topology) {
        case MatchingTopology::LSection:
            desc = "L-Section: ";
            break;
        case MatchingTopology::LSection_Reversed:
            desc = "L-Section (Reversed): ";
            break;
        case MatchingTopology::PiNetwork:
            desc = "Pi-Network: ";
            break;
        case MatchingTopology::TNetwork:
            desc = "T-Network: ";
            break;
        default:
            desc = "Matching: ";
    }
    
    for (size_t i = 0; i < elements.size(); ++i) {
        if (i > 0) desc += " → ";
        QString connStr = (elements[i].connection == ConnectionType::Series) ? "Series" : "Shunt";
        desc += QString("%1 %2").arg(connStr).arg(elements[i].valueString());
    }
    
    return desc;
}

// MatchingCalculator methods
MatchingCalculator::MatchingCalculator()
    : m_sourceZ(50.0, 0.0)
    , m_loadZ(50.0, 0.0)
    , m_frequency(1e9)
    , m_z0(50.0)
{
}

void MatchingCalculator::setSourceImpedance(const Complex& zs)
{
    m_sourceZ = zs;
}

void MatchingCalculator::setLoadImpedance(const Complex& zl)
{
    m_loadZ = zl;
}

void MatchingCalculator::setFrequency(double freq_hz)
{
    m_frequency = freq_hz;
}

void MatchingCalculator::setZ0(double z0)
{
    m_z0 = z0;
}

double MatchingCalculator::reactanceToInductance(double x, double freq)
{
    if (freq <= 0) return 0;
    return x / (2.0 * SmithMath::PI * freq);
}

double MatchingCalculator::reactanceToCapacitance(double x, double freq)
{
    if (freq <= 0 || x == 0) return 0;
    return -1.0 / (2.0 * SmithMath::PI * freq * x);
}

double MatchingCalculator::susceptanceToCapacitance(double b, double freq)
{
    if (freq <= 0) return 0;
    return b / (2.0 * SmithMath::PI * freq);
}

double MatchingCalculator::susceptanceToInductance(double b, double freq)
{
    if (freq <= 0 || b == 0) return 0;
    return -1.0 / (2.0 * SmithMath::PI * freq * b);
}

std::vector<MatchingSolution> MatchingCalculator::calculateLSection() const
{
    std::vector<MatchingSolution> solutions;
    
    double rs = m_sourceZ.real();
    double xs = m_sourceZ.imag();
    double rl = m_loadZ.real();
    double xl = m_loadZ.imag();
    
    if (rs <= 0 || rl <= 0) {
        return solutions;
    }
    
    // Case 1: Rs > Rl - Shunt element at source side
    if (rs > rl) {
        // Calculate Q
        double q = std::sqrt(rs / rl - 1.0);
        
        // Solution 1a: Shunt L, Series C (or Shunt C, Series L)
        double b1 = q / rs;  // Shunt susceptance
        double x1 = q * rl - xl;  // Series reactance to cancel load reactance
        
        // Solution 1b: Opposite signs
        double b2 = -q / rs;
        double x2 = -q * rl - xl;
        
        // Create solutions
        solutions.push_back(createLSectionSolution(x1, b1, true));
        solutions.push_back(createLSectionSolution(x2, b2, true));
    }
    // Case 2: Rs < Rl - Shunt element at load side
    else if (rs < rl) {
        double q = std::sqrt(rl / rs - 1.0);
        
        // Solution 2a
        double x1 = q * rs - xs;  // Series reactance
        double b1 = q / rl;       // Shunt susceptance
        
        // Solution 2b: Opposite signs
        double x2 = -q * rs - xs;
        double b2 = -q / rl;
        
        solutions.push_back(createLSectionSolution(x1, b1, false));
        solutions.push_back(createLSectionSolution(x2, b2, false));
    }
    // Case 3: Rs == Rl - Only need to cancel reactance
    else {
        if (std::abs(xl - xs) > 1e-12) {
            MatchingSolution sol;
            sol.topology = MatchingTopology::LSection;
            sol.frequency = m_frequency;
            sol.sourceZ = m_sourceZ;
            sol.loadZ = m_loadZ;
            sol.valid = true;
            
            double xCancel = -(xl - xs);
            MatchingElement elem;
            elem.connection = ConnectionType::Series;
            
            if (xCancel > 0) {
                elem.type = ComponentType::Inductor;
                elem.value = reactanceToInductance(xCancel, m_frequency);
            } else {
                elem.type = ComponentType::Capacitor;
                elem.value = reactanceToCapacitance(xCancel, m_frequency);
            }
            sol.elements.push_back(elem);
            solutions.push_back(sol);
        }
    }
    
    return solutions;
}

MatchingSolution MatchingCalculator::createLSectionSolution(
    double x_series, double b_shunt, bool shuntFirst) const
{
    MatchingSolution sol;
    sol.topology = shuntFirst ? MatchingTopology::LSection : MatchingTopology::LSection_Reversed;
    sol.frequency = m_frequency;
    sol.sourceZ = m_sourceZ;
    sol.loadZ = m_loadZ;
    sol.valid = true;
    
    // Create shunt element
    MatchingElement shuntElem;
    shuntElem.connection = ConnectionType::Shunt;
    if (b_shunt > 0) {
        shuntElem.type = ComponentType::Capacitor;
        shuntElem.value = susceptanceToCapacitance(b_shunt, m_frequency);
    } else {
        shuntElem.type = ComponentType::Inductor;
        shuntElem.value = susceptanceToInductance(b_shunt, m_frequency);
    }
    
    // Create series element
    MatchingElement seriesElem;
    seriesElem.connection = ConnectionType::Series;
    if (x_series > 0) {
        seriesElem.type = ComponentType::Inductor;
        seriesElem.value = reactanceToInductance(x_series, m_frequency);
    } else {
        seriesElem.type = ComponentType::Capacitor;
        seriesElem.value = reactanceToCapacitance(x_series, m_frequency);
    }
    
    // Add in correct order
    if (shuntFirst) {
        sol.elements.push_back(shuntElem);
        sol.elements.push_back(seriesElem);
    } else {
        sol.elements.push_back(seriesElem);
        sol.elements.push_back(shuntElem);
    }
    
    return sol;
}

std::vector<MatchingSolution> MatchingCalculator::calculatePiNetwork(double targetQ) const
{
    std::vector<MatchingSolution> solutions;
    
    double rs = m_sourceZ.real();
    double rl = m_loadZ.real();
    
    if (rs <= 0 || rl <= 0) return solutions;
    
    // Pi network: C1-L-C2 or L1-C-L2
    // Use virtual resistor method
    double rVirt = std::min(rs, rl) / (1.0 + targetQ * targetQ);
    
    // Q1 for source side
    double q1 = std::sqrt(rs / rVirt - 1.0);
    double b1 = q1 / rs;  // Shunt susceptance at source
    double x1 = q1 * rVirt;  // Series reactance (part 1)
    
    // Q2 for load side
    double q2 = std::sqrt(rl / rVirt - 1.0);
    double b2 = q2 / rl;  // Shunt susceptance at load
    double x2 = q2 * rVirt;  // Series reactance (part 2)
    
    double xTotal = x1 + x2;  // Total series reactance
    
    // Create solution with shunt capacitors and series inductor
    MatchingSolution sol;
    sol.topology = MatchingTopology::PiNetwork;
    sol.frequency = m_frequency;
    sol.sourceZ = m_sourceZ;
    sol.loadZ = m_loadZ;
    sol.valid = true;
    
    // Shunt C at source
    MatchingElement c1;
    c1.connection = ConnectionType::Shunt;
    c1.type = ComponentType::Capacitor;
    c1.value = susceptanceToCapacitance(b1, m_frequency);
    sol.elements.push_back(c1);
    
    // Series L
    MatchingElement l;
    l.connection = ConnectionType::Series;
    l.type = ComponentType::Inductor;
    l.value = reactanceToInductance(xTotal, m_frequency);
    sol.elements.push_back(l);
    
    // Shunt C at load
    MatchingElement c2;
    c2.connection = ConnectionType::Shunt;
    c2.type = ComponentType::Capacitor;
    c2.value = susceptanceToCapacitance(b2, m_frequency);
    sol.elements.push_back(c2);
    
    solutions.push_back(sol);
    
    return solutions;
}

std::vector<MatchingSolution> MatchingCalculator::calculateTNetwork(double targetQ) const
{
    std::vector<MatchingSolution> solutions;
    
    double rs = m_sourceZ.real();
    double rl = m_loadZ.real();
    
    if (rs <= 0 || rl <= 0) return solutions;
    
    // T network: L1-C-L2 or C1-L-C2
    double rVirt = std::max(rs, rl) * (1.0 + targetQ * targetQ);
    
    double q1 = std::sqrt(rVirt / rs - 1.0);
    double x1 = q1 * rs;  // Series reactance at source
    double b = q1 / rVirt;  // Shunt susceptance
    
    double q2 = std::sqrt(rVirt / rl - 1.0);
    double x2 = q2 * rl;  // Series reactance at load
    double b2 = q2 / rVirt;
    
    double bTotal = b + b2;
    
    // Create solution
    MatchingSolution sol;
    sol.topology = MatchingTopology::TNetwork;
    sol.frequency = m_frequency;
    sol.sourceZ = m_sourceZ;
    sol.loadZ = m_loadZ;
    sol.valid = true;
    
    // Series L at source
    MatchingElement l1;
    l1.connection = ConnectionType::Series;
    l1.type = ComponentType::Inductor;
    l1.value = reactanceToInductance(x1, m_frequency);
    sol.elements.push_back(l1);
    
    // Shunt C
    MatchingElement c;
    c.connection = ConnectionType::Shunt;
    c.type = ComponentType::Capacitor;
    c.value = susceptanceToCapacitance(bTotal, m_frequency);
    sol.elements.push_back(c);
    
    // Series L at load
    MatchingElement l2;
    l2.connection = ConnectionType::Series;
    l2.type = ComponentType::Inductor;
    l2.value = reactanceToInductance(x2, m_frequency);
    sol.elements.push_back(l2);
    
    solutions.push_back(sol);
    
    return solutions;
}

std::vector<MatchingSolution> MatchingCalculator::calculateSingleStub() const
{
    std::vector<MatchingSolution> solutions;
    
    // Normalize load impedance
    Complex yL = 1.0 / (m_loadZ / m_z0);  // Normalized load admittance
    double g = yL.real();
    double b = yL.imag();
    
    // Skip if load is already matched (g = 1, b = 0)
    if (std::abs(g - 1.0) < 1e-10 && std::abs(b) < 1e-10) {
        return solutions;
    }
    
    // For single stub matching, we need g <= 1
    // If g > 1, we need to use series stub or different approach
    if (g > 0) {
        double lambda = 3e8 / m_frequency;  // Wavelength
        double beta = 2.0 * M_PI / lambda;  // Propagation constant
        
        // Calculate distance to stub (two solutions)
        // tan(beta*d) = (b +/- sqrt(g*((1-g)^2 + b^2))) / (g - 1 - b^2/g)
        // For g = 1: special case
        
        double t1, t2;  // tan(beta*d) values
        
        if (std::abs(g - 1.0) < 1e-10) {
            // Special case: g = 1
            t1 = -b / 2.0;
            t2 = t1;
        } else {
            double A = (1 - g) * (1 - g) + b * b;
            double B_val = b + std::sqrt(g * A / g);
            double C_val = b - std::sqrt(g * A / g);
            double D = g - 1.0;
            
            if (std::abs(D) > 1e-10) {
                t1 = B_val / D;
                t2 = C_val / D;
            } else {
                return solutions;  // Cannot solve
            }
        }
        
        // Calculate stub susceptances
        for (int i = 0; i < 2; ++i) {
            double t = (i == 0) ? t1 : t2;
            double d = std::atan(t) / beta;  // Distance to stub
            if (d < 0) d += lambda / 2.0;    // Ensure positive distance
            
            // Input admittance at stub location
            Complex numerator = yL + Complex(0, t);
            Complex denominator = Complex(1, 0) + Complex(0, t) * yL;
            Complex y_in = numerator / denominator;
            
            // Required stub susceptance
            double B_stub = -y_in.imag();
            
            // Open stub length: l = atan(B_stub) / beta
            // Short stub length: l = atan(-1/B_stub) / beta
            
            // Open stub solution
            {
                MatchingSolution sol;
                sol.topology = MatchingTopology::SingleStubOpen;
                sol.frequency = m_frequency;
                sol.sourceZ = m_sourceZ;
                sol.loadZ = m_loadZ;
                sol.valid = true;
                
                double l_open = std::atan(B_stub) / beta;
                if (l_open < 0) l_open += lambda / 2.0;
                
                // Transmission line element (distance to stub)
                MatchingElement elem1;
                elem1.type = ComponentType::TransmissionLine;
                elem1.connection = ConnectionType::Series;
                elem1.value = d;  // Store length in meters
                elem1.label = QString("TL: %1 mm").arg(d * 1000, 0, 'f', 2);
                sol.elements.push_back(elem1);
                
                // Open stub element
                MatchingElement elem2;
                elem2.type = ComponentType::OpenStub;
                elem2.connection = ConnectionType::Shunt;
                elem2.value = l_open;
                elem2.label = QString("Open Stub: %1 mm").arg(l_open * 1000, 0, 'f', 2);
                sol.elements.push_back(elem2);
                
                sol.description = QString("Open stub at d=%1mm, l=%2mm")
                    .arg(d * 1000, 0, 'f', 2)
                    .arg(l_open * 1000, 0, 'f', 2);
                    
                solutions.push_back(sol);
            }
            
            // Short stub solution
            {
                MatchingSolution sol;
                sol.topology = MatchingTopology::SingleStubShort;
                sol.frequency = m_frequency;
                sol.sourceZ = m_sourceZ;
                sol.loadZ = m_loadZ;
                sol.valid = true;
                
                double l_short = -std::atan(1.0 / B_stub) / beta;
                if (l_short < 0) l_short += lambda / 2.0;
                
                MatchingElement elem1;
                elem1.type = ComponentType::TransmissionLine;
                elem1.connection = ConnectionType::Series;
                elem1.value = d;
                elem1.label = QString("TL: %1 mm").arg(d * 1000, 0, 'f', 2);
                sol.elements.push_back(elem1);
                
                MatchingElement elem2;
                elem2.type = ComponentType::ShortStub;
                elem2.connection = ConnectionType::Shunt;
                elem2.value = l_short;
                elem2.label = QString("Short Stub: %1 mm").arg(l_short * 1000, 0, 'f', 2);
                sol.elements.push_back(elem2);
                
                sol.description = QString("Short stub at d=%1mm, l=%2mm")
                    .arg(d * 1000, 0, 'f', 2)
                    .arg(l_short * 1000, 0, 'f', 2);
                    
                solutions.push_back(sol);
            }
        }
    }
    
    return solutions;
}

std::vector<MatchingSolution> MatchingCalculator::calculateQuarterWave() const
{
    std::vector<MatchingSolution> solutions;
    
    // Quarter-wave transformer matching
    // Works when load is purely resistive or after adding series/parallel element
    double Rl = m_loadZ.real();
    double Xl = m_loadZ.imag();
    double Rs = m_sourceZ.real();
    
    // Check if load is purely resistive
    if (std::abs(Xl) < 1e-10 && Rl > 0 && Rs > 0) {
        // Characteristic impedance of quarter-wave section
        double Zqw = std::sqrt(Rs * Rl);
        double lambda = 3e8 / m_frequency;
        double length = lambda / 4.0;
        
        MatchingSolution sol;
        sol.topology = MatchingTopology::QuarterWave;
        sol.frequency = m_frequency;
        sol.sourceZ = m_sourceZ;
        sol.loadZ = m_loadZ;
        sol.valid = true;
        
        MatchingElement elem;
        elem.type = ComponentType::TransmissionLine;
        elem.connection = ConnectionType::Series;
        elem.value = Zqw;  // Store characteristic impedance
        elem.label = QString("λ/4 TL: Z0=%1Ω, L=%2mm")
            .arg(Zqw, 0, 'f', 1)
            .arg(length * 1000, 0, 'f', 2);
        sol.elements.push_back(elem);
        
        sol.description = QString("Quarter-wave transformer Z0=%1Ω")
            .arg(Zqw, 0, 'f', 1);
            
        solutions.push_back(sol);
    }
    // If load has reactive component, need additional element first
    else if (Rl > 0) {
        // Add series element to cancel reactance, then quarter-wave
        double Xcancel = -Xl;
        double Zqw = std::sqrt(Rs * Rl);
        
        MatchingSolution sol;
        sol.topology = MatchingTopology::QuarterWave;
        sol.frequency = m_frequency;
        sol.sourceZ = m_sourceZ;
        sol.loadZ = m_loadZ;
        sol.valid = true;
        
        // Add reactive element to cancel load reactance
        MatchingElement elem1;
        elem1.connection = ConnectionType::Series;
        if (Xcancel > 0) {
            elem1.type = ComponentType::Inductor;
            elem1.value = reactanceToInductance(Xcancel, m_frequency);
            elem1.label = QString("L: %1").arg(formatValue(elem1.value, "H"));
        } else {
            elem1.type = ComponentType::Capacitor;
            elem1.value = reactanceToCapacitance(Xcancel, m_frequency);
            elem1.label = QString("C: %1").arg(formatValue(elem1.value, "F"));
        }
        sol.elements.push_back(elem1);
        
        // Quarter-wave section
        MatchingElement elem2;
        elem2.type = ComponentType::TransmissionLine;
        elem2.connection = ConnectionType::Series;
        elem2.value = Zqw;
        elem2.label = QString("λ/4 TL: Z0=%1Ω")
            .arg(Zqw, 0, 'f', 1);
        sol.elements.push_back(elem2);
        
        sol.description = QString("λ/4 transformer with reactance cancellation");
        
        solutions.push_back(sol);
    }
    
    return solutions;
}

std::vector<MatchingSolution> MatchingCalculator::calculateAll() const
{
    std::vector<MatchingSolution> all;
    
    auto lSec = calculateLSection();
    all.insert(all.end(), lSec.begin(), lSec.end());
    
    auto pi = calculatePiNetwork();
    all.insert(all.end(), pi.begin(), pi.end());
    
    auto t = calculateTNetwork();
    all.insert(all.end(), t.begin(), t.end());
    
    return all;
}

} // namespace SmithTool
