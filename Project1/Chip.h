#pragma once
#include <string>
#include <iostream>
#include <cstdlib>
#include <string>
#include <deque>

#define W 64
#define H 32

class Chip
{
public:
    bool m_end = false;

private:

    union uMemory
    {
	   uint8_t m_memory[4096]{0x12, 0x00};
	   struct 
	   {
		  uint8_t m_v[16];				   //16*8
		  uint8_t m_delay_timer;			   //1*8
		  uint8_t m_sound_timer;			   //1*8
		  uint16_t m_sp;				   //1*16
		  uint8_t m_key[16];			   //16*8
		  uint8_t m_waiting_key;			   //1*8
		  uint8_t m_disp_mem[W * H / 8];	   //256*8
		  uint8_t m_font[80];			   //80*8
		  uint16_t m_pc;				   //1*16
		  uint16_t m_stack[16];			   //16*16
		  uint16_t m_i;				   //1*16
	   };
    };

    uMemory memory;
   
    uint8_t m_fontset[80] =
    {
	  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	  0x20, 0x60, 0x20, 0x20, 0x70, // 1
	  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };


    char m_Screen[W * H] = {0};
    bool m_DrawFlag = true;

    bool m_WaitingKeyPress = false;
    int m_CurrentKeyPress = -1;
    
    const int MAX_INS_IN_QUEUE = 10;
    std::deque<std::string> m_CurrentInsStringQueue;

private:
    void pushInsStringToQueue(const std::string& str);

public:
    Chip();
    ~Chip() {}

    void init();
    void loadGame(const std::string& name, unsigned int pos = 0x200);
    void emulateCycle();
    char* getScreenChars();
    void keyDown(uint8_t key);
    void keyUp(uint8_t key);
    void updateTimer(int count);

    const std::deque<std::string>& getCurrentInstruction() { return m_CurrentInsStringQueue; }
    const uMemory* getMemory() const { return &(memory); }
    
    bool needDraw() { return m_DrawFlag; }
    void resetDrawFlag() { m_DrawFlag = false; }
};

