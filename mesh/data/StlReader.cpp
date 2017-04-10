#include "StlReader.h"

#include "Stl.h"

#include <cstring>

using namespace mesh::Data;

namespace mesh {

namespace Impl {

bool ReadASCIIStl(std::istream & is, Stl & o_stl)
{
    // TODO : write this

    char buffer[1024];
    is.getline(buffer, 1024);

    /*
    solid name
    facet normal ni nj nk
      outer loop
      vertex v1x v1y v1z
      vertex v2x v2y v2z
      vertex v3x v3y v3z
      endloop
    endfacet

    endsolid name
    */

    return false;
}

bool ReadBinaryStl(std::istream & is, Stl & o_stl)
{
    /*
    UINT8[80] – Header
    UINT32 – Number of triangles

    foreach triangle
        REAL32[3] – Normal vector
        REAL32[3] – Vertex 1
        REAL32[3] – Vertex 2
        REAL32[3] – Vertex 3
        UINT16 – Attribute byte count
    end
    */

#pragma pack(1)
    struct TriangleData
    {
        float normal[3];
        float tri0[3];
        float tri1[3];
        float tri2[3];
        short int attr;
    };

    if (sizeof(TriangleData) != 50)
        return false;

    is.ignore(75);
    if (is.eof())
        return false;

    unsigned long number_of_triangles;
    is.read(reinterpret_cast<char *>(&number_of_triangles), 4);
    if (is.eof())
        return false;

    for (unsigned long i = 0; i < number_of_triangles; ++i)
    { 
        TriangleData data;
        size_t data_sz = sizeof(TriangleData);
        is.read(reinterpret_cast<char *>(&data), data_sz);

        if (is.eof() || is.bad() || !is.good())
        {
            return false;
        }

        o_stl.addTriangle(
            o_stl.addPoint(data.tri0[0], data.tri0[1], data.tri0[2]),
            o_stl.addPoint(data.tri1[0], data.tri1[1], data.tri1[2]),
            o_stl.addPoint(data.tri2[0], data.tri2[1], data.tri2[2])
        );

        // TODO : process color
    }

    return true;
}

}

bool Data::ReadStl(std::istream & is, Stl & o_stl)
{
    char buffer[100];
    is.read(buffer, 5);
    if (is.eof())
        return false;

    buffer[5] = 0;
    if (strcmp("solid", buffer) == 0)
        return Impl::ReadASCIIStl(is, o_stl);
    else
        return Impl::ReadBinaryStl(is, o_stl);
}

}

std::istream & operator >> (std::istream & is, mesh::Data::Stl & o_stl)
{
    ReadStl(is, o_stl);
    return is;
}