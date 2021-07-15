#ifndef CHIP8_H
#define CHIP8_H 1
#include "types.h"
#include "log.h"
#include <stdio.h>
#include "../gui/gui.h"
#include <fstream>
#include <stdlib.h>
#include <thread> 
/*
 * Reference documentation
 * http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
 */

#define MEM_SIZE 4096
#define STACK_SIZE 16
#define NUM_V_REGS 16

class CHIP8
{
  uint8 * memory;
  uint8 * V_reg; 
  uint8 timer_reg, sound_reg, sp_reg;
  uint16 I_reg, pc_reg;
  uint16 * stack;
  LOG logger;
  GUI * gui;

public:
  CHIP8();
  void decode_instructions(uint16);
  void load_ROM(const char *);
  void init(const char *);
  void initialise_sprites();
};

void gui_thread_wrapper(GUI *);

#endif
