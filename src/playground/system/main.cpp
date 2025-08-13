#include "playground/core/macro_define.h"

#include "engine/core/math/math.h"

#include <SDL3/SDL.h>

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
    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL initialization failed: %s.\n", SDL_GetError());
        return -1;
    }

    SDL_Window* sdl_window = SDL_CreateWindow("VoxPlane", 1920, 1080, 0);
    if (!sdl_window)
    {
        SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "Window creation failed: %s", SDL_GetError());
        return -1;
    }

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

    const uint64_t last_time_point = SDL_GetTicks();
    while (true)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                goto exit_point;
            }
            else
            {
                handle_event(e);
            }
        }

        const uint64_t curr_time_point = SDL_GetTicks();
        const uint64_t delta_time_milliseconds = std::max<uint64_t>(curr_time_point - last_time_point, 1);
        const float    delta_time_seconds = (float)delta_time_milliseconds * 0.001f;
    }

exit_point:
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();

    {
        Vec2 vec2_a(3.0f, 4.0f);
        Vec2 vec2_b(1.0f, 2.0f);
        Vec2 vec2_add = vec2_a + vec2_b;
        Vec2 vec2_sub = vec2_a - vec2_b;
        Vec2 vec2_mul = vec2_a * 2.0f;
        Vec2 vec2_div = vec2_a / 2.0f;
        Vec2 vec2_neg = -vec2_a;
        Vec2 vec2_norm = vec2_a.GetNormalized();
        float vec2_length = vec2_a.Length();
    }

    {
        Vec3 vec3_a(1.0f, 2.0f, 3.0f);
        Vec3 vec3_b(4.0f, 5.0f, 6.0f);
        Vec3 vec3_add = vec3_a + vec3_b;
        Vec3 vec3_sub = vec3_a - vec3_b;
        Vec3 vec3_mul = vec3_a * 2.0f;
        Vec3 vec3_div = vec3_a / 2.0f;
        Vec3 vec3_neg = -vec3_a;
        Vec3 vec3_norm = vec3_a.GetNormalized();
        float vec3_length = vec3_a.Length();
        float dot_product = vec3_a.Dot(vec3_b);
        Vec3 cross_product = vec3_a.Cross(vec3_b);
    }

    {
        Vec4 vec4_a(1.0f, 2.0f, 3.0f, 4.0f);
        Vec4 vec4_b(5.0f, 6.0f, 7.0f, 8.0f);
        Vec4 vec4_add = vec4_a + vec4_b;
        Vec4 vec4_sub = vec4_a - vec4_b;
        Vec4 vec4_mul = vec4_a * 2.0f;
        Vec4 vec4_div = vec4_a / 2.0f;
        Vec4 vec4_neg = -vec4_a;
        Vec4 vec4_norm = vec4_a.GetNormalized();
        float vec4_length = vec4_a.Length();
        float dot_product = vec4_a.Dot(vec4_b);
    }

    {
        Mat2 mat2_a(1.0f, 2.0f, 3.0f, 4.0f);
        Mat2 mat2_b(5.0f, 6.0f, 7.0f, 8.0f);
        Mat2 mat2_add = mat2_a + mat2_b;
        Mat2 mat2_sub = mat2_a - mat2_b;
        Mat2 mat2_mul = mat2_a * 2.0f;
        Mat2 mat2_div = mat2_a / 2.0f;
        Mat2 mat2_neg = -mat2_a;
        Mat2 mat2_identity = Mat2::Identity();
        Mat2 mat2_zero = Mat2::Zero();
        Vec2 vec2_a(1.0f, 2.0f);
        Vec2 vec2_b = mat2_a * vec2_a; // Matrix-vector multiplication
        Vec2 mat2_row0 = mat2_a.Row0(); // Get first row
        Vec2 mat2_row1 = mat2_a.Row1(); // Get second row
        float det = mat2_a.Determinant(); // Determinant
        float trace = mat2_a.Trace(); // Trace
        Mat2 mat2_mul_mat = mat2_a * mat2_b; // Matrix-matrix multiplication
    }

    {
        Mat2 mat2_a(1.0f, 2.0f, 3.0f, 4.0f);
        Mat2 mat2_b(5.0f, 6.0f, 7.0f, 8.0f);
        Mat2 mat2_mul = mat2_a * mat2_b;
        Mat2 mat2_transpose = mat2_a.Transpose();
    }

    {
        Mat3 mat3_a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
        Mat3 mat3_b(9.0f, 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
        Mat3 mat3_add = mat3_a + mat3_b;
        Mat3 mat3_sub = mat3_a - mat3_b;
        Mat3 mat3_mul = mat3_a * 2.0f;
        Mat3 mat3_div = mat3_a / 2.0f;
        Mat3 mat3_neg = -mat3_a;
        Mat3 mat3_identity = Mat3::Identity();
        Mat3 mat3_zero = Mat3::Zero();
        Vec3 vec3_a(1.0f, 2.0f, 3.0f);
        Vec3 vec3_b = mat3_a * vec3_a; // Matrix-vector multiplication
        Vec3 mat3_row0 = mat3_a.Row0(); // Get first row
        Vec3 mat3_row1 = mat3_a.Row1(); // Get second row
        Vec3 mat3_row2 = mat3_a.Row2(); // Get third row
        float det = mat3_a.Determinant(); // Determinant
        float trace = mat3_a.Trace(); // Trace
        Mat3 mat3_mul_mat = mat3_a * mat3_b; // Matrix-matrix multiplication
    }

    {
        Mat4 mat4_a(1.0f, 2.0f, 3.0f, 4.0f,
            5.0f, 6.0f, 7.0f, 8.0f,
            9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f);
        Mat4 mat4_b(16.0f, 15.0f, 14.0f, 13.0f,
            12.0f, 11.0f, 10.0f, 9.0f,
            8.0f, 7.0f, 6.0f, 5.0f,
            4.0f, 3.0f, 2.0f, 1.0f);
        Mat4 mat4_add = mat4_a + mat4_b;
        Mat4 mat4_sub = mat4_a - mat4_b;
        Mat4 mat4_mul = mat4_a * 2.0f;
        Mat4 mat4_div = mat4_a / 2.0f;
        Mat4 mat4_neg = -mat4_a;
        Mat4 mat4_identity = Mat4::Identity();
        Mat4 mat4_zero = Mat4::Zero();
        Vec4 vec4_a(1.0f, 2.0f, 3.0f, 4.0f);
        Vec4 vec4_b = mat4_a * vec4_a; // Matrix-vector multiplication
        Vec4 mat4_row0 = mat4_a.Row0(); // Get first row
        Vec4 mat4_row1 = mat4_a.Row1(); // Get second row
        Vec4 mat4_row2 = mat4_a.Row2(); // Get third row
        Vec4 mat4_row3 = mat4_a.Row3(); // Get fourth row
        float det = mat4_a.Determinant(); // Determinant
        float trace = mat4_a.Trace(); // Trace
    }

    return 0;
}
