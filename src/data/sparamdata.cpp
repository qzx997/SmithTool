/**
 * @file sparamdata.cpp
 * @brief S-parameter data implementation
 */

#include "sparamdata.h"
#include <algorithm>
#include <cmath>

namespace SmithTool {

SParamData::SParamData()
    : m_z0(50.0)
    , m_portCount(PortCount::OnePort)
{
}

const SParamPoint& SParamData::point(int index) const
{
    static SParamPoint empty;
    if (index < 0 || index >= m_points.size()) {
        return empty;
    }
    return m_points[index];
}

SParamPoint& SParamData::point(int index)
{
    static SParamPoint empty;
    if (index < 0 || index >= m_points.size()) {
        return empty;
    }
    return m_points[index];
}

double SParamData::minFrequency() const
{
    if (m_points.isEmpty()) return 0.0;
    
    double minF = m_points[0].frequency;
    for (const auto& p : m_points) {
        if (p.frequency < minF) minF = p.frequency;
    }
    return minF;
}

double SParamData::maxFrequency() const
{
    if (m_points.isEmpty()) return 0.0;
    
    double maxF = m_points[0].frequency;
    for (const auto& p : m_points) {
        if (p.frequency > maxF) maxF = p.frequency;
    }
    return maxF;
}

int SParamData::closestIndex(double freq) const
{
    if (m_points.isEmpty()) return -1;
    
    int closest = 0;
    double minDiff = std::abs(m_points[0].frequency - freq);
    
    for (int i = 1; i < m_points.size(); ++i) {
        double diff = std::abs(m_points[i].frequency - freq);
        if (diff < minDiff) {
            minDiff = diff;
            closest = i;
        }
    }
    return closest;
}

Complex SParamData::interpolate(double freq, int idx1, int idx2,
                                 const Complex& v1, const Complex& v2) const
{
    if (idx1 == idx2) return v1;
    
    double f1 = m_points[idx1].frequency;
    double f2 = m_points[idx2].frequency;
    
    if (std::abs(f2 - f1) < 1e-12) return v1;
    
    double t = (freq - f1) / (f2 - f1);
    return v1 + t * (v2 - v1);
}

Complex SParamData::s11At(double freq) const
{
    if (m_points.isEmpty()) return Complex(0, 0);
    if (m_points.size() == 1) return m_points[0].s11;
    
    // Find surrounding indices
    int idx = closestIndex(freq);
    
    if (m_points[idx].frequency >= freq && idx > 0) {
        return interpolate(freq, idx - 1, idx,
                          m_points[idx - 1].s11, m_points[idx].s11);
    } else if (idx < m_points.size() - 1) {
        return interpolate(freq, idx, idx + 1,
                          m_points[idx].s11, m_points[idx + 1].s11);
    }
    return m_points[idx].s11;
}

Complex SParamData::s21At(double freq) const
{
    if (m_points.isEmpty()) return Complex(0, 0);
    int idx = closestIndex(freq);
    return m_points[idx].s21;
}

Complex SParamData::s12At(double freq) const
{
    if (m_points.isEmpty()) return Complex(0, 0);
    int idx = closestIndex(freq);
    return m_points[idx].s12;
}

Complex SParamData::s22At(double freq) const
{
    if (m_points.isEmpty()) return Complex(0, 0);
    int idx = closestIndex(freq);
    return m_points[idx].s22;
}

void SParamData::addPoint(const SParamPoint& point)
{
    m_points.append(point);
}

void SParamData::clear()
{
    m_points.clear();
}

void SParamData::sortByFrequency()
{
    std::sort(m_points.begin(), m_points.end(),
              [](const SParamPoint& a, const SParamPoint& b) {
                  return a.frequency < b.frequency;
              });
}

QVector<double> SParamData::frequencies() const
{
    QVector<double> freqs;
    freqs.reserve(m_points.size());
    for (const auto& p : m_points) {
        freqs.append(p.frequency);
    }
    return freqs;
}

QVector<Complex> SParamData::s11Vector() const
{
    QVector<Complex> s11s;
    s11s.reserve(m_points.size());
    for (const auto& p : m_points) {
        s11s.append(p.s11);
    }
    return s11s;
}

} // namespace SmithTool
