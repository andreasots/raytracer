#include "loader.h"
//#include "scene.h"

int main(int argc, char *argv[])
{
  Raytracer::Loader loader;
  loader.open(argv[1]);
//  Raytracer::Scene scene = loader.scene();
    
  return 0;
}
