#include "Sprite.h"


Sprite::Sprite(MTexture* texture) :
    Component(),
    m_Texture(texture)
{
    auto size = m_Texture->getSize();
    this->setSize(size.x, size.y);
}

Sprite::Sprite():
    Component(),
    m_Texture(nullptr)
{
}

Sprite::~Sprite()
{
   
}

void Sprite::render()
{
    auto pos = this->getRectInWorldPosition();
    m_Texture->render(nullptr, &(this->getRectInWorldPosition()));
    
    this->renderChilds();
}



