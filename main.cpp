#include "gui/gui.h"
#include "emulator/chip8.h"

int main()
{
    CHIP8 x;
    x.init("roms/Cave.ch8");
    return 0;
}