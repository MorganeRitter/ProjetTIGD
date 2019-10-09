#ifndef SVM_CELL_H
#define SVM_CELL_H

#include <cstddef> // for size_t

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
    inline void level(std::size_t l);
    inline void parent(SVMCell<T> *parent);
    inline void zpar(SVMCell<T> *zpar);

    // getters
    inline CellType type() const;
    inline T value() const;
    inline T min() const;
    inline T max() const;
    inline bool visited() const;
    inline int posX() const;
    inline int posY() const;
    inline std::size_t level() const;
    inline SVMCell<T> *parent() const;
    inline SVMCell<T> *zpar() const;

private:
    // iage interpolation
    CellType m_type;     // type of the cell, determine if m_value or m_in/m_max is used
    bool m_visited;      // if the cell as been visited
    std::size_t m_level; // memorization of the level where the queue handled the face
    int m_x, m_y;        // cell coordinates in the SVMImage

    // tree generation
    SVMCell<T> *m_parent;
    SVMCell<T> *m_zpar;

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