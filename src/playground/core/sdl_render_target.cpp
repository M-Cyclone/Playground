#include "sdl_render_target.h"

SDLRenderTarget::SDLRenderTarget(SDL_Renderer* renderer)
    : RenderTarget()
    , m_renderer(renderer)
{
#if PLAYGROUND_DEBUG
    if (!m_renderer)
    {
        SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "SDL_Renderer is null.");
        exit(-1);
    }
#endif
}

void SDLRenderTarget::DrawPixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
    SDL_RenderPoint(m_renderer, x, y);
}

void SDLRenderTarget::DrawTexture(int32_t x, int32_t y, int32_t width, int32_t height, SDL_Texture* texture) const
{
    SDL_FRect dst_rect = { x, y, width, height };
    SDL_RenderTexture(m_renderer, texture, nullptr, &dst_rect);
}

SDLWindowRenderTarget::SDLWindowRenderTarget(SDL_Renderer* renderer)
    : SDLRenderTarget(renderer)
{
    if (!SDL_GetWindowSize(SDL_GetRenderWindow(m_renderer), &m_width, &m_height))
    {
#if PLAYGROUND_DEBUG
        SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "Failed to get window size: %s", SDL_GetError());
        m_width = 0;
        m_height = 0;
        exit(-1);
#endif
    }
}

void SDLWindowRenderTarget::Resize(int32_t width, int32_t height)
{
    m_width = width;
    m_height = height;
}

void SDLWindowRenderTarget::Clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool bind_if_unbound)
{
    if (bind_if_unbound && !IsBound())
    {
        Bind();
    }

    if (IsBound())
    {
        SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
        SDL_RenderClear(m_renderer);
    }
}

void SDLWindowRenderTarget::Bind() const
{
    // Bind to the default window target
    SDL_SetRenderTarget(m_renderer, nullptr);
}

void SDLWindowRenderTarget::Unbind() const
{
    // Unable to unbind from the default window target, as it is always bound.
    SDL_SetRenderTarget(m_renderer, nullptr);
}

bool SDLWindowRenderTarget::IsBound() const
{
    // The default window target is always bound, so we return true.
    return SDL_GetRenderTarget(m_renderer) == nullptr;
}

void SDLWindowRenderTarget::Present() const
{
    SDL_RenderPresent(m_renderer);
}

SDLTextureRenderTarget::SDLTextureRenderTarget(SDL_Renderer* renderer)
    : SDLRenderTarget(renderer)
    , m_texture{ nullptr, SDL_DestroyTexture }
{
}

void SDLTextureRenderTarget::Resize(int32_t width, int32_t height)
{
    m_width = width;
    m_height = height;

    m_texture.reset(SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_width, m_height));

    if (!m_texture)
    {
#if PLAYGROUND_DEBUG
        SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "Failed to create texture rt: %s", SDL_GetError());
        exit(-1);
#endif
    }
}

void SDLTextureRenderTarget::Clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool bind_if_unbound)
{
    if (bind_if_unbound && !IsBound())
    {
        Bind();
    }

    if (IsBound())
    {
        SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
        SDL_RenderClear(m_renderer);
    }
}

void SDLTextureRenderTarget::Bind() const
{
    // Bind to the texture target
    SDL_SetRenderTarget(m_renderer, m_texture.get());
}

void SDLTextureRenderTarget::Unbind() const
{
    // Unbind from the texture target
    SDL_SetRenderTarget(m_renderer, nullptr);
}

bool SDLTextureRenderTarget::IsBound() const
{
    return SDL_GetRenderTarget(m_renderer) == m_texture.get();
}
