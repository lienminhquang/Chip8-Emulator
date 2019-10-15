#pragma once
#include <SDL.h>

class Timer
{
public:
    Timer();


    void start();
    Uint32 restart();
    Uint32 getElep();

private:
    Uint32 m_StartTick;
    bool m_Started = false;

};

