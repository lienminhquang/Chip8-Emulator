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
    Sprite();

    ~Sprite();

    virtual void render() override;
   

private:

    MTexture* m_Texture;
};

