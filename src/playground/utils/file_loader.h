#pragma once

#include "engine/sdl_wrapper/sdl_gpu_types.h"

std::unique_ptr<GpuShader> LoadShader(
    const GpuDevice& device,
    const char* shader_filename,
    Uint32 sampler_count,
    Uint32 uniform_buffer_count,
    Uint32 storage_buffer_count,
    Uint32 storage_texture_count
);
