#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <set>
#include <memory>
#include <functional>
#include "Vec2.h"

class Component
{
public:
    Component();
    virtual ~Component();

    void Show();
    void Hide();

    virtual void render();

    virtual void ProcessEvent(SDL_Event* event);
    void addChild(Component* child);
    void deleteChild(Component* child);
   
protected:
    void setParent(Component* parent);
    void renderChilds();

private:
    std::set<std::unique_ptr<Component>>   m_Childs;
    Component* m_Parent;

    bool m_Visible;

};

