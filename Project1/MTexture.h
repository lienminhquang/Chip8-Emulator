#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "Vec2.h"

class MTexture 
{
public:
    explicit MTexture(const char* texture, SDL_Renderer* renderer);
    virtual ~MTexture();

    SDL_Texture* getSDLTexture() { return m_SDLTexture; }

    void render(const SDL_Rect* sourceRect, const SDL_Rect* desRect);
    Vec2 getSize() { return m_Size; }

private:
    SDL_Texture* m_SDLTexture;
    SDL_Renderer* m_Renderer;
    Vec2 m_Size;
};

