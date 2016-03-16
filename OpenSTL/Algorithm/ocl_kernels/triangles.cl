#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#pragma OPENCL EXTENSION cl_khr_global_int32_base_atomics : enable

// Returns if triangles intersect while sharing one edge
// First two points of each triangle are shared edge, the last one - not
bool doTrianglesIntersect_EdgeShared(const double4 p00, const double4 p01, const double4 p02,
								     const double4 p10, const double4 p11, const double4 p12)
{ 
    double4 e1 = p01 - p00;
    double4 e2 = p02 - p00;
    double4 e3 = p12 - p00;

    double4 t = cross(e1, e2);
    double d = dot(e3, t);

    return (d == 0);
}

// Returns if triangles intersect while sharing one point
// First points of each triangle is shared, the last two - not
bool doTrianglesIntersect_PointShared(const double4 p00, const double4 p01, const double4 p02,
								      const double4 p10, const double4 p11, const double4 p12)
{ 
    return false;
}


bool doTrianglesIntersect(const double4 p00, const double4 p01, const double4 p02,
                          const double4 p10, const double4 p11, const double4 p12)
{
    if(all(p00 == p10))
    {
		if(all(p01 == p11))
		{ 
		    // Triangles are the same, thus no intersection			
			if(all(p02 == p12))
			{
				return false;
			}
			else
				return doTrianglesIntersect_EdgeShared(p00, p01, p02, p10, p11, p12);
		}
		else if(all(p02 == p12))
		{ 
			return doTrianglesIntersect_EdgeShared(p00, p02, p01, p10, p12, p11);
		}
        else
        { 
            return doTrianglesIntersect_PointShared(p00, p01, p02, p10, p11, p12);
        }
	}
    else if(all(p01 == p11))
    { 
        // Triangles are the same, thus no intersection			
		if(all(p02 == p12))
		{
			return doTrianglesIntersect_EdgeShared(p01, p02, p00, p11, p12, p10);
		}
		else
		    return doTrianglesIntersect_PointShared(p01, p00, p02, p11, p10, p12);
    }
    else if(all(p02 == p12))
    {
        return doTrianglesIntersect_PointShared(p02, p00, p01, p12, p10, p11);
    }

    // No point are shared - calculate if there is true intesection
    // http://web.stanford.edu/class/cs277/resources/papers/Moller1997b.pdf

    double4 N0 = cross(p01 - p00, p02 - p00);
    double d0 = -dot(N0, p00);

    double dp10 = dot(N0, p10) + d0;
    double dp11 = dot(N0, p11) + d0;
    double dp12 = dot(N0, p12) + d0;

    if(dp10 == 0 || dp11 == 0 || dp12 == 0)
    { 
        return false;
    }

    if(dp10 * dp11 > 0 && dp10 * dp12 > 0)
        return false;


    double4 N1 = cross(p11 - p10, p12 - p10);
    double d1 = -dot(N1, p10);

    double dp00 = dot(N0, p00) + d1;
    double dp01 = dot(N0, p01) + d1;
    double dp02 = dot(N0, p02) + d1;


    double4 D = cross(N0, N1);

    //double m = max(abs(D));
    double pp00 = dot(D, p00);
    double pp01 = dot(D, p01);
    double pp02 = dot(D, p02);
    double pp10 = dot(D, p10);
    double pp11 = dot(D, p11);
    double pp12 = dot(D, p12);

    // double t00



    return false;
}

__kernel void findTrianglesIntersections(__global const double4 * p00, __global const double4 * p01, __global const double4 * p02,
                                         __global const double4 * p10, __global const double4 * p11, __global const double4 * p12,
										 __global uint * count)
{
    int i = get_global_id(0);
    int j = get_global_id(1);

    if(i <= j)
        return;

    bool a = doTrianglesIntersect(p00[i], p01[i], p02[i], p10[j], p11[j], p12[j]);
    if(a)
    {
        atom_inc(count);
    }
}

void swap(__global double4 * p0, __global double4 * p1, int i)
{
    double4 t;
    
    t = p0[i];
    p0[i] = p1[i];
    p1[i] = t;
}

__kernel void arrangeTriangleCoordinatesZ(__global double4 * p0, __global double4 * p1, __global double4 * p2)
{
    int i = get_global_id(0);

    if(p0[i].s2 > p1[i].s2)
        swap(p0, p1, i);
        
    if(p1[i].s2 > p2[i].s2)
    {
        swap(p1, p2, i);
        
        if(p0[i].s2 > p1[i].s2)
            swap(p0, p1, i);
    }
        
}