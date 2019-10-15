#include "Sprite.h"

Sprite::Sprite():
    m_Texture(nullptr)
{
}

Sprite::~Sprite()
{
    SDL_DestroyTexture(m_Texture);
}

void Sprite::loadText(const std::string& text)
{
    if (m_Texture != nullptr)
	   SDL_DestroyTexture(m_Texture);

    SDL_Surface* surface = TTF_RenderText_Solid(m_Font, text.c_str(), { 0, 0, 255, 255 });
    m_Texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
    m_Width = surface->w;
    m_Height = surface->h;
    SDL_FreeSurface(surface);
}

void Sprite::render(int x, int y, int w, int h)
{
    SDL_Rect desRect = { x, y, w, h };
    SDL_RenderCopy(m_Renderer, m_Texture, nullptr, &desRect);
}
