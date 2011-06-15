#include "exception.h"

namespace Raytracer
{
  Exception::Exception(std::string what) throw(): mWhat(what) 
  {
  }
  
  const char* Exception::what() const throw()
  {
    return this->mWhat.c_str();
  }
  
  Exception::~Exception() throw()
  {
  }
}

