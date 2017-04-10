//#pragma OPENCL EXTENSION cl_khr_fp64 : enable
//#pragma OPENCL EXTENSION cl_khr_global_int32_base_atomics : enable

// Returns if triangles intersect while sharing one edge
// First two points of each triangle are shared edge, the last one - not
bool doTrianglesIntersect_EdgeShared(const float4 p00, const float4 p01, const float4 p02,
								     const float4 p10, const float4 p11, const float4 p12)
{ 
    float4 e1 = p01 - p00;
    float4 e2 = p02 - p00;
    float4 e3 = p12 - p00;

    float4 t1 = cross(e1, e2);
    float d = dot(e3, t1);

    if (d != 0)
		return false;

	  float4 t2 = cross(e1, e3);

	  return (dot(t1, t2) > 0);
}

// Returns if triangles intersect while sharing one point
// First points of each triangle is shared, the last two - not
bool doTrianglesIntersect_PointShared(const float4 p00, const float4 p01, const float4 p02,
								      const float4 p10, const float4 p11, const float4 p12)
{ 
    return false;
}

float calculateT(float p0, float p1, float d0, float d1)
{
	const float a_d0 = fabs(d0);
	const float a_d1 = fabs(d1);
	return (a_d0*p1 + a_d1*p0) / (a_d0 + a_d1);
}

void calculateIntersectionInterval(float p0, float p1, float p2, float d0, float d1, float d2, float * t1, float * t2)
{
    float tt1, tt2;


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
    else if (d0 * d1 <= 0)
	  {
		    tt1 = calculateT(p0, p1, d0, d1);

		    if(d0 * d2 <= 0)
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
	  if(tt1 > tt2)
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

bool doTrianglesIntersect(const float4 p00, const float4 p01, const float4 p02,
                          const float4 p10, const float4 p11, const float4 p12)
{
	  if(p00.z > p12.z || p02.z < p10.z)
		  return false;

    if(all(p00 == p10))
    {
		    if(all(p01 == p11))
		    { 
		        // Triangles are the same, thus no intersection			
			      if(all(p02 == p12))
				        return false;
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
    else if (all(p00 == p11))
    {
        if (all(p01 == p12))
            return doTrianglesIntersect_EdgeShared(p00, p01, p02, p11, p10, p12);
        else if(all(p02 == p12))
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
    else if(all(p01 == p11))
    {        
		    if(all(p02 == p12))
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
    else if(all(p02 == p12))
        return doTrianglesIntersect_PointShared(p02, p00, p01, p12, p10, p11);



    // No point are shared - calculate if there is true intesection
    // http://web.stanford.edu/class/cs277/resources/papers/Moller1997b.pdf

    float4 N0 = cross(p01 - p00, p02 - p00);
    float d0 = -dot(N0, p00);

    float dp10 = dot(N0, p10) + d0;
    float dp11 = dot(N0, p11) + d0;
    float dp12 = dot(N0, p12) + d0;

    if(dp10 == 0 && dp11 == 0 && dp12 == 0)
        return false;

    if(dp10 * dp11 > 0 && dp10 * dp12 > 0 && dp11 * dp12 > 0)
        return false;

    float4 N1 = cross(p11 - p10, p12 - p10);
    float d1 = -dot(N1, p10);

    float dp00 = dot(N1, p00) + d1;
    float dp01 = dot(N1, p01) + d1;
    float dp02 = dot(N1, p02) + d1;

    if (dp00 == 0 && dp01 == 0 && dp02 == 0)
      return false;

    if (dp00 * dp01 > 0 && dp00 * dp02 > 0 && dp01 * dp02 > 0)
      return false;

    float4 D = cross(N0, N1);

    // Calculate point projections
    float pp00 = dot(D, p00);
    float pp01 = dot(D, p01);
    float pp02 = dot(D, p02);
    float pp10 = dot(D, p10);
    float pp11 = dot(D, p11);
    float pp12 = dot(D, p12);

    // Calculate intersection intervals
	  float t00, t01, t10, t11;
	  calculateIntersectionInterval(pp00, pp01, pp02, dp00, dp01, dp02, &t00, &t01);
	  calculateIntersectionInterval(pp10, pp11, pp12, dp10, dp11, dp12, &t10, &t11);

	  // Check if intervals do intersect
	  if(t00 > t11 || t01 < t10)
		    return false;
    else
		    return true;
}

__kernel void findTrianglesIntersections(__global const float4 * p00, __global const float4 * p01, __global const float4 * p02,
                                         __global const float4 * p10, __global const float4 * p11, __global const float4 * p12,
										                     __global uint * count)
{
    int i = get_global_id(0);
    int j = get_global_id(1);

    if(i <= j)
        return;

    bool a = doTrianglesIntersect(p00[i], p01[i], p02[i], p10[j], p11[j], p12[j]);
    if(a)
    {
        // atom_inc(count);
        atomic_inc(count);
    }
}

void swap(__global float4 * p0, __global float4 * p1, int i)
{
    float4 t;
    
    t = p0[i];
    p0[i] = p1[i];
    p1[i] = t;
}

__kernel void arrangeTriangleCoordinatesZ(__global float4 * p0, __global float4 * p1, __global float4 * p2)
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