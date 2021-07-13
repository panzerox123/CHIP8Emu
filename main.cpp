#include "gui/gui.h"
#include "emulator/chip8.h"

int main()
{
    CHIP8 x;
    x.load_ROM("roms/maze.ch8");
    return 0;
}