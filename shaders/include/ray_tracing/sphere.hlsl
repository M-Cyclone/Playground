#ifndef RAYTRACING_Sphere_HLSL
#define RAYTRACING_Sphere_HLSL

#include "ray_tracing/ray.hlsl"

struct Sphere
{
    float3 center;
    float radius;
};

struct Intersection
{
    bool is_hit;
    float hit_time;
    float3 hit_pos;
};

// If the ray's origin is on the surface, we consider it is not a hit point.
Intersection Sphere_Intersect(Sphere sphere, Ray ray, float t_min = 1e-4f, float t_max = 1e8f)
{
    float3 oc = sphere.center - ray.origin;

    float a = dot(ray.dir, ray.dir);
    float b = -2.0f * dot(ray.dir, oc);
    float c = dot(oc, oc) - sphere.radius * sphere.radius;

    float discriminant = b * b - 4 * a * c;

    float hit_time = -1.0f;

    Intersection intersection;
    intersection.is_hit = false;
    intersection.hit_pos = float3(0.0f, 0.0f, 0.0f);

    if (discriminant >= 0)
    {
        float sqrt_discriminant = sqrt(discriminant);

        float a_times_2_inv = 1.0f / (2.0f * a);

        float t1 = (-b - sqrt_discriminant) * a_times_2_inv;
        float t2 = (-b + sqrt_discriminant) * a_times_2_inv;

        intersection.hit_time = (t1 > t_min && t1 < t_max) ? t1 : ((t2 > t_min && t2 < t_max) ? t2 : hit_time);
        intersection.is_hit = (intersection.hit_time > t_min && intersection.hit_time < t_max);

        if (intersection.is_hit)
        {
            intersection.hit_pos = Ray_At(ray, intersection.hit_time);
        }
    }

    return intersection;
}

#endif