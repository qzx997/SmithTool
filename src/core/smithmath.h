/**
 * @file smithmath.h
 * @brief Smith Chart mathematical calculations
 * 
 * Provides coordinate transformations between screen coordinates,
 * reflection coefficient (Gamma), and impedance/admittance values.
 */

#ifndef SMITHTOOL_SMITHMATH_H
#define SMITHTOOL_SMITHMATH_H

#include <complex>
#include <cmath>
#include <QPointF>
#include <QRectF>

namespace SmithTool {

using Complex = std::complex<double>;

/**
 * @brief Smith Chart mathematical utility class
 */
class SmithMath {
public:
    /**
     * @brief Convert impedance to reflection coefficient
     * @param z Complex impedance
     * @param z0 Reference impedance (default 50 ohm)
     * @return Reflection coefficient Gamma
     */
    static Complex impedanceToGamma(const Complex& z, double z0 = 50.0);
    
    /**
     * @brief Convert reflection coefficient to impedance
     * @param gamma Reflection coefficient
     * @param z0 Reference impedance (default 50 ohm)
     * @return Complex impedance
     */
    static Complex gammaToImpedance(const Complex& gamma, double z0 = 50.0);
    
    /**
     * @brief Convert admittance to reflection coefficient
     * @param y Complex admittance
     * @param y0 Reference admittance (default 0.02 S for 50 ohm)
     * @return Reflection coefficient Gamma
     */
    static Complex admittanceToGamma(const Complex& y, double y0 = 0.02);
    
    /**
     * @brief Convert reflection coefficient to admittance
     * @param gamma Reflection coefficient
     * @param y0 Reference admittance (default 0.02 S for 50 ohm)
     * @return Complex admittance
     */
    static Complex gammaToAdmittance(const Complex& gamma, double y0 = 0.02);
    
    /**
     * @brief Convert normalized impedance to Gamma
     * @param zn Normalized impedance (z/Z0)
     * @return Reflection coefficient Gamma
     */
    static Complex normalizedZToGamma(const Complex& zn);
    
    /**
     * @brief Convert Gamma to normalized impedance
     * @param gamma Reflection coefficient
     * @return Normalized impedance (z/Z0)
     */
    static Complex gammaToNormalizedZ(const Complex& gamma);
    
    /**
     * @brief Convert Gamma coordinates to screen coordinates
     * @param gamma Reflection coefficient
     * @param center Center point of the chart
     * @param radius Radius of the chart
     * @return Screen coordinates
     */
    static QPointF gammaToScreen(const Complex& gamma, 
                                  const QPointF& center, 
                                  double radius);
    
    /**
     * @brief Convert screen coordinates to Gamma
     * @param point Screen coordinates
     * @param center Center point of the chart
     * @param radius Radius of the chart
     * @return Reflection coefficient Gamma
     */
    static Complex screenToGamma(const QPointF& point, 
                                  const QPointF& center, 
                                  double radius);
    
    /**
     * @brief Calculate the center of a constant resistance circle
     * @param r Normalized resistance value
     * @return Circle center in Gamma plane
     */
    static Complex constantRCircleCenter(double r);
    
    /**
     * @brief Calculate the radius of a constant resistance circle
     * @param r Normalized resistance value
     * @return Circle radius
     */
    static double constantRCircleRadius(double r);
    
    /**
     * @brief Calculate the center of a constant reactance arc
     * @param x Normalized reactance value
     * @return Arc center in Gamma plane
     */
    static Complex constantXArcCenter(double x);
    
    /**
     * @brief Calculate the radius of a constant reactance arc
     * @param x Normalized reactance value
     * @return Arc radius
     */
    static double constantXArcRadius(double x);
    
    /**
     * @brief Calculate VSWR from reflection coefficient magnitude
     * @param gammaMag Magnitude of Gamma (|Gamma|)
     * @return VSWR value
     */
    static double gammaToVSWR(double gammaMag);
    
    /**
     * @brief Calculate reflection coefficient magnitude from VSWR
     * @param vswr VSWR value
     * @return Magnitude of Gamma
     */
    static double vswrToGamma(double vswr);
    
    /**
     * @brief Calculate return loss in dB from reflection coefficient
     * @param gamma Reflection coefficient
     * @return Return loss in dB (negative value)
     */
    static double gammaToReturnLoss(const Complex& gamma);
    
    /**
     * @brief Check if a point is inside the unit circle
     * @param gamma Reflection coefficient
     * @return true if |Gamma| <= 1
     */
    static bool isInsideUnitCircle(const Complex& gamma);
    
    /**
     * @brief Calculate the phase angle of Gamma in degrees
     * @param gamma Reflection coefficient
     * @return Phase angle in degrees
     */
    static double gammaPhaseDegrees(const Complex& gamma);
    
    // Mathematical constants
    static constexpr double PI = 3.14159265358979323846;
    static constexpr double TWO_PI = 2.0 * PI;
};

} // namespace SmithTool

#endif // SMITHTOOL_SMITHMATH_H
