#pragma once

#include <cstdint>

class RenderTarget
{
protected:
    RenderTarget() = default;
    RenderTarget(const RenderTarget&) = delete;
    RenderTarget& operator=(const RenderTarget&) = delete;
    RenderTarget(RenderTarget&&) = delete;
    RenderTarget& operator=(RenderTarget&&) = delete;

public:
    virtual ~RenderTarget() = default;

public:
    virtual void Resize(int32_t width, int32_t height) = 0;
    virtual void Clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool bind_if_unbound = false) = 0;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual bool IsBound() const = 0;

    virtual int32_t GetWidth() const = 0;
    virtual int32_t GetHeight() const = 0;

    virtual void DrawPixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;
};
