/**
 * @file touchstone.cpp
 * @brief Touchstone file parser implementation
 */

#include "touchstone.h"
#include <QFileInfo>
#include <QRegularExpression>
#include <cmath>

namespace SmithTool {

// TouchstoneParser implementation

TouchstoneParser::TouchstoneParser()
    : m_format(SParamFormat::MA)
    , m_freqMultiplier(1e9)  // Default GHz
{
}

bool TouchstoneParser::parse(const QString& filename)
{
    m_data.clear();
    m_lastError.clear();
    
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_lastError = QString("Cannot open file: %1").arg(filename);
        return false;
    }
    
    PortCount ports = detectPortCount(filename);
    m_data.setPortCount(ports);
    m_data.setFilename(filename);
    
    QTextStream in(&file);
    bool optionFound = false;
    
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        
        // Skip empty lines and comments
        if (line.isEmpty()) continue;
        if (line.startsWith('!')) continue;
        
        // Option line
        if (line.startsWith('#')) {
            if (!parseOptionLine(line)) {
                file.close();
                return false;
            }
            optionFound = true;
            continue;
        }
        
        // Data line
        if (optionFound) {
            if (!parseDataLine(line, ports)) {
                // Non-fatal: might be a comment without !
                continue;
            }
        }
    }
    
    file.close();
    m_data.sortByFrequency();
    return true;
}

bool TouchstoneParser::parseOptionLine(const QString& line)
{
    // Format: # [freq unit] [parameter] [format] R [reference]
    // Example: # GHz S MA R 50
    
    QStringList parts = line.mid(1).trimmed().split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    
    m_data.setReferenceImpedance(50.0);  // Default
    m_format = SParamFormat::MA;          // Default
    m_freqMultiplier = 1e9;               // Default GHz
    
    for (int i = 0; i < parts.size(); ++i) {
        QString part = parts[i].toUpper();
        
        // Frequency unit
        if (part == "HZ") m_freqMultiplier = 1.0;
        else if (part == "KHZ") m_freqMultiplier = 1e3;
        else if (part == "MHZ") m_freqMultiplier = 1e6;
        else if (part == "GHZ") m_freqMultiplier = 1e9;
        
        // Format
        else if (part == "RI") m_format = SParamFormat::RI;
        else if (part == "MA") m_format = SParamFormat::MA;
        else if (part == "DB") m_format = SParamFormat::DB;
        
        // Reference impedance
        else if (part == "R" && i + 1 < parts.size()) {
            m_data.setReferenceImpedance(parts[i + 1].toDouble());
            ++i;
        }
    }
    
    return true;
}

bool TouchstoneParser::parseDataLine(const QString& line, PortCount ports)
{
    QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    
    int expectedParts = (ports == PortCount::OnePort) ? 3 : 9;
    if (parts.size() < expectedParts) {
        return false;
    }
    
    bool ok;
    double freq = parts[0].toDouble(&ok) * m_freqMultiplier;
    if (!ok) return false;
    
    SParamPoint point;
    point.frequency = freq;
    
    if (ports == PortCount::OnePort) {
        point.s11 = parseValue(parts[1].toDouble(), parts[2].toDouble());
    } else {
        // Two port: S11, S21, S12, S22
        point.s11 = parseValue(parts[1].toDouble(), parts[2].toDouble());
        point.s21 = parseValue(parts[3].toDouble(), parts[4].toDouble());
        point.s12 = parseValue(parts[5].toDouble(), parts[6].toDouble());
        point.s22 = parseValue(parts[7].toDouble(), parts[8].toDouble());
    }
    
    m_data.addPoint(point);
    return true;
}

Complex TouchstoneParser::parseValue(double v1, double v2) const
{
    switch (m_format) {
        case SParamFormat::RI:
            return Complex(v1, v2);
            
        case SParamFormat::MA: {
            // v1 = magnitude, v2 = angle in degrees
            double rad = v2 * 3.14159265358979323846 / 180.0;
            return Complex(v1 * std::cos(rad), v1 * std::sin(rad));
        }
            
        case SParamFormat::DB: {
            // v1 = magnitude in dB, v2 = angle in degrees
            double mag = std::pow(10.0, v1 / 20.0);
            double rad = v2 * 3.14159265358979323846 / 180.0;
            return Complex(mag * std::cos(rad), mag * std::sin(rad));
        }
    }
    return Complex(0, 0);
}

PortCount TouchstoneParser::detectPortCount(const QString& filename) const
{
    QFileInfo fi(filename);
    QString suffix = fi.suffix().toLower();
    
    if (suffix == "s1p") return PortCount::OnePort;
    if (suffix == "s2p") return PortCount::TwoPort;
    
    // Default to one port
    return PortCount::OnePort;
}

// TouchstoneWriter implementation

TouchstoneWriter::TouchstoneWriter()
{
}

bool TouchstoneWriter::write(const QString& filename, 
                              const SParamData& data,
                              SParamFormat format)
{
    m_lastError.clear();
    
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        m_lastError = QString("Cannot create file: %1").arg(filename);
        return false;
    }
    
    QTextStream out(&file);
    
    // Write header
    out << "! Touchstone file generated by SmithTool\n";
    out << "! Reference impedance: " << data.referenceImpedance() << " ohm\n";
    
    // Option line
    QString formatStr;
    switch (format) {
        case SParamFormat::RI: formatStr = "RI"; break;
        case SParamFormat::MA: formatStr = "MA"; break;
        case SParamFormat::DB: formatStr = "DB"; break;
    }
    
    out << "# GHz S " << formatStr << " R " << data.referenceImpedance() << "\n";
    
    // Data lines
    for (const auto& p : data.points()) {
        double freq_ghz = p.frequency / 1e9;
        out << QString::number(freq_ghz, 'f', 6);
        
        if (data.portCount() == PortCount::OnePort) {
            out << " " << formatValue(p.s11, format);
        } else {
            out << " " << formatValue(p.s11, format);
            out << " " << formatValue(p.s21, format);
            out << " " << formatValue(p.s12, format);
            out << " " << formatValue(p.s22, format);
        }
        out << "\n";
    }
    
    file.close();
    return true;
}

QString TouchstoneWriter::formatValue(const Complex& s, SParamFormat format) const
{
    switch (format) {
        case SParamFormat::RI:
            return QString("%1 %2")
                .arg(s.real(), 0, 'e', 6)
                .arg(s.imag(), 0, 'e', 6);
            
        case SParamFormat::MA: {
            double mag = std::abs(s);
            double ang = std::arg(s) * 180.0 / 3.14159265358979323846;
            return QString("%1 %2")
                .arg(mag, 0, 'e', 6)
                .arg(ang, 0, 'f', 2);
        }
            
        case SParamFormat::DB: {
            double mag = std::abs(s);
            double db = (mag > 1e-12) ? 20.0 * std::log10(mag) : -200.0;
            double ang = std::arg(s) * 180.0 / 3.14159265358979323846;
            return QString("%1 %2")
                .arg(db, 0, 'f', 3)
                .arg(ang, 0, 'f', 2);
        }
    }
    return "";
}

QString TouchstoneWriter::frequencyUnitString(double multiplier) const
{
    if (multiplier == 1.0) return "Hz";
    if (multiplier == 1e3) return "kHz";
    if (multiplier == 1e6) return "MHz";
    if (multiplier == 1e9) return "GHz";
    return "Hz";
}

} // namespace SmithTool
