#include "svm_cell.h"

template <typename T>
SVMCell<T>::SVMCell(CellType type, T a, T b) : m_type(type)
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

template <typename T>
void SVMCell<T>::type(CellType type) { m_type = type; }
template <typename T>
void SVMCell<T>::value(T value) { m_value = value; }
template <typename T>
void SVMCell<T>::min(T min) { m_min = min; }
template <typename T>
void SVMCell<T>::max(T max) { m_max = max; }

template <typename T>
CellType SVMCell<T>::type() const { return m_type; }
template <typename T>
T SVMCell<T>::value() const { return m_value; }
template <typename T>
T SVMCell<T>::min() const { return m_min; }
template <typename T>
T SVMCell<T>::max() const { return m_max; }