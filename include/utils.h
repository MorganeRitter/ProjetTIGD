#ifndef UTILS_H
#define UTILS_H

static bool verbose = false;

#define VERBOSE(str) if(verbose) {std::cout << str ;}

template<class T>
const T& clamp( const T& v, const T& lo, const T& hi )
{
    assert( !(hi < lo) );
    return (v < lo) ? lo : (hi < v) ? hi : v;
}

#endif // UTILS_H
