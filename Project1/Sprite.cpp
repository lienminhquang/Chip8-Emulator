#include "Sprite.h"

Sprite::Sprite(MTexture* texture):
    m_Texture(texture),
    m_Rect()
{
}

Sprite::~Sprite()
{
   
}

void Sprite::render()
{
    m_Texture->render(nullptr, &m_Rect);
    
    this->renderChilds();
}


void Sprite::setPosition(int x, int y)
{
    m_Rect.x = x;
    m_Rect.y = y;
}

void Sprite::setSize(int w, int h)
{
    m_Rect.w = w;
    m_Rect.h = h;
}

SDL_Rect Sprite::getRect()
{
    return m_Rect;
}
