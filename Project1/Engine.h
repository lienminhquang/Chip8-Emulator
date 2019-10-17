/*
    Use to display graphics and handle events, audio (future)
*/

#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <string>
#include "Chip.h"
#include "Timer.h"
#include "Sprite.h"
#include "SDL_FontCache.h"
#include "Helper.h"
#include <list>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define TILE_SIZE 10



class Engine
{
public:
    Engine();
    ~Engine();

    bool init();
    void emulate();
    void quit();
    void setPath(const std::string& path) { m_Path = path; }

private:
    void update(float dt);
    void processInput();
    void display();

    void displayScreen();

    void displayDebug();
    void displayInstructions();
    void displayRegister();
    void displayMemory();
    void displayStack();
    void displayKeyStates();
    void displayDispMem();
    void displayTimers();

private:
    Chip m_Chip;

    SDL_Window* m_Window;
    SDL_Renderer* m_Renderer;
    SDL_Texture* m_Texture;

    FC_Font* m_Font;
    

    bool m_Quit = false;
    bool m_Pause = false;
    bool m_ExecuteNext1Ins = false;
    std::string m_Path;

    char m_CurrentScreenOfChip[W * H];
    

    unsigned char m_KeyMap[16][2] = 
    {
	   {SDLK_1, 0x1},{SDLK_2, 0x2},{SDLK_3, 0x3},{SDLK_4, 0xc},
	   {SDLK_q, 0x4},{SDLK_w, 0x5},{SDLK_e, 0x6},{SDLK_r, 0xd},
	   {SDLK_a, 0x7},{SDLK_s, 0x8},{SDLK_d, 0x9},{SDLK_f, 0xe},
	   {SDLK_z, 0xa},{SDLK_x, 0x0},{SDLK_c, 0xb},{SDLK_v, 0xf}
    };

    std::list < std::string > m_ListFilesFound;
};


