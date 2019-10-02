#ifndef SVM_CELL_H
#define SVM_CELL_H

enum CellType
{
    Original,
    New,
    Inter2,
    Inter4
};

/* A SVMCell represent a Set Value Map Cell, which compose a Set Value Map*/
template <typename T>
class SVMCell
{
public:
    SVMCell(CellType t, T a, T b = 0);

    // setters
    inline void type(CellType type);
    inline void value(T value);
    inline void min(T min);
    inline void max(T max);
    inline void visited(bool visited);
    inline void posX(int x);
    inline void posY(int y);

    // getters
    inline CellType type() const;
    inline T value() const;
    inline T min() const;
    inline T max() const;
    inline bool visited() const;
    inline int posX() const;
    inline int posY() const;

private:
    CellType m_type; // type of the cell, determine if m_value or m_in/m_max is used
    bool m_visited;  // if the cell as been visited (see tos generation algorithm)
    int m_x, m_y;    // cell coordinates in the SVMImage
    union {
        struct
        {
            T m_value; // the value of the cell, for Original and New type
        };
        struct
        {
            T m_min; // the value range, for Inter2/4 type
            T m_max;
        };
    };
};

#endif // SVM_CELL_H