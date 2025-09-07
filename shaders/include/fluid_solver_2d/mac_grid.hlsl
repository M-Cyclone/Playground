#ifndef FLUID_SOLVER_2D_COORDINATE_CONVERT_HLSL
#define FLUID_SOLVER_2D_COORDINATE_CONVERT_HLSL

struct FluidFieldInfo
{
    float dx;
    float rdx;
    float dt;
    float rdt;
    int resolution;
    float inv_res;          // 1 / res
    float inv_res_plus_one; // 1 / (res + 1)
    float gravity;
};

float2 get_mac_pos_from_edge_u_uv(float2 uv, FluidFieldInfo field_info)
{
    return float2(uv.x * (float)(1 + field_info.resolution), uv.y * (float)field_info.resolution) * field_info.dx;
}

float2 get_mac_pos_from_edge_v_uv(float2 uv, FluidFieldInfo field_info)
{
    return float2(uv.x * (float)field_info.resolution, uv.y * (float)(1 + field_info.resolution)) * field_info.dx;
}

float2 get_mac_pos_from_edge_u_pixel_coord(uint2 pixel_coord, FluidFieldInfo field_info)
{
    return (float2(pixel_coord) + float2(0.0f, 0.5f)) * field_info.dx;
}

float2 get_mac_pos_from_edge_v_pixel_coord(uint2 pixel_coord, FluidFieldInfo field_info)
{
    return (float2(pixel_coord) + float2(0.5f, 0.0f)) * field_info.dx;
}

float2 get_edge_u_uv_from_mac_pos(float2 pos, FluidFieldInfo field_info)
{
    return float2((pos.x * field_info.rdx + 0.5f) * field_info.inv_res_plus_one, pos.y * field_info.rdx * field_info.inv_res);
}

float2 get_edge_v_uv_from_mac_pos(float2 pos, FluidFieldInfo field_info)
{
    return float2(pos.x * field_info.rdx * field_info.inv_res, (pos.y * field_info.rdx + 0.5f) * field_info.inv_res_plus_one);
}

float2 sample_vel_at_pos(Texture2D<float> vel_field_u, Texture2D<float> vel_field_v, SamplerState field_sampler, float2 pos, FluidFieldInfo field_info)
{
    float boundary_length = field_info.resolution * field_info.dx;
    bool is_out_of_range = pos.x < 0.0f || pos.x > boundary_length || pos.y < 0.0f || pos.y > boundary_length;

    float2 vel_u_uv = get_edge_u_uv_from_mac_pos(pos, field_info);
    float2 vel_v_uv = get_edge_v_uv_from_mac_pos(pos, field_info);

    float vel_u = is_out_of_range ? 0.0f : vel_field_u.SampleLevel(field_sampler, vel_u_uv, 0);
    float vel_v = is_out_of_range ? 0.0f : vel_field_v.SampleLevel(field_sampler, vel_v_uv, 0);

    return float2(vel_u, vel_v);
}

#endif
