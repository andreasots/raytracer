#include "exception.h"
#include "loader.h"
#include "point.h"
#include "vector.h"
#include "mesh.h"
//#include "scene.h"

#include <fstream>
#include <string>

#include <assimp/assimp.hpp>

#include <boost/spirit/include/qi.hpp>
using boost::spirit::qi::phrase_parse;
using boost::spirit::ascii::space;

namespace Raytracer
{
    void Loader::open(std::string file) throw(Exception)
    {
        std::ifstream in(file.c_str(), std::ios::binary | std::ios::in);

        while(!in.eof())
        {
            std::string name, link;
            unsigned size=0, type=0;

            char *buf;
            buf = new char[101];
            in.get(buf, 101);
            name = buf;
            delete[] buf;

            // Mode
            in.ignore(8);
            // Owner
            in.ignore(8);
            // Group
            in.ignore(8);

            buf = new char[13];
            in.get(buf, 13);
            for(int i = 0; i < 13 && buf[i]; i++)
                size = size*8+buf[i]-'0';
            delete[] buf;

            // Last modification
            in.ignore(12);
            // Header checksum
            in.ignore(8);

            buf = new char[2];
            in.get(buf, 2);
            type = buf[0];
            delete[] buf;

            buf = new char[101];
            in.get(buf, 101);
            link = buf;
            delete[] buf;

            in.ignore(512-100-8-8-8-12-12-8-1-100);
            int blocks = size/512;
            if(size > blocks*512)
                blocks++;
            if(size > 0)
            {
                std::cout << "File: " << name << std::endl;
                std::cout << "Size: " << size << " bytes (" << blocks << " blocks)" << std::endl;
                std::cout << "Link name: " << link << std::endl;
                std::cout << std::endl;
            }
            in.ignore(blocks*512);
        }
    }
}
