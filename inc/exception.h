#ifndef RAYTRACER_EXCEPTION_H
#define RAYTRACER_EXCEPTION_H
#include <exception>
#include <string>

namespace Raytracer
{
  class Exception: public std::exception
  {
    public:
      Exception(std::string what) throw();
      const char* what() const throw();
      virtual ~Exception() throw();
    private:
      std::string mWhat;
  };
};
#endif
