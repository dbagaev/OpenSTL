bool __doTrianglesIntersect(const float4 p00, const float4 p01, const float4 p02,
                            const float4 p10, const float4 p11, const float4 p12)
{
    return true;
}

__kernel void doTrianglesIntersect(__global const float4 * p00, __global const float4 * p01, __global const float4 * p02,
                                   __global const float4 * p10, __global const float4 * p11, __global const float4 * p12)
{
    bool a = __doTrianglesIntersect(*p00, *p01, *p02, *p10, *p11, *p12);
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
        
    //p0[i] = (float4)(01, 02, 03, i);
    //p1[i] = (float4)(11, 12, 13, i);
    //p2[i] = (float4)(21, 22, 23, i);
}