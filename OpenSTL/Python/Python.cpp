#include "Python.h"

#include <Math/Vector3d.h>

#include <boost/python.hpp>

char const * test()
{
    return "Hello from OpenSTL";
}

using namespace boost::python;

BOOST_PYTHON_MODULE(OpenSTL)
{
    def("test", test);
}



