#include "app.h"

#include "playground/utils/file_loader.h"

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
        Render();

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
    m_window.reset(SDL_CreateWindow("Misaka Playground - FPS(0)", 1920, 1080, 0));
    if (!m_window)
    {
        SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "Window creation failed: %s", SDL_GetError());
        return -1;
    }

    m_gpu_device = std::make_unique<GpuDevice>(
        SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL,
        nullptr
    );
    m_gpu_device->ClaimWindow(m_window.get());

    m_vert_shader = LoadShader(*m_gpu_device, "triangle.vert", 0, 0, 0, 0);
    m_frag_shader = LoadShader(*m_gpu_device, "triangle.frag", 0, 0, 0, 0);

    {
        SDL_GPUColorTargetDescription color_descs[1] = {};
        color_descs[0].format = m_gpu_device->GetSwapchainTextureFormat(m_window.get());

        SDL_GPUGraphicsPipelineCreateInfo create_info =
        {
            .vertex_shader = m_vert_shader->Get(),
            .fragment_shader = m_frag_shader->Get(),
            .vertex_input_state =
            {
                .vertex_buffer_descriptions = nullptr,
                .num_vertex_buffers = 0,
                .vertex_attributes = nullptr,
                .num_vertex_attributes = 0,
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
                .num_color_targets = 1,
                .depth_stencil_format = {},
                .has_depth_stencil_target = false,
            },
        };

        m_gfx_pipeline = std::make_unique<GpuGraphicsPipeline>(*m_gpu_device, create_info);
    }

    return 0;
}

void App::Exit()
{
    m_vert_shader.reset();
    m_frag_shader.reset();

    m_gpu_device.reset();
    m_window.reset();
}

void App::Update(float delta_seconds)
{
}

void App::Render()
{
    GpuCmdBuffer cmd(*m_gpu_device);

    SDL_GPUTexture* swapchain_texture = nullptr;
    if (!cmd.WaitAndAcquireSwapchainTexture(m_window.get(), swapchain_texture))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to acquire swapchain texture. Info: %s.\n", SDL_GetError());
        return;
    }

    if (swapchain_texture)
    {
        SDL_GPUColorTargetInfo color_targets[1] = {};
        color_targets[0].texture = swapchain_texture;
        color_targets[0].clear_color = SDL_FColor{ 0.0f, 0.0f, 0.0f, 1.0f };
        color_targets[0].load_op = SDL_GPU_LOADOP_CLEAR;
        color_targets[0].store_op = SDL_GPU_STOREOP_STORE;

        GpuRenderPass render_pass(cmd);
        if (render_pass.BeginRenderPass(color_targets, nullptr))
        {
            render_pass.BindGraphicsPipeline(*m_gfx_pipeline);

            render_pass.SetScissor(SDL_Rect{ 0, 0, 1920, 1080 });
            render_pass.SetViewport(SDL_GPUViewport{ 0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f });

            render_pass.Draw(3, 1, 0, 0);

            render_pass.EndRenderPass();
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
