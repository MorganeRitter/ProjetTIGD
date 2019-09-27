#ifndef SVM_H
#define SVM_H

enum CellType
{
    Original,
    New,
    Inter2,
    Inter4
};

class SVMCell
{
public:
    SVMCell(CellType t, float a, float b = 0);

    inline void type(CellType type);
    inline void value(float value);
    inline void min(float min);
    inline void max(float max);

    inline CellType type() const;
    inline float value() const;
    inline float min() const;
    inline float max() const;

private:
    CellType m_type;
    union {
        struct
        {
            float m_value;
        };
        struct
        {
            float m_min;
            float m_max;
        };
    };
};

#endif SVM_H