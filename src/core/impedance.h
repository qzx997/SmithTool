/**
 * @file impedance.h
 * @brief Impedance and Admittance data structures and conversions
 */

#ifndef SMITHTOOL_IMPEDANCE_H
#define SMITHTOOL_IMPEDANCE_H

#include <complex>
#include <QString>

namespace SmithTool {

using Complex = std::complex<double>;

/**
 * @brief Impedance data structure with various representations
 */
struct Impedance {
    Complex value;          // Complex impedance Z = R + jX
    double z0 = 50.0;       // Reference impedance
    
    // Constructors
    Impedance() : value(50.0, 0.0), z0(50.0) {}
    Impedance(double r, double x, double ref = 50.0) : value(r, x), z0(ref) {}
    Impedance(const Complex& z, double ref = 50.0) : value(z), z0(ref) {}
    
    // Real and imaginary parts
    double resistance() const { return value.real(); }
    double reactance() const { return value.imag(); }
    
    // Normalized impedance
    Complex normalized() const { return value / z0; }
    double normalizedR() const { return value.real() / z0; }
    double normalizedX() const { return value.imag() / z0; }
    
    // Magnitude and phase
    double magnitude() const { return std::abs(value); }
    double phaseRadians() const { return std::arg(value); }
    double phaseDegrees() const { return std::arg(value) * 180.0 / 3.14159265358979323846; }
    
    // Convert to admittance
    Complex toAdmittance() const;
    
    // Convert to reflection coefficient
    Complex toGamma() const;
    
    // String representation
    QString toString() const;
    QString toNormalizedString() const;
};

/**
 * @brief Admittance data structure
 */
struct Admittance {
    Complex value;          // Complex admittance Y = G + jB
    double y0 = 0.02;       // Reference admittance (1/50 S)
    
    // Constructors
    Admittance() : value(0.02, 0.0), y0(0.02) {}
    Admittance(double g, double b, double ref = 0.02) : value(g, b), y0(ref) {}
    Admittance(const Complex& y, double ref = 0.02) : value(y), y0(ref) {}
    
    // Conductance and susceptance
    double conductance() const { return value.real(); }
    double susceptance() const { return value.imag(); }
    
    // Normalized admittance
    Complex normalized() const { return value / y0; }
    double normalizedG() const { return value.real() / y0; }
    double normalizedB() const { return value.imag() / y0; }
    
    // Convert to impedance
    Complex toImpedance() const;
    
    // Convert to reflection coefficient
    Complex toGamma() const;
    
    // String representation
    QString toString() const;
};

/**
 * @brief Reflection coefficient data structure
 */
struct ReflectionCoeff {
    Complex gamma;          // Reflection coefficient
    double z0 = 50.0;       // Reference impedance
    
    // Constructors
    ReflectionCoeff() : gamma(0.0, 0.0), z0(50.0) {}
    ReflectionCoeff(double real, double imag, double ref = 50.0) 
        : gamma(real, imag), z0(ref) {}
    ReflectionCoeff(const Complex& g, double ref = 50.0) : gamma(g), z0(ref) {}
    
    // Polar form
    double magnitude() const { return std::abs(gamma); }
    double phaseRadians() const { return std::arg(gamma); }
    double phaseDegrees() const { return std::arg(gamma) * 180.0 / 3.14159265358979323846; }
    
    // Derived quantities
    double vswr() const;
    double returnLossDb() const;
    double mismatchLossDb() const;
    
    // Conversions
    Impedance toImpedance() const;
    Admittance toAdmittance() const;
    
    // Check validity
    bool isPassive() const { return magnitude() <= 1.0; }
    
    // String representations
    QString toRectString() const;
    QString toPolarString() const;
};

} // namespace SmithTool

#endif // SMITHTOOL_IMPEDANCE_H
