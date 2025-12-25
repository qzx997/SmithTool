/**
 * @file impedance.cpp
 * @brief Impedance and Admittance implementation
 */

#include "impedance.h"
#include <cmath>

namespace SmithTool {

// Impedance methods

Complex Impedance::toAdmittance() const
{
    if (std::abs(value) < 1e-12) {
        return Complex(1e12, 0.0);
    }
    return Complex(1.0, 0.0) / value;
}

Complex Impedance::toGamma() const
{
    return (value - z0) / (value + z0);
}

QString Impedance::toString() const
{
    QString sign = value.imag() >= 0 ? "+" : "-";
    return QString("%1 %2 j%3 Ω")
        .arg(value.real(), 0, 'f', 2)
        .arg(sign)
        .arg(std::abs(value.imag()), 0, 'f', 2);
}

QString Impedance::toNormalizedString() const
{
    Complex zn = normalized();
    QString sign = zn.imag() >= 0 ? "+" : "-";
    return QString("%1 %2 j%3")
        .arg(zn.real(), 0, 'f', 3)
        .arg(sign)
        .arg(std::abs(zn.imag()), 0, 'f', 3);
}

// Admittance methods

Complex Admittance::toImpedance() const
{
    if (std::abs(value) < 1e-12) {
        return Complex(1e12, 0.0);
    }
    return Complex(1.0, 0.0) / value;
}

Complex Admittance::toGamma() const
{
    return (y0 - value) / (y0 + value);
}

QString Admittance::toString() const
{
    QString sign = value.imag() >= 0 ? "+" : "-";
    return QString("%1 %2 j%3 S")
        .arg(value.real(), 0, 'e', 3)
        .arg(sign)
        .arg(std::abs(value.imag()), 0, 'e', 3);
}

// ReflectionCoeff methods

double ReflectionCoeff::vswr() const
{
    double mag = magnitude();
    if (mag >= 1.0) {
        return 1e6;
    }
    return (1.0 + mag) / (1.0 - mag);
}

double ReflectionCoeff::returnLossDb() const
{
    double mag = magnitude();
    if (mag < 1e-12) {
        return -200.0;
    }
    return 20.0 * std::log10(mag);
}

double ReflectionCoeff::mismatchLossDb() const
{
    double mag = magnitude();
    double mag2 = mag * mag;
    if (mag2 >= 1.0) {
        return -100.0;
    }
    return 10.0 * std::log10(1.0 - mag2);
}

Impedance ReflectionCoeff::toImpedance() const
{
    Complex one(1.0, 0.0);
    if (std::abs(one - gamma) < 1e-12) {
        return Impedance(1e12, 0.0, z0);
    }
    Complex z = z0 * (one + gamma) / (one - gamma);
    return Impedance(z, z0);
}

Admittance ReflectionCoeff::toAdmittance() const
{
    double y0 = 1.0 / z0;
    Complex one(1.0, 0.0);
    if (std::abs(one + gamma) < 1e-12) {
        return Admittance(1e12, 0.0, y0);
    }
    Complex y = y0 * (one - gamma) / (one + gamma);
    return Admittance(y, y0);
}

QString ReflectionCoeff::toRectString() const
{
    QString sign = gamma.imag() >= 0 ? "+" : "-";
    return QString("Γ = %1 %2 j%3")
        .arg(gamma.real(), 0, 'f', 4)
        .arg(sign)
        .arg(std::abs(gamma.imag()), 0, 'f', 4);
}

QString ReflectionCoeff::toPolarString() const
{
    return QString("|Γ| = %1  ∠%2°")
        .arg(magnitude(), 0, 'f', 4)
        .arg(phaseDegrees(), 0, 'f', 1);
}

} // namespace SmithTool
