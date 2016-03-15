#include "FindIntersections.h"

#include <CL/cl.hpp>

#include <fstream>

namespace  {

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

cl::Context g_context;
cl::Program g_program;

std::vector<cl::Device> g_devices;
std::vector<cl::Platform> g_platforms;

bool initOpenCL()
{
    // Find appropriate OpenCL device

    cl::Platform::get(&g_platforms);

    for (auto & p : g_platforms)
    {
        p.getDevices(CL_DEVICE_TYPE_GPU, &g_devices);

        if (!g_devices.empty())
            break;
    }

    if (g_devices.empty())
        return false;

    cl::Context context(g_devices);
    Sources source;
    loadProgramSources("D:\\Development\\OpenSTL\\Source\\OpenSTL\\Algorithm\\ocl_kernels\\triangles.cl", source);

    cl::Program program(context, source.second);
    cl_int error = program.build(g_devices);

    if (error != CL_SUCCESS)
    {
        std::string build_log;
        program.getBuildInfo(g_devices[0], CL_PROGRAM_BUILD_LOG, &build_log);

        return false;
        //std::cout << build_log << std::endl;
    }

    g_context = context;
    g_program = program;

    return true;
}

void arrangeTriangleCoordinatesZ(std::vector<cl_float4> & p0, std::vector<cl_float4> & p1, std::vector<cl_float4> & p2)
{
    cl_int error;

    // Allocate buffers and move data
    size_t point_size = sizeof(cl_float4);
    size_t num_triangles = p0.size();
    size_t buff_size = num_triangles * point_size;
    
    cl::Buffer b0(g_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size, p0.data(), &error);
    cl::Buffer b1(g_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size, p1.data(), &error);
    cl::Buffer b2(g_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size, p2.data(), &error);

    // Create kernel 
    cl::Kernel arrangePoints(g_program, "arrangeTriangleCoordinatesZ");

    arrangePoints.setArg(0, b0);
    arrangePoints.setArg(1, b1);
    arrangePoints.setArg(2, b2);

    cl::CommandQueue queue(g_context, g_devices[0]);

    error = queue.enqueueWriteBuffer(b0, CL_TRUE, 0, buff_size, p0.data());
    error = queue.enqueueWriteBuffer(b1, CL_TRUE, 0, buff_size, p1.data());
    error = queue.enqueueWriteBuffer(b2, CL_TRUE, 0, buff_size, p2.data());

    error = queue.enqueueNDRangeKernel(arrangePoints, cl::NDRange(0), cl::NDRange(num_triangles));

    error = queue.enqueueReadBuffer(b0, CL_TRUE, 0, buff_size, p0.data());
    error = queue.enqueueReadBuffer(b1, CL_TRUE, 0, buff_size, p1.data());
    error = queue.enqueueReadBuffer(b2, CL_TRUE, 0, buff_size, p2.data());

}

void findIntersections(std::vector<cl_float4> & p00, std::vector<cl_float4> & p01, std::vector<cl_float4> & p02, 
                       std::vector<cl_float4> & p10, std::vector<cl_float4> & p11, std::vector<cl_float4> & p12,
                       std::vector<std::pair<size_t, size_t>> & result)
{  
    // Allocate buffers and move data
    size_t point_size = sizeof(cl_float4);
    size_t num_triangles0 = p00.size();
    size_t num_triangles1 = p10.size();
    size_t buff_size0 = num_triangles0 * point_size;
    size_t buff_size1 = num_triangles1 * point_size;

    cl_int error;

    cl::Buffer b00(g_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size0, p00.data(), &error);
    cl::Buffer b01(g_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size0, p01.data(), &error);
    cl::Buffer b02(g_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size0, p02.data(), &error);

    cl::Buffer b10(g_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size1, p10.data(), &error);
    cl::Buffer b11(g_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size1, p11.data(), &error);
    cl::Buffer b12(g_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size1, p12.data(), &error);

    cl_uint res_sz = 0;
    cl::Buffer b_res_sz(g_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_uint), &res_sz, &error);

    // Create kernel 
    cl::Kernel kernel_findIntersections(g_program, "findTrianglesIntersections");

    kernel_findIntersections.setArg(0, b00);
    kernel_findIntersections.setArg(1, b01);
    kernel_findIntersections.setArg(2, b02);

    kernel_findIntersections.setArg(3, b10);
    kernel_findIntersections.setArg(4, b11);
    kernel_findIntersections.setArg(5, b12);

    kernel_findIntersections.setArg(6, b_res_sz);

    cl::CommandQueue queue(g_context, g_devices[0]);

    error = queue.enqueueWriteBuffer(b00, CL_TRUE, 0, buff_size0, p00.data());
    error = queue.enqueueWriteBuffer(b01, CL_TRUE, 0, buff_size0, p01.data());
    error = queue.enqueueWriteBuffer(b02, CL_TRUE, 0, buff_size0, p02.data());

    error = queue.enqueueWriteBuffer(b10, CL_TRUE, 0, buff_size1, p10.data());
    error = queue.enqueueWriteBuffer(b11, CL_TRUE, 0, buff_size1, p11.data());
    error = queue.enqueueWriteBuffer(b12, CL_TRUE, 0, buff_size1, p12.data());

    error = queue.enqueueWriteBuffer(b_res_sz, CL_TRUE, 0, sizeof(cl_uint), &res_sz);


    error = queue.enqueueNDRangeKernel(kernel_findIntersections, cl::NDRange(0), cl::NDRange(num_triangles0, num_triangles1));

    //error = queue.enqueueReadBuffer(b0, CL_TRUE, 0, buff_size, p0.data());
    //error = queue.enqueueReadBuffer(b1, CL_TRUE, 0, buff_size, p1.data());
    //error = queue.enqueueReadBuffer(b2, CL_TRUE, 0, buff_size, p2.data());

    error = queue.enqueueReadBuffer(b_res_sz, CL_TRUE, 0, sizeof(cl_uint), &res_sz);


}

}

void OpenSTL::Algorithm::FindIntersections(Data::Stl & model)
{

    if (!initOpenCL())
        return;

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

    arrangeTriangleCoordinatesZ(pts[0], pts[1], pts[2]);

    std::vector<std::pair<size_t, size_t>> intersections;

    findIntersections(pts[0], pts[1], pts[2], pts[0], pts[1], pts[2], intersections);


    g_devices.clear();
    g_platforms.clear();

  }

  