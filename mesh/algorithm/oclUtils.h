#pragma once

#include "Algorithm.h"
#include "oclException.h"

#include <exception>
#include <string>

#pragma warning (push)
#pragma warning (disable : 4996)
#include <CL/cl.hpp>
#pragma warning (pop)


namespace mesh {

namespace ocl {


class Manager
{
public:
    static Manager & instance();
    static cl::Program loadProgramFromSources(std::string file_name);

    static cl::Device & getDefaultGPUDevice();
    static cl::Context & getDefaultContext();
};


}

}