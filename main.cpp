#include "loader.h"
//#include "scene.h"
#include "vector.h"
#include <iostream>

int main(int argc, char *argv[])
{
//  Raytracer::Loader loader;
//  loader.open(argv[1]);
//  Raytracer::Scene scene = loader.scene();

    Raytracer::Vector i(1,0,0), j(0,1,0);
    Raytracer::VecMult p(i, j);

    std::cout << Raytracer::Vector(p) << std::endl;
    std::cout << "5/0 = " << 5.0/0.0 << std::endl;
    std::cout << "0/0 = " << 0.0/0.0 << std::endl;
  
  return 0;
}
