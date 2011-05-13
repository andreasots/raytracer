#include "exception.h"
#include "loader.h"
//#include "scene.h"
#include "vector.h"

#include <fstream>
#include <iostream>
#include <string>

#include <boost/regex.hpp>

#define CPPSTRTOD(x) strtod((x).str().c_str(), NULL)

namespace Raytracer
{
  void Loader::open(std::string file) throw(Exception)
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
        Vector disp(CPPSTRTOD(match[3]),CPPSTRTOD(match[4]),CPPSTRTOD(match[5]));
        Vector rot(CPPSTRTOD(match[6]),CPPSTRTOD(match[7]),CPPSTRTOD(match[8]));
        std::cout << "displacement: " << disp << std::endl;
        std::cout << "rotation: " << rot << std::endl;
      }
      std::cout << std::endl;
      std::getline(in, line);
    }
  }
  
/*  Scene Loader::scene()
  {
    return this->mScene();
  }*/
}
