#pragma once

#include "engine/render/buffer/render_target.h"

#include <SDL3/SDL.h>

#include <memory>

class SDLRenderTarget : public RenderTarget
{
public:
    explicit SDLRenderTarget(SDL_Renderer* renderer);
    ~SDLRenderTarget() override = default;

public:
    void DrawPixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;

public:
    void DrawTexture(int32_t x, int32_t y, int32_t width, int32_t height, SDL_Texture* texture) const;

protected:
    SDL_Renderer* m_renderer = nullptr;
};

class SDLWindowRenderTarget : public SDLRenderTarget
{
public:
    explicit SDLWindowRenderTarget(SDL_Renderer* renderer);
    ~SDLWindowRenderTarget() override = default;

public:
    void Resize(int32_t width, int32_t height) override;
    void Clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool bind_if_unbound = false) override;

    void Bind() const override;
    void Unbind() const override;
    bool IsBound() const override;

    int32_t GetWidth() const override { return m_width; }
    int32_t GetHeight() const override { return m_height; }

    void Present() const;

private:
    int32_t m_width = 0;
    int32_t m_height = 0;
};

class SDLTextureRenderTarget : public SDLRenderTarget
{
public:
    explicit SDLTextureRenderTarget(SDL_Renderer* renderer);
    ~SDLTextureRenderTarget() override = default;

public:
    void Resize(int32_t width, int32_t height) override;
    void Clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool bind_if_unbound = false) override;

    void Bind() const override;
    void Unbind() const override;
    bool IsBound() const override;

    int32_t GetWidth() const override { return m_width; }
    int32_t GetHeight() const override { return m_height; }

    SDL_Texture* GetRawTexture() const { return m_texture.get(); }

private:
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_texture;

    int32_t m_width = 0;
    int32_t m_height = 0;
};
