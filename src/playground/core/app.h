#pragma once

#include "engine/sdl_wrapper/sdl_gpu_types.h"

#include <SDL3/SDL.h>

#include <memory>

class App
{
public:
    App();
    App(const App&) = delete;
    App& operator=(const App&) = delete;
    App(App&&) = delete;
    App& operator=(App&&) = delete;
    ~App() noexcept;

public:
    int32_t Run();

private:
    int32_t Init();
    void Exit();

    void Update(float delta_seconds);
    void Render();

private:
    static void HandleEvents(SDL_Event& e);

private:
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window = { nullptr, SDL_DestroyWindow };

    std::unique_ptr<GpuDevice> m_gpu_device;

private:
    std::unique_ptr<GpuGraphicsPipeline> m_triangle_pipeline;
    std::unique_ptr<GpuGraphicsPipeline> m_to_swapchain_pipeline;

    std::unique_ptr<GpuBuffer> m_triangle_vertex_buffer;

    std::unique_ptr<GpuBuffer> m_vertex_buffer;
    std::unique_ptr<GpuBuffer> m_index_buffer;

    std::unique_ptr<GpuSampler> m_sampler;

    std::unique_ptr<GpuTexture> m_texture;
};
