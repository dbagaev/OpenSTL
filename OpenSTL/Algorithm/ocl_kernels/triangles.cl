// Returns if triangles intersect while sharing one edge
// First two points of each triangle are shared edge, the last one - not
bool doTrianglesIntersect_EdgeShared(const float4 p00, const float4 p01, const float4 p02,
								     const float4 p10, const float4 p11, const float4 p12)
{ 
    float4 e1 = p01 - p00;
    float4 e2 = p02 - p00;
    float4 e3 = p12 - p00;

    float4 t = cross(e1, e2);
    float d = dot(e3, t);

    return (d == 0);
}

// Returns if triangles intersect while sharing one point
// First points of each triangle is shared, the last two - not
bool doTrianglesIntersect_PointShared(const float4 p00, const float4 p01, const float4 p02,
								      const float4 p10, const float4 p11, const float4 p12)
{ 
    return false;
}


bool doTrianglesIntersect(const float4 p00, const float4 p01, const float4 p02,
                          const float4 p10, const float4 p11, const float4 p12)
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

    float4 N0 = cross(p01 - p00, p02 - p00);
    float d0 = -dot(N0, p00);

    float dp10 = N0 * p10 + d0;
    float dp11 = N0 * p11 + d0;
    float dp12 = N0 * p12 + d0;

    if(dp10 == 0 || dp11 == 0 || dp12 == 0)
    { 
        return false;
    }

    if(dp0 * dp1 > 0 && dp0 * dp2 > 0)
        return false;


    float4 N1 = cross(p11 - p10, p12 - p10);
    float d1 = -dot(N1, p10);

    float dp00 = N0 * p00 + d1;
    float dp01 = N0 * p01 + d1;
    float dp02 = N0 * p02 + d1;


    float4 D = cross(N1, N2);

    //float m = max(abs(D));
    float pp00 = dot(D, p00);
    float pp01 = dot(D, p01);
    float pp02 = dot(D, p02);
    float pp10 = dot(D, p10);
    float pp11 = dot(D, p11);
    float pp12 = dot(D, p12);

    float t00



    return false;
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