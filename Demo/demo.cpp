#include <mesh/Data/TriangleSurface.h>
#include <mesh/Data/StlReader.h>

#include <mesh/Algorithm/FindIntersections.h>
#include <mesh/Algorithm/oclException.h>

#include <fstream>
#include <iostream>

int main(int argc, char ** argv)
{
    if (argc > 1)
    {
        mesh::Data::TriangleSurface surface;

        std::ifstream ifs(argv[1], std::ios::binary);
        ifs >> surface;

        std::cout << "Model read!" << std::endl;
        std::cout << "  Points: " << surface.getNumberPoints() << std::endl;
        std::cout << "  Triangles: " << surface.getNumberTriangles() << std::endl;

    }

    try {

      

    mesh::Data::TriangleSurface surface;


    std::vector<mesh::Data::Point *> p_points;

    p_points.push_back(surface.addPoint(0, 0, 10));
    p_points.push_back(surface.addPoint(10, 0, 30));
    p_points.push_back(surface.addPoint(0, 10, 20));
            
    p_points.push_back(surface.addPoint(10, 10, 30));
    p_points.push_back(surface.addPoint(0, 20, 20));
    p_points.push_back(surface.addPoint(0, 0, 0));

    surface.addTriangle(p_points[0], p_points[1], p_points[2]);
    surface.addTriangle(p_points[3], p_points[4], p_points[5]);

    // Triangles intersect along the edge
    std::vector<mesh::Data::Point *> p_points1;
    p_points1.push_back(surface.addPoint(1000, 1000, 1000));
    p_points1.push_back(surface.addPoint(1000, 1020, 1000));
    p_points1.push_back(surface.addPoint(1010, 1010, 1020));
    p_points1.push_back(surface.addPoint(1005, 1010, 1010));

    surface.addTriangle(p_points1[0], p_points1[1], p_points1[2]);
    surface.addTriangle(p_points1[0], p_points1[3], p_points1[1]);

    // Intersecting triangles
    std::vector<mesh::Data::Point *> p_points2;
    p_points2.push_back(surface.addPoint(2000, 2000, 2015));
    p_points2.push_back(surface.addPoint(2100, 2000, 2010));
    p_points2.push_back(surface.addPoint(2000, 2100, 2010));

    p_points2.push_back(surface.addPoint(2020, 2020, 2050));
    p_points2.push_back(surface.addPoint(2050, 2000, 2000));
    p_points2.push_back(surface.addPoint(2000, 2050, 2000));

    surface.addTriangle(p_points2[0], p_points2[1], p_points2[2]);
    surface.addTriangle(p_points2[3], p_points2[4], p_points2[5]);



    size_t n = 0;
    // n = mesh::Algorithm::FindIntersections(model);


    mesh::Data::TriangleSurface surface2;
    //std::ifstream ifs("E:\\Temp\\merge2.stl", std::ios::binary);
    std::ifstream ifs("E:\\Temp\\golf2.stl", std::ios::binary);
    if (!ifs.good())
        return 0;
    ifs >> surface2;

    n = mesh::Algorithm::FindIntersections(surface2);

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