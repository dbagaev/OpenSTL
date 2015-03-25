#pragma once

#include "Data.h"
#include "Stl.h"

#include <iostream>

namespace OpenSTL {
namespace Data {

bool ReadStl(std::istream & is, Stl & o_stl);

}
}

__OPENSTL_DATA_EXPORT std::istream & operator >> (std::istream & is, OpenSTL::Data::Stl & o_stl);