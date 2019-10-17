#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

class MTexture 
{
public:
    explicit MTexture(const char* texture, SDL_Renderer* renderer);
    virtual ~MTexture();

    SDL_Texture* getSDLTexture() { return m_SDLTexture; }

    void render(const SDL_Rect* sourceRect, const SDL_Rect* desRect);

private:
    SDL_Texture* m_SDLTexture;
    SDL_Renderer* m_Renderer;
};

