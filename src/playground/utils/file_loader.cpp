#include "file_loader.h"

std::unique_ptr<GpuShader> LoadShader(const GpuDevice& device, const char* shader_filename, Uint32 sampler_count, Uint32 uniform_buffer_count, Uint32 storage_buffer_count, Uint32 storage_texture_count)
{
    // Auto-detect the shader stage from the file name for convenience
    SDL_GPUShaderStage stage;
    if (SDL_strstr(shader_filename, ".vert"))
    {
        stage = SDL_GPU_SHADERSTAGE_VERTEX;
    }
    else if (SDL_strstr(shader_filename, ".frag"))
    {
        stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
    }
    // TODO: compute shader.
    else
    {
        SDL_Log("Invalid shader stage!");
        return nullptr;
    }

    const char* BasePath = SDL_GetBasePath();

    char full_path[256];
    SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(device.Get());
    SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;

    const char* entrypoint;
    if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV)
    {
        SDL_snprintf(full_path, sizeof(full_path), "%sshaders/%s.spv", BasePath, shader_filename);
        format = SDL_GPU_SHADERFORMAT_SPIRV;
        entrypoint = "main";
    }
    else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL)
    {
        SDL_snprintf(full_path, sizeof(full_path), "%sshaders/%s.msl", BasePath, shader_filename);
        format = SDL_GPU_SHADERFORMAT_MSL;
        entrypoint = "main0";
    }
    else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL)
    {
        SDL_snprintf(full_path, sizeof(full_path), "%sshaders/%s.dxil", BasePath, shader_filename);
        format = SDL_GPU_SHADERFORMAT_DXIL;
        entrypoint = "main";
    }
    else
    {
        SDL_Log("%s", "Unrecognized backend shader format!");
        return nullptr;
    }

    size_t code_size;
    void* code = SDL_LoadFile(full_path, &code_size);
    if (!code)
    {
        SDL_Log("Failed to load shader from disk! %s", full_path);
        return nullptr;
    }

    SDL_GPUShaderCreateInfo shader_info{};
    shader_info.code_size = code_size;
    shader_info.code = (uint8_t*)code;
    shader_info.entrypoint = entrypoint;
    shader_info.format = format;
    shader_info.stage = stage;
    shader_info.num_samplers = sampler_count;
    shader_info.num_storage_textures = storage_texture_count;
    shader_info.num_storage_buffers = storage_buffer_count;
    shader_info.num_uniform_buffers = uniform_buffer_count;

    auto shader = std::make_unique<GpuShader>(device, shader_info);
    SDL_free(code);

    if (!shader)
    {
        SDL_Log("Failed to create shader!");
    }

    return shader;
}
