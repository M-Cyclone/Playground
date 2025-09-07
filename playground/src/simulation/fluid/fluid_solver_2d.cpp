#include "playground/simulation/fluid/fluid_solver_2d.h"

#include <cmath>

#include "advection_mac_center_comp.h"
#include "advection_mac_edge_comp.h"
#include "add_force_comp.h"
#include "cal_divergence_comp.h"
#include "projection_jacobi_iteraction_comp.h"
#include "subtract_presure_gradient_comp.h"

namespace
{
    // Find the smallest number of power of two which is larger than expected resolution.
    int32_t GetExpectedResolution(float range, float rdx)
    {
        const float expected_resolution = range * rdx;

        uint32_t res_int = std::ceil(expected_resolution);
        res_int |= res_int >> 1;
        res_int |= res_int >> 2;
        res_int |= res_int >> 4;
        res_int |= res_int >> 8;
        res_int |= res_int >> 16;

        return (int32_t)((res_int + 1) >> 1);
    }

    struct FluidFieldInfo
    {
        float dx;
        float rdx;
        float dt;
        float rdt;
        int32_t resolution;
        float inv_res;
        float inv_res_plus_one;
        float gravity;
        float density;
        float inv_density;
    };
}

FluidSolver2d::FluidSolver2d(GpuDevice& gpu_device, float range, float expected_dx)
    : m_range(range)
    , m_dx(std::max(expected_dx, FluidConsts::k_min_dx))
    , m_rdx(1.0f / m_dx)
    , m_resolution(GetExpectedResolution(m_range, m_rdx))
{
    m_dx = m_range / m_resolution;
    m_rdx = 1.0f / m_dx;

    {
        // SDL_GPU_TEXTUREUSAGE_SAMPLER is used for advection.
        SDL_GPUTextureCreateInfo default_field_create_info{};
        default_field_create_info.type = SDL_GPU_TEXTURETYPE_2D;
        default_field_create_info.format = SDL_GPU_TEXTUREFORMAT_R32_FLOAT;
        default_field_create_info.usage = SDL_GPU_TEXTUREUSAGE_COMPUTE_STORAGE_READ | SDL_GPU_TEXTUREUSAGE_COMPUTE_STORAGE_WRITE;
        default_field_create_info.layer_count_or_depth = 1;
        default_field_create_info.num_levels = 1;
        default_field_create_info.sample_count = SDL_GPU_SAMPLECOUNT_1;


        SDL_GPUTextureCreateInfo velocity_field_u_create_info = default_field_create_info;
        velocity_field_u_create_info.usage |= SDL_GPU_TEXTUREUSAGE_SAMPLER;
        velocity_field_u_create_info.width = m_resolution + 1;
        velocity_field_u_create_info.height = m_resolution;

        m_velocity_field_u = std::make_unique<PingpongBuffer<GpuTexture>>(gpu_device, velocity_field_u_create_info);


        SDL_GPUTextureCreateInfo velocity_field_v_create_info = default_field_create_info;
        velocity_field_v_create_info.usage |= SDL_GPU_TEXTUREUSAGE_SAMPLER;
        velocity_field_v_create_info.width = m_resolution;
        velocity_field_v_create_info.height = m_resolution + 1;

        m_velocity_field_v = std::make_unique<PingpongBuffer<GpuTexture>>(gpu_device, velocity_field_v_create_info);


        SDL_GPUTextureCreateInfo divergence_field_create_info = default_field_create_info;
        divergence_field_create_info.width = m_resolution;
        divergence_field_create_info.height = m_resolution;
        m_velocity_divergence_field = std::make_unique<GpuTexture>(gpu_device, divergence_field_create_info);


        SDL_GPUTextureCreateInfo presure_field_create_info = default_field_create_info;
        presure_field_create_info.width = m_resolution;
        presure_field_create_info.height = m_resolution;

        m_presure_field = std::make_unique<PingpongBuffer<GpuTexture>>(gpu_device, presure_field_create_info);
    }


    {
        SDL_GPUSamplerCreateInfo vel_field_sampler_create_info{};
        vel_field_sampler_create_info.min_filter = SDL_GPU_FILTER_LINEAR;
        vel_field_sampler_create_info.mag_filter = SDL_GPU_FILTER_LINEAR;
        vel_field_sampler_create_info.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
        vel_field_sampler_create_info.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
        vel_field_sampler_create_info.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
        vel_field_sampler_create_info.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;

        m_bilinear_field_sampler = std::make_unique<GpuSampler>(gpu_device, vel_field_sampler_create_info);
    }

    {
        SDL_GPUComputePipelineCreateInfo advection_pipeline_create_info{};
        advection_pipeline_create_info.code_size = sizeof(ADVECTION_MAC_EDGE_COMP);
        advection_pipeline_create_info.code = ADVECTION_MAC_EDGE_COMP;
        advection_pipeline_create_info.entrypoint = "main";
        advection_pipeline_create_info.format = SDL_GPU_SHADERFORMAT_DXIL;
        advection_pipeline_create_info.num_samplers = 2;
        advection_pipeline_create_info.num_readonly_storage_textures = 2;
        advection_pipeline_create_info.num_readonly_storage_buffers = 0;
        advection_pipeline_create_info.num_readwrite_storage_textures = 2;
        advection_pipeline_create_info.num_readwrite_storage_buffers = 0;
        advection_pipeline_create_info.num_uniform_buffers = 1;
        advection_pipeline_create_info.threadcount_x = FluidConsts::k_num_thread_count;
        advection_pipeline_create_info.threadcount_y = FluidConsts::k_num_thread_count;
        advection_pipeline_create_info.threadcount_z = 1;

        m_advection_edge_pipeline = std::make_unique<GpuComputePipeline>(gpu_device, advection_pipeline_create_info);
    }

    {
        SDL_GPUComputePipelineCreateInfo add_force_pipeline_create_info{};
        add_force_pipeline_create_info.code_size = sizeof(ADD_FORCE_COMP);
        add_force_pipeline_create_info.code = ADD_FORCE_COMP;
        add_force_pipeline_create_info.entrypoint = "main";
        add_force_pipeline_create_info.format = SDL_GPU_SHADERFORMAT_DXIL;
        add_force_pipeline_create_info.num_samplers = 0;
        add_force_pipeline_create_info.num_readonly_storage_textures = 2;
        add_force_pipeline_create_info.num_readonly_storage_buffers = 0;
        add_force_pipeline_create_info.num_readwrite_storage_textures = 2;
        add_force_pipeline_create_info.num_readwrite_storage_buffers = 0;
        add_force_pipeline_create_info.num_uniform_buffers = 1;
        add_force_pipeline_create_info.threadcount_x = FluidConsts::k_num_thread_count;
        add_force_pipeline_create_info.threadcount_y = FluidConsts::k_num_thread_count;
        add_force_pipeline_create_info.threadcount_z = 1;

        m_add_force_pipeline = std::make_unique<GpuComputePipeline>(gpu_device, add_force_pipeline_create_info);
    }

    {
        SDL_GPUComputePipelineCreateInfo cal_divergence_pieline_create_info{};
        cal_divergence_pieline_create_info.code_size = sizeof(CAL_DIVERGENCE_COMP);
        cal_divergence_pieline_create_info.code = CAL_DIVERGENCE_COMP;
        cal_divergence_pieline_create_info.entrypoint = "main";
        cal_divergence_pieline_create_info.format = SDL_GPU_SHADERFORMAT_DXIL;
        cal_divergence_pieline_create_info.num_samplers = 0;
        cal_divergence_pieline_create_info.num_readonly_storage_textures = 2;
        cal_divergence_pieline_create_info.num_readonly_storage_buffers = 0;
        cal_divergence_pieline_create_info.num_readwrite_storage_textures = 1;
        cal_divergence_pieline_create_info.num_readwrite_storage_buffers = 0;
        cal_divergence_pieline_create_info.num_uniform_buffers = 1;
        cal_divergence_pieline_create_info.threadcount_x = FluidConsts::k_num_thread_count;
        cal_divergence_pieline_create_info.threadcount_y = FluidConsts::k_num_thread_count;
        cal_divergence_pieline_create_info.threadcount_z = 1;

        m_cal_divergence_pipeline = std::make_unique<GpuComputePipeline>(gpu_device, cal_divergence_pieline_create_info);
    }

    {
        SDL_GPUComputePipelineCreateInfo jacobi_iteration_pipeline_create_info{};
        jacobi_iteration_pipeline_create_info.code_size = sizeof(PROJECTION_JACOBI_ITERACTION_COMP);
        jacobi_iteration_pipeline_create_info.code = PROJECTION_JACOBI_ITERACTION_COMP;
        jacobi_iteration_pipeline_create_info.entrypoint = "main";
        jacobi_iteration_pipeline_create_info.format = SDL_GPU_SHADERFORMAT_DXIL;
        jacobi_iteration_pipeline_create_info.num_samplers = 0;
        jacobi_iteration_pipeline_create_info.num_readonly_storage_textures = 2;
        jacobi_iteration_pipeline_create_info.num_readonly_storage_buffers = 0;
        jacobi_iteration_pipeline_create_info.num_readwrite_storage_textures = 1;
        jacobi_iteration_pipeline_create_info.num_readwrite_storage_buffers = 0;
        jacobi_iteration_pipeline_create_info.num_uniform_buffers = 1;
        jacobi_iteration_pipeline_create_info.threadcount_x = FluidConsts::k_num_thread_count;
        jacobi_iteration_pipeline_create_info.threadcount_y = FluidConsts::k_num_thread_count;
        jacobi_iteration_pipeline_create_info.threadcount_z = 1;

        m_jacobi_iteration_pipeline = std::make_unique<GpuComputePipeline>(gpu_device, jacobi_iteration_pipeline_create_info);
    }

    {
        SDL_GPUComputePipelineCreateInfo substruct_presure_create_info{};
        substruct_presure_create_info.code_size = sizeof(SUBTRACT_PRESURE_GRADIENT_COMP);
        substruct_presure_create_info.code = SUBTRACT_PRESURE_GRADIENT_COMP;
        substruct_presure_create_info.entrypoint = "main";
        substruct_presure_create_info.format = SDL_GPU_SHADERFORMAT_DXIL;
        substruct_presure_create_info.num_samplers = 0;
        substruct_presure_create_info.num_readonly_storage_textures = 3;
        substruct_presure_create_info.num_readonly_storage_buffers = 0;
        substruct_presure_create_info.num_readwrite_storage_textures = 2;
        substruct_presure_create_info.num_readwrite_storage_buffers = 0;
        substruct_presure_create_info.num_uniform_buffers = 1;
        substruct_presure_create_info.threadcount_x = FluidConsts::k_num_thread_count;
        substruct_presure_create_info.threadcount_y = FluidConsts::k_num_thread_count;
        substruct_presure_create_info.threadcount_z = 1;

        m_subtract_presure_pipeline = std::make_unique<GpuComputePipeline>(gpu_device, substruct_presure_create_info);
    }

    {
        SDL_GPUComputePipelineCreateInfo advection_center_create_info{};
        advection_center_create_info.code_size = sizeof(ADVECTION_MAC_CENTER_COMP);
        advection_center_create_info.code = ADVECTION_MAC_CENTER_COMP;
        advection_center_create_info.entrypoint = "main";
        advection_center_create_info.format = SDL_GPU_SHADERFORMAT_DXIL;
        advection_center_create_info.num_samplers = 2;
        advection_center_create_info.num_readonly_storage_textures = 1;
        advection_center_create_info.num_readonly_storage_buffers = 0;
        advection_center_create_info.num_readwrite_storage_textures = 1;
        advection_center_create_info.num_readwrite_storage_buffers = 0;
        advection_center_create_info.num_uniform_buffers = 1;
        advection_center_create_info.threadcount_x = FluidConsts::k_num_thread_count;
        advection_center_create_info.threadcount_y = FluidConsts::k_num_thread_count;
        advection_center_create_info.threadcount_z = 1;

        m_advection_center_pipeline = std::make_unique<GpuComputePipeline>(gpu_device, advection_center_create_info);
    }
}

void FluidSolver2d::ApplyZeroInitializationCondition(GpuDevice& gpu_device)
{
    SDL_GPUTransferBufferCreateInfo tb_info{};
    tb_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    tb_info.size = m_resolution * (m_resolution + 1) * sizeof(float);

    GpuTransferBuffer upload_buffer(gpu_device, tb_info);

    float* ptr = (float*)upload_buffer.Map(false);
    if (ptr)
    {
        std::memset(ptr, 0, tb_info.size);
        upload_buffer.Unmap();
    }

    GpuCmdBuffer cmd(gpu_device);
    GpuCopyPass copy_pass(cmd);
    if (copy_pass.BeginCopyPass())
    {
        SDL_GPUTextureTransferInfo source_buffer{};
        source_buffer.transfer_buffer = upload_buffer.Get();
        source_buffer.offset = 0;

        SDL_GPUTextureRegion target_texture{};
        target_texture.mip_level = 0;
        target_texture.layer = 0;
        target_texture.x = 0;
        target_texture.y = 0;
        target_texture.z = 0;
        target_texture.d = 1;

        {
            source_buffer.pixels_per_row = m_resolution + 1;
            source_buffer.rows_per_layer = m_resolution;

            target_texture.w = m_resolution + 1;
            target_texture.h = m_resolution;

            target_texture.texture = m_velocity_field_u->GetCurr();
            copy_pass.UploadToGPUTexture(source_buffer, target_texture, false);

            target_texture.texture = m_velocity_field_u->GetPrev();
            copy_pass.UploadToGPUTexture(source_buffer, target_texture, false);
        }

        {
            source_buffer.pixels_per_row = m_resolution;
            source_buffer.rows_per_layer = m_resolution + 1;

            target_texture.w = m_resolution;
            target_texture.h = m_resolution + 1;

            target_texture.texture = m_velocity_field_v->GetCurr();
            copy_pass.UploadToGPUTexture(source_buffer, target_texture, false);

            target_texture.texture = m_velocity_field_v->GetPrev();
            copy_pass.UploadToGPUTexture(source_buffer, target_texture, false);
        }

        {
            source_buffer.pixels_per_row = m_resolution;
            source_buffer.rows_per_layer = m_resolution;

            target_texture.w = m_resolution;
            target_texture.h = m_resolution;

            target_texture.texture = m_presure_field->GetPrev();
            copy_pass.UploadToGPUTexture(source_buffer, target_texture, false);

            target_texture.texture = m_presure_field->GetCurr();
            copy_pass.UploadToGPUTexture(source_buffer, target_texture, false);
        }

        copy_pass.EndCopyPass();
    }

    cmd.Submit();
}

void FluidSolver2d::ApplyGaussianDistributionPresure(GpuDevice& gpu_device)
{
    SDL_GPUTransferBufferCreateInfo tb_info{};
    tb_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    tb_info.size = m_resolution * (m_resolution + 1) * sizeof(float);

    GpuTransferBuffer upload_buffer(gpu_device, tb_info);

    float* ptr = (float*)upload_buffer.Map(false);
    if (ptr)
    {
        std::memset(ptr, 0, tb_info.size);
        upload_buffer.Unmap();
    }

    {
        GpuCmdBuffer cmd(gpu_device);
        GpuCopyPass copy_pass(cmd);
        if (copy_pass.BeginCopyPass())
        {
            SDL_GPUTextureTransferInfo source_buffer{};
            source_buffer.transfer_buffer = upload_buffer.Get();
            source_buffer.offset = 0;

            SDL_GPUTextureRegion target_texture{};
            target_texture.mip_level = 0;
            target_texture.layer = 0;
            target_texture.x = 0;
            target_texture.y = 0;
            target_texture.z = 0;
            target_texture.d = 1;

            {
                source_buffer.pixels_per_row = m_resolution + 1;
                source_buffer.rows_per_layer = m_resolution;

                target_texture.w = m_resolution + 1;
                target_texture.h = m_resolution;

                target_texture.texture = m_velocity_field_u->GetCurr();
                copy_pass.UploadToGPUTexture(source_buffer, target_texture, false);

                target_texture.texture = m_velocity_field_u->GetPrev();
                copy_pass.UploadToGPUTexture(source_buffer, target_texture, false);
            }

            {
                source_buffer.pixels_per_row = m_resolution;
                source_buffer.rows_per_layer = m_resolution + 1;

                target_texture.w = m_resolution;
                target_texture.h = m_resolution + 1;

                target_texture.texture = m_velocity_field_v->GetCurr();
                copy_pass.UploadToGPUTexture(source_buffer, target_texture, false);

                target_texture.texture = m_velocity_field_v->GetPrev();
                copy_pass.UploadToGPUTexture(source_buffer, target_texture, false);
            }

            copy_pass.EndCopyPass();
        }
        cmd.Submit();
    }


    {
        GpuCmdBuffer cmd(gpu_device);
        ptr = (float*)upload_buffer.Map(false);
        if (ptr)
        {
            const float sigma = m_range * 0.001f;
            const float sigma_sqr = sigma * sigma;
            const float inv_coe = 1.0f / (2.0f * sigma_sqr);
            for (int32_t y = 0; y < m_resolution; ++y)
            {
                for (int32_t x = 0; x < m_resolution; ++x)
                {
                    const int32_t idx = x + y * m_resolution;

                    const float u = (x + 0.5f) / m_resolution;
                    const float v = (y + 0.5f) / m_resolution;

                    const float pos_x = u * 2.0f - 1.0f;
                    const float pos_y = v * 2.0f - 1.0f;

                    ptr[idx] = 100000.0f * std::exp(-inv_coe * (pos_x * pos_x + pos_y * pos_y));
                }
            }

            upload_buffer.Unmap();
        }

        GpuCopyPass copy_pass(cmd);
        if (copy_pass.BeginCopyPass())
        {
            SDL_GPUTextureTransferInfo source_buffer{};
            source_buffer.transfer_buffer = upload_buffer.Get();
            source_buffer.offset = 0;

            SDL_GPUTextureRegion target_texture{};
            target_texture.mip_level = 0;
            target_texture.layer = 0;
            target_texture.x = 0;
            target_texture.y = 0;
            target_texture.z = 0;
            target_texture.d = 1;

            {
                source_buffer.pixels_per_row = m_resolution;
                source_buffer.rows_per_layer = m_resolution;

                target_texture.w = m_resolution;
                target_texture.h = m_resolution;

                target_texture.texture = m_presure_field->GetPrev();
                copy_pass.UploadToGPUTexture(source_buffer, target_texture, false);

                target_texture.texture = m_presure_field->GetCurr();
                copy_pass.UploadToGPUTexture(source_buffer, target_texture, false);
            }

            copy_pass.EndCopyPass();
        }

        cmd.Submit();
    }
}

void FluidSolver2d::AddAdvectedField(GpuDevice& gpu_device, EAdvectedFieldType field_type, int32_t resolution)
{
    if (m_advected_fields.find(field_type) == m_advected_fields.end())
    {
        SDL_GPUTextureCreateInfo create_info{};
        create_info.type = SDL_GPU_TEXTURETYPE_2D;
        create_info.format = SDL_GPU_TEXTUREFORMAT_R32_FLOAT;
        create_info.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COMPUTE_STORAGE_READ | SDL_GPU_TEXTUREUSAGE_COMPUTE_STORAGE_WRITE;
        create_info.width = resolution;
        create_info.height = resolution;
        create_info.layer_count_or_depth = 1;
        create_info.num_levels = 1;
        create_info.sample_count = SDL_GPU_SAMPLECOUNT_1;

        m_advected_fields.emplace(field_type, std::make_unique<PingpongBuffer<GpuTexture>>(gpu_device, create_info));
        m_advected_field_resolutions[field_type] = resolution;
    }
}

SDL_GPUTexture* FluidSolver2d::GetTypedAdvectedField(EAdvectedFieldType field_type) const
{
    auto it = m_advected_fields.find(field_type);
    return it != m_advected_fields.end() ? it->second->GetCurr() : nullptr;
}

SDL_GPUTexture* FluidSolver2d::GetTypedAdvectedFieldPrev(EAdvectedFieldType field_type) const
{
    auto it = m_advected_fields.find(field_type);
    return it != m_advected_fields.end() ? it->second->GetCurr() : nullptr;
}

void FluidSolver2d::Tick(GpuCmdBuffer& cmd, float dt)
{
    m_elapsed_time += dt;
    if (m_elapsed_time >= FluidConsts::k_sim_dt)
    {
        // This makes we only tick one time event dt is extremely large.
        m_elapsed_time = std::fmod(m_elapsed_time, FluidConsts::k_sim_dt);

        FluidFieldInfo field_info{};
        field_info.dx = m_dx;
        field_info.rdx = m_rdx;
        field_info.dt = FluidConsts::k_sim_dt;
        field_info.rdt = FluidConsts::k_sim_rdt;
        field_info.resolution = m_resolution;
        field_info.inv_res = 1.0f / m_resolution;
        field_info.inv_res_plus_one = 1.0f / (m_resolution + 1);
        field_info.gravity = FluidConsts::k_gravity;
        field_info.density = FluidConsts::k_water_density;
        field_info.inv_density = FluidConsts::k_water_inv_density;

        cmd.PushCompShaderUniformData(0, &field_info, sizeof(field_info));

        {
            m_velocity_field_u->Swap();
            m_velocity_field_v->Swap();

            SDL_GPUStorageTextureReadWriteBinding advection_result_bindings[2] = {};
            advection_result_bindings[0].texture = m_velocity_field_u->GetCurr();
            advection_result_bindings[1].texture = m_velocity_field_v->GetCurr();

            GpuComputePass advection_pass(cmd);
            if (advection_pass.BeginComputePass(advection_result_bindings, {}))
            {
                advection_pass.BindComputePipeline(*m_advection_edge_pipeline);

                SDL_GPUTextureSamplerBinding prev_vel_bindings[2] = {};
                prev_vel_bindings[0].texture = m_velocity_field_u->GetPrev();
                prev_vel_bindings[0].sampler = m_bilinear_field_sampler->Get();
                prev_vel_bindings[1].texture = m_velocity_field_v->GetPrev();
                prev_vel_bindings[1].sampler = m_bilinear_field_sampler->Get();

                advection_pass.BindComputeSamplers(0, prev_vel_bindings);

                SDL_GPUTexture* prev_edge_fields[2] = { m_velocity_field_u->GetPrev(), m_velocity_field_v->GetPrev() };
                advection_pass.BindComputeStorageTextures(0, prev_edge_fields);

                advection_pass.Dispatch((m_resolution + FluidConsts::k_num_thread_count) / FluidConsts::k_num_thread_count, m_resolution / FluidConsts::k_num_thread_count, 1);

                advection_pass.EndComputePass();
            }
        }
        {
            // Now curr buffer is the advection result.
            // However, we don't want to read and write a texture simultaneously, because it may be unsafe and efficient.
            // So we still need to read from advection result and write new result into another buffer.
            // We swap the buffer, which means we read the advection result from GetPrev() buffer, and write to the real output.
            m_velocity_field_u->Swap();
            m_velocity_field_v->Swap();

            SDL_GPUStorageTextureReadWriteBinding add_force_result_bindings[2] = {};
            add_force_result_bindings[0].texture = m_velocity_field_u->GetCurr();
            add_force_result_bindings[1].texture = m_velocity_field_v->GetCurr();

            GpuComputePass add_force_pass(cmd);
            if (add_force_pass.BeginComputePass(add_force_result_bindings, {}))
            {
                add_force_pass.BindComputePipeline(*m_add_force_pipeline);

                SDL_GPUTexture* advection_results[2] = { m_velocity_field_u->GetPrev(), m_velocity_field_v->GetPrev() };
                add_force_pass.BindComputeStorageTextures(0, advection_results);

                add_force_pass.Dispatch((m_resolution + FluidConsts::k_num_thread_count) / FluidConsts::k_num_thread_count, m_resolution / FluidConsts::k_num_thread_count, 1);

                add_force_pass.EndComputePass();
            }
        }
        {
            SDL_GPUStorageTextureReadWriteBinding divergence_result_bindings[1] = {};
            divergence_result_bindings[0].texture = m_velocity_divergence_field->Get();

            GpuComputePass cal_divergence_pass(cmd);
            if (cal_divergence_pass.BeginComputePass(divergence_result_bindings, {}))
            {
                cal_divergence_pass.BindComputePipeline(*m_cal_divergence_pipeline);

                SDL_GPUTexture* advection_results[2] = { m_velocity_field_u->GetCurr(), m_velocity_field_v->GetCurr() };
                cal_divergence_pass.BindComputeStorageTextures(0, advection_results);

                cal_divergence_pass.Dispatch(m_resolution / FluidConsts::k_num_thread_count, m_resolution / FluidConsts::k_num_thread_count, 1);

                cal_divergence_pass.EndComputePass();
            }
        }
        {
            static constexpr int32_t k_max_iteration_count = 50;

            for (int32_t idx = 0; idx < k_max_iteration_count; ++idx)
            {
                m_presure_field->Swap();

                SDL_GPUStorageTextureReadWriteBinding next_presure_iter_binding[1] = {};
                next_presure_iter_binding[0].texture = m_presure_field->GetCurr();

                GpuComputePass jacobi_iteration_pass(cmd);
                if (jacobi_iteration_pass.BeginComputePass(next_presure_iter_binding, {}))
                {
                    jacobi_iteration_pass.BindComputePipeline(*m_jacobi_iteration_pipeline);

                    SDL_GPUTexture* divergence_and_prev_presure[2] = { m_velocity_divergence_field->Get(), m_presure_field->GetPrev() };
                    jacobi_iteration_pass.BindComputeStorageTextures(0, divergence_and_prev_presure);

                    jacobi_iteration_pass.Dispatch(m_resolution / FluidConsts::k_num_thread_count, m_resolution / FluidConsts::k_num_thread_count, 1);

                    jacobi_iteration_pass.EndComputePass();
                }
            }
        }
        {
            m_velocity_field_u->Swap();
            m_velocity_field_v->Swap();

            SDL_GPUStorageTextureReadWriteBinding subtract_presure_vel_field_bindings[2] = {};
            subtract_presure_vel_field_bindings[0].texture = m_velocity_field_u->GetCurr();
            subtract_presure_vel_field_bindings[1].texture = m_velocity_field_v->GetCurr();

            GpuComputePass subtract_presure_pass(cmd);
            if (subtract_presure_pass.BeginComputePass(subtract_presure_vel_field_bindings, {}))
            {
                subtract_presure_pass.BindComputePipeline(*m_subtract_presure_pipeline);

                SDL_GPUTexture* input_fields[3] =
                {
                    m_velocity_field_u->GetPrev(),
                    m_velocity_field_v->GetPrev(),
                    m_presure_field->GetCurr()
                };

                subtract_presure_pass.BindComputeStorageTextures(0, input_fields);

                subtract_presure_pass.Dispatch((m_resolution + FluidConsts::k_num_thread_count) / FluidConsts::k_num_thread_count, m_resolution / FluidConsts::k_num_thread_count, 1);

                subtract_presure_pass.EndComputePass();
            }
        }
        {
            SDL_GPUTextureSamplerBinding velocity_fields[2] = {};
            velocity_fields[0].texture = m_velocity_field_u->GetCurr();
            velocity_fields[0].sampler = m_bilinear_field_sampler->Get();
            velocity_fields[1].texture = m_velocity_field_v->GetCurr();
            velocity_fields[1].sampler = m_bilinear_field_sampler->Get();

            for (auto& [type, field] : m_advected_fields)
            {
                field->Swap();

                SDL_GPUStorageTextureReadWriteBinding advected_field_bindings[1] = {};
                advected_field_bindings[0].texture = field->GetCurr();

                GpuComputePass advected_field_pass(cmd);
                if (advected_field_pass.BeginComputePass(advected_field_bindings, {}))
                {
                    advected_field_pass.BindComputePipeline(*m_advection_center_pipeline);

                    advected_field_pass.BindComputeSamplers(0, velocity_fields);

                    SDL_GPUTexture* prev_field[1] = { field->GetPrev() };
                    advected_field_pass.BindComputeStorageTextures(0, prev_field);

                    const int32_t resolution = m_advected_field_resolutions[type];
                    advected_field_pass.Dispatch((resolution + FluidConsts::k_num_thread_count - 1) / FluidConsts::k_num_thread_count, (resolution + FluidConsts::k_num_thread_count - 1) / FluidConsts::k_num_thread_count, 1);

                    advected_field_pass.EndComputePass();
                }
            }
        }
    }
}
