#include "FindIntersections.h"

#pragma warning (push)
#pragma warning (disable : 4996)
#include <CL/cl.hpp>
#pragma warning (pop)

#include "omp.h"

#include <fstream>
#include <algorithm>
#include <iostream>
#include <chrono>

#include "oclUtils.h"

size_t nocl_findTrianglesIntersections(std::vector<cl_float4> p00, std::vector<cl_float4> p01, std::vector<cl_float4> p02,
  std::vector<cl_float4> p10, std::vector<cl_float4> p11, std::vector<cl_float4> p12);


namespace  {

cl::Program program;

void arrangeTriangleCoordinatesZ(std::vector<cl_float4> & p0, std::vector<cl_float4> & p1, std::vector<cl_float4> & p2)
{
    cl_int error;

    cl::Context & context = mesh::ocl::Manager::getDefaultContext();

    // Allocate buffers and move data
    size_t point_size = sizeof(cl_float4);
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

    cl::CommandQueue queue(context, mesh::ocl::Manager::getDefaultGPUDevice());

    error = queue.enqueueWriteBuffer(b0, CL_TRUE, 0, buff_size, p0.data());
    error = queue.enqueueWriteBuffer(b1, CL_TRUE, 0, buff_size, p1.data());
    error = queue.enqueueWriteBuffer(b2, CL_TRUE, 0, buff_size, p2.data());

    error = queue.enqueueNDRangeKernel(arrangePoints, cl::NDRange(0), cl::NDRange(num_triangles));

    error = queue.enqueueReadBuffer(b0, CL_TRUE, 0, buff_size, p0.data());
    error = queue.enqueueReadBuffer(b1, CL_TRUE, 0, buff_size, p1.data());
    error = queue.enqueueReadBuffer(b2, CL_TRUE, 0, buff_size, p2.data());

}

size_t findIntersections(std::vector<cl_float4> & p00, std::vector<cl_float4> & p01, std::vector<cl_float4> & p02, 
                         std::vector<cl_float4> & p10, std::vector<cl_float4> & p11, std::vector<cl_float4> & p12,
                         std::vector<std::pair<size_t, size_t>> & result)
{  
    cl::Context & context = mesh::ocl::Manager::getDefaultContext();

    // Allocate buffers and move data
    size_t point_size = sizeof(cl_float4);
    size_t num_triangles0 = p00.size();
    size_t num_triangles1 = p10.size();

    size_t group_size0 = 128;
    size_t group_size1 = 1;

    num_triangles0 += 128 - num_triangles0 % group_size0;
    num_triangles1 += 128 - num_triangles1 % group_size1;

    static cl_float4 t;
    t.x = std::max(p02.back().x, p12.back().x) + 1;
    t.y = std::max(p02.back().y, p12.back().y) + 1;
    t.z = std::max(p02.back().z, p12.back().z) + 1;
    t.w = 0;

    while (p00.size() < num_triangles0)
    {
        p00.push_back(t);
        p01.push_back(t);
        p02.push_back(t);
    }

    while (p10.size() < num_triangles1)
    {
        p10.push_back(t);
        p11.push_back(t);
        p12.push_back(t);
    }

    size_t buff_size0 = num_triangles0 * point_size;
    size_t buff_size1 = num_triangles1 * point_size;

    cl_int error;

    auto time_start = std::chrono::steady_clock::now();

    cl::Buffer b00(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, buff_size0, p00.data(), &error);
    cl::Buffer b01(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, buff_size0, p01.data(), &error);
    cl::Buffer b02(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, buff_size0, p02.data(), &error);

    cl::Buffer b10(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, buff_size1, p10.data(), &error);
    cl::Buffer b11(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, buff_size1, p11.data(), &error);
    cl::Buffer b12(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, buff_size1, p12.data(), &error);

    cl_uint res_sz = 0;
    size_t total_count = 0;
    cl::Buffer b_res_sz(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_uint), &res_sz, &error);

    auto time_buff = std::chrono::steady_clock::now();

    // Create kernel 
    cl::Kernel kernel_findIntersections(program, "findTrianglesIntersections");

    auto time_kernel = std::chrono::steady_clock::now();

    cl::CommandQueue queue(context, mesh::ocl::Manager::getDefaultGPUDevice());

    error = queue.enqueueWriteBuffer(b00, CL_TRUE, 0, buff_size0, p00.data());
    error = queue.enqueueWriteBuffer(b01, CL_TRUE, 0, buff_size0, p01.data());
    error = queue.enqueueWriteBuffer(b02, CL_TRUE, 0, buff_size0, p02.data());

    error = queue.enqueueWriteBuffer(b10, CL_TRUE, 0, buff_size1, p10.data());
    error = queue.enqueueWriteBuffer(b11, CL_TRUE, 0, buff_size1, p11.data());
    error = queue.enqueueWriteBuffer(b12, CL_TRUE, 0, buff_size1, p12.data());

    error = queue.enqueueWriteBuffer(b_res_sz, CL_TRUE, 0, sizeof(cl_uint), &res_sz);

    error = queue.finish();

    auto time_copy = std::chrono::steady_clock::now();

    size_t sub_size = 8192;
    for (size_t i = 0; i < num_triangles0 / sub_size + 1; ++i)
    {
        size_t offs0 = i * sub_size;
        size_t size0 = (i + 1) * sub_size <= num_triangles0 ? sub_size : num_triangles0 % sub_size;

        for (size_t j = 0; j < num_triangles1 / sub_size + 1; ++j)
        {
            size_t offs1 = j * sub_size;
            size_t size1 = (j + 1) * sub_size <= num_triangles1 ? sub_size : num_triangles1 % sub_size;

            if (p12[offs1 + size1 - 1].z < p00[offs0].z)
                continue;

            if (p10[offs1].z > p02[offs0 + size0 - 1].z)
                continue;

            error = kernel_findIntersections.setArg(0, b00);
            error = kernel_findIntersections.setArg(1, b01);
            error = kernel_findIntersections.setArg(2, b02);

            error = kernel_findIntersections.setArg(3, b10);
            error = kernel_findIntersections.setArg(4, b11);
            error = kernel_findIntersections.setArg(5, b12);

            error = kernel_findIntersections.setArg(6, b_res_sz);

            res_sz = 0;
            error = queue.enqueueWriteBuffer(b_res_sz, CL_TRUE, 0, sizeof(cl_uint), &res_sz);

            error = queue.enqueueNDRangeKernel(kernel_findIntersections, 
                cl::NDRange(offs0, offs1), 
                cl::NDRange(size0, size1),
                cl::NDRange(group_size0, group_size1));
            // error = queue.enqueueNDRangeKernel(kernel_findIntersections, cl::NDRange(0), cl::NDRange(num_triangles0, num_triangles1), cl::NDRange(group_size0, group_size1));
            error = queue.finish();

            error = queue.enqueueReadBuffer(b_res_sz, CL_TRUE, 0, sizeof(cl_uint), &res_sz);
            total_count += res_sz;
        }
    }

    auto time_calculations = std::chrono::steady_clock::now();


    auto time_end = std::chrono::steady_clock::now();

    std::cout << "Setup:         " << (time_buff - time_start).count() << std::endl;
    std::cout << "Create kernel: " << (time_kernel - time_buff).count() << std::endl;
    std::cout << "Copy memory:   " << (time_copy - time_kernel).count() << std::endl;
    std::cout << "Calcualtions:  " << (time_calculations - time_copy).count() << std::endl;
    std::cout << "Finalize:      " << (time_end - time_calculations).count() << std::endl;

    return total_count;
}

}

size_t mesh::Algorithm::FindIntersections(Data::TriangleSurface & surface)
{
    // Set parameters and run algorithm
    std::vector<cl_float4> pts[3];
    auto p_end_tri = surface.endTriangle();

    typedef std::pair<mesh::Data::Triangle *, double> tri_m_t;
    std::vector<tri_m_t> triangles;
    triangles.reserve(surface.getNumberTriangles());
    for (auto p_tri = surface.beginTriangle(); p_tri != p_end_tri; ++p_tri)
    {
        const double m = std::min(p_tri->getPoint(0)->position()[2], std::min(p_tri->getPoint(1)->position()[2], p_tri->getPoint(2)->position()[2]));
        triangles.push_back(std::make_pair(&*p_tri, m));
    }

    std::stable_sort(triangles.begin(), triangles.end(), [](const tri_m_t & t1, const tri_m_t & t2)
    {
        return (t1.second < t2.second);
    });

    for (auto & p_tri : triangles)
    {
        for (size_t i = 0; i < 3; ++i)
        {
            cl_float4 pos;
            auto p_pt = p_tri.first->getPoint(i);
            pos.x = p_pt->position()[0];
            pos.y = p_pt->position()[1];
            pos.z = p_pt->position()[2];
            pos.w = 0;
            pts[i].push_back(pos);
        }
    }

    program = mesh::ocl::Manager::loadProgramFromSources("D:\\Development\\mesh\\Source\\mesh\\Algorithm\\ocl_kernels\\triangles.cl");
    // program = mesh::ocl::Manager::loadProgramFromSources("E:\\Development\\MyResearch\\mesh\\Source\\mesh\\Algorithm\\ocl_kernels\\triangles.cl");

    arrangeTriangleCoordinatesZ(pts[0], pts[1], pts[2]);

    std::vector<std::pair<size_t, size_t>> intersections;

    auto time_start = std::chrono::steady_clock::now();

    auto res = findIntersections(pts[0], pts[1], pts[2], pts[0], pts[1], pts[2], intersections);
    // auto res =  nocl_findTrianglesIntersections(pts[0], pts[1], pts[2], pts[0], pts[1], pts[2]);

    auto time_end = std::chrono::steady_clock::now();

    std::cout << "Calculating: " << (time_end - time_start).count() << std::endl;

    return res;


  }




typedef cl_float4 double4;

double dot(cl_float4 a, cl_float4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

cl_float4 cross(cl_float4 a, cl_float4 b)
{
    cl_float4 r;
  
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    r.w = 0;
  
    return r;
}

bool all(bool a) { return a; }

cl_float4 operator-(cl_float4 a, cl_float4 b)
{
    cl_float4 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    r.w = a.w - b.w;
    return r;
}

bool operator==(cl_float4 a, cl_float4 b)
{
    return (a.x == b.x && a.y == b.y && a.z == b.z);
}



// Returns if triangles intersect while sharing one edge
// First two points of each triangle are shared edge, the last one - not
bool doTrianglesIntersect_EdgeShared(const double4 p00, const double4 p01, const double4 p02,
                                     const double4 p10, const double4 p11, const double4 p12)
{
    double4 e1 = p01 - p00;
    double4 e2 = p02 - p00;
    double4 e3 = p12 - p00;
  
    double4 t1 = cross(e1, e2);
    double d = dot(e3, t1);
  
    if (d != 0)
        return false;
  
    double4 t2 = cross(e1, e3);
  
    return (dot(t1, t2) > 0);
}

// Returns if triangles intersect while sharing one point
// First points of each triangle is shared, the last two - not
bool doTrianglesIntersect_PointShared(const double4 p00, const double4 p01, const double4 p02,
                                      const double4 p10, const double4 p11, const double4 p12)
{
    return false;
}

double calculateT(double p0, double p1, double d0, double d1)
{
    const double a_d0 = fabs(d0);
    const double a_d1 = fabs(d1);
    return (a_d0*p1 + a_d1*p0) / (a_d0 + a_d1);
}

void calculateIntersectionInterval(double p0, double p1, double p2, double d0, double d1, double d2, double * t1, double * t2)
{
    double tt1, tt2;
  
    if (d0 == 0)
    {
        if (d1 == 0)
        {
            tt1 = p0;
            tt2 = p1;
        }
        else if (d2 == 0)
        {
            tt1 = p0;
            tt2 = p2;
        }
        else 
        {
            *t1 = p0;
            *t2 = p0;
            return;
        }
    }
    else if (d1 == 0)
    {
        if (d2 == 0)
        {
            tt1 = p1;
            tt2 = p2;
        }
        else
        {
            *t1 = p1;
            *t2 = p1;
            return;
        }
    }
    else if (d2 == 0)
    {
        *t1 = p2;
        *t2 = p2;
        return;
    }
    else if (d0 * d1 < 0)
    {
        tt1 = calculateT(p0, p1, d0, d1);
  
        if (d0 * d2 < 0)
            tt2 = calculateT(p0, p2, d0, d2);
        else
            tt2 = calculateT(p1, p2, d1, d2);
    }
    else
    {
        tt1 = calculateT(p0, p2, d0, d2);
        tt2 = calculateT(p1, p2, d1, d2);
    }
  
    // Arrange result values
    if (tt1 > tt2)
    {
        *t2 = tt1;
        *t1 = tt2;
    }
    else
    {
        *t1 = tt1;
        *t2 = tt2;
    }
}

bool doTrianglesIntersect(const double4 p00, const double4 p01, const double4 p02,
                          const double4 p10, const double4 p11, const double4 p12)
{
    if (p00.z > p12.z || p02.z < p10.z)
        return false;

    if (all(p00 == p10))
    {
        if (all(p01 == p11))
        {
            // Triangles are the same, thus no intersection			
            if (all(p02 == p12))
                return false;
            else
                return doTrianglesIntersect_EdgeShared(p00, p01, p02, p10, p11, p12);
        }
        else if (all(p02 == p12))
        {
            return doTrianglesIntersect_EdgeShared(p00, p02, p01, p10, p12, p11);
        }
        else
        {
            return doTrianglesIntersect_PointShared(p00, p01, p02, p10, p11, p12);
        }
    }
    else if (all(p00 == p11))
    {
        if (all(p01 == p12))
            return doTrianglesIntersect_EdgeShared(p00, p01, p02, p11, p10, p12);
        else if (all(p02 == p12))
            return doTrianglesIntersect_EdgeShared(p00, p02, p01, p11, p10, p12);
        else
            return doTrianglesIntersect_PointShared(p00, p02, p01, p11, p12, p10);
    }
    else if (all(p00 == p12))
    {
        return doTrianglesIntersect_PointShared(p00, p02, p01, p12, p10, p11);
    }



    else if (all(p01 == p10))
    {
        if (all(p02 == p11))
            return doTrianglesIntersect_EdgeShared(p01, p02, p00, p10, p11, p12);
        else if (all(p02 == p12))
            return doTrianglesIntersect_EdgeShared(p01, p02, p00, p10, p12, p11);
        else
            return doTrianglesIntersect_PointShared(p01, p00, p02, p10, p11, p12);
    }
    else if (all(p01 == p11))
    {
        if (all(p02 == p12))
            return doTrianglesIntersect_EdgeShared(p01, p02, p00, p11, p12, p10);
        else
            return doTrianglesIntersect_PointShared(p01, p00, p02, p11, p10, p12);
    }
    else if (all(p01 == p12))
    {
        return doTrianglesIntersect_PointShared(p01, p00, p01, p12, p10, p11);
    }
  
  
  
    else if (all(p02 == p10))
        return doTrianglesIntersect_PointShared(p02, p00, p01, p10, p11, p12);
    else if (all(p02 == p11))
        return doTrianglesIntersect_PointShared(p02, p00, p01, p11, p10, p12);
    else if (all(p02 == p12))
        return doTrianglesIntersect_PointShared(p02, p00, p01, p12, p10, p11);
  
    // No point are shared - calculate if there is true intesection
    // http://web.stanford.edu/class/cs277/resources/papers/Moller1997b.pdf
  
    double4 N0 = cross(p01 - p00, p02 - p00);
    double d0 = -dot(N0, p00);
  
    double dp10 = dot(N0, p10) + d0;
    double dp11 = dot(N0, p11) + d0;
    double dp12 = dot(N0, p12) + d0;
  
    // Triangle is in the same plane
    if (dp10 == 0 && dp11 == 0 && dp12 == 0)
        return false;
  
    // Triangle does not intersect the other's plane
    if (dp10 * dp11 > 0 && dp10 * dp12 > 0 && dp11 * dp12 > 0)
        return false;
  
    double4 N1 = cross(p11 - p10, p12 - p10);
    double d1 = -dot(N1, p10);
  
    double dp00 = dot(N1, p00) + d1;
    double dp01 = dot(N1, p01) + d1;
    double dp02 = dot(N1, p02) + d1;
  
    // Triangle is in the same plane
    if (dp00 == 0 && dp01 == 0 && dp02 == 0)
        return false;
  
    // Triangle does not intersect the other's plane
    if (dp00 * dp01 > 0 && dp00 * dp02 > 0 && dp01 * dp02 > 0)
        return false;
  
    double4 D = cross(N0, N1);
  
    // Calculate point projections
    double pp00 = dot(D, p00);
    double pp01 = dot(D, p01);
    double pp02 = dot(D, p02);
    double pp10 = dot(D, p10);
    double pp11 = dot(D, p11);
    double pp12 = dot(D, p12);
  
    // Calculate intersection intervals
    double t00, t01, t10, t11;
    calculateIntersectionInterval(pp00, pp01, pp02, dp00, dp01, dp02, &t00, &t01);
    calculateIntersectionInterval(pp10, pp11, pp12, dp10, dp11, dp12, &t10, &t11);

    // Check if intervals do intersect
    if (t00 > t11 || t01 < t10)
        return false;
    else
        return true;
}



size_t nocl_findTrianglesIntersections(std::vector<cl_float4> p00, std::vector<cl_float4> p01, std::vector<cl_float4> p02,
                                       std::vector<cl_float4> p10, std::vector<cl_float4> p11, std::vector<cl_float4> p12)
{
    size_t count = 0;


    int i_size = p00.size();
#pragma omp parallel for
    for (int i = 0; i < i_size; ++i)
    {
        for (int j = 0; j < (int)p10.size(); ++j)
        {
            if (i <= j)
                continue;

            const bool a = doTrianglesIntersect(p00[i], p01[i], p02[i], p10[j], p11[j], p12[j]);
            if (a)
            {
                ++count;
            }
        }
    }

    return count;


}
