#include "Timer.h"

Timer::Timer():
    m_StartTick(0)
{
}

void Timer::start()
{
    m_StartTick = SDL_GetTicks();
    m_Started = true;
}

Uint32 Timer::restart()
{
    if (!m_Started)
	   start();

    Uint32 e = SDL_GetTicks() - m_StartTick;
    m_StartTick = SDL_GetTicks();

    return e;
}

Uint32 Timer::getElep()
{
    return  SDL_GetTicks() - m_StartTick;
}
