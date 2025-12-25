/**
 * @file touchstone.h
 * @brief Touchstone file parser (.s1p, .s2p)
 */

#ifndef SMITHTOOL_TOUCHSTONE_H
#define SMITHTOOL_TOUCHSTONE_H

#include "sparamdata.h"
#include <QString>
#include <QFile>
#include <QTextStream>

namespace SmithTool {

/**
 * @brief Touchstone file parser
 */
class TouchstoneParser {
public:
    TouchstoneParser();
    ~TouchstoneParser() = default;
    
    /**
     * @brief Parse a Touchstone file
     * @param filename Path to the file
     * @return true if successful
     */
    bool parse(const QString& filename);
    
    /**
     * @brief Get the parsed data
     * @return S-parameter data
     */
    const SParamData& data() const { return m_data; }
    SParamData& data() { return m_data; }
    
    /**
     * @brief Get last error message
     * @return Error message
     */
    QString lastError() const { return m_lastError; }
    
    /**
     * @brief Get file format information
     */
    SParamFormat format() const { return m_format; }
    double frequencyMultiplier() const { return m_freqMultiplier; }
    
private:
    SParamData m_data;
    SParamFormat m_format;
    double m_freqMultiplier;
    QString m_lastError;
    
    bool parseOptionLine(const QString& line);
    bool parseDataLine(const QString& line, PortCount ports);
    Complex parseValue(double v1, double v2) const;
    
    PortCount detectPortCount(const QString& filename) const;
};

/**
 * @brief Touchstone file writer
 */
class TouchstoneWriter {
public:
    TouchstoneWriter();
    ~TouchstoneWriter() = default;
    
    /**
     * @brief Write S-parameter data to file
     * @param filename Output file path
     * @param data S-parameter data
     * @param format Output format (RI, MA, or DB)
     * @return true if successful
     */
    bool write(const QString& filename, 
               const SParamData& data,
               SParamFormat format = SParamFormat::RI);
    
    /**
     * @brief Get last error message
     */
    QString lastError() const { return m_lastError; }
    
private:
    QString m_lastError;
    
    QString formatValue(const Complex& s, SParamFormat format) const;
    QString frequencyUnitString(double multiplier) const;
};

} // namespace SmithTool

#endif // SMITHTOOL_TOUCHSTONE_H
