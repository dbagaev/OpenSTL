#include <OpenSTL/Data/Stl.h>
#include <OpenSTL/Data/StlReader.h>

#include <OpenSTL/Algorithm/FindIntersections.h>

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

    OpenSTL::Data::Stl model;


    std::vector<OpenSTL::Data::Point *> p_points;

    p_points.push_back(model.addPoint(0, 0, 10));
    p_points.push_back(model.addPoint(10, 0, 30));
    p_points.push_back(model.addPoint(0, 10, 20));
            
    p_points.push_back(model.addPoint(10, 10, 30));
    p_points.push_back(model.addPoint(0, 20, 20));
    p_points.push_back(model.addPoint(0, 0, 0));

    model.addTriangle(p_points[0], p_points[1], p_points[2]);
    model.addTriangle(p_points[3], p_points[4], p_points[5]);

    // Triangles intersect along the edge
    std::vector<OpenSTL::Data::Point *> p_points1;
    p_points1.push_back(model.addPoint(1000, 1000, 1000));
    p_points1.push_back(model.addPoint(1000, 1020, 1000));
    p_points1.push_back(model.addPoint(1000, 1010, 1020));
    p_points1.push_back(model.addPoint(1000, 1005, 1010));

    //model.addTriangle(p_points1[0], p_points1[1], p_points1[2]);
    //model.addTriangle(p_points1[0], p_points1[3], p_points1[1]);
    



    OpenSTL::Algorithm::FindIntersections(model);

}