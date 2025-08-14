#include "playground/core/macro_define.h"

#include "playground/core/sdl_render_target.h"

#include "engine/math/math.h"

#include <SDL3/SDL.h>

#include <memory>

void handle_event(SDL_Event& e)
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

int main(int argc, char** argv)
{
    int system_result = 0;

    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL initialization failed: %s.\n", SDL_GetError());
        return -1;
    }

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> sdl_window{
        SDL_CreateWindow("VoxPlane", 1920, 1080, 0),
        SDL_DestroyWindow
    };
    if (!sdl_window)
    {
        SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "Window creation failed: %s", SDL_GetError());
        return -1;
    }

    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> sdl_renderer{
        SDL_CreateRenderer(sdl_window.get(), nullptr),
        SDL_DestroyRenderer
    };
    if (!sdl_renderer)
    {
        SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "Renderer creation failed: %s", SDL_GetError());
        return -1;
    }

    // TODO: add render_target resize to event handling.
    SDLWindowRenderTarget render_target(sdl_renderer.get());

    SDLTextureRenderTarget texture_render_target(sdl_renderer.get());
    texture_render_target.Resize(512, 512);

    // We will create a color attachment which has the same size with the screen.
    // So when we resize the viewport window, we just need to change the viewport when we render the scene.
    SDL_Rect display_bounds;
    {
        int                  display_count = 0;
        const SDL_DisplayID* display_ids = SDL_GetDisplays(&display_count);
        if (!display_ids)
        {
            SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "Failed to get displays: %s", SDL_GetError());
            return -1;
        }

        if (!SDL_GetDisplayBounds(display_ids[0], &display_bounds))
        {
            SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "Failed to get display info at (0): %s", SDL_GetError());
            return -1;
        }
    }

    int32_t index = 0;

    const uint64_t last_time_point = SDL_GetTicks();
    while (true)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                return 0;
            }
            else
            {
                handle_event(e);
            }
        }

        const uint64_t curr_time_point = SDL_GetTicks();
        const uint64_t delta_time_milliseconds = std::max<uint64_t>(curr_time_point - last_time_point, 1);
        const float    delta_time_seconds = (float)delta_time_milliseconds * 0.001f;

        render_target.Clear(0, 0, 0, 0, true);

        texture_render_target.Bind();
        for (int i = 0; i < 16; ++i)
        {
            render_target.DrawPixel(index % 512, index / 512, 255, 255, 255, 255);
            ++index;
        }
        texture_render_target.Unbind();

        render_target.DrawTexture(0, 0, 512, 512, texture_render_target.GetRawTexture());

        render_target.Present();
    }
}
