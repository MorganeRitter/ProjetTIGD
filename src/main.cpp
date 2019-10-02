#include "pqueue.h"
#include "svm_cell.h"
#include <Common/Image.h>
#include <iostream>

using namespace LibTIM;
using namespace std;

int main(int argc, char *argv[])
{

    PQueue<int> pq;

    pq.push(new SVMCell<int>(CellType::Original, 1), 2);
    pq.push(new SVMCell<int>(CellType::Original, 3), 0);
    pq.push(new SVMCell<int>(CellType::Original, 4), 0);
    pq.pop(0);

    std::cout << pq << " | " << (pq.empty() == true ? "true" : "false") << std::endl;

    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <image.pgm> \n";
        exit(1);
    }
    // Image est une classe générique paramétrée par le type des points contenus dans l'image
    Image<unsigned char> im;
    if (Image<U8>::load(argv[1], im))
        std::cout << "Great, image is loaded\n";
    else
        return 1;
}