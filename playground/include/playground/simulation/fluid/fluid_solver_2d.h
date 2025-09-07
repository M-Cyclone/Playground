#pragma once

#include <engine/gpu/gpu_types.h>

namespace FluidConsts
{
    // unit: kg*m/s^2
    static constexpr float k_gravity = 9.8f;

    // unit: s
    static constexpr float k_sim_dt = 1.0f / 60.0f;
    static constexpr float k_sim_rdt = 60.0f;

    // unit: m/s
    // We assume the max of the speed 10m/s.
    // In fact, some very fast water flow are only 2~4m/s.
    static constexpr float k_max_vel = 10.0f;

    // unit: m
    static constexpr float k_min_dx = k_sim_dt * k_max_vel / 5.0f;
    // unit: m^-1
    static constexpr float k_max_rdx = 1.0f / k_min_dx;

    // unit: kg/m^3
    static constexpr float k_water_density = 1000.0f;
    static constexpr float k_water_inv_density = 1.0f / k_water_density;

    static constexpr uint32_t k_num_thread_count = 8;
}

template <typename T>
class PingpongBuffer
{
public:
    template <typename TCreateInfo>
    explicit PingpongBuffer(GpuDevice& gpu_device, const TCreateInfo& create_info)
        : m_buffers{ T(gpu_device, create_info), T(gpu_device, create_info) }
    {
    }

    int32_t GetCurrIndex() const { return m_curr_buffer_index; }
    int32_t GetPrevIndex() const { return 1 - m_curr_buffer_index; }
    void Swap() { m_curr_buffer_index = 1 - m_curr_buffer_index; }

    typename T::TSdlObj* GetCurr() const { return m_buffers[GetCurrIndex()].Get(); }
    typename T::TSdlObj* GetPrev() const { return m_buffers[GetPrevIndex()].Get(); }

private:
    T m_buffers[2];
    int32_t m_curr_buffer_index = 0;
};

class FluidSolver2d
{
public:
    FluidSolver2d(GpuDevice& gpu_device, float range, float expected_dx = FluidConsts::k_min_dx);

private:
    float m_range; // unit: m.
    float m_dx;  // unit: m.
    float m_rdx; // unit: m^-1.
    int32_t m_resolution;

public:
    void ApplyZeroInitializationCondition(GpuDevice& gpu_device);
    void ApplyGaussianDistributionPresure(GpuDevice& gpu_device);

    SDL_GPUTexture* GetPresureField() const { return m_presure_field->GetCurr(); }

private:
    // unit: m/s.
    std::unique_ptr<PingpongBuffer<GpuTexture>> m_velocity_field_u;
    std::unique_ptr<PingpongBuffer<GpuTexture>> m_velocity_field_v;

    std::unique_ptr<GpuTexture> m_velocity_divergence_field;

    // unit: kg/(m*s^2)
    std::unique_ptr<PingpongBuffer<GpuTexture>> m_presure_field;

public:
    void Tick(GpuCmdBuffer& cmd, float dt);
    //void RenderToRt();

private:
    float m_elapsed_time = 0.0f;

    std::unique_ptr<GpuSampler> m_bilinear_field_sampler;

    std::unique_ptr<GpuComputePipeline> m_advection_edge_pipeline;
    std::unique_ptr<GpuComputePipeline> m_advection_center_pipeline;
    std::unique_ptr<GpuComputePipeline> m_add_force_pipeline;
    std::unique_ptr<GpuComputePipeline> m_cal_divergence_pipeline;
    std::unique_ptr<GpuComputePipeline> m_jacobi_iteration_pipeline;
    std::unique_ptr<GpuComputePipeline> m_subtract_presure_pipeline;

    std::unique_ptr<GpuGraphicsPipeline> m_present_pipeline;
};
