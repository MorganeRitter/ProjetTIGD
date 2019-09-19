#include <Common/Image.h>
#include <iostream>

using namespace LibTIM;
using namespace std;

int main(int argc, char *argv[])
{

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