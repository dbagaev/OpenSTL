#include <OpenSTL/Data/Stl.h>
#include <OpenSTL/Data/StlReader.h>

#include <fstream>
#include <iostream>

int main(int argc, char ** argv)
{
    if (argc > 1)
    {
        OpenSTL::Data::Stl stl;

        std::ifstream ifs(argv[1], std::ios::binary);
        ifs >> stl;

        std::cout << "Model read!" << std::endl;
        std::cout << "  Points: " << stl.getNumberPoints() << std::endl;
        std::cout << "  Triangles: " << stl.getNumberTriangles() << std::endl;

    }
}