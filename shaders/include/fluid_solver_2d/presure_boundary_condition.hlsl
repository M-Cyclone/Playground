#ifndef FLUID_SOLVER_2D_PRESURE_BOUNDARY_CONDITION_HLSL
#define FLUID_SOLVER_2D_PRESURE_BOUNDARY_CONDITION_HLSL

float get_presure_safe(int2 pixel_coord, int resolution, Texture2D<float> presure_field)
{
    if (pixel_coord.x < 0)
    {
        pixel_coord.x = -pixel_coord.x - 1;
    }
    else if (pixel_coord.x >= resolution)
    {
        pixel_coord.x = 2 * resolution - pixel_coord.x - 1;
    }

    if (pixel_coord.y < 0)
    {
        pixel_coord.y = -pixel_coord.y - 1;
    }
    else if (pixel_coord.y >= resolution)
    {
        pixel_coord.y = 2 * resolution - pixel_coord.y - 1;
    }

    return presure_field[pixel_coord];
}

#endif
