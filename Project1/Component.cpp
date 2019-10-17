#include "Component.h"

Component::Component():
    m_Childs(),
    m_Parent(nullptr),
    m_Visible(true)
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
}

void Component::deleteChild(Component* child)
{
    auto found = m_Childs.find(child);
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
