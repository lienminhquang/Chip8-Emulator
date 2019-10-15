#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include "SDL.h"
#include <SDL_image.h>
#include "Engine.h"



int main(int argc, char* argv[])
{

    std::cout << "Enter game: ";
    std::string path = "roms/hanoi.bin";
    std::cin >> path;

    Engine engine;
    engine.setPath(path);
    engine.init();
    engine.emulate();
    engine.quit();

    return 0;
}