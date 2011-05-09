#include "exception.h"
#include "loader.h"
//#include "scene.h"

#include <fstream>
#include <iostream>
#include <string>

#include <boost/regex.hpp>

namespace Raytracer
{
  void Loader::open(std::string file)
  {
    std::ifstream in(file.c_str());
    
    std::string line;
    std::getline(in, line);
    if(line != "raytracer")
      throw Exception("\""+file+"\" is not a scene file");
    boost::regex re("^(\\l*) \"(.*)\", \\(([-]?\\d*\\.?\\d*), ([-]?\\d*\\.?\\d*), ([-]?\\d*\\.?\\d*)\\), \\(([-]?\\d*\\.?\\d*), ([-]?\\d*\\.?\\d*), ([-]?\\d*\\.?\\d*)\\);$");
    
    std::getline(in, line);
    while(!!in)
    {
      boost::smatch match;
      if(!boost::regex_match(line, match, re))
        throw Exception("Malformed line in file \""+file+"\": '"+line+"'");
      if(match[1] == "file")
      {
        std::cout << "loading file " << match[2] << std::endl;
        std::cout << "displacement: (" << match[3] << ", " << match[4] << ", " << match[5] << ")" << std::endl;
        std::cout << "rotation: (" << match[6] << ", " << match[7] << ", " << match[8] << ")" << std::endl;
      }
      std::cout << std::endl;
      std::getline(in, line);
    }
  }
  
/*  Scene Loader::scene()
  {
    return this->mScene();
  }*/
};
