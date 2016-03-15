#include "FindIntersections.h"

#include <CL/cl.hpp>

#include <fstream>

namespace OpenSTL { namespace Algorithm {

typedef std::pair<std::vector<char>, cl::Program::Sources> Sources;

void loadProgramSources(std::string file_name, Sources & sources)
{
    std::ifstream ifs(file_name);
    ifs.seekg(0, std::ios::end);
    size_t sz = ifs.tellg();
    sources.first.resize(sz + 1);
    ifs.seekg(0);
    ifs.read(sources.first.data(), sz);
    sources.first[sz] = 0;
    sources.second.clear();
    sources.second.push_back(std::make_pair(sources.first.data(), sources.first.size()));
}


void FindIntersections(Data::Stl & model)
{

    // Some test OpenCL code
    
    
    // Find appropriate OpenCL device
    std::vector<cl::Platform> platforms;
    std::vector<cl::Device> devices;
    
    cl::Platform::get(&platforms);
    
    for (auto & p : platforms)
    {
        p.getDevices(CL_DEVICE_TYPE_GPU, &devices);
    
        if (!devices.empty())
            break;
    }
    
    cl::Context context(devices);
    Sources source;
    loadProgramSources("triangles.cl", source);

    cl::Program program(context, source.second);
    cl_int error = program.build(devices);

    if (error != CL_SUCCESS)
      {
      std::string build_log;
      program.getBuildInfo(devices[0], CL_PROGRAM_BUILD_LOG, &build_log);

      return;
      //std::cout << build_log << std::endl;
      }


    // Set parameters and run algorithm
    std::vector<cl_float4> pts[3];
    auto p_end_tri = model.endTriangle();
    for (auto p_tri = model.beginTriangle(); p_tri != p_end_tri; ++p_tri)
      {
      for (size_t i = 0; i < 3; ++i)
        {
        cl_float4 pos;
        auto p_pt = p_tri->getPoint(i);
        pos.s0 = (float)p_pt->position()[0];
        pos.s1 = (float)p_pt->position()[1];
        pos.s2 = (float)p_pt->position()[2];
        pos.s3 = 0;
        pts[i].push_back(pos);
        }
      }

    size_t float4_size = sizeof(cl_float4);
    size_t buff_size = pts[0].size() * sizeof(cl_float4);
    cl::Buffer b0(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size, pts[0].data(), &error);
    cl::Buffer b1(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size, pts[1].data(), &error);
    cl::Buffer b2(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size, pts[2].data(), &error);

    // Create kernel 
    cl::Kernel arrangePoints(program, "arrangeTriangleCoordinatesZ");

    arrangePoints.setArg(0, b0);
    arrangePoints.setArg(1, b1);
    arrangePoints.setArg(2, b2);

    cl::CommandQueue queue(context, devices[0]);

    error = queue.enqueueWriteBuffer(b0, CL_TRUE, 0, buff_size, pts[0].data());
    error = queue.enqueueWriteBuffer(b1, CL_TRUE, 0, buff_size, pts[1].data());
    error = queue.enqueueWriteBuffer(b2, CL_TRUE, 0, buff_size, pts[2].data());

    error = queue.enqueueNDRangeKernel(arrangePoints, cl::NDRange(0), cl::NDRange(pts[0].size()));

    error = queue.enqueueReadBuffer(b0, CL_TRUE, 0, buff_size, pts[0].data());
    error = queue.enqueueReadBuffer(b1, CL_TRUE, 0, buff_size, pts[1].data());
    error = queue.enqueueReadBuffer(b2, CL_TRUE, 0, buff_size, pts[2].data());





  }

  
}  

}