#include "svm.h"

SVMCell::SVMCell(CellType type, float a, float b) : m_type(type)
{
    switch (m_type)
    {
    case CellType::New:
        m_value = a;
        break;
    case CellType::Original:
        m_value = a;
        break;
    case CellType::Inter2:
        m_min = a;
        m_max = b;
        break;
    case CellType::Inter4:
        m_min = a;
        m_max = b;
        break;
    default:
        break;
    }
}

void SVMCell::type(CellType type) { m_type = type; }
void SVMCell::value(float value) { m_value = value; }
void SVMCell::min(float min) { m_min = min; }
void SVMCell::max(float max) { m_max = max; }

CellType SVMCell::type() const { return m_type; }
float SVMCell::value() const { return m_value; }
float SVMCell::min() const { return m_min; }
float SVMCell::max() const { return m_max; }