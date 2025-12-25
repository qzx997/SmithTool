/**
 * @file component.h
 * @brief RF component value calculations (R, L, C)
 */

#ifndef SMITHTOOL_COMPONENT_H
#define SMITHTOOL_COMPONENT_H

#include <complex>
#include <QString>
#include <cmath>

namespace SmithTool {

using Complex = std::complex<double>;

/**
 * @brief Component types
 */
enum class ComponentType {
    Resistor,
    Inductor,
    Capacitor,
    TransmissionLine,
    OpenStub,
    ShortStub,
    None
};

/**
 * @brief Connection types
 */
enum class ConnectionType {
    Series,
    Shunt
};

/**
 * @brief Component value with proper unit
 */
struct ComponentValue {
    ComponentType type = ComponentType::None;
    double value = 0.0;         // Base value in ohms/henries/farads
    double frequency = 1e9;     // Frequency in Hz
    
    // Constructors
    ComponentValue() = default;
    ComponentValue(ComponentType t, double v, double f = 1e9) 
        : type(t), value(v), frequency(f) {}
    
    // Get value with auto-scaled unit
    QString valueWithUnit() const;
    
    // Get just the unit string
    QString unitString() const;
    
    // Get scaled value (e.g., nH instead of H)
    double scaledValue() const;
    QString scaledUnitPrefix() const;
};

/**
 * @brief Component calculator class
 */
class ComponentCalculator {
public:
    static constexpr double PI = 3.14159265358979323846;
    static constexpr double TWO_PI = 2.0 * PI;
    
    /**
     * @brief Calculate R/L/C values from impedance
     * @param z Complex impedance
     * @param freq_hz Frequency in Hz
     * @return Component values
     */
    static ComponentValue calculateFromImpedance(const Complex& z, double freq_hz);
    
    /**
     * @brief Calculate resistance from real part
     * @param r Real part of impedance
     * @return Resistance value
     */
    static ComponentValue calculateResistance(double r);
    
    /**
     * @brief Calculate inductance from positive reactance
     * @param x Positive reactance (ohms)
     * @param freq_hz Frequency in Hz
     * @return Inductance value
     */
    static ComponentValue calculateInductance(double x, double freq_hz);
    
    /**
     * @brief Calculate capacitance from negative reactance
     * @param x Negative reactance (ohms)
     * @param freq_hz Frequency in Hz
     * @return Capacitance value
     */
    static ComponentValue calculateCapacitance(double x, double freq_hz);
    
    /**
     * @brief Calculate reactance of an inductor
     * @param l_henry Inductance in henries
     * @param freq_hz Frequency in Hz
     * @return Reactance in ohms (positive)
     */
    static double inductorReactance(double l_henry, double freq_hz);
    
    /**
     * @brief Calculate reactance of a capacitor
     * @param c_farad Capacitance in farads
     * @param freq_hz Frequency in Hz
     * @return Reactance in ohms (negative)
     */
    static double capacitorReactance(double c_farad, double freq_hz);
    
    /**
     * @brief Calculate susceptance of an inductor (parallel)
     * @param l_henry Inductance in henries
     * @param freq_hz Frequency in Hz
     * @return Susceptance in siemens (negative)
     */
    static double inductorSusceptance(double l_henry, double freq_hz);
    
    /**
     * @brief Calculate susceptance of a capacitor (parallel)
     * @param c_farad Capacitance in farads
     * @param freq_hz Frequency in Hz
     * @return Susceptance in siemens (positive)
     */
    static double capacitorSusceptance(double c_farad, double freq_hz);
    
    /**
     * @brief Calculate component for series connection to reach target
     * @param z_current Current impedance
     * @param z_target Target impedance (only reactance matters)
     * @param freq_hz Frequency in Hz
     * @return Required component
     */
    static ComponentValue calculateSeriesComponent(
        const Complex& z_current, 
        const Complex& z_target, 
        double freq_hz);
    
    /**
     * @brief Calculate component for shunt connection to reach target
     * @param y_current Current admittance
     * @param y_target Target admittance (only susceptance matters)
     * @param freq_hz Frequency in Hz
     * @return Required component
     */
    static ComponentValue calculateShuntComponent(
        const Complex& y_current, 
        const Complex& y_target, 
        double freq_hz);
    
    /**
     * @brief Get component type from reactance sign
     * @param x Reactance value
     * @return Inductor for X>0, Capacitor for X<0, None for X≈0
     */
    static ComponentType typeFromReactance(double x);
    
    /**
     * @brief Get component type from susceptance sign
     * @param b Susceptance value
     * @return Capacitor for B>0, Inductor for B<0, None for B≈0
     */
    static ComponentType typeFromSusceptance(double b);
};

/**
 * @brief RF Component representation for matching networks
 */
struct RFComponent {
    ComponentType type;
    ConnectionType connection;
    double value;               // Base value
    double frequency;
    int position;               // Position in matching network (1, 2, 3...)
    
    RFComponent() : type(ComponentType::None), connection(ConnectionType::Series),
                    value(0.0), frequency(1e9), position(0) {}
    
    RFComponent(ComponentType t, ConnectionType c, double v, double f, int pos = 0)
        : type(t), connection(c), value(v), frequency(f), position(pos) {}
    
    // Get reactance or susceptance at frequency
    double reactance() const;
    double susceptance() const;
    
    // String representations
    QString toString() const;
    QString toNetlist() const;
};

} // namespace SmithTool

#endif // SMITHTOOL_COMPONENT_H
