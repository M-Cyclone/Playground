#include "sdl_gpu_types.h"

GpuDevice::GpuDevice(SDL_GPUShaderFormat format_flags, const char* backend_name)
    : m_device(SDL_CreateGPUDevice(format_flags, ENGINE_DEBUG, backend_name))
{
    SDL_assert(m_device);
}

GpuDevice::GpuDevice(SDL_PropertiesID props)
    : m_device(SDL_CreateGPUDeviceWithProperties(props))
{
    SDL_assert(m_device);
}

GpuDevice::~GpuDevice()
{
    SDL_assert(m_device);
    if (m_device)
    {
        if (m_window)
        {
            ReleaseWindow();
            m_window = nullptr;
        }

        SDL_DestroyGPUDevice(m_device);
        m_device = nullptr;
    }
}

int32_t GpuDevice::GetNumGpuDrivers()
{
    return SDL_GetNumGPUDrivers();
}

const char* GpuDevice::GetGpuDriverName(int32_t index)
{
    return SDL_GetGPUDriver(index);
}

const char* GpuDevice::GetGpuDeviceDriver() const
{
    SDL_assert(m_device);
    return SDL_GetGPUDeviceDriver(m_device);
}

SDL_GPUShaderFormat GpuDevice::GetGpuShaderFormat() const
{
    SDL_assert(m_device);
    return SDL_GetGPUShaderFormats(m_device);
}

bool GpuDevice::CheckWindowSuppoertsSwapchainComposition(SDL_Window* window, SDL_GPUSwapchainComposition swapchain_composition) const
{
    SDL_assert(m_device);
    return m_device ? SDL_WindowSupportsGPUSwapchainComposition(m_device, window, swapchain_composition) : false;
}

bool GpuDevice::CheckWindowSupportsPresentMode(SDL_Window* window, SDL_GPUPresentMode present_mode) const
{
    SDL_assert(m_device);
    return m_device ? SDL_WindowSupportsGPUPresentMode(m_device, window, present_mode) : false;
}

bool GpuDevice::ClaimWindow(SDL_Window* window)
{
    SDL_assert(m_device);
    if (m_device)
    {
        if (m_window)
        {
            ReleaseWindow();
        }

        if (SDL_ClaimWindowForGPUDevice(m_device, window))
        {
            m_window = window;
            return true;
        }
        else
        {
            SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "Failed to clain window: %s", SDL_GetError());
        }
    }
    return false;
}

void GpuDevice::ReleaseWindow()
{
    SDL_assert(m_device);
    if (m_device && m_window)
    {
        SDL_ReleaseWindowFromGPUDevice(m_device, m_window);
        m_window = nullptr;
    }
}

bool GpuDevice::SetSwapchainParameters(SDL_Window* window, SDL_GPUSwapchainComposition swapchain_composition, SDL_GPUPresentMode present_mode)
{
    SDL_assert(m_device);
    if (m_device)
    {
        return SDL_SetGPUSwapchainParameters(m_device, window, swapchain_composition, present_mode);
    }
    return false;
}

bool GpuDevice::SetAllowedFramesInFlight(uint32_t allowed_frames_in_flight)
{
    SDL_assert(m_device);
    if (m_device)
    {
        return SDL_SetGPUAllowedFramesInFlight(m_device, allowed_frames_in_flight);
    }
    return false;
}

SDL_GPUTextureFormat GpuDevice::GetSwapchainTextureFormat(SDL_Window* window) const
{
    SDL_assert(m_device);
    if (m_device)
    {
        return SDL_GetGPUSwapchainTextureFormat(m_device, window);
    }
    return SDL_GPU_TEXTUREFORMAT_INVALID;
}

bool GpuDevice::WaitForSwapchain(SDL_Window* window) const
{
    SDL_assert(m_device);
    if (m_device)
    {
        return SDL_WaitForGPUSwapchain(m_device, window);
    }
    return false;
}

bool GpuDevice::WaitForGpuIdle()
{
    SDL_assert(m_device);
    if (m_device)
    {
        return SDL_WaitForGPUIdle(m_device);
    }
    return false;
}

bool GpuDevice::WaitForGpuFences(bool wait_all, std::span<SDL_GPUFence* const> fences)
{
    SDL_assert(m_device);
    if (m_device)
    {
        return SDL_WaitForGPUFences(m_device, wait_all, fences.data(), (Uint32)fences.size());
    }
    return false;
}

bool GpuDevice::QueryGpuFence(SDL_GPUFence* fence) const
{
    SDL_assert(m_device && fence);
    if (m_device && fence)
    {
        return SDL_QueryGPUFence(m_device, fence);
    }
    return false;
}

void GpuDevice::ReleaseGpuFence(SDL_GPUFence* fence)
{
    SDL_assert(m_device && fence);
    if (m_device && fence)
    {
        SDL_ReleaseGPUFence(m_device, fence);
    }
}

uint32_t GpuDevice::GetGpuTextureFormatTexelBlockSize(SDL_GPUTextureFormat format)
{
    return SDL_GPUTextureFormatTexelBlockSize(format);
}

bool GpuDevice::CheckTextureSupportsFormat(SDL_GPUTextureFormat format, SDL_GPUTextureType type, SDL_GPUTextureUsageFlags usage) const
{
    SDL_assert(m_device);
    if (m_device)
    {
        return SDL_GPUTextureSupportsFormat(m_device, format, type, usage);
    }
    return false;
}

bool GpuDevice::CheckTextureSupportsSampleCount(SDL_GPUTextureFormat format, SDL_GPUSampleCount sample_count) const
{
    SDL_assert(m_device);
    if (m_device)
    {
        return SDL_GPUTextureSupportsSampleCount(m_device, format, sample_count);
    }
    return false;
}

uint32_t GpuDevice::CalculateGPUTextureFormatSize(SDL_GPUTextureFormat format, Uint32 width, Uint32 height, Uint32 depth_or_layer_count)
{
    return SDL_CalculateGPUTextureFormatSize(format, width, height, depth_or_layer_count);
}

GpuComputePipeline::GpuComputePipeline(const GpuDevice& gpu_device, const SDL_GPUComputePipelineCreateInfo& create_info)
    : m_device(gpu_device.Get())
    , m_pipeline(SDL_CreateGPUComputePipeline(m_device, &create_info))
{
    SDL_assert(m_device && m_pipeline);
}

GpuComputePipeline::~GpuComputePipeline()
{
    SDL_assert(m_device && m_pipeline);
    if (m_device && m_pipeline)
    {
        SDL_ReleaseGPUComputePipeline(m_device, m_pipeline);
        m_device = nullptr;
        m_pipeline = nullptr;
    }
}

GpuGraphicsPipeline::GpuGraphicsPipeline(const GpuDevice& gpu_device, const SDL_GPUGraphicsPipelineCreateInfo& create_info)
    : m_device(gpu_device.Get())
    , m_pipeline(SDL_CreateGPUGraphicsPipeline(m_device, &create_info))
{
    SDL_assert(m_device && m_pipeline);
}

GpuGraphicsPipeline::~GpuGraphicsPipeline()
{
    SDL_assert(m_device && m_pipeline);
    if (m_device && m_pipeline)
    {
        SDL_ReleaseGPUGraphicsPipeline(m_device, m_pipeline);
        m_device = nullptr;
        m_pipeline = nullptr;
    }
}

GpuSampler::GpuSampler(const GpuDevice& gpu_device, const SDL_GPUSamplerCreateInfo& create_info)
    : m_device(gpu_device.Get())
    , m_sampler(SDL_CreateGPUSampler(m_device, &create_info))
{
    SDL_assert(m_device && m_sampler);
}

GpuSampler::~GpuSampler()
{
    SDL_assert(m_device && m_sampler);
    if (m_device && m_sampler)
    {
        SDL_ReleaseGPUSampler(m_device, m_sampler);
        m_device = nullptr;
        m_sampler = nullptr;
    }
}

GpuShader::GpuShader(const GpuDevice& gpu_device, const SDL_GPUShaderCreateInfo& create_info)
    : m_device(gpu_device.Get())
    , m_shader(SDL_CreateGPUShader(m_device, &create_info))
{
    SDL_assert(m_device && m_shader);
}

GpuShader::~GpuShader()
{
    SDL_assert(m_device && m_shader);
    if (m_device && m_shader)
    {
        SDL_ReleaseGPUShader(m_device, m_shader);
        m_device = nullptr;
        m_shader = nullptr;
    }
}

GpuTexture::GpuTexture(const GpuDevice& gpu_device, const SDL_GPUTextureCreateInfo& create_info)
    : m_device(gpu_device.Get())
    , m_texture(SDL_CreateGPUTexture(m_device, &create_info))
{
    SDL_assert(m_device && m_texture);
}

GpuTexture::~GpuTexture()
{
    SDL_assert(m_device && m_texture);
    if (m_device && m_texture)
    {
        SDL_ReleaseGPUTexture(m_device, m_texture);
        m_device = nullptr;
        m_texture = nullptr;
    }
}

void GpuTexture::SetName(const char* new_name)
{
    SDL_assert(m_device && m_texture);
    if (m_device && m_texture)
    {
        SDL_SetGPUTextureName(m_device, m_texture, new_name);
    }
}

GpuBuffer::GpuBuffer(const GpuDevice& gpu_device, const SDL_GPUBufferCreateInfo& create_info)
    : m_device(gpu_device.Get())
    , m_buffer(SDL_CreateGPUBuffer(m_device, &create_info))
{
    SDL_assert(m_device && m_buffer);
}

GpuBuffer::~GpuBuffer()
{
    SDL_assert(m_device && m_buffer);
    if (m_device && m_buffer)
    {
        SDL_ReleaseGPUBuffer(m_device, m_buffer);
        m_device = nullptr;
        m_buffer = nullptr;
    }
}

void GpuBuffer::SetName(const char* new_name)
{
    SDL_assert(m_device && m_buffer);
    if (m_device && m_buffer)
    {
        SDL_SetGPUBufferName(m_device, m_buffer, new_name);
    }
}

GpuTransferBuffer::GpuTransferBuffer(const GpuDevice& gpu_device, const SDL_GPUTransferBufferCreateInfo& create_info)
    : m_device(gpu_device.Get())
    , m_transfer_buffer(SDL_CreateGPUTransferBuffer(m_device, &create_info))
{
    SDL_assert(m_device && m_transfer_buffer);
}

GpuTransferBuffer::~GpuTransferBuffer()
{
    SDL_assert(m_device && m_transfer_buffer);
    if (m_device && m_transfer_buffer)
    {
        SDL_ReleaseGPUTransferBuffer(m_device, m_transfer_buffer);
        m_device = nullptr;
        m_transfer_buffer = nullptr;
    }
}

void* GpuTransferBuffer::Map(bool cycle)
{
    SDL_assert(m_device && m_transfer_buffer);
    if (m_device && m_transfer_buffer)
    {
        return SDL_MapGPUTransferBuffer(m_device, m_transfer_buffer, cycle);
    }
    return nullptr;
}

void GpuTransferBuffer::Unmap()
{
    SDL_assert(m_device && m_transfer_buffer);
    if (m_device && m_transfer_buffer)
    {
        SDL_UnmapGPUTransferBuffer(m_device, m_transfer_buffer);
    }
}

GpuCmdBuffer::GpuCmdBuffer(const GpuDevice& gpu_device)
    : m_cmd_buffer(SDL_AcquireGPUCommandBuffer(gpu_device.Get()))
{
    SDL_assert(m_cmd_buffer);
}

void GpuCmdBuffer::PushVertShaderUniformData(uint32_t slot_index, const void* data, uint32_t data_size)
{
    SDL_assert(m_cmd_buffer);
    if (m_cmd_buffer)
    {
        SDL_PushGPUVertexUniformData(m_cmd_buffer, slot_index, data, data_size);
    }
}

void GpuCmdBuffer::PushFragShaderUniformData(uint32_t slot_index, const void* data, uint32_t data_size)
{
    SDL_assert(m_cmd_buffer);
    if (m_cmd_buffer)
    {
        SDL_PushGPUFragmentUniformData(m_cmd_buffer, slot_index, data, data_size);
    }
}

void GpuCmdBuffer::PushCompShaderUniformData(uint32_t slot_index, const void* data, uint32_t data_size)
{
    SDL_assert(m_cmd_buffer);
    if (m_cmd_buffer)
    {
        SDL_PushGPUComputeUniformData(m_cmd_buffer, slot_index, data, data_size);
    }
}

void GpuCmdBuffer::GenerateMipmaps(const GpuTexture& texture)
{
    SDL_assert(m_cmd_buffer);
    if (m_cmd_buffer)
    {
        SDL_GPUTexture* gpu_texture = texture.Get();
        SDL_assert(gpu_texture);
        SDL_GenerateMipmapsForGPUTexture(m_cmd_buffer, gpu_texture);
    }
}

void GpuCmdBuffer::BlitTexture(const SDL_GPUBlitInfo& info)
{
    SDL_assert(m_cmd_buffer);
    if (m_cmd_buffer)
    {
        SDL_BlitGPUTexture(m_cmd_buffer, &info);
    }
}

bool GpuCmdBuffer::AcquireSwapchainTexture(SDL_Window* window, SDL_GPUTexture*& swapchain_texture, uint32_t* swapchain_texture_width, uint32_t* swapchain_texture_height)
{
    SDL_assert(m_cmd_buffer);
    if (!m_cmd_buffer)
    {
        return false;
    }
    SDL_GPUTexture* texture = nullptr;
    if (SDL_AcquireGPUSwapchainTexture(m_cmd_buffer, window, &texture, swapchain_texture_width, swapchain_texture_height))
    {
        swapchain_texture = texture;
        return true;
    }
    return false;
}

bool GpuCmdBuffer::WaitAndAcquireSwapchainTexture(SDL_Window* window, SDL_GPUTexture*& swapchain_texture, uint32_t* swapchain_texture_width, uint32_t* swapchain_texture_height)
{
    SDL_assert(m_cmd_buffer);
    if (!m_cmd_buffer)
    {
        return false;
    }
    SDL_GPUTexture* texture = nullptr;
    if (SDL_WaitAndAcquireGPUSwapchainTexture(m_cmd_buffer, window, &texture, swapchain_texture_width, swapchain_texture_height))
    {
        swapchain_texture = texture;
        return true;
    }
    return false;
}

bool GpuCmdBuffer::Submit()
{
    SDL_assert(m_cmd_buffer);
    if (m_cmd_buffer)
    {
        return SDL_SubmitGPUCommandBuffer(m_cmd_buffer);
    }
    return false;
}

SDL_GPUFence* GpuCmdBuffer::AcquireFence()
{
    SDL_assert(m_cmd_buffer);
    if (m_cmd_buffer)
    {
        return SDL_SubmitGPUCommandBufferAndAcquireFence(m_cmd_buffer);
    }
    return nullptr;
}

bool GpuCmdBuffer::Cancel()
{
    SDL_assert(m_cmd_buffer);
    if (m_cmd_buffer)
    {
        return SDL_CancelGPUCommandBuffer(m_cmd_buffer);
    }
    return false;
}

GpuRenderPass::GpuRenderPass(const GpuCmdBuffer& cmd)
    : m_cmd_buffer(cmd.Get())
    , m_render_pass(nullptr)
{
    SDL_assert(m_cmd_buffer);
}

bool GpuRenderPass::BeginRenderPass(
    std::span<const SDL_GPUColorTargetInfo> color_target_infos,
    const SDL_GPUDepthStencilTargetInfo* depth_stencil_target_info
)
{
    SDL_assert(m_cmd_buffer);
    if (!m_cmd_buffer)
    {
        return false;
    }
    m_render_pass = SDL_BeginGPURenderPass(
        m_cmd_buffer,
        color_target_infos.data(),
        (uint32_t)color_target_infos.size(),
        depth_stencil_target_info
    );
    return m_render_pass != nullptr;
}

void GpuRenderPass::BindGraphicsPipeline(const GpuGraphicsPipeline& graphics_pipeline)
{
    SDL_assert(m_render_pass);
    if (m_render_pass)
    {
        SDL_GPUGraphicsPipeline* gpu_graphics_pipeline = graphics_pipeline.Get();
        SDL_assert(gpu_graphics_pipeline);
        SDL_BindGPUGraphicsPipeline(m_render_pass, gpu_graphics_pipeline);
    }
}

void GpuRenderPass::SetViewport(const SDL_GPUViewport& viewport)
{
    SDL_assert(m_render_pass);
    if (m_render_pass)
    {
        SDL_SetGPUViewport(m_render_pass, &viewport);
    }
}

void GpuRenderPass::SetScissor(const SDL_Rect& scissor)
{
    SDL_assert(m_render_pass);
    if (m_render_pass)
    {
        SDL_SetGPUScissor(m_render_pass, &scissor);
    }
}

void GpuRenderPass::SetBlendConstants(SDL_FColor blend_constants)
{
    SDL_assert(m_render_pass);
    if (m_render_pass)
    {
        SDL_SetGPUBlendConstants(m_render_pass, blend_constants);
    }
}

void GpuRenderPass::SetStencilReference(uint8_t reference)
{
    SDL_assert(m_render_pass);
    if (m_render_pass)
    {
        SDL_SetGPUStencilReference(m_render_pass, reference);
    }
}

void GpuRenderPass::BindVertexBuffers(uint32_t first_slot, std::span<const SDL_GPUBufferBinding> bindings)
{
    SDL_assert(m_render_pass);
    if (m_render_pass)
    {
        SDL_BindGPUVertexBuffers(m_render_pass, first_slot, bindings.data(), (uint32_t)bindings.size());
    }
}

void GpuRenderPass::BindIndexBuffer(const SDL_GPUBufferBinding& binding, SDL_GPUIndexElementSize index_element_size)
{
    SDL_assert(m_render_pass);
    if (m_render_pass)
    {
        SDL_BindGPUIndexBuffer(m_render_pass, &binding, index_element_size);
    }
}

void GpuRenderPass::BindVertShaderSamplers(uint32_t first_slot, std::span<const SDL_GPUTextureSamplerBinding> texture_sampler_bindings)
{
    SDL_assert(m_render_pass);
    if (m_render_pass)
    {
        SDL_BindGPUVertexSamplers(m_render_pass, first_slot, texture_sampler_bindings.data(), (uint32_t)texture_sampler_bindings.size());
    }
}

void GpuRenderPass::BindVertShaderStorageTextures(uint32_t first_slot, std::span<const GpuTexture*> storage_textures)
{
    SDL_assert(m_render_pass);
    if (m_render_pass)
    {
        for (uint32_t i = 0; i < (uint32_t)storage_textures.size(); ++i)
        {
            const GpuTexture* texture = storage_textures[i];
            SDL_assert(texture);
            SDL_GPUTexture* gpu_texture = texture->Get();
            SDL_assert(gpu_texture);
            SDL_BindGPUVertexStorageTextures(m_render_pass, first_slot + i, &gpu_texture, 1);
        }
    }
}

void GpuRenderPass::BindVertShaderStorageBuffers(uint32_t first_slot, std::span<const GpuBuffer*> storage_buffers)
{
    SDL_assert(m_render_pass);
    if (m_render_pass)
    {
        for (uint32_t i = 0; i < (uint32_t)storage_buffers.size(); ++i)
        {
            const GpuBuffer* buffer = storage_buffers[i];
            SDL_assert(buffer);
            SDL_GPUBuffer* gpu_buffer = buffer->Get();
            SDL_assert(gpu_buffer);
            SDL_BindGPUVertexStorageBuffers(m_render_pass, first_slot + i, &gpu_buffer, 1);
        }
    }
}

void GpuRenderPass::BindFragShaderSamplers(uint32_t first_slot, std::span<const SDL_GPUTextureSamplerBinding> texture_sampler_bindings)
{
    SDL_assert(m_render_pass);
    if (m_render_pass)
    {
        SDL_BindGPUFragmentSamplers(m_render_pass, first_slot, texture_sampler_bindings.data(), (uint32_t)texture_sampler_bindings.size());
    }
}

void GpuRenderPass::BindFragShaderStorageTextures(uint32_t first_slot, std::span<const GpuTexture*> storage_textures)
{
    SDL_assert(m_render_pass);
    if (m_render_pass)
    {
        for (uint32_t i = 0; i < (uint32_t)storage_textures.size(); ++i)
        {
            const GpuTexture* texture = storage_textures[i];
            SDL_assert(texture);
            SDL_GPUTexture* gpu_texture = texture->Get();
            SDL_assert(gpu_texture);
            SDL_BindGPUFragmentStorageTextures(m_render_pass, first_slot + i, &gpu_texture, 1);
        }
    }
}

void GpuRenderPass::BindFragShaderStorageBuffers(uint32_t first_slot, std::span<const GpuBuffer*> storage_buffers)
{
    SDL_assert(m_render_pass);
    if (m_render_pass)
    {
        for (uint32_t i = 0; i < (uint32_t)storage_buffers.size(); ++i)
        {
            const GpuBuffer* buffer = storage_buffers[i];
            SDL_assert(buffer);
            SDL_GPUBuffer* gpu_buffer = buffer->Get();
            SDL_assert(gpu_buffer);
            SDL_BindGPUFragmentStorageBuffers(m_render_pass, first_slot + i, &gpu_buffer, 1);
        }
    }
}

void GpuRenderPass::DrawIndexed(uint32_t num_indices, uint32_t num_instances, uint32_t first_index, Sint32 vertex_offset, uint32_t first_instance)
{
    SDL_assert(m_render_pass);
    if (m_render_pass)
    {
        SDL_DrawGPUIndexedPrimitives(m_render_pass, num_indices, num_instances, first_index, vertex_offset, first_instance);
    }
}

void GpuRenderPass::Draw(uint32_t num_vertices, uint32_t num_instances, uint32_t first_vertex, uint32_t first_instance)
{
    SDL_assert(m_render_pass);
    if (m_render_pass)
    {
        SDL_DrawGPUPrimitives(m_render_pass, num_vertices, num_instances, first_vertex, first_instance);
    }
}

void GpuRenderPass::DrawIndirect(const GpuBuffer& buffer, uint32_t offset, uint32_t draw_count)
{
    SDL_assert(m_render_pass);
    if (m_render_pass)
    {
        SDL_GPUBuffer* gpu_buffer = buffer.Get();
        SDL_assert(gpu_buffer);
        SDL_DrawGPUPrimitivesIndirect(m_render_pass, gpu_buffer, offset, draw_count);
    }
}

void GpuRenderPass::DrawIndexedIndirect(const GpuBuffer& buffer, uint32_t offset, uint32_t draw_count)
{
    SDL_assert(m_render_pass);
    if (m_render_pass)
    {
        SDL_GPUBuffer* gpu_buffer = buffer.Get();
        SDL_assert(gpu_buffer);
        SDL_DrawGPUIndexedPrimitivesIndirect(m_render_pass, gpu_buffer, offset, draw_count);
    }
}

void GpuRenderPass::EndRenderPass()
{
    SDL_assert(m_cmd_buffer && m_render_pass);
    if (m_cmd_buffer && m_render_pass)
    {
        SDL_EndGPURenderPass(m_render_pass);
        m_render_pass = nullptr;
    }
}

GpuComputePass::GpuComputePass(const GpuCmdBuffer& cmd)
    : m_cmd_buffer(cmd.Get())
    , m_compute_pass(nullptr)
{
    SDL_assert(m_cmd_buffer);
}

bool GpuComputePass::BeginComputePass(std::span<const SDL_GPUStorageTextureReadWriteBinding> storage_texture_bindings, std::span<const SDL_GPUStorageBufferReadWriteBinding> storage_buffer_bindings)
{
    SDL_assert(m_cmd_buffer);
    if (!m_cmd_buffer)
    {
        return false;
    }

    m_compute_pass = SDL_BeginGPUComputePass(
        m_cmd_buffer,
        storage_texture_bindings.data(),
        (uint32_t)storage_texture_bindings.size(),
        storage_buffer_bindings.data(),
        (uint32_t)storage_buffer_bindings.size()
    );
    return m_compute_pass != nullptr;
}

void GpuComputePass::BindComputePipeline(const GpuComputePipeline& compute_pipeline)
{
    SDL_assert(m_compute_pass);
    if (m_compute_pass)
    {
        SDL_GPUComputePipeline* gpu_compute_pipeline = compute_pipeline.Get();
        SDL_assert(gpu_compute_pipeline);
        SDL_BindGPUComputePipeline(m_compute_pass, gpu_compute_pipeline);
    }
}

void GpuComputePass::BindComputeSamplers(uint32_t first_slot, std::span<const SDL_GPUTextureSamplerBinding> texture_sampler_bindings)
{
    SDL_assert(m_compute_pass);
    if (m_compute_pass)
    {
        SDL_BindGPUComputeSamplers(m_compute_pass, first_slot, texture_sampler_bindings.data(), (uint32_t)texture_sampler_bindings.size());
    }
}

void GpuComputePass::BindComputeStorageTextures(uint32_t first_slot, std::span<const GpuTexture*> storage_textures)
{
    SDL_assert(m_compute_pass);
    if (m_compute_pass)
    {
        for (uint32_t i = 0; i < (uint32_t)storage_textures.size(); ++i)
        {
            const GpuTexture* texture = storage_textures[i];
            SDL_assert(texture);
            SDL_GPUTexture* gpu_texture = texture->Get();
            SDL_assert(gpu_texture);

            SDL_BindGPUComputeStorageTextures(m_compute_pass, first_slot + i, &gpu_texture, 1);
        }
    }
}

void GpuComputePass::BindComputeStorageBuffers(uint32_t first_slot, std::span<const GpuBuffer*> storage_buffers)
{
    SDL_assert(m_compute_pass);
    if (m_compute_pass)
    {
        for (uint32_t i = 0; i < (uint32_t)storage_buffers.size(); ++i)
        {
            const GpuBuffer* buffer = storage_buffers[i];
            SDL_assert(buffer);
            SDL_GPUBuffer* gpu_buffer = buffer->Get();
            SDL_assert(gpu_buffer);
            SDL_BindGPUComputeStorageBuffers(m_compute_pass, first_slot + i, &gpu_buffer, 1);
        }
    }
}

void GpuComputePass::Dispatch(uint32_t groupcount_x, uint32_t groupcount_y, uint32_t groupcount_z)
{
    SDL_assert(m_compute_pass);
    if (m_compute_pass)
    {
        SDL_DispatchGPUCompute(m_compute_pass, groupcount_x, groupcount_y, groupcount_z);
    }
}

void GpuComputePass::DispatchIndirect(const GpuBuffer& buffer, uint32_t offset)
{
    SDL_assert(m_compute_pass);
    if (m_compute_pass)
    {
        SDL_GPUBuffer* gpu_buffer = buffer.Get();
        SDL_DispatchGPUComputeIndirect(m_compute_pass, gpu_buffer, offset);
    }
}

void GpuComputePass::EndComputePass()
{
    SDL_assert(m_cmd_buffer && m_compute_pass);
    if (m_cmd_buffer && m_compute_pass)
    {
        SDL_EndGPUComputePass(m_compute_pass);
        m_compute_pass = nullptr;
    }
}

GpuCopyPass::GpuCopyPass(const GpuCmdBuffer& cmd)
    : m_cmd_buffer(cmd.Get())
    , m_copy_pass(nullptr)
{
    SDL_assert(m_cmd_buffer);
}

bool GpuCopyPass::BeginCopyPass()
{
    SDL_assert(m_cmd_buffer);
    if (!m_cmd_buffer)
    {
        return false;
    }
    m_copy_pass = SDL_BeginGPUCopyPass(m_cmd_buffer);
    return m_copy_pass != nullptr;
}

void GpuCopyPass::UploadToGPUTexture(const SDL_GPUTextureTransferInfo& source, const SDL_GPUTextureRegion& destination, bool cycle)
{
    SDL_assert(m_cmd_buffer && m_copy_pass);
    if (m_cmd_buffer && m_copy_pass)
    {
        SDL_UploadToGPUTexture(m_copy_pass, &source, &destination, cycle);
    }
}

void GpuCopyPass::UploadToGPUBuffer(const SDL_GPUTransferBufferLocation& source, const SDL_GPUBufferRegion& destination, bool cycle)
{
    SDL_assert(m_cmd_buffer && m_copy_pass);
    if (m_cmd_buffer && m_copy_pass)
    {
        SDL_UploadToGPUBuffer(m_copy_pass, &source, &destination, cycle);
    }
}

void GpuCopyPass::CopyGPUTextureToTexture(const SDL_GPUTextureLocation& source, const SDL_GPUTextureLocation& destination, Uint32 w, Uint32 h, Uint32 d, bool cycle)
{
    SDL_assert(m_cmd_buffer && m_copy_pass);
    if (m_cmd_buffer && m_copy_pass)
    {
        SDL_CopyGPUTextureToTexture(m_copy_pass, &source, &destination, w, h, d, cycle);
    }
}

void GpuCopyPass::CopyGPUBufferToBuffer(const SDL_GPUBufferLocation& source, const SDL_GPUBufferLocation& destination, Uint32 size, bool cycle)
{
    SDL_assert(m_cmd_buffer && m_copy_pass);
    if (m_cmd_buffer && m_copy_pass)
    {
        SDL_CopyGPUBufferToBuffer(m_copy_pass, &source, &destination, size, cycle);
    }
}

void GpuCopyPass::DownloadFromGPUTexture(const SDL_GPUTextureRegion& source, const SDL_GPUTextureTransferInfo& destination)
{
    SDL_assert(m_cmd_buffer && m_copy_pass);
    if (m_cmd_buffer && m_copy_pass)
    {
        SDL_DownloadFromGPUTexture(m_copy_pass, &source, &destination);
    }
}

void GpuCopyPass::DownloadFromGPUBuffer(const SDL_GPUBufferRegion& source, const SDL_GPUTransferBufferLocation& destination)
{
    SDL_assert(m_cmd_buffer && m_copy_pass);
    if (m_cmd_buffer && m_copy_pass)
    {
        SDL_DownloadFromGPUBuffer(m_copy_pass, &source, &destination);
    }
}

void GpuCopyPass::EndCopyPass()
{
    SDL_assert(m_cmd_buffer && m_copy_pass);
    if (m_cmd_buffer && m_copy_pass)
    {
        SDL_EndGPUCopyPass(m_copy_pass);
        m_copy_pass = nullptr;
    }
}
