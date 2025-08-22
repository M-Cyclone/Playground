#ifndef RAYTRACING_CAMERA_HLSL
#define RAYTRACING_CAMERA_HLSL

#include "ray_tracing/math_constant.hlsl"
#include "ray_tracing/ray.hlsl"

struct Camera
{
    float img_width;
    float img_height;
    float img_width_inv;
    float img_height_inv;
    float aspect_ratio;
    float fov;

    float viewport_width;
    float viewport_height;
    float viewport_width_inv;
    float viewport_height_inv;

    float3 pos;
    float3 dir_left;
    float3 dir_up;
    float3 dir_forward;
};

void Camera_InitViewport(inout Camera camera, float w, float h, float fov)
{
    camera.img_width = w;
    camera.img_height = h;
    camera.img_width_inv = 1.0f / w;
    camera.img_height_inv = 1.0f / h;

    camera.aspect_ratio = camera.img_width / camera.img_height;
    camera.fov = fov;

    camera.viewport_height = 2.0f * tan(fov * 0.5f);
    camera.viewport_width = camera.viewport_height * camera.aspect_ratio;
    camera.viewport_height_inv = 1.0f / camera.viewport_height;
    camera.viewport_width_inv = 1.0f / camera.viewport_width;
}

void Camera_InitPose(inout Camera camera, float3 camera_pos, float3 target_pos)
{
    camera.pos = camera_pos;
    camera.dir_forward = normalize(target_pos - camera_pos);
    camera.dir_left = normalize(cross(float3(0.0f, 1.0f, 0.0f), camera.dir_forward));
    camera.dir_up = normalize(cross(camera.dir_forward, camera.dir_left));
}

Ray Camera_GetRayByPixelUv(Camera camera, float2 pixel_uv)
{
    float viewport_u = 1.0f - (2.0f * pixel_uv.x + 1.0f) * camera.img_width_inv;
    float viewport_v = 1.0f - (2.0f * pixel_uv.y + 1.0f) * camera.img_height_inv;

    float3 dir = camera.dir_forward + viewport_u * camera.dir_left + viewport_v * camera.dir_up;

    Ray ray;
    ray.origin = camera.pos;
    ray.dir = normalize(dir);

    return ray;
}

#endif