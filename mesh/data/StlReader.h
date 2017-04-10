#pragma once

#include "Data.h"
#include "Stl.h"

#include <iostream>

namespace mesh {
namespace Data {

bool ReadStl(std::istream & is, Stl & o_stl);

}
}

__mesh_DATA_EXPORT std::istream & operator >> (std::istream & is, mesh::Data::Stl & o_stl);
