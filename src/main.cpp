#include "loader.h"
//#include "scene.h"

#include <iostream>

int main(int argc, char *argv[])
{
  Raytracer::Loader loader;
  loader.open(argv[1]);
//  Raytracer::Scene scene = loader.scene();

    std::cout << "5/0 = " << 5.0/0.0 << std::endl;
    std::cout << "0/0 = " << 0.0/0.0 << std::endl;
  
  return 0;
}
