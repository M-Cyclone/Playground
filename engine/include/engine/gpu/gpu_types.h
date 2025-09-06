#pragma once

#include "engine/system/macro_define.h"

#include <SDL3/SDL.h>

#include <span>
#include <memory>

class GpuDevice;
class GpuComputePipeline;
class GpuGraphicsPipeline;
class GpuSampler;
class GpuShader;
class GpuTexture;
class GpuBuffer;
class GpuTransferBuffer;
class GpuCmdBuffer;
class GpuRenderPass;
class GpuComputePass;
class GpuCopyPass;

template <typename TSdl, typename TWrapper>
struct GpuObject
{
    using TSdlObj = TSdl;
    TSdl* Get() const { return static_cast<const TWrapper*>(this)->GetImpl(); }
};

class GpuDevice : public GpuObject<SDL_GPUDevice, GpuDevice>
{
    friend class GpuManager;
    friend class GpuObject<SDL_GPUDevice, GpuDevice>;

public:
    GpuDevice(SDL_GPUShaderFormat format_flags, const char* backend_name);
    explicit GpuDevice(SDL_PropertiesID props);

    GpuDevice(const GpuDevice&) = delete;
    GpuDevice& operator=(const GpuDevice&) = delete;
    GpuDevice(GpuDevice&&) = delete;
    GpuDevice& operator=(GpuDevice&&) = delete;

    ~GpuDevice();

private:
    SDL_GPUDevice* GetImpl() const { return m_device; }

public:
    static int32_t GetNumGpuDrivers();
    static const char* GetGpuDriverName(int32_t index);

    const char* GetGpuDeviceDriver() const;

    SDL_GPUShaderFormat GetGpuShaderFormat() const;

public:
    bool CheckWindowSuppoertsSwapchainComposition(SDL_Window* window, SDL_GPUSwapchainComposition swapchain_composition) const;
    bool CheckWindowSupportsPresentMode(SDL_Window* window, SDL_GPUPresentMode present_mode) const;

    bool ClaimWindow(SDL_Window* window);
    void ReleaseWindow();

    bool SetSwapchainParameters(SDL_Window* window, SDL_GPUSwapchainComposition swapchain_composition, SDL_GPUPresentMode present_mode);
    bool SetAllowedFramesInFlight(uint32_t allowed_frames_in_flight);

    SDL_GPUTextureFormat GetSwapchainTextureFormat(SDL_Window* window) const;

    bool WaitForSwapchain(SDL_Window* window) const;

public:
    bool WaitForGpuIdle();
    bool WaitForGpuFences(bool wait_all, std::span<SDL_GPUFence* const> fences);
    bool QueryGpuFence(SDL_GPUFence* fence) const;
    void ReleaseGpuFence(SDL_GPUFence* fence);

public:
    static uint32_t GetGpuTextureFormatTexelBlockSize(SDL_GPUTextureFormat format);

    bool CheckTextureSupportsFormat(SDL_GPUTextureFormat format, SDL_GPUTextureType type, SDL_GPUTextureUsageFlags usage) const;
    bool CheckTextureSupportsSampleCount(SDL_GPUTextureFormat format, SDL_GPUSampleCount sample_count) const;

    static uint32_t CalculateGPUTextureFormatSize(SDL_GPUTextureFormat format, Uint32 width, Uint32 height, Uint32 depth_or_layer_count);

private:
    SDL_GPUDevice* m_device = nullptr;
    SDL_Window* m_window = nullptr;
};

class GpuComputePipeline : public GpuObject<SDL_GPUComputePipeline, GpuComputePipeline>
{
    friend class GpuManager;
    friend class GpuObject<SDL_GPUComputePipeline, GpuComputePipeline>;

public:
    GpuComputePipeline(const GpuDevice& gpu_device, const SDL_GPUComputePipelineCreateInfo& create_info);
    GpuComputePipeline(const GpuComputePipeline&) = delete;
    GpuComputePipeline& operator=(const GpuComputePipeline&) = delete;
    GpuComputePipeline(GpuComputePipeline&&) = delete;
    GpuComputePipeline& operator=(GpuComputePipeline&&) = delete;
    ~GpuComputePipeline();

private:
    SDL_GPUComputePipeline* GetImpl() const { return m_pipeline; }

private:
    SDL_GPUDevice* m_device = nullptr;
    SDL_GPUComputePipeline* m_pipeline = nullptr;
};

class GpuGraphicsPipeline : public GpuObject<SDL_GPUGraphicsPipeline, GpuGraphicsPipeline>
{
    friend class GpuManager;
    friend class GpuObject<SDL_GPUGraphicsPipeline, GpuGraphicsPipeline>;

public:
    GpuGraphicsPipeline(const GpuDevice& gpu_device, const SDL_GPUGraphicsPipelineCreateInfo& create_info);
    GpuGraphicsPipeline(const GpuGraphicsPipeline&) = delete;
    GpuGraphicsPipeline& operator=(const GpuGraphicsPipeline&) = delete;
    GpuGraphicsPipeline(GpuGraphicsPipeline&&) = delete;
    GpuGraphicsPipeline& operator=(GpuGraphicsPipeline&&) = delete;
    ~GpuGraphicsPipeline();

private:
    SDL_GPUGraphicsPipeline* GetImpl() const { return m_pipeline; }

private:
    SDL_GPUDevice* m_device = nullptr;
    SDL_GPUGraphicsPipeline* m_pipeline = nullptr;
};

class GpuSampler : public GpuObject<SDL_GPUSampler, GpuSampler>
{
    friend class GpuManager;
    friend class GpuObject<SDL_GPUSampler, GpuSampler>;

public:
    GpuSampler(const GpuDevice& gpu_device, const SDL_GPUSamplerCreateInfo& create_info);
    GpuSampler(const GpuSampler&) = delete;
    GpuSampler& operator=(const GpuSampler&) = delete;
    GpuSampler(GpuSampler&&) = delete;
    GpuSampler& operator=(GpuSampler&&) = delete;
    ~GpuSampler();

private:
    SDL_GPUSampler* GetImpl() const { return m_sampler; }

private:
    SDL_GPUDevice* m_device = nullptr;
    SDL_GPUSampler* m_sampler = nullptr;
};

class GpuShader : public GpuObject<SDL_GPUShader, GpuShader>
{
    friend class GpuManager;
    friend class GpuObject<SDL_GPUShader, GpuShader>;

public:
    GpuShader(const GpuDevice& gpu_device, const SDL_GPUShaderCreateInfo& create_info);
    GpuShader(const GpuShader&) = delete;
    GpuShader& operator=(const GpuShader&) = delete;
    GpuShader(GpuShader&&) = delete;
    GpuShader& operator=(GpuShader&&) = delete;
    ~GpuShader();

private:
    SDL_GPUShader* GetImpl() const { return m_shader; }

private:
    SDL_GPUDevice* m_device = nullptr;
    SDL_GPUShader* m_shader = nullptr;
};

class GpuTexture : public GpuObject<SDL_GPUTexture, GpuTexture>
{
    friend class GpuManager;
    friend class GpuObject<SDL_GPUTexture, GpuTexture>;

public:
    GpuTexture(const GpuDevice& gpu_device, const SDL_GPUTextureCreateInfo& create_info);
    GpuTexture(const GpuTexture&) = delete;
    GpuTexture& operator=(const GpuTexture&) = delete;
    GpuTexture(GpuTexture&&) = delete;
    GpuTexture& operator=(GpuTexture&&) = delete;
    ~GpuTexture();

private:
    SDL_GPUTexture* GetImpl() const { return m_texture; }

public:
    void SetName(const char* new_name);

private:
    SDL_GPUDevice* m_device = nullptr;
    SDL_GPUTexture* m_texture = nullptr;
};

class GpuBuffer : public GpuObject<SDL_GPUBuffer, GpuBuffer>
{
    friend class GpuManager;
    friend class GpuObject<SDL_GPUBuffer, GpuBuffer>;

public:
    GpuBuffer(const GpuDevice& gpu_device, const SDL_GPUBufferCreateInfo& create_info);
    GpuBuffer(const GpuBuffer&) = delete;
    GpuBuffer& operator=(const GpuBuffer&) = delete;
    GpuBuffer(GpuBuffer&&) = delete;
    GpuBuffer& operator=(GpuBuffer&&) = delete;
    ~GpuBuffer();

private:
    SDL_GPUBuffer* GetImpl() const { return m_buffer; }

public:
    void SetName(const char* new_name);

private:
    SDL_GPUDevice* m_device = nullptr;
    SDL_GPUBuffer* m_buffer = nullptr;
};

class GpuTransferBuffer : public GpuObject<SDL_GPUTransferBuffer, GpuTransferBuffer>
{
    friend class GpuManager;
    friend class GpuObject<SDL_GPUTransferBuffer, GpuTransferBuffer>;

public:
    GpuTransferBuffer(const GpuDevice& gpu_device, const SDL_GPUTransferBufferCreateInfo& create_info);
    GpuTransferBuffer(const GpuTransferBuffer&) = delete;
    GpuTransferBuffer& operator=(const GpuTransferBuffer&) = delete;
    GpuTransferBuffer(GpuTransferBuffer&&) = delete;
    GpuTransferBuffer& operator=(GpuTransferBuffer&&) = delete;
    ~GpuTransferBuffer();

private:
    SDL_GPUTransferBuffer* GetImpl() const { return m_transfer_buffer; }

public:
    void* Map(bool cycle);
    void Unmap();

private:
    SDL_GPUDevice* m_device = nullptr;
    SDL_GPUTransferBuffer* m_transfer_buffer = nullptr;
};

class GpuCmdBuffer : public GpuObject<SDL_GPUCommandBuffer, GpuCmdBuffer>
{
    friend class GpuManager;
    friend class GpuObject<SDL_GPUCommandBuffer, GpuCmdBuffer>;

public:
    explicit GpuCmdBuffer(const GpuDevice& gpu_device);
    GpuCmdBuffer(const GpuCmdBuffer&) = default;
    GpuCmdBuffer& operator=(const GpuCmdBuffer&) = default;
    GpuCmdBuffer(GpuCmdBuffer&&) = default;
    GpuCmdBuffer& operator=(GpuCmdBuffer&&) = default;

private:
    SDL_GPUCommandBuffer* GetImpl() const { return m_cmd_buffer; }

public:
    void PushVertShaderUniformData(uint32_t slot_index, const void* data, uint32_t data_size);
    void PushFragShaderUniformData(uint32_t slot_index, const void* data, uint32_t data_size);
    void PushCompShaderUniformData(uint32_t slot_index, const void* data, uint32_t data_size);

public:
    void GenerateMipmaps(SDL_GPUTexture* texture);
    void BlitTexture(const SDL_GPUBlitInfo& info);

public:
    bool AcquireSwapchainTexture(SDL_Window* window, SDL_GPUTexture*& swapchain_texture, uint32_t* swapchain_texture_width = nullptr, Uint32* swapchain_texture_height = nullptr);
    bool WaitAndAcquireSwapchainTexture(SDL_Window* window, SDL_GPUTexture*& swapchain_texture, uint32_t* swapchain_texture_width = nullptr, Uint32* swapchain_texture_height = nullptr);

public:
    bool Submit();
    SDL_GPUFence* AcquireFence();
    bool Cancel();

private:
    SDL_GPUCommandBuffer* m_cmd_buffer = nullptr;
};

class GpuRenderPass : public GpuObject<SDL_GPURenderPass, GpuRenderPass>
{
    friend class GpuManager;
    friend class GpuObject<SDL_GPURenderPass, GpuRenderPass>;

public:
    explicit GpuRenderPass(const GpuCmdBuffer& cmd);
    GpuRenderPass(const GpuRenderPass&) = default;
    GpuRenderPass& operator=(const GpuRenderPass&) = default;
    GpuRenderPass(GpuRenderPass&&) = default;
    GpuRenderPass& operator=(GpuRenderPass&&) = default;

private:
    SDL_GPURenderPass* GetImpl() const { return m_render_pass; }

public:
    bool IsRecording() const { return m_render_pass != nullptr; }

public:
    bool BeginRenderPass(std::span<const SDL_GPUColorTargetInfo> color_target_infos, const SDL_GPUDepthStencilTargetInfo* depth_stencil_target_info);

    void BindGraphicsPipeline(const GpuGraphicsPipeline& graphics_pipeline);

    void SetViewport(const SDL_GPUViewport& viewport);
    void SetScissor(const SDL_Rect& scissor);
    void SetBlendConstants(SDL_FColor blend_constants);
    void SetStencilReference(uint8_t reference);

    void BindVertexBuffers(uint32_t first_slot, std::span<const SDL_GPUBufferBinding> bindings);
    void BindIndexBuffer(const SDL_GPUBufferBinding& binding, SDL_GPUIndexElementSize index_element_size);

    void BindVertShaderSamplers(uint32_t first_slot, std::span<const SDL_GPUTextureSamplerBinding> texture_sampler_bindings);
    void BindVertShaderStorageTextures(uint32_t first_slot, std::span<SDL_GPUTexture*> storage_textures);
    void BindVertShaderStorageBuffers(uint32_t first_slot, std::span<SDL_GPUBuffer*> storage_buffers);

    void BindFragShaderSamplers(uint32_t first_slot, std::span<const SDL_GPUTextureSamplerBinding> texture_sampler_bindings);
    void BindFragShaderStorageTextures(uint32_t first_slot, std::span<SDL_GPUTexture*> storage_textures);
    void BindFragShaderStorageBuffers(uint32_t first_slot, std::span<SDL_GPUBuffer*> storage_buffers);

    void DrawIndexed(uint32_t num_indices, uint32_t num_instances, uint32_t first_index, Sint32 vertex_offset, uint32_t first_instance);
    void Draw(uint32_t num_vertices, uint32_t num_instances, uint32_t first_vertex, uint32_t first_instance);
    void DrawIndirect(SDL_GPUBuffer* buffer, uint32_t offset, uint32_t draw_count);
    void DrawIndexedIndirect(SDL_GPUBuffer* buffer, uint32_t offset, uint32_t draw_count);

    void EndRenderPass();

private:
    SDL_GPUCommandBuffer* m_cmd_buffer = nullptr;
    SDL_GPURenderPass* m_render_pass = nullptr;
};

class GpuComputePass : public GpuObject<SDL_GPUComputePass, GpuComputePass>
{
    friend class GpuManager;
    friend class GpuObject<SDL_GPUComputePass, GpuComputePass>;

public:
    explicit GpuComputePass(const GpuCmdBuffer& cmd);
    GpuComputePass(const GpuComputePass&) = default;
    GpuComputePass& operator=(const GpuComputePass&) = default;
    GpuComputePass(GpuComputePass&&) = default;
    GpuComputePass& operator=(GpuComputePass&&) = default;

private:
    SDL_GPUComputePass* GetImpl() const { return m_compute_pass; }

public:
    bool IsRecording() const { return m_compute_pass != nullptr; }

public:
    bool BeginComputePass(std::span<const SDL_GPUStorageTextureReadWriteBinding> storage_texture_bindings, std::span<const SDL_GPUStorageBufferReadWriteBinding> storage_buffer_bindings);

    void BindComputePipeline(const GpuComputePipeline& compute_pipeline);

    void BindComputeSamplers(uint32_t first_slot, std::span<const SDL_GPUTextureSamplerBinding> texture_sampler_bindings);
    void BindComputeStorageTextures(uint32_t first_slot, std::span<SDL_GPUTexture*> storage_textures);
    void BindComputeStorageBuffers(uint32_t first_slot, std::span<SDL_GPUBuffer*> storage_buffers);

    void Dispatch(uint32_t groupcount_x, uint32_t groupcount_y, uint32_t groupcount_z);
    void DispatchIndirect(SDL_GPUBuffer* buffer, uint32_t offset);

    void EndComputePass();

private:
    SDL_GPUCommandBuffer* m_cmd_buffer = nullptr;
    SDL_GPUComputePass* m_compute_pass = nullptr;
};

class GpuCopyPass : public GpuObject<SDL_GPUCopyPass, GpuCopyPass>
{
    friend class GpuManager;
    friend class GpuObject<SDL_GPUCopyPass, GpuCopyPass>;

public:
    explicit GpuCopyPass(const GpuCmdBuffer& cmd);
    GpuCopyPass(const GpuCopyPass&) = default;
    GpuCopyPass& operator=(const GpuCopyPass&) = default;
    GpuCopyPass(GpuCopyPass&&) = default;
    GpuCopyPass& operator=(GpuCopyPass&&) = default;

private:
    SDL_GPUCopyPass* GetImpl() const { return m_copy_pass; }

public:
    bool IsRecording() const { return m_copy_pass != nullptr; }

public:
    bool BeginCopyPass();

    void UploadToGPUTexture(const SDL_GPUTextureTransferInfo& source, const SDL_GPUTextureRegion& destination, bool cycle);
    void UploadToGPUBuffer(const SDL_GPUTransferBufferLocation& source, const SDL_GPUBufferRegion& destination, bool cycle);

    void CopyGPUTextureToTexture(const SDL_GPUTextureLocation& source, const SDL_GPUTextureLocation& destination, Uint32 w, Uint32 h, Uint32 d, bool cycle);
    void CopyGPUBufferToBuffer(const SDL_GPUBufferLocation& source, const SDL_GPUBufferLocation& destination, Uint32 size, bool cycle);

    void DownloadFromGPUTexture(const SDL_GPUTextureRegion& source, const SDL_GPUTextureTransferInfo& destination);
    void DownloadFromGPUBuffer(const SDL_GPUBufferRegion& source, const SDL_GPUTransferBufferLocation& destination);

    void EndCopyPass();

private:
    SDL_GPUCommandBuffer* m_cmd_buffer = nullptr;
    SDL_GPUCopyPass* m_copy_pass = nullptr;
};
