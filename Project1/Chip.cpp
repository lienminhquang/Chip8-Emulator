#include "Chip.h"
#include <fstream>
#include <sstream>


//#define PRINT_INSTRCTION

Chip::Chip()
{
    for (int i = 0; i < H; i++)
    {
	   for (int j = 0; j < W; j++)
	   {
		  m_Screen[j + i * W] = 0;
	   }
    }
}

void Chip::init()
{
    //Clear memory
    //for (int i = 0; i < 4096; i++)
    //   memory.m_memory[i] = 0x00;

    memory.m_pc = 0x200; //Program counter start at 0x200
    memory.m_i = 0x0000;
    memory.m_sp = 0x0;

    //Clear keys
    for (int i = 0; i < 0xf; i++)
    {
	   memory.m_key[i] = 0;
    }

    //Clear display
    for (int i = 0; i < W * H / 8; i++)
	   memory.m_disp_mem[i] = 0x00;
    //Clear stack
    for (int i = 0; i < 16; i++)
	   memory.m_stack[i] = 0x0;
    //Clear registers V0-VF
    for (int i = 0; i < 0xF; i++)
	   memory.m_v[i] = 0x0;

    //Load fontset
    for (int i = 0; i < 16 * 5; i++)
	   memory.m_font[i] = m_fontset[i];

    //Reset timers
    memory.m_delay_timer = 0x00;
    memory.m_sound_timer = 0x00;

}

void Chip::loadGame(const std::string& name, unsigned int pos)
{
    for (std::ifstream f(name, std::ios::binary); f.good();)
	   memory.m_memory[pos++ & 0xFFF] = f.get();
}

void Chip::emulateCycle()
{
    uint16_t opcode = memory.m_memory[memory.m_pc] << 8 | memory.m_memory[memory.m_pc + 1];
    unsigned int nnn = opcode & 0x0FFF;
    unsigned int x = (opcode & 0x0F00) >> 8;
    unsigned int kk = opcode & 0x00FF;
    unsigned int y = (opcode & 0x00F0) >> 4;
    unsigned int n = opcode & 0x000F;

    switch (opcode & 0xF000)
    {
    case 0x0000:
    {
	   switch (opcode & 0x00FF)
	   {
	   case 0x00E0: //00E0 clear screen
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "CLS" << "\n";
#endif // PRINT_INSTRCTION

		  for (int i = 0; i < W * H; i++)
			 m_Screen[i] = 0;

		  m_DrawFlag = true;

		  memory.m_pc += 2;
	   }
	   break;

	   case 0x00EE: // 0x00EE: Returns from subroutine
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "RET" << "\n";
#endif // PRINT_INSTRCTION
		  memory.m_pc = memory.m_stack[memory.m_sp--];
		  memory.m_pc += 2;
	   }
	   break;

	   default:
	   {
		  std::cout << "Error: Unknow opcode\n";
		  //m_end = true;

	   }
	   break;
	   }
    }
    break;

    case 0x1000: // 1NNN: Jump to location NNN
    {
#ifdef PRINT_INSTRCTION
	   std::cout << "JP " << std::hex << nnn << "\n";
#endif // PRINT_INSTRCTION

	   memory.m_pc = nnn;
    }
    break;

    case 0x2000: // 2NNN: Call Subroutine at NNN
    {
#ifdef PRINT_INSTRCTION
	   std::cout << "CALL " << std::hex << nnn << "\n";
#endif // PRINT_INSTRCTION

	   // memory.m_pc += 2;
	   memory.m_stack[++memory.m_sp] = memory.m_pc;
	   memory.m_pc = nnn;
    }
    break;

    case 0x3000: // 0x3XKK: Skip next instruction if Vx = KK
    {
#ifdef PRINT_INSTRCTION
	   std::cout << "SE V" << std::hex << x << ", " << std::hex << kk << "\n";
#endif // PRINT_INSTRCTION

	   if (memory.m_v[x] == kk)
	   {
		  memory.m_pc += 4;
	   }
	   else
	   {
		  memory.m_pc += 2;
	   }
    }
    break;

    case 0x4000: // 0x4XKK: skip next instruction if Vx != kk
    {
#ifdef PRINT_INSTRCTION
	   std::cout << "SNE V" << std::hex << x << ", " << std::hex << kk << "\n";
#endif // PRINT_INSTRCTION
	   if (memory.m_v[x] != kk)
	   {
		  memory.m_pc += 4;
	   }
	   else
	   {
		  memory.m_pc += 2;
	   }
    }
    break;

    case 0x5000: // 0x5XY0: Skip next instruction if Vx = Vy
    {
#ifdef PRINT_INSTRCTION
	   std::cout << "SE V" << std::hex << x << ", V" << std::hex << y << "\n";
#endif // PRINT_INSTRCTION

	   if (memory.m_v[x] == memory.m_v[y])
	   {
		  memory.m_pc += 4;
	   }
	   else
	   {
		  memory.m_pc += 2;
	   }
    }
    break;

    case 0x6000: // 0x6XKK: set Vx to KK
    {
#ifdef PRINT_INSTRCTION
	   std::cout << "LD V" << std::hex << x << ", " << std::hex << kk << "\n";
#endif // PRINT_INSTRCTION

	   memory.m_v[x] = kk;

	   memory.m_pc += 2;
    }
    break;

    case 0x7000: // 0x7XKK: set Vx = Vx + kk
    {
#ifdef PRINT_INSTRCTION
	   std::cout << "ADD V" << std::hex << x << ", " << std::hex << kk << "\n";
#endif // PRINT_INSTRCTION

	   memory.m_v[x] += kk;

	   memory.m_pc += 2;
    }
    break;

    case 0x8000:
    {
	   switch (opcode & 0x000F)
	   {
	   case 0x0000:// 0x8XY0: set Vx = Vy
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "LD V" << std::hex << x << ", V" << std::hex << y << "\n";
#endif // PRINT_INSTRCTION

		  memory.m_v[x] = memory.m_v[y];

		  memory.m_pc += 2;
	   }
	   break;
	   case 0x0001: //0x8XY1: set Vx = Vx OR Vy
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "OR V" << std::hex << x << ", V" << std::hex << y << "\n";
#endif // PRINT_INSTRCTION

		  memory.m_v[x] = memory.m_v[x] | memory.m_v[y];

		  memory.m_pc += 2;
	   }
	   break;
	   case 0x0002: // 0x8XY2: set Vx = Vx AND Vy
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "AND V" << std::hex << x << ", V" << std::hex << y << "\n";
#endif // PRINT_INSTRCTION
		  memory.m_v[x] = memory.m_v[x] & memory.m_v[y];

		  memory.m_pc += 2;
	   }
	   break;
	   case 0x0003: // 0x8XY3: set Vx = Vx XOR Vy
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "XOR V" << std::hex << x << ", V" << std::hex << y << "\n";
#endif // PRINT_INSTRCTION

		  memory.m_v[x] = memory.m_v[x] ^ memory.m_v[y];

		  memory.m_pc += 2;
	   }
	   break;
	   case 0x0004: // 0x8XY4: set Vx = Vx + Vy, set VF = carry.
	   {

#ifdef PRINT_INSTRCTION
		  std::cout << "ADD V" << std::hex << x << ", V" << std::hex << y << "\n";
#endif // PRINT_INSTRCTION

		  if (memory.m_v[x] + memory.m_v[y] > 255)
			 memory.m_v[0xF] = 1;
		  else
			 memory.m_v[0xF] = 0;

		  memory.m_v[x] += memory.m_v[y];


		  memory.m_pc += 2;
	   }
	   break;
	   case 0x0005: // 0x8XY5: set Vx = Vx - Vy, set VF = NOT borrow
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "SUB V" << std::hex << x << ", V" << std::hex << y << "\n";
#endif // PRINT_INSTRCTION

		  /*if (memory.m_v[x] > memory.m_v[y])
			 memory.m_v[0xF] = 1;
		  else
			 memory.m_v[0xF] = 0;

		  memory.m_v[x] -= memory.m_v[y];*/

		  int a = memory.m_v[x] - memory.m_v[y];
		  memory.m_v[0xF] = !(a >> 8);
		  memory.m_v[x] = a;

		  memory.m_pc += 2;
	   }
	   break;
	   case 0x0006: // 0x8XY6: set Vx = Vx SHR 1
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "SHR V" << std::hex << x << "\n";
#endif // PRINT_INSTRCTION

		  memory.m_v[0xF] = memory.m_v[x] & 0x01;
		  memory.m_v[x] >>= 1;

		  memory.m_pc += 2;
	   }
	   break;
	   case 0x0007: // 0x8XY7: set Vx = Vy - Vx, set VF = NOT borrow
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "SUBN V" << std::hex << x << ", V" << std::hex << y << "\n";
#endif // PRINT_INSTRCTION

		  int a = memory.m_v[y] - memory.m_v[x];
		  memory.m_v[0xF] = !(a >> 8);
		  memory.m_v[x] = a;

		  memory.m_pc += 2;
	   }
	   break;
	   case 0x000E: // 0x8XYE: set Vx = Vx SHL 1
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "SHL V" << std::hex << x << "\n";
#endif // PRINT_INSTRCTION

		  memory.m_v[0xF] = (memory.m_v[x] & 0x80) == 0x80 ? 1 : 0;

		  memory.m_v[x] <<= 1;

		  memory.m_pc += 2;
	   }
	   break;

	   default:
	   {
		  std::cout << "Error: Unknow opcode\n";
		  m_end = true;
	   }
	   break;
	   }
    }
    break;

    case 0x9000: // 0x9XY0: Skip next instruction if Vx != Vy
    {
#ifdef PRINT_INSTRCTION
	   std::cout << "SNE V" << std::hex << x << ", V" << std::hex << y << "\n";
#endif // PRINT_INSTRCTION

	   if (memory.m_v[x] != memory.m_v[y])
		  memory.m_pc += 4;
	   else
		  memory.m_pc += 2;
    }
    break;

    case 0xA000: //ANNN: Sets I to the address NNN
    {
#ifdef PRINT_INSTRCTION
	   std::cout << "LD I" << std::hex << nnn << "\n";
#endif // PRINT_INSTRCTION

	   memory.m_i = nnn;
	   memory.m_pc += 2;
    }
    break;

    case 0xB000: // BNNN: Jump to location NNN + V0
    {
#ifdef PRINT_INSTRCTION
	   std::cout << "JP V0" << std::hex << nnn << "\n";
#endif // PRINT_INSTRCTION

	   memory.m_pc = nnn + memory.m_v[0];
    }
    break;

    case 0xC000: // CXKK: set Vx = random byte AND kk
    {
#ifdef PRINT_INSTRCTION
	   std::cout << "RND V" << std::hex << x << ", " << std::hex << nnn << "\n";
#endif // PRINT_INSTRCTION

	   //srand(123);
	   int rd = rand() % 256;
	   memory.m_v[x] = rd & kk;

	   memory.m_pc += 2;
    }
    break;

    case 0xD000: // Dxyn: Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
    {
#ifdef PRINT_INSTRCTION
	   std::cout << "DRW V" << std::hex << x << ", V" << std::hex << y << "\n";
#endif // PRINT_INSTRCTION

	   memory.m_v[0xf] = 0;

	   int posX = memory.m_v[x];
	   int posY = memory.m_v[y];

	   int startAddress = memory.m_i;


	   for (unsigned int i = 0; i < n; i++)
	   {
		  uint8_t byte = memory.m_memory[startAddress + i];
		  for (int j = 7; j >= 0; j--)
		  {
			 int bit = (byte >> j) & 1;
			 if (bit == 1)
			 {

				int pos = (posX + 7 - j) + ((posY + i) * 64);

				/*if ((((memory.m_disp_mem[pos / 8]) >> j) & 1) == 1)
				{
				    memory.m_v[0xf] = 1;
				}
				memory.m_disp_mem[pos / 8] ^= 1;*/
				if (m_Screen[pos] == 1)
				    memory.m_v[0xf] = 1;

				m_Screen[pos] ^= 1;
			 }

		  }
	   }

	   //m_DrawFlag = true;

	   memory.m_pc += 2;
    }
    break;

    case 0xE000: // E
    {
	   switch (opcode & 0x00FF)
	   {
	   case 0x009E: // EX9E: Skip next instruction if key with value of Vx is pressed
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "SKP V" << std::hex << x << "\n";
#endif // PRINT_INSTRCTION

		  if (memory.m_key[memory.m_v[x]] != 0)
		  {
			 memory.m_pc += 4;
		  }
		  else
			 memory.m_pc += 2;
	   }
	   case 0x00A1: // EXA1: Skip next instruction if key with value of Vx is not pressed
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "SKNP V" << std::hex << x << "\n";
#endif // PRINT_INSTRCTION

		  if (memory.m_key[memory.m_v[x] & 0xf] == 0)
		  {
			 memory.m_pc += 4;
		  }
		  else
			 memory.m_pc += 2;
	   }
	   break;
	   default:
	   {
		  std::cout << "Error: Unknow opcode\n";
		  m_end = true;

	   }
	   break;
	   }
    }
    break;

    case 0xF000:
    {
	   switch (opcode & 0x00FF)
	   {
	   case 0x0007: // Fx07: set Vx = delay timer value
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "LD V" << std::hex << x << ", DT" << "\n";
#endif // PRINT_INSTRCTION

		  memory.m_v[x] = memory.m_delay_timer;
		  memory.m_pc += 2;
	   }
	   break;
	   case 0x000A: // Fx0A: Wait for a key press, store value of the key in Vx
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "LD V" << std::hex << x << ", K" << memory.m_v[x] << "\n";
#endif // PRINT_INSTRCTION

		  /*TODO Wait for key press*/

		  if (!m_WaitingKeyPress)
		  {
			 std::cout << "Begin waiting for key press\n";
			 m_WaitingKeyPress = true;
			 m_CurrentKeyPress = -1;
		  }
		  else if (m_CurrentKeyPress >= 0 && m_CurrentKeyPress <= 0xf)
		  {
			 std::cout << "Key press! End wait for key. \n";
			 memory.m_v[x] = m_CurrentKeyPress; //

			 memory.m_pc += 2;
		  }
		  std::cout << "Wait for key press\n";
	   }
	   break;
	   case 0x0015: // Fx15: set delay timer to value of Vx
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "LD DT, V" << std::hex << x << "\n";
#endif // PRINT_INSTRCTION

		  memory.m_delay_timer = memory.m_v[x];
		  memory.m_pc += 2;
	   }
	   break;
	   case 0x0018: // Fx18: set sound timer to value of Vx
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "LD ST, V" << std::hex << x << "\n";
#endif // PRINT_INSTRCTION

		  memory.m_sound_timer = memory.m_v[x];
		  memory.m_pc += 2;
	   }
	   break;
	   case 0x001E: // Fx1E: set I = I + Vx
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "ADD I, V" << std::hex << x << "\n";
#endif // PRINT_INSTRCTION

		  memory.m_i += memory.m_v[x];
		  memory.m_pc += 2;
	   }
	   break;
	   case 0x0029: // Fx29: set I = location of sprite for digit Vx
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "LD F, V" << std::hex << x << "\n";
#endif // PRINT_INSTRCTION

		  memory.m_i = (uint16_t)(memory.m_font - memory.m_memory) + (memory.m_v[x] % 0xf * 5);
		  memory.m_pc += 2;
	   }
	   break;
	   case 0x0033: // Fx33: store BCD representation of Vx in memory locations I, I + 1, I + 2
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "LD B, V" << std::hex << x << "\n";
#endif // PRINT_INSTRCTION

		  memory.m_memory[memory.m_i + 0] = (memory.m_v[x] / 100) % 10;
		  memory.m_memory[memory.m_i + 1] = (memory.m_v[x] / 10) % 10;
		  memory.m_memory[memory.m_i + 2] = (memory.m_v[x] / 1) % 10;

		  memory.m_pc += 2;
	   }
	   break;
	   case 0x0055: // Fx55: store registers V0 through Vx in memory starting at location I
	   {
#ifdef PRINT_INSTRCTION
		  std::cout << "LD [I], V" << std::hex << x << "\n";
#endif // PRINT_INSTRCTION

		  for (unsigned int i = 0; i <= x; i++)
		  {
			 memory.m_memory[memory.m_i++] = memory.m_v[i];
		  }

		  memory.m_pc += 2;
	   }
	   break;
	   case 0x0065: // Fx65: read register V0 through Vx from memory starting at location I
	   {

#ifdef PRINT_INSTRCTION
		  std::cout << "LD V" << std::hex << x << ", I" << "\n";
#endif // PRINT_INSTRCTION

		  for (unsigned int i = 0; i <= x; i++)
		  {
			 memory.m_v[i] = memory.m_memory[memory.m_i++];
		  }

		  memory.m_pc += 2;
	   }
	   break;
	   default:
	   {
		  std::cout << "Error: Unknow opcode\n";
		  m_end = true;
		  return;
	   }
	   break;
    }
    }
    break;

    default:
    {
	   std::cout << "Error: Unknow opcode\n";
	   m_end = true;
	   return;
    }
    break;
}



    //system("cls");
    //std::cout << "Delay Timer: " << (int)memory.m_delay_timer << "\n";
    //std::cout << "Sound Timer: " << (int)memory.m_sound_timer << "\n";
}

char* Chip::getScreenChars()
{
    return m_Screen;
}

void Chip::keyDown(uint8_t key)
{
    memory.m_key[key] = 1;
    m_CurrentKeyPress = key;
}

void Chip::keyUp(uint8_t key)
{
    memory.m_key[key] = 0;
}

void Chip::display()
{
    if (m_DrawFlag == false) return;

    system("cls");

    std::stringstream str;

    for (int i = 0; i < H / 1; i++)
    {
	   for (int j = 0; j < W; j++)
	   {
		  int pos = j + i * W;



		  // std::cout << (char)(((memory.m_disp_mem[pos / 8] >> (7 - pos % 8)) & 1) == 1 ? 219 : 176);
		  str << (char)(m_Screen[pos] == 1 ? 219 : 176);
	   }
	   str << "\n";
    }

    std::cout << str.str();



}

void Chip::updateTimer(int count)
{
    //Update timers
    if (memory.m_delay_timer > 0)
    {
	   memory.m_delay_timer -= count;
	   if (memory.m_delay_timer < 0)
		  memory.m_delay_timer = 0;
    }

    if (memory.m_sound_timer > 0)
    {
	   if (memory.m_sound_timer == 1)
		  std::cout << "BEEP\n";
	   memory.m_sound_timer -= count;

	   if (memory.m_sound_timer < 0)
		  memory.m_sound_timer = 0;
    }
}
