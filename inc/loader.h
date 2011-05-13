#ifndef RAYTRACER_LOADER_H
#define RAYTRACER_LOADER_H

#include <string>
#include "exception.h"

namespace Raytracer
{
  class Scene;
  
  class Loader
  {
    public:
      void open(std::string file) throw(Exception);
//      Scene scene();
    private:
//      Scene mScene;
  };
}

#endif
