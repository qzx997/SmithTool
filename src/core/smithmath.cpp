/**
 * @file smithmath.cpp
 * @brief Smith Chart mathematical calculations implementation
 */

#include "smithmath.h"

namespace SmithTool {

Complex SmithMath::impedanceToGamma(const Complex& z, double z0)
{
    return (z - z0) / (z + z0);
}

Complex SmithMath::gammaToImpedance(const Complex& gamma, double z0)
{
    Complex one(1.0, 0.0);
    if (std::abs(one - gamma) < 1e-12) {
        // Avoid division by zero - return very high impedance
        return Complex(1e12, 0.0);
    }
    return z0 * (one + gamma) / (one - gamma);
}

Complex SmithMath::admittanceToGamma(const Complex& y, double y0)
{
    return (y0 - y) / (y0 + y);
}

Complex SmithMath::gammaToAdmittance(const Complex& gamma, double y0)
{
    Complex one(1.0, 0.0);
    if (std::abs(one + gamma) < 1e-12) {
        // Avoid division by zero
        return Complex(1e12, 0.0);
    }
    return y0 * (one - gamma) / (one + gamma);
}

Complex SmithMath::normalizedZToGamma(const Complex& zn)
{
    Complex one(1.0, 0.0);
    return (zn - one) / (zn + one);
}

Complex SmithMath::gammaToNormalizedZ(const Complex& gamma)
{
    Complex one(1.0, 0.0);
    if (std::abs(one - gamma) < 1e-12) {
        return Complex(1e12, 0.0);
    }
    return (one + gamma) / (one - gamma);
}

QPointF SmithMath::gammaToScreen(const Complex& gamma, 
                                  const QPointF& center, 
                                  double radius)
{
    double x = center.x() + gamma.real() * radius;
    double y = center.y() - gamma.imag() * radius;  // Y-axis is flipped
    return QPointF(x, y);
}

Complex SmithMath::screenToGamma(const QPointF& point, 
                                  const QPointF& center, 
                                  double radius)
{
    double real = (point.x() - center.x()) / radius;
    double imag = -(point.y() - center.y()) / radius;  // Y-axis is flipped
    return Complex(real, imag);
}

Complex SmithMath::constantRCircleCenter(double r)
{
    // Center of constant resistance circle: (r/(r+1), 0)
    return Complex(r / (r + 1.0), 0.0);
}

double SmithMath::constantRCircleRadius(double r)
{
    // Radius of constant resistance circle: 1/(r+1)
    return 1.0 / (r + 1.0);
}

Complex SmithMath::constantXArcCenter(double x)
{
    // Center of constant reactance arc: (1, 1/x)
    if (std::abs(x) < 1e-12) {
        // For x = 0, the arc becomes the real axis
        return Complex(0.0, 1e12);
    }
    return Complex(1.0, 1.0 / x);
}

double SmithMath::constantXArcRadius(double x)
{
    // Radius of constant reactance arc: 1/|x|
    if (std::abs(x) < 1e-12) {
        return 1e12;  // Very large radius (straight line)
    }
    return 1.0 / std::abs(x);
}

double SmithMath::gammaToVSWR(double gammaMag)
{
    if (gammaMag >= 1.0) {
        return 1e6;  // Infinity (or very high)
    }
    if (gammaMag < 0.0) {
        gammaMag = 0.0;
    }
    return (1.0 + gammaMag) / (1.0 - gammaMag);
}

double SmithMath::vswrToGamma(double vswr)
{
    if (vswr < 1.0) {
        vswr = 1.0;
    }
    return (vswr - 1.0) / (vswr + 1.0);
}

double SmithMath::gammaToReturnLoss(const Complex& gamma)
{
    double mag = std::abs(gamma);
    if (mag < 1e-12) {
        return -200.0;  // Very good match (essentially -infinity dB)
    }
    return 20.0 * std::log10(mag);
}

bool SmithMath::isInsideUnitCircle(const Complex& gamma)
{
    return std::abs(gamma) <= 1.0;
}

double SmithMath::gammaPhaseDegrees(const Complex& gamma)
{
    return std::arg(gamma) * 180.0 / PI;
}

} // namespace SmithTool
