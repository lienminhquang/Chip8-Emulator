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

    Component* getParent() { return m_Parent; }
    
    Vec2 getPosition() { return m_Position; }
    Vec2 getSize() { return m_Size; }
    Vec2 getWorldPosition();
    void setPosition(int x, int y);
    void setSize(int w, int h);
    SDL_Rect getRect();
    SDL_Rect getRectInWorldPosition();
   
protected:
    void setParent(Component* parent);
    void renderChilds();

protected:
    std::set<std::unique_ptr<Component>>   m_Childs;
    Component* m_Parent;

    bool m_Visible;
    Vec2 m_Position;
    Vec2 m_Size;
};

