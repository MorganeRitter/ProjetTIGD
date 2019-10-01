#ifndef SVM_CELL_H
#define SVM_CELL_H

enum CellType
{
    Original,
    New,
    Inter2,
    Inter4
};

template <typename T>
class SVMCell
{
public:
    SVMCell(CellType t, T a, T b = 0);

    inline void type(CellType type);
    inline void value(T value);
    inline void min(T min);
    inline void max(T max);
    inline void visited(bool visited);
    inline void posX(int x);
    inline void posY(int y);

    inline CellType type() const;
    inline T value() const;
    inline T min() const;
    inline T max() const;
    inline bool visited() const;
    inline int posX() const;
    inline int posY() const;

private:
    CellType m_type;
    bool m_visited;
    int m_x, m_y;
    union {
        struct
        {
            T m_value;
        };
        struct
        {
            T m_min;
            T m_max;
        };
    };
};

#endif // SVM_CELL_H