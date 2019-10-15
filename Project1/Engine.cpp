#include "Engine.h"
#include <string>

Engine::Engine() :
    m_Window(nullptr),
    m_Renderer(nullptr),
    m_Texture(nullptr)
{
}

Engine::~Engine()
{
   

}

bool Engine::init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
	   printf("Could not initialize SDL, SDL_Error: %s/n", SDL_GetError());
	   success = false;
    }
    else
    {
	   m_Window = SDL_CreateWindow("Chip8-Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * TILE_SIZE, SCREEN_HEIGHT * TILE_SIZE, 0);
	   if (m_Window == nullptr)
	   {
		  printf("Unable to create window ! SDL_Error: %s\n", SDL_GetError());
		  success = false;
	   }
	   else
	   {
		  m_Renderer = SDL_CreateRenderer(m_Window, -1, 0);
		  if (m_Renderer == nullptr)
		  {
			 printf("Unable to create renderer ! SDL_Error: %s\n", SDL_GetError());
			 success = false;
		  }
		  else
		  {
			 if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) <= 0)
			 {
				printf("Unable to initialize SDL_image, SDL_Image_Error: %s\n", IMG_GetError());
				success = false;
			 }
			 else
			 {
				std::string path = "res/sheet.png";
				auto surface = IMG_Load(path.c_str());
				if (surface == nullptr)
				{
				    printf("Unable to load image %s, SDL_Image_Error: %s\n", path.c_str(), IMG_GetError());
				    success = false;
				}
				else
				{
				    m_Texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
				    if (m_Texture == nullptr)
				    {
					   printf("Unable create surface for image %s, SDL_Error: %s\n", path.c_str(), SDL_GetError());
					   success = false;
				    }
				    SDL_FreeSurface(surface);
				}
			 }
		  }

	   }
    }

   

    m_Chip.init();
    m_Chip.loadGame(m_Path);

    return success;
}

void Engine::emulate()
{
    float time_since_last_update = 0.f;
    const float time_per_frame = 1 / 300.f;
    float fps_time = 0.f;
    float timer_chip = 0.f;
    const float CHIP_TIMER_CYCLE = 1000 / 60.f;

    Timer timer;
    timer.start();

    int fps = 0;

    while (!m_Quit)
    {
	   fps_time += timer.getElep() / 1000.f;
	   if (fps_time >= 1.f)
	   {
		  //system("cls");
		  //printf("FPS: %d", fps);
		  fps = 0;
		  fps_time -= 1.f;
	   }

	   //update timer of chip
	   timer_chip += timer.getElep();
	   while (timer_chip >= CHIP_TIMER_CYCLE)
	   {
		  timer_chip -= CHIP_TIMER_CYCLE;
		  m_Chip.updateTimer(1);
	   }


	   time_since_last_update += timer.restart() / 1000.f;


	   if (time_since_last_update >= time_per_frame)
	   {
		  time_since_last_update = 0;

		  processInput();

		  update(time_per_frame);
	   }

	   display();
	   fps++;

    }

}

void Engine::quit()
{
    if (m_Texture != nullptr)
	   SDL_DestroyTexture(m_Texture);
    m_Texture = nullptr;

    if (m_Renderer != nullptr)
	   SDL_DestroyRenderer(m_Renderer);
    m_Renderer = nullptr;

    if (m_Window)
	   SDL_DestroyWindow(m_Window);
    m_Window = nullptr;

    IMG_Quit();
    SDL_Quit();
}

void Engine::update(float dt)
{
    m_Chip.emulateCycle();
}

void Engine::processInput()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
	   switch (e.type)
	   {
	   case SDL_QUIT:
		  m_Quit = true;
		  break;
	   case SDL_KEYDOWN:
	   {
		  int key = e.key.keysym.sym;
		  for (int i = 0; i < 16; i++)
		  {
			 if (key == m_KeyMap[i][0])
				m_Chip.keyDown(m_KeyMap[i][1]);
		  }
	   }
	   break;
	   case SDL_KEYUP:
	   {
		  int key = e.key.keysym.sym;
		  for (int i = 0; i < 16; i++)
		  {
			 if (key == m_KeyMap[i][0])
				m_Chip.keyUp(m_KeyMap[i][1]);
		  }
	   }
	   break;
	   default:
		  break;
	   }
    }
}

void Engine::display()
{
    //Clear screen
    SDL_SetRenderDrawColor(m_Renderer, 0x0, 0x00, 0x00, 0xff);
    SDL_RenderClear(m_Renderer);

    SDL_Rect clip = { TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };

   auto m_ChipScreenChars = m_Chip.getScreenChars();

    for (int height = 0; height < SCREEN_HEIGHT; height++)
    {
	   for (int width = 0; width < SCREEN_WIDTH; width++)
	   {
		  if (m_ChipScreenChars[height * SCREEN_WIDTH + width] > 0)
		  {
			 SDL_Rect desRect = { width * TILE_SIZE, height * TILE_SIZE, TILE_SIZE, TILE_SIZE };
			 SDL_RenderCopy(m_Renderer, m_Texture, &clip, &desRect);
		  }
	   }
    }


    SDL_RenderPresent(m_Renderer);
}
