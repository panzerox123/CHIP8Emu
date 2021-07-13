#ifndef CHIP8_H
#define CHIP8_H 1
#include "types.h"
#include "log.h"
#include <stdio.h>
#include "../gui/gui.h"

/*
 * Reference documentation
 * http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
 */

class CHIP8
{
  uint8 memory[4096] {0};
  uint8 V_reg[16], timer_reg, sound_reg, sp_reg;
  uint16 I_reg, pc_reg, stack[16];
  LOG logger;
  GUI * gui;

public:
  CHIP8();
  void decode_instructions(uint16);
  void load_ROM(const char *);

};

#endif
