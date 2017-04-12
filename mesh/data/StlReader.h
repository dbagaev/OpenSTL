#pragma once

#include "Data.h"
#include "TriangleSurface.h"

#include <iostream>

namespace mesh {
namespace Data {

bool ReadStl(std::istream & is, TriangleSurface & o_stl);

}
}

__mesh_DATA_EXPORT std::istream & operator >> (std::istream & is, mesh::Data::TriangleSurface & o_stl);
