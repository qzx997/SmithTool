/**
 * @file component.cpp
 * @brief RF component value calculations implementation
 */

#include "component.h"

namespace SmithTool {

// ComponentValue methods

QString ComponentValue::valueWithUnit() const
{
    return QString("%1 %2").arg(scaledValue(), 0, 'f', 3).arg(scaledUnitPrefix() + unitString());
}

QString ComponentValue::unitString() const
{
    switch (type) {
        case ComponentType::Resistor: return "Ω";
        case ComponentType::Inductor: return "H";
        case ComponentType::Capacitor: return "F";
        default: return "";
    }
}

double ComponentValue::scaledValue() const
{
    double absVal = std::abs(value);
    
    switch (type) {
        case ComponentType::Resistor:
            if (absVal >= 1e6) return value / 1e6;
            if (absVal >= 1e3) return value / 1e3;
            if (absVal >= 1) return value;
            return value * 1e3;  // mΩ
            
        case ComponentType::Inductor:
            if (absVal >= 1e-3) return value * 1e3;      // mH
            if (absVal >= 1e-6) return value * 1e6;      // µH
            if (absVal >= 1e-9) return value * 1e9;      // nH
            return value * 1e12;  // pH
            
        case ComponentType::Capacitor:
            if (absVal >= 1e-6) return value * 1e6;      // µF
            if (absVal >= 1e-9) return value * 1e9;      // nF
            if (absVal >= 1e-12) return value * 1e12;    // pF
            return value * 1e15;  // fF
            
        default:
            return value;
    }
}

QString ComponentValue::scaledUnitPrefix() const
{
    double absVal = std::abs(value);
    
    switch (type) {
        case ComponentType::Resistor:
            if (absVal >= 1e6) return "M";
            if (absVal >= 1e3) return "k";
            if (absVal >= 1) return "";
            return "m";
            
        case ComponentType::Inductor:
            if (absVal >= 1e-3) return "m";
            if (absVal >= 1e-6) return "µ";
            if (absVal >= 1e-9) return "n";
            return "p";
            
        case ComponentType::Capacitor:
            if (absVal >= 1e-6) return "µ";
            if (absVal >= 1e-9) return "n";
            if (absVal >= 1e-12) return "p";
            return "f";
            
        default:
            return "";
    }
}

// ComponentCalculator methods

ComponentValue ComponentCalculator::calculateFromImpedance(const Complex& z, double freq_hz)
{
    double x = z.imag();
    
    if (std::abs(x) < 1e-12) {
        // Pure resistance
        return calculateResistance(z.real());
    } else if (x > 0) {
        // Inductive reactance
        return calculateInductance(x, freq_hz);
    } else {
        // Capacitive reactance
        return calculateCapacitance(x, freq_hz);
    }
}

ComponentValue ComponentCalculator::calculateResistance(double r)
{
    return ComponentValue(ComponentType::Resistor, r, 0.0);
}

ComponentValue ComponentCalculator::calculateInductance(double x, double freq_hz)
{
    // X = 2πfL  =>  L = X / (2πf)
    if (freq_hz < 1e-12) {
        return ComponentValue(ComponentType::Inductor, 0.0, freq_hz);
    }
    double l = x / (TWO_PI * freq_hz);
    return ComponentValue(ComponentType::Inductor, l, freq_hz);
}

ComponentValue ComponentCalculator::calculateCapacitance(double x, double freq_hz)
{
    // X = -1/(2πfC)  =>  C = -1/(2πfX)
    if (freq_hz < 1e-12 || std::abs(x) < 1e-12) {
        return ComponentValue(ComponentType::Capacitor, 0.0, freq_hz);
    }
    double c = -1.0 / (TWO_PI * freq_hz * x);
    return ComponentValue(ComponentType::Capacitor, c, freq_hz);
}

double ComponentCalculator::inductorReactance(double l_henry, double freq_hz)
{
    return TWO_PI * freq_hz * l_henry;
}

double ComponentCalculator::capacitorReactance(double c_farad, double freq_hz)
{
    if (c_farad < 1e-18) return -1e12;
    return -1.0 / (TWO_PI * freq_hz * c_farad);
}

double ComponentCalculator::inductorSusceptance(double l_henry, double freq_hz)
{
    if (l_henry < 1e-18) return -1e12;
    return -1.0 / (TWO_PI * freq_hz * l_henry);
}

double ComponentCalculator::capacitorSusceptance(double c_farad, double freq_hz)
{
    return TWO_PI * freq_hz * c_farad;
}

ComponentValue ComponentCalculator::calculateSeriesComponent(
    const Complex& z_current, 
    const Complex& z_target, 
    double freq_hz)
{
    double deltaX = z_target.imag() - z_current.imag();
    
    if (std::abs(deltaX) < 1e-12) {
        return ComponentValue();  // No component needed
    } else if (deltaX > 0) {
        return calculateInductance(deltaX, freq_hz);
    } else {
        return calculateCapacitance(deltaX, freq_hz);
    }
}

ComponentValue ComponentCalculator::calculateShuntComponent(
    const Complex& y_current, 
    const Complex& y_target, 
    double freq_hz)
{
    double deltaB = y_target.imag() - y_current.imag();
    
    if (std::abs(deltaB) < 1e-12) {
        return ComponentValue();  // No component needed
    } else if (deltaB > 0) {
        // Positive susceptance = capacitor
        double c = deltaB / TWO_PI / freq_hz;
        return ComponentValue(ComponentType::Capacitor, c, freq_hz);
    } else {
        // Negative susceptance = inductor
        double l = -1.0 / (TWO_PI * freq_hz * deltaB);
        return ComponentValue(ComponentType::Inductor, l, freq_hz);
    }
}

ComponentType ComponentCalculator::typeFromReactance(double x)
{
    if (std::abs(x) < 1e-12) {
        return ComponentType::None;
    } else if (x > 0) {
        return ComponentType::Inductor;
    } else {
        return ComponentType::Capacitor;
    }
}

ComponentType ComponentCalculator::typeFromSusceptance(double b)
{
    if (std::abs(b) < 1e-12) {
        return ComponentType::None;
    } else if (b > 0) {
        return ComponentType::Capacitor;
    } else {
        return ComponentType::Inductor;
    }
}

// RFComponent methods

double RFComponent::reactance() const
{
    switch (type) {
        case ComponentType::Inductor:
            return ComponentCalculator::inductorReactance(value, frequency);
        case ComponentType::Capacitor:
            return ComponentCalculator::capacitorReactance(value, frequency);
        case ComponentType::Resistor:
            return 0.0;
        default:
            return 0.0;
    }
}

double RFComponent::susceptance() const
{
    switch (type) {
        case ComponentType::Inductor:
            return ComponentCalculator::inductorSusceptance(value, frequency);
        case ComponentType::Capacitor:
            return ComponentCalculator::capacitorSusceptance(value, frequency);
        case ComponentType::Resistor:
            return 1.0 / value;
        default:
            return 0.0;
    }
}

QString RFComponent::toString() const
{
    QString connStr = (connection == ConnectionType::Series) ? "Series" : "Shunt";
    ComponentValue cv(type, value, frequency);
    
    QString typeName;
    switch (type) {
        case ComponentType::Resistor: typeName = "R"; break;
        case ComponentType::Inductor: typeName = "L"; break;
        case ComponentType::Capacitor: typeName = "C"; break;
        default: typeName = "?"; break;
    }
    
    return QString("%1 %2 = %3").arg(connStr).arg(typeName).arg(cv.valueWithUnit());
}

QString RFComponent::toNetlist() const
{
    ComponentValue cv(type, value, frequency);
    QString name;
    
    switch (type) {
        case ComponentType::Resistor: 
            name = QString("R%1").arg(position);
            break;
        case ComponentType::Inductor:
            name = QString("L%1").arg(position);
            break;
        case ComponentType::Capacitor:
            name = QString("C%1").arg(position);
            break;
        default:
            return "";
    }
    
    // Generate SPICE-style netlist entry
    return QString("%1 n%2 n%3 %4")
        .arg(name)
        .arg(position)
        .arg(position + 1)
        .arg(value, 0, 'e', 6);
}

} // namespace SmithTool
