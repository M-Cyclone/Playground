#ifndef RAYTRACING_RAY_HLSL
#define RAYTRACING_RAY_HLSL

struct Ray
{
    float3 origin;
    float3 dir;
};

float3 Ray_At(Ray ray, float t)
{
    return ray.origin  + ray.dir * t;
}

#endif
