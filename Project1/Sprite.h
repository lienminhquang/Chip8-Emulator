#pragma once
#include <SDL.h>
#include <string>
#include "Vec2.h"
#include "MTexture.h"
#include "Component.h"




class Sprite : public Component
{
public:
    explicit Sprite(MTexture* texture);
    ~Sprite();

    virtual void render() override;

    void setPosition(int x, int y);
    void setSize(int w, int h);
    SDL_Rect getRect();

private:

    MTexture* m_Texture;
    SDL_Rect m_Rect;
};

