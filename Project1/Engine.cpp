#include "Engine.h"
#include <string>
#include <sstream>
#include <iostream>

#define DEBUG_OFFSET 40

Engine::Engine() :
    m_Window(nullptr),
    m_Renderer(nullptr),
    m_Texture(nullptr),
    m_Font(nullptr),
    m_CurrentScreenOfChip()
{
    //Helper::find_file("roms", m_ListFilesFound);
}

Engine::~Engine()
{
 

}

bool Engine::init()
{
    bool success = true;

    TTF_Init();

    

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
	   printf("Could not initialize SDL, SDL_Error: %s/n", SDL_GetError());
	   success = false;
    }
    else
    {
	   m_Window = SDL_CreateWindow("Chip8-Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * TILE_SIZE + 300, SCREEN_HEIGHT * TILE_SIZE + 300, 0);
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

   
    m_Font = FC_CreateFont();
    FC_LoadFont(m_Font, m_Renderer, "res/Roboto-Black.ttf", 12, FC_MakeColor(0x00, 0x00, 0xff, 0xff), TTF_STYLE_NORMAL);
    FC_SetLineSpacing(m_Font, 0);

    m_TestTexture = std::unique_ptr<MTexture>(new MTexture("res/HelloWorld.png", m_Renderer));
    m_TestTexture2 = std::unique_ptr<MTexture>(new MTexture("res/CloseSelected.png", m_Renderer));
    m_Sprite = std::unique_ptr<Sprite>(new Sprite(m_TestTexture.get()));
    m_Sprite->setPosition(100, 100);
    Sprite* sprite = new Sprite(m_TestTexture2.get());
    sprite->setPosition(0, 0);
    m_Sprite->addChild(sprite);

    m_Chip.init();
    m_Chip.loadGame(m_Path);

    return success;
}

void Engine::emulate()
{
    float time_since_last_update = 0.f;
    constexpr float TIME_PER_UPDATE = 1 / 300.f; //default 300
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
		  //std::stringstream ss;
		  //ss << "FPS: " << fps << "\nLMQ";

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


	   if (time_since_last_update >= TIME_PER_UPDATE)
	   {
		  time_since_last_update = 0;

		  processInput();

		  update(TIME_PER_UPDATE);
	   }

	   display();
	   fps++;

    }

}

void Engine::quit()
{
    
    FC_FreeFont(m_Font);
    

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
    TTF_Quit();
}

void Engine::update(float dt)
{
    if (m_Pause)
    {
	   if (m_ExecuteNext1Ins)
	   {
		  m_Chip.emulateCycle();
		  m_ExecuteNext1Ins = false;
	   }
    }
    else
    {
	   m_Chip.emulateCycle();
    }
    
}

void Engine::processInput()
{
    /*
    P: Pause/Resum
    N: Next Instruction
    */

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
		  switch (e.key.keysym.sym)
		  {
		  case SDLK_n:
		  {
			 if (m_Pause) m_ExecuteNext1Ins = true;
		  }
		  break;
		  default:
		  {
			 int key = e.key.keysym.sym;
			 for (int i = 0; i < 16; i++)
			 {
				if (key == m_KeyMap[i][0])
				    m_Chip.keyDown(m_KeyMap[i][1]);
			 }
		  }
			 break;
		  }

		 
	   }
	   break;
	   case SDL_KEYUP:
	   {
		  switch (e.key.keysym.sym)
		  {
		  case SDLK_p:
		  {
			 m_Pause = !m_Pause;
		  }
		  break;
		  default:
		  {
			 int key = e.key.keysym.sym;
			 for (int i = 0; i < 16; i++)
			 {
				if (key == m_KeyMap[i][0])
				    m_Chip.keyUp(m_KeyMap[i][1]);
			 }
		  }
			 break;
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

    this->displayScreen();
    this->displayDebug();

    m_Sprite->render();

    SDL_RenderPresent(m_Renderer);
}

void Engine::displayScreen()
{
    if (m_Chip.needDraw())
    {
	   m_Chip.resetDrawFlag();
	   auto chars = m_Chip.getScreenChars();
	   std::memcpy(m_CurrentScreenOfChip, chars, W * H);
    }
    
  
    SDL_Rect clip = { TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };

    for (int height = 0; height < SCREEN_HEIGHT; height++)
    {
	   for (int width = 0; width < SCREEN_WIDTH; width++)
	   {
		  if (m_CurrentScreenOfChip[height * SCREEN_WIDTH + width] > 0)
		  {
			 SDL_Rect desRect = { width * TILE_SIZE, height * TILE_SIZE, TILE_SIZE, TILE_SIZE };
			 SDL_RenderCopy(m_Renderer, m_Texture, &clip, &desRect);
		  }
	   }
    }
}

void Engine::displayDebug()
{
    if(m_Pause)
	   FC_Draw(m_Font, m_Renderer, 0, SCREEN_HEIGHT * TILE_SIZE + 20 , "P: Resume - N: Next instruction");
    else
	   FC_Draw(m_Font, m_Renderer, 0, SCREEN_HEIGHT * TILE_SIZE + 20 , "P: Pause");

    this->displayInstructions();
    this->displayRegister();
    this->displayMemory();
    this->displayStack();
    this->displayKeyStates();
    this->displayTimers();
}

void Engine::displayInstructions()
{
    auto queue = m_Chip.getCurrentInstruction();
    auto itr = queue.begin();

    int i = 0;

    FC_Draw(m_Font, m_Renderer, 0, SCREEN_HEIGHT * TILE_SIZE + 10 + DEBUG_OFFSET, "Instructions");
    int offset = SCREEN_HEIGHT * TILE_SIZE + 30 + DEBUG_OFFSET;

    while (itr != queue.end())
    {
	   if(i == queue.size() - 1)
		  FC_Draw(m_Font, m_Renderer, 0,offset + FC_GetLineHeight(m_Font) * i, "->%s", (*itr).c_str());
	   else
		  FC_Draw(m_Font, m_Renderer, 0,offset + FC_GetLineHeight(m_Font) * i, (*itr).c_str());
	   i++;
	   itr++;
    }

}

void Engine::displayRegister()
{
    const uint8_t* v = m_Chip.getMemory()->m_v;
    
    int offset = SCREEN_HEIGHT * TILE_SIZE + 30 + DEBUG_OFFSET;

    FC_Draw(m_Font, m_Renderer, 100, SCREEN_HEIGHT * TILE_SIZE + 10 + DEBUG_OFFSET, "Registers");
    int i = 0;
    for (i = 0; i < 0xf; i++)
    {
	   FC_Draw(m_Font, m_Renderer, 100, offset  + FC_GetLineHeight(m_Font) * i, "V%02x: 0x%02x", i, v[i]);
    }
   
}

void Engine::displayMemory()
{
    uint16_t address = 0x200;
    auto memory = m_Chip.getMemory()->m_memory;

    int bytePerLine = 8;
    int offset = 30;

    FC_Draw(m_Font, m_Renderer, SCREEN_WIDTH * TILE_SIZE + 10 + DEBUG_OFFSET, 10, "Memory");
    for (int i = 0; address < 0x300 /*4096*/; i++, address += bytePerLine)
    {
	   FC_Draw(m_Font, m_Renderer, SCREEN_WIDTH * TILE_SIZE + 10, offset + FC_GetLineHeight(m_Font) * i, "0x%04x: %02x%02x %02x%02x %02x%02x %02x%02x", address, 
		  memory[address + 0], memory[address + 1], memory[address + 2], memory[address + 3],
		  memory[address + 4], memory[address + 5], memory[address + 6], memory[address + 7]);
    }
}

void Engine::displayStack()
{
    /*TODO: kiem tra lai address cua stack*/
   
    auto stack = m_Chip.getMemory()->m_stack;
    int address = int(stack) - int(m_Chip.getMemory());
    auto mem = m_Chip.getMemory();

    int bytePerLine = 2;
    int i = 0;
    int offset = SCREEN_HEIGHT * TILE_SIZE + 30 + DEBUG_OFFSET;

    FC_Draw(m_Font, m_Renderer, 200, SCREEN_HEIGHT * TILE_SIZE + 10 + DEBUG_OFFSET, "Stack");
    for (i = 0; i < 16 / bytePerLine; i++, address += bytePerLine)
    {
	   FC_Draw(m_Font, m_Renderer, 200, offset  + FC_GetLineHeight(m_Font) * i, "0x%04x: %04x", address,
		  stack[i + 0]);
    }
    
    FC_Draw(m_Font, m_Renderer, 200, offset + FC_GetLineHeight(m_Font) * ++i, "I    : 0x%04x", mem->m_i);
    FC_Draw(m_Font, m_Renderer, 200, offset + FC_GetLineHeight(m_Font) * ++i, "SP   : 0x%04x", mem->m_sp);
    FC_Draw(m_Font, m_Renderer, 200, offset + FC_GetLineHeight(m_Font) * ++i, "PC   : 0x%04x", mem->m_pc);
}

void Engine::displayKeyStates()
{
    auto keys = m_Chip.getMemory()->m_key;
    
    int offset = SCREEN_HEIGHT * TILE_SIZE + 30 + DEBUG_OFFSET;
    FC_Draw(m_Font, m_Renderer, 300, SCREEN_HEIGHT * TILE_SIZE + 10 + DEBUG_OFFSET, "Keys");
    for (int i = 0; i < 16; i+=4)
    {
	   FC_Draw(m_Font, m_Renderer, 300, offset + FC_GetLineHeight(m_Font) * (i / 4), "%02x %02x %02x %02x", keys[i+0], keys[i + 1], keys[i + 2], keys[i + 3]);
    }
}

void Engine::displayDispMem()
{
   /* auto dispMem = m_Chip.getMemory()->m_disp_mem;


    FC_Draw(m_Font, m_Renderer, 400, SCREEN_HEIGHT * TILE_SIZE + 10, "Display mem");
    for (int i = 0; i < 16; i += 4)
    {
	   FC_Draw(m_Font, m_Renderer, 300, SCREEN_HEIGHT * TILE_SIZE + 30 + FC_GetLineHeight(m_Font) * (i / 4), "%02x %02x %02x %02x", keys[i + 0], keys[i + 1], keys[i + 2], keys[i + 3]);
    }*/
}

void Engine::displayTimers()
{
    auto delay = m_Chip.getMemory()->m_delay_timer;
    auto sound = m_Chip.getMemory()->m_sound_timer;

    int offset = SCREEN_HEIGHT * TILE_SIZE + 100 + DEBUG_OFFSET;

    FC_Draw(m_Font, m_Renderer, 300, offset						 , "Delay timer: 0x%02X", delay);
    FC_Draw(m_Font, m_Renderer, 300, offset + FC_GetLineHeight(m_Font) , "Sound timer: 0x%02X", sound);
}
