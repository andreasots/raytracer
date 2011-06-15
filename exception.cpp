#include "exception.h"

namespace Raytracer
{
    Exception::Exception(std::string str) throw(): mWhat(str)
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

