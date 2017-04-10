#include <mesh/Data/Stl.h>
#include <mesh/Data/StlReader.h>

#include <mesh/Algorithm/FindIntersections.h>
#include <mesh/Algorithm/oclException.h>

#include <fstream>
#include <iostream>

int main(int argc, char ** argv)
{
    if (argc > 1)
    {
        mesh::Data::Stl stl;

        std::ifstream ifs(argv[1], std::ios::binary);
        ifs >> stl;

        std::cout << "Model read!" << std::endl;
        std::cout << "  Points: " << stl.getNumberPoints() << std::endl;
        std::cout << "  Triangles: " << stl.getNumberTriangles() << std::endl;

    }

    try {

      

    mesh::Data::Stl model;


    std::vector<mesh::Data::Point *> p_points;

    p_points.push_back(model.addPoint(0, 0, 10));
    p_points.push_back(model.addPoint(10, 0, 30));
    p_points.push_back(model.addPoint(0, 10, 20));
            
    p_points.push_back(model.addPoint(10, 10, 30));
    p_points.push_back(model.addPoint(0, 20, 20));
    p_points.push_back(model.addPoint(0, 0, 0));

    model.addTriangle(p_points[0], p_points[1], p_points[2]);
    model.addTriangle(p_points[3], p_points[4], p_points[5]);

    // Triangles intersect along the edge
    std::vector<mesh::Data::Point *> p_points1;
    p_points1.push_back(model.addPoint(1000, 1000, 1000));
    p_points1.push_back(model.addPoint(1000, 1020, 1000));
    p_points1.push_back(model.addPoint(1010, 1010, 1020));
    p_points1.push_back(model.addPoint(1005, 1010, 1010));

    model.addTriangle(p_points1[0], p_points1[1], p_points1[2]);
    model.addTriangle(p_points1[0], p_points1[3], p_points1[1]);

    // Intersecting triangles
    std::vector<mesh::Data::Point *> p_points2;
    p_points2.push_back(model.addPoint(2000, 2000, 2015));
    p_points2.push_back(model.addPoint(2100, 2000, 2010));
    p_points2.push_back(model.addPoint(2000, 2100, 2010));

    p_points2.push_back(model.addPoint(2020, 2020, 2050));
    p_points2.push_back(model.addPoint(2050, 2000, 2000));
    p_points2.push_back(model.addPoint(2000, 2050, 2000));

    model.addTriangle(p_points2[0], p_points2[1], p_points2[2]);
    model.addTriangle(p_points2[3], p_points2[4], p_points2[5]);



    size_t n = 0;
    // n = mesh::Algorithm::FindIntersections(model);


    mesh::Data::Stl model2;
    //std::ifstream ifs("E:\\Temp\\merge2.stl", std::ios::binary);
    std::ifstream ifs("E:\\Temp\\golf2.stl", std::ios::binary);
    if (!ifs.good())
        return 0;
    ifs >> model2;

    n = mesh::Algorithm::FindIntersections(model2);

    std::cout << "Found intersections: " << n << std::endl;

    }
    catch (mesh::ocl::BuildError & e)
    {
        std::cout << "OpenCL error: " << e.what() << std::endl
                  << e.log().c_str() << std::endl;
    }
    catch (mesh::ocl::Exception & e)
    {
        std::cout << "OpenCL error: " << e.what() << std::endl;
    }

    return 0;

}