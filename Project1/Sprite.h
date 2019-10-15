#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

class Sprite
{
public:
    Sprite();
    ~Sprite();

    int getWidth() { return m_Width; }
    int getHeight() { return m_Height; }
    void setFont(TTF_Font* font) { m_Font = font; }
    void setRenderer(SDL_Renderer* r) { m_Renderer = r; }

    void loadText(const std::string& text);
    void render(int x, int y, int w, int h);

private:
    SDL_Texture* m_Texture;
    int m_Width;
    int m_Height;

    TTF_Font* m_Font;
    SDL_Renderer* m_Renderer;
};

