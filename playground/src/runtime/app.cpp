#include "playground/runtime/app.h"

#include <engine/math/math.h>

#include "render_texture_vert.h"
#include "render_texture_frag.h"

struct Vertex
{
    Vec2 position;
    Vec2 uv;
};

App::App()
{
    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL initialization failed: %s.\n", SDL_GetError());
        exit(-1);
    }
}

App::~App() noexcept
{
    SDL_Quit();
}

int32_t App::Run()
{
    if (const int32_t init_result = Init(); init_result != 0)
    {
        return init_result;
    }

    uint64_t last_time_point = SDL_GetTicksNS();
    while (true)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                m_gpu_device->WaitForGpuIdle();
                Exit();
                return 0;
            }
            else
            {
                HandleEvents(e);
            }
        }

        const uint64_t curr_time_point = SDL_GetTicksNS();
        const uint64_t delta_time_nanoseconds = std::max<uint64_t>(curr_time_point - last_time_point, 1);
        const double delta_time_seconds = (double)delta_time_nanoseconds * 1e-9;

        Update((float)delta_time_seconds);
        Render((float)delta_time_seconds);

        last_time_point = curr_time_point;

        {
            static uint64_t accumulated_time_for_update_title = curr_time_point;
            accumulated_time_for_update_title += delta_time_nanoseconds;
            if (accumulated_time_for_update_title >= 1000'000'000)
            {
                accumulated_time_for_update_title -= 1000'000'000;

                static char title_buffer[256] = {};
                std::sprintf(title_buffer, "Misaka Playground - FPS(%.3f)", 1e9f / delta_time_nanoseconds);
                SDL_SetWindowTitle(m_window.get(), title_buffer);
            }
        }
    }
}

int32_t App::Init()
{
    m_window.reset(SDL_CreateWindow("Misaka Playground - FPS(0)", 1024, 1024, 0));
    if (!m_window)
    {
        SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "Window creation failed: %s", SDL_GetError());
        return -1;
    }

    m_gpu_device = std::make_unique<GpuDevice>(SDL_GPU_SHADERFORMAT_DXIL, nullptr);
    m_gpu_device->ClaimWindow(m_window.get());

    {
        SDL_GPUShaderCreateInfo vs_info{};
        vs_info.code_size = sizeof(RENDER_TEXTURE_VERT);
        vs_info.code = RENDER_TEXTURE_VERT;
        vs_info.entrypoint = "main";
        vs_info.format = SDL_GPU_SHADERFORMAT_DXIL;
        vs_info.stage = SDL_GPU_SHADERSTAGE_VERTEX;
        vs_info.num_samplers = 0;
        vs_info.num_storage_textures = 0;
        vs_info.num_storage_buffers = 0;
        vs_info.num_uniform_buffers = 0;

        GpuShader vert_shader(*m_gpu_device, vs_info);

        SDL_GPUShaderCreateInfo ps_info{};
        ps_info.code_size = sizeof(RENDER_TEXTURE_FRAG);
        ps_info.code = RENDER_TEXTURE_FRAG;
        ps_info.entrypoint = "main";
        ps_info.format = SDL_GPU_SHADERFORMAT_DXIL;
        ps_info.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
        ps_info.num_samplers = 1;
        ps_info.num_storage_textures = 0;
        ps_info.num_storage_buffers = 0;
        ps_info.num_uniform_buffers = 0;

        GpuShader frag_shader(*m_gpu_device, ps_info);


        SDL_GPUColorTargetDescription color_descs[1] = {};
        color_descs[0].format = m_gpu_device->GetSwapchainTextureFormat(m_window.get());

        SDL_GPUVertexBufferDescription vertex_buffer_descs[1] = {};
        vertex_buffer_descs[0].slot = 0;
        vertex_buffer_descs[0].pitch = sizeof(Vertex);
        vertex_buffer_descs[0].input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
        vertex_buffer_descs[0].instance_step_rate = 0;

        SDL_GPUVertexAttribute vertex_attributes[2] = {};
        vertex_attributes[0].location = 0;
        vertex_attributes[0].buffer_slot = 0;
        vertex_attributes[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
        vertex_attributes[0].offset = offsetof(Vertex, position);
        vertex_attributes[1].location = 1;
        vertex_attributes[1].buffer_slot = 0;
        vertex_attributes[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
        vertex_attributes[1].offset = offsetof(Vertex, uv);

        SDL_GPUGraphicsPipelineCreateInfo create_info =
        {
            .vertex_shader = vert_shader.Get(),
            .fragment_shader = frag_shader.Get(),
            .vertex_input_state =
            {
                .vertex_buffer_descriptions = vertex_buffer_descs,
                .num_vertex_buffers = (uint32_t)std::size(vertex_buffer_descs),
                .vertex_attributes = vertex_attributes,
                .num_vertex_attributes = (uint32_t)std::size(vertex_attributes),
            },
            .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
            .rasterizer_state =
            {
                .fill_mode = SDL_GPU_FILLMODE_FILL,
                .cull_mode = SDL_GPU_CULLMODE_BACK,
                .front_face = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE,
                .depth_bias_constant_factor = 0.0f,
                .depth_bias_clamp = 0.0f,
                .depth_bias_slope_factor = 0.0f,
                .enable_depth_bias = false,
                .enable_depth_clip = false,
            },
            .multisample_state = {},
            .depth_stencil_state = {},
            .target_info =
            {
                .color_target_descriptions = color_descs,
                .num_color_targets = (uint32_t)std::size(color_descs),
                .depth_stencil_format = {},
                .has_depth_stencil_target = false,
            },
        };

        m_to_swapchain_pipeline = std::make_unique<GpuGraphicsPipeline>(*m_gpu_device, create_info);
    }

    {
        {
            const Vertex vertices[] =
            {
                { Vec2(-1.0f, -1.0f), Vec2(0.0f, 0.0f) },
                { Vec2(-1.0f, +1.0f), Vec2(0.0f, 1.0f) },
                { Vec2(+1.0f, +1.0f), Vec2(1.0f, 1.0f) },
                { Vec2(+1.0f, -1.0f), Vec2(1.0f, 0.0f) },
            };

            SDL_GPUBufferCreateInfo vb_info{};
            vb_info.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
            vb_info.size = (uint32_t)sizeof(vertices);

            m_vertex_buffer = std::make_unique<GpuBuffer>(*m_gpu_device, vb_info);

            SDL_GPUTransferBufferCreateInfo tb_info{};
            tb_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
            tb_info.size = (uint32_t)sizeof(vertices);

            GpuTransferBuffer upload_buffer(*m_gpu_device, tb_info);

            Vertex* ptr = (Vertex*)upload_buffer.Map(false);
            if (ptr)
            {
                std::memcpy(ptr, vertices, sizeof(vertices));

                upload_buffer.Unmap();
            }

            GpuCmdBuffer cmd(*m_gpu_device);
            GpuCopyPass copy_pass(cmd);
            if (copy_pass.BeginCopyPass())
            {
                SDL_GPUTransferBufferLocation source_buffer{};
                source_buffer.transfer_buffer = upload_buffer.Get();
                source_buffer.offset = 0;

                SDL_GPUBufferRegion target_buffer{};
                target_buffer.buffer = m_vertex_buffer->Get();
                target_buffer.offset = 0;
                target_buffer.size = sizeof(vertices);

                copy_pass.UploadToGPUBuffer(source_buffer, target_buffer, false);

                copy_pass.EndCopyPass();
            }

            cmd.Submit();
        }

        {
            const int16_t indices[] =
            {
                0, 2, 1,
                0, 3, 2
            };

            SDL_GPUBufferCreateInfo ib_info{};
            ib_info.usage = SDL_GPU_BUFFERUSAGE_INDEX;
            ib_info.size = (uint32_t)sizeof(indices);

            m_index_buffer = std::make_unique<GpuBuffer>(*m_gpu_device, ib_info);

            SDL_GPUTransferBufferCreateInfo tb_info{};
            tb_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
            tb_info.size = (uint32_t)sizeof(indices);

            GpuTransferBuffer upload_buffer(*m_gpu_device, tb_info);

            int16_t* ptr = (int16_t*)upload_buffer.Map(false);
            if (ptr)
            {
                std::memcpy(ptr, indices, sizeof(indices));

                upload_buffer.Unmap();
            }

            GpuCmdBuffer cmd(*m_gpu_device);
            GpuCopyPass copy_pass(cmd);
            if (copy_pass.BeginCopyPass())
            {
                SDL_GPUTransferBufferLocation source_buffer{};
                source_buffer.transfer_buffer = upload_buffer.Get();
                source_buffer.offset = 0;

                SDL_GPUBufferRegion target_buffer{};
                target_buffer.buffer = m_index_buffer->Get();
                target_buffer.offset = 0;
                target_buffer.size = sizeof(indices);

                copy_pass.UploadToGPUBuffer(source_buffer, target_buffer, false);

                copy_pass.EndCopyPass();
            }

            cmd.Submit();
        }
    }

    {
        SDL_GPUSamplerCreateInfo create_info{};
        create_info.min_filter = SDL_GPU_FILTER_LINEAR;
        create_info.mag_filter = SDL_GPU_FILTER_LINEAR;
        create_info.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
        create_info.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_MIRRORED_REPEAT;
        create_info.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_MIRRORED_REPEAT;
        create_info.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_MIRRORED_REPEAT;

        m_sampler = std::make_unique<GpuSampler>(*m_gpu_device, create_info);
    }

    {
        m_fluid_solver = std::make_unique<FluidSolver2d>(*m_gpu_device, 10.0f);
        m_fluid_solver->ApplyZeroInitializationCondition(*m_gpu_device);
        //m_fluid_solver->ApplyGaussianDistributionPresure(*m_gpu_device);
        m_fluid_solver->AddAdvectedField(*m_gpu_device, EAdvectedFieldType::Dye, 512);

        {
            SDL_GPUTransferBufferCreateInfo tb_info{};
            tb_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
            tb_info.size = 512 * 512 * sizeof(float);

            GpuTransferBuffer upload_buffer(*m_gpu_device, tb_info);

            float* ptr = (float*)upload_buffer.Map(false);
            if (ptr)
            {
                for (int32_t y = 0; y < 512; ++y)
                {
                    for (int32_t x = 0; x < 512; ++x)
                    {
                        const int32_t idx = x + 512 * y;

                        if (x % 32 < 16)
                        {
                            if (y % 32 < 16)
                            {
                                ptr[idx] = 0.0f;
                            }
                            else
                            {
                                ptr[idx] = 1.0f;
                            }
                        }
                        else
                        {
                            if (y % 32 < 16)
                            {
                                ptr[idx] = 1.0f;
                            }
                            else
                            {
                                ptr[idx] = 0.0f;
                            }
                        }
                    }
                }

                upload_buffer.Unmap();
            }

            GpuCmdBuffer cmd(*m_gpu_device);
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
                    source_buffer.pixels_per_row = 512;
                    source_buffer.rows_per_layer = 512;

                    target_texture.w = 512;
                    target_texture.h = 512;

                    target_texture.texture = m_fluid_solver->GetTypedAdvectedField(EAdvectedFieldType::Dye);
                    copy_pass.UploadToGPUTexture(source_buffer, target_texture, false);

                    target_texture.texture = m_fluid_solver->GetTypedAdvectedFieldPrev(EAdvectedFieldType::Dye);
                    copy_pass.UploadToGPUTexture(source_buffer, target_texture, false);
                }

                copy_pass.EndCopyPass();
            }

            cmd.Submit();
        }
    }

    return 0;
}

void App::Exit()
{
    m_fluid_solver.reset();

    m_sampler.reset();

    m_vertex_buffer.reset();
    m_index_buffer.reset();

    m_to_swapchain_pipeline.reset();

    m_gpu_device.reset();
    m_window.reset();
}

void App::Update(float delta_seconds)
{
}

void App::Render(float delta_seconds)
{
    GpuCmdBuffer cmd(*m_gpu_device);

    {
        m_fluid_solver->Tick(cmd, delta_seconds);
    }

    SDL_GPUTexture* swapchain_texture = nullptr;
    if (!cmd.WaitAndAcquireSwapchainTexture(m_window.get(), swapchain_texture))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to acquire swapchain texture. Info: %s.\n", SDL_GetError());
        return;
    }

    if (swapchain_texture)
    {
        {
            SDL_GPUColorTargetInfo color_targets[1] = {};
            color_targets[0].texture = swapchain_texture;
            color_targets[0].clear_color = SDL_FColor{ 0.0f, 0.0f, 0.0f, 1.0f };
            color_targets[0].load_op = SDL_GPU_LOADOP_CLEAR;
            color_targets[0].store_op = SDL_GPU_STOREOP_STORE;

            GpuRenderPass render_pass(cmd);
            if (render_pass.BeginRenderPass(color_targets, nullptr))
            {
                render_pass.BindGraphicsPipeline(*m_to_swapchain_pipeline);

                render_pass.SetScissor(SDL_Rect{ 0, 0, 1024, 1024 });
                render_pass.SetViewport(SDL_GPUViewport{ 0.0f, 0.0f, 1024.0f, 1024.0f, 0.0f, 1.0f });

                SDL_GPUBufferBinding vertex_buffer_bindings[1] = {};
                vertex_buffer_bindings[0].buffer = m_vertex_buffer->Get();
                vertex_buffer_bindings[0].offset = 0;
                render_pass.BindVertexBuffers(0, vertex_buffer_bindings);

                SDL_GPUBufferBinding index_buffer_binding = {};
                index_buffer_binding.buffer = m_index_buffer->Get();
                index_buffer_binding.offset = 0;
                render_pass.BindIndexBuffer(index_buffer_binding, SDL_GPU_INDEXELEMENTSIZE_16BIT);

                SDL_GPUTextureSamplerBinding textures[1] = {};
                textures[0].texture = m_fluid_solver->GetTypedAdvectedField(EAdvectedFieldType::Dye);
                textures[0].sampler = m_sampler->Get();
                render_pass.BindFragShaderSamplers(0, textures);

                render_pass.DrawIndexed(6, 1, 0, 0, 0);

                render_pass.EndRenderPass();
            }
        }
    }

    cmd.Submit();
}

void App::HandleEvents(SDL_Event& e)
{
    switch (e.type)
    {
    case SDL_EVENT_WINDOW_SHOWN:
    case SDL_EVENT_WINDOW_HIDDEN:
    case SDL_EVENT_WINDOW_EXPOSED:
    case SDL_EVENT_WINDOW_MOVED:
    case SDL_EVENT_WINDOW_RESIZED:
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
    case SDL_EVENT_WINDOW_METAL_VIEW_RESIZED:
    case SDL_EVENT_WINDOW_MINIMIZED:
    case SDL_EVENT_WINDOW_MAXIMIZED:
    case SDL_EVENT_WINDOW_RESTORED:
    case SDL_EVENT_WINDOW_MOUSE_ENTER:
    case SDL_EVENT_WINDOW_MOUSE_LEAVE:
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
    case SDL_EVENT_WINDOW_FOCUS_LOST:
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
    case SDL_EVENT_WINDOW_HIT_TEST:
    case SDL_EVENT_WINDOW_ICCPROF_CHANGED:
    case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
    case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
    case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED:
    case SDL_EVENT_WINDOW_OCCLUDED:
    case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
    case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
    case SDL_EVENT_WINDOW_DESTROYED:
    case SDL_EVENT_WINDOW_HDR_STATE_CHANGED:
    {
        //if (g_window_events)
        //{
        //    g_window_events->broadcast(e);
        //}
        break;
    }
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
    case SDL_EVENT_TEXT_EDITING:
    case SDL_EVENT_TEXT_INPUT:
    case SDL_EVENT_KEYMAP_CHANGED:
    case SDL_EVENT_KEYBOARD_ADDED:
    case SDL_EVENT_KEYBOARD_REMOVED:
    case SDL_EVENT_TEXT_EDITING_CANDIDATES:
    {
        //if (g_keyboard_events)
        //{
        //    g_keyboard_events->broadcast(e);
        //}
        break;
    }
    case SDL_EVENT_MOUSE_MOTION:
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
    case SDL_EVENT_MOUSE_WHEEL:
    case SDL_EVENT_MOUSE_ADDED:
    case SDL_EVENT_MOUSE_REMOVED:
    {
        //if (g_mouse_events)
        //{
        //    g_mouse_events->broadcast(e);
        //}
        break;
    }
    case SDL_EVENT_GAMEPAD_AXIS_MOTION:
    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
    case SDL_EVENT_GAMEPAD_BUTTON_UP:
    case SDL_EVENT_GAMEPAD_ADDED:
    case SDL_EVENT_GAMEPAD_REMOVED:
    case SDL_EVENT_GAMEPAD_REMAPPED:
    case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
    case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION:
    case SDL_EVENT_GAMEPAD_TOUCHPAD_UP:
    case SDL_EVENT_GAMEPAD_SENSOR_UPDATE:
    case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE:
    case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED:
    {
        //if (g_gamepad_events)
        //{
        //    g_gamepad_events->broadcast(e);
        //}
        break;
    }
    case SDL_EVENT_DROP_FILE:
    case SDL_EVENT_DROP_TEXT:
    case SDL_EVENT_DROP_BEGIN:
    case SDL_EVENT_DROP_COMPLETE:
    case SDL_EVENT_DROP_POSITION:
    {
        //if (g_drop_events)
        //{
        //    g_drop_events->broadcast(e);
        //}
        break;
    }
    case SDL_EVENT_AUDIO_DEVICE_ADDED:
    case SDL_EVENT_AUDIO_DEVICE_REMOVED:
    case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED:
    {
        //if (g_audio_events)
        //{
        //    g_audio_events->broadcast(e);
        //}
        break;
    }
    default:
    {
#if PLAYGROUND_DEBUG
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Event of type(%u) will not be handled for now.\n", e.type);
#endif
        break;
    }
    }
}
