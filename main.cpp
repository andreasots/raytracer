#include <iostream>

#include "raytracer/vector.h"

#include "version.h"

int main(int argc, char *argv[])
{
    std::cout << "Raytracer " << AutoVersion::FULLVERSION_STRING << " build ";
    std::cout << AutoVersion::BUILD << " (" << AutoVersion::STATUS << ")";
    std::endl(std::cout);

    std::cout << "Libraries: " << std::endl;
    std::cout << "\tAllegro: >5.0.0 (no exact version information)"<<std::endl;

    return 0;
}
