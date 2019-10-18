#include "Component.h"

Component::Component():
    m_Childs(),
    m_Parent(nullptr),
    m_Visible(true),
    m_Position(0,0),
    m_Size(0,0)
{
    
}

Component::~Component()
{
}

void Component::Show()
{
}

void Component::Hide()
{
}

void Component::render()
{
}

void Component::ProcessEvent(SDL_Event* event)
{
}

void Component::addChild(Component* child)
{
    m_Childs.insert(std::move(std::unique_ptr<Component>(child)));
    child->setParent(this);
}

//TODO
void Component::deleteChild(Component* child)
{
    //auto found = m_Childs.find(child);
}

//Test
Vec2 Component::getWorldPosition()
{
    auto parent = this->getParent();
    Vec2 rs = this->getPosition();
    while (parent != nullptr)
    {
	   rs = rs + parent->getPosition();
	   parent = parent->getParent();
    }

    return rs;
}

void Component::setParent(Component* parent)
{
    m_Parent = parent;
}

void Component::renderChilds()
{
    for (auto itr = m_Childs.begin(); itr != m_Childs.end(); ++itr)
    {
	   itr->get()->render();
    }
}

void Component::setPosition(int x, int y)
{
    m_Position.x = x;
    m_Position.y = y;
}

void Component::setSize(int w, int h)
{
    m_Size.x = w;
    m_Size.y = h;
}

SDL_Rect Component::getRect()
{
    return { m_Position.x, m_Position.y, m_Size.x, m_Size.y };
}

SDL_Rect Component::getRectInWorldPosition()
{
    Vec2 pos = this->getWorldPosition();
    return { pos.x, pos.y, m_Size.x, m_Size.y };
}
