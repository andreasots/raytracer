#include "exception.h"
#include "loader.h"
//#include "scene.h"
#include "vector.h"

#include <iostream>
#include <sstream>

#include <boost/version.hpp>
#include <boost/spirit/version.hpp>

std::string boost_version()
{
    std::stringstream ret;

    ret << BOOST_VERSION / 100000 << "." << BOOST_VERSION / 100 % 1000 << ".";
    ret << BOOST_VERSION % 100;

    return ret.str();
}

std::string spirit_version()
{
    std::stringstream ret;

    ret << std::hex;

    for(int i = 3; i >= 0; i--)
    {
        if((SPIRIT_VERSION >> i*4) & 0xF)
            ret << ((SPIRIT_VERSION >> i*4) & 0xF) << ".";
    }

    return ret.str().substr(0, ret.str().size()-1);
}

int main(int argc, char *argv[])
{
    std::cout << "Libraries:" << std::endl;
    std::cout << "\tBoost " << boost_version() << std::endl;
    std::cout << "\tBoost::Spirit " << spirit_version() << std::endl;
    std::cout << "\tSFML " << "(no version information)" << std::endl;

    if(argv[1]==NULL)
        throw Raytracer::Exception("No file was provided");
    std::cout << "Loading file " << argv[1] << std::endl;
    Raytracer::Loader loader;
    loader.open(argv[1]);
//    Raytracer::Scene scene = loader.scene();

    return 0;
}
