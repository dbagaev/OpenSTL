#include "FindIntersections.h"

#pragma warning (push)
#pragma warning (disable : 4996)
#include <CL/cl.hpp>
#pragma warning (pop)


#include <fstream>
#include <iostream>

#include "oclUtils.h"

namespace  {

cl::Program program;

void arrangeTriangleCoordinatesZ(std::vector<cl_double4> & p0, std::vector<cl_double4> & p1, std::vector<cl_double4> & p2)
{
    cl_int error;

    cl::Context & context = OpenSTL::ocl::Manager::getDefaultContext();

    // Allocate buffers and move data
    size_t point_size = sizeof(cl_double4);
    size_t num_triangles = p0.size();
    size_t buff_size = num_triangles * point_size;
    
    cl::Buffer b0(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size, p0.data(), &error);
    cl::Buffer b1(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size, p1.data(), &error);
    cl::Buffer b2(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size, p2.data(), &error);

    // Create kernel 
    cl::Kernel arrangePoints(program, "arrangeTriangleCoordinatesZ");

    arrangePoints.setArg(0, b0);
    arrangePoints.setArg(1, b1);
    arrangePoints.setArg(2, b2);

    cl::CommandQueue queue(context, OpenSTL::ocl::Manager::getDefaultGPUDevice());

    error = queue.enqueueWriteBuffer(b0, CL_TRUE, 0, buff_size, p0.data());
    error = queue.enqueueWriteBuffer(b1, CL_TRUE, 0, buff_size, p1.data());
    error = queue.enqueueWriteBuffer(b2, CL_TRUE, 0, buff_size, p2.data());

    error = queue.enqueueNDRangeKernel(arrangePoints, cl::NDRange(0), cl::NDRange(num_triangles));

    error = queue.enqueueReadBuffer(b0, CL_TRUE, 0, buff_size, p0.data());
    error = queue.enqueueReadBuffer(b1, CL_TRUE, 0, buff_size, p1.data());
    error = queue.enqueueReadBuffer(b2, CL_TRUE, 0, buff_size, p2.data());

}

void findIntersections(std::vector<cl_double4> & p00, std::vector<cl_double4> & p01, std::vector<cl_double4> & p02, 
                       std::vector<cl_double4> & p10, std::vector<cl_double4> & p11, std::vector<cl_double4> & p12,
                       std::vector<std::pair<size_t, size_t>> & result)
{  
    cl::Context & context = OpenSTL::ocl::Manager::getDefaultContext();

    // Allocate buffers and move data
    size_t point_size = sizeof(cl_double4);
    size_t num_triangles0 = p00.size();
    size_t num_triangles1 = p10.size();
    size_t buff_size0 = num_triangles0 * point_size;
    size_t buff_size1 = num_triangles1 * point_size;

    cl_int error;

    cl::Buffer b00(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size0, p00.data(), &error);
    cl::Buffer b01(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size0, p01.data(), &error);
    cl::Buffer b02(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size0, p02.data(), &error);

    cl::Buffer b10(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size1, p10.data(), &error);
    cl::Buffer b11(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size1, p11.data(), &error);
    cl::Buffer b12(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, buff_size1, p12.data(), &error);

    cl_uint res_sz = 0;
    cl::Buffer b_res_sz(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_uint), &res_sz, &error);

    // Create kernel 
    cl::Kernel kernel_findIntersections(program, "findTrianglesIntersections");

    kernel_findIntersections.setArg(0, b00);
    kernel_findIntersections.setArg(1, b01);
    kernel_findIntersections.setArg(2, b02);

    kernel_findIntersections.setArg(3, b10);
    kernel_findIntersections.setArg(4, b11);
    kernel_findIntersections.setArg(5, b12);

    kernel_findIntersections.setArg(6, b_res_sz);

    cl::CommandQueue queue(context, OpenSTL::ocl::Manager::getDefaultGPUDevice());

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
    // Set parameters and run algorithm
    std::vector<cl_double4> pts[3];
    auto p_end_tri = model.endTriangle();
    for (auto p_tri = model.beginTriangle(); p_tri != p_end_tri; ++p_tri)
      {
      for (size_t i = 0; i < 3; ++i)
        {
        cl_double4 pos;
        auto p_pt = p_tri->getPoint(i);
        pos.x = p_pt->position()[0];
        pos.y = p_pt->position()[1];
        pos.z = p_pt->position()[2];
        pos.w = 0;
        pts[i].push_back(pos);
        }
      }

    // program = OpenSTL::ocl::Manager::loadProgramFromSources("D:\\Development\\OpenSTL\\Source\\OpenSTL\\Algorithm\\ocl_kernels\\triangles.cl");
    program = OpenSTL::ocl::Manager::loadProgramFromSources("E:\\Development\\MyResearch\\OpenSTL\\Source\\OpenSTL\\Algorithm\\ocl_kernels\\triangles.cl");

    arrangeTriangleCoordinatesZ(pts[0], pts[1], pts[2]);

    std::vector<std::pair<size_t, size_t>> intersections;

    findIntersections(pts[0], pts[1], pts[2], pts[0], pts[1], pts[2], intersections);


  }
