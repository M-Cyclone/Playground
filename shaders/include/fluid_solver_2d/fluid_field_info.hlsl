#ifndef FLUID_SOLVER_2D_FLUID_FIELD_INFO_HLSL
#define FLUID_SOLVER_2D_FLUID_FIELD_INFO_HLSL

cbuffer FluidFieldInfoUniform : register(b0, space2)
{
    float dx;
    float rdx;
    float dt;
    float rdt;
    int resolution;
    float inv_res;          // 1 / res
    float inv_res_plus_one; // 1 / (res + 1)
    float gravity;
    float density;
    float inv_density;
};

#endif
