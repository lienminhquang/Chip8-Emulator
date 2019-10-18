#include "MTexture.h"

MTexture::MTexture(const char* fileName, SDL_Renderer* renderer) :
    m_Renderer(renderer)
{
    SDL_Surface* surface = IMG_Load(fileName);
    m_SDLTexture = SDL_CreateTextureFromSurface(renderer, surface);
    m_Size = {surface->w, surface->h};
    SDL_FreeSurface(surface);
    surface = nullptr;
}

MTexture::~MTexture()
{
    SDL_DestroyTexture(m_SDLTexture);
    m_SDLTexture = nullptr;
}

void MTexture::render(const SDL_Rect* sourceRect, const SDL_Rect* desRect)
{
    SDL_RenderCopy(m_Renderer, m_SDLTexture, sourceRect, desRect);
}
