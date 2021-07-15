#include "chip8.h"

void gui_thread_wrapper(GUI *gui)
{
    gui->loop();
}

CHIP8::CHIP8()
{
    srand(time(0));
    this->gui = new GUI(800, 600, 64, 32);
    this->memory = (uint8 *)calloc(MEM_SIZE, sizeof(uint8));
    this->V_reg = (uint8 *)calloc(NUM_V_REGS, sizeof(uint8));
    this->stack = (uint16 *)calloc(STACK_SIZE, sizeof(uint16));
    for (int i = 0; i < MEM_SIZE; i++)
    {
        this->memory[i] = 0;
    }
    for (int i = 0; i < STACK_SIZE; i++)
    {
        this->stack[i] = 0;
    }
    for (int i = 0; i < NUM_V_REGS; i++)
    {
        this->V_reg[i] = 0;
    }
    this->initialise_sprites();
    this->pc_reg = 512;
    this->timer_reg = 0;
    this->sound_reg = 0;
    this->sp_reg = 0;
}

void CHIP8::initialise_sprites()
{
    uint8 sprite_arr[] = {
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
    for (int i = 0; i < 80; i++)
    {
        this->memory[i] = sprite_arr[i];
    }
}

void CHIP8::init(const char *rom_file)
{
    std::thread gui_thread(gui_thread_wrapper, this->gui);
    load_ROM(rom_file);
    while (this->pc_reg < MEM_SIZE - 2)
    {
        uint16 opcode = memory[this->pc_reg] << 8 | memory[this->pc_reg + 1];
        if (gui_thread.joinable())
        {
            decode_instructions(opcode);
        }
        if (opcode == 0)
        {
            break;
        }
    }
    gui_thread.join();
}

void CHIP8::decode_instructions(uint16 opcode)
{
    char str[100];
    uint16 addr;
    uint8 vreg;
    uint8 vreg_1;
    uint8 vreg_2;
    uint8 byte;
    uint8 nibble;
    switch (opcode & 0xF000)
    {

    case 0x0000:
        switch (opcode & 0x00FF)
        {
        case 0x00E0:
            // Clear the display
            logger.log_opcode(opcode, "CLS");
            this->gui->clear_screen();
            break;
        case 0x00EE:
            // Return from function
            logger.log_opcode(opcode, "RET");
            this->pc_reg = this->stack[this->sp_reg];
            (this->sp_reg)--;
            break;
        default:
            // Deprecated instruction; Jump to machine code at addr
            addr = opcode & 0x0FFF;
            sprintf(str, "SYS to addr %x", addr);
            logger.log_opcode(opcode, str);
            this->pc_reg = addr;
            return;
        }
        break;
    case 0x1000:
        // Jump to addr
        addr = opcode & 0x0FFF;
        sprintf(str, "JP to addr %x", addr);
        logger.log_opcode(opcode, str);
        this->pc_reg = addr;
        return;
        break;
    case 0x2000:
        // Call function at addr, after pushing return pointer to stack
        addr = opcode & 0x0FFF;
        sprintf(str, "CALL to addr %x", addr);
        logger.log_opcode(opcode, str);
        (this->sp_reg)++;
        this->stack[this->sp_reg] = this->pc_reg;
        this->pc_reg = addr;
        return;
        break;
    case 0x3000:
        // Skip instruction if vreg == byte
        vreg = (opcode & 0x0F00) >> 8; // x
        byte = (opcode & 0x00FF);      // kk
        sprintf(str, "Skip if equal; vreg : %x ; byte : %x", vreg, byte);
        logger.log_opcode(opcode, str);
        if (this->V_reg[vreg] == byte)
        {
            this->pc_reg += 2;
        }
        break;
    case 0x4000:
        // Skip instruction if vreg != byte
        vreg = (opcode & 0x0F00) >> 8; // x
        byte = (opcode & 0x00FF);      // kk
        sprintf(str, "Skip if not equal; vreg : %x ; byte : %x", vreg, byte);
        logger.log_opcode(opcode, str);
        if (this->V_reg[vreg] != byte)
        {
            this->pc_reg += 2;
        }
        break;
    case 0x5000:
        // Skip instruction if vreg_1 == vreg_2
        vreg_1 = (opcode & 0x0F00) >> 8; // x
        vreg_2 = (opcode & 0x00F0) >> 4; // y
        sprintf(str, "Skip if equal; vreg_1 : %x ; vreg_2 : %x", vreg_1, vreg_2);
        logger.log_opcode(opcode, str);
        if (this->V_reg[vreg_1] == this->V_reg[vreg_2])
        {
            this->pc_reg += 2;
        }
        break;
    case 0x6000:
        // Set vreg = byte
        vreg = (opcode & 0x0F00) >> 8; // x
        byte = (opcode & 0x00FF);      // kk
        sprintf(str, "Load; vreg(%x) = byte(%x)", vreg, byte);
        logger.log_opcode(opcode, str);
        this->V_reg[vreg] = byte;
        break;
    case 0x7000:
        // Add Vx and byte
        vreg = (opcode & 0x0F00) >> 8; // x
        byte = (opcode & 0x00FF);      // kk
        sprintf(str, "ADD; vreg(%x) + byte(%x)", vreg, byte);
        logger.log_opcode(opcode, str);
        this->V_reg += byte;
        break;
    case 0x8000:
        // Load from register
        vreg_1 = (opcode & 0x0F00) >> 8; // x
        vreg_2 = (opcode & 0x00F0) >> 4; // y
        switch (opcode & 0x000F)
        {
        case 0x0000:
            // Vx=Vy
            sprintf(str, "Load; vreg_1 : %x = vreg_2 : %x", vreg_1, vreg_2);
            logger.log_opcode(opcode, str);
            this->V_reg[vreg_1] = this->V_reg[vreg_2];
            break;
        case 0x0001:
            // Vx = Vx | Vy
            sprintf(str, "OR; vreg_1(%x) | vreg_2(%x)", vreg_1, vreg_2);
            logger.log_opcode(opcode, str);
            this->V_reg[vreg_1] |= this->V_reg[vreg_2];
            break;
        case 0x0002:
            // Vx = Vx & Vy
            sprintf(str, "AND; vreg_1(%x) & vreg_2(%x)", vreg_1, vreg_2);
            logger.log_opcode(opcode, str);
            this->V_reg[vreg_1] &= this->V_reg[vreg_2];
            break;
        case 0x0003:
            // Vx = Vx ^ Vy
            sprintf(str, "XOR; vreg_1(%x) ^ vreg_2(%x)", vreg_1, vreg_2);
            logger.log_opcode(opcode, str);
            this->V_reg[vreg_1] ^= this->V_reg[vreg_2];
            break;
        case 0x0004:
            // Vx = Vx+Vy ; If result > 8 bits, VF set to 1 (carry)
            sprintf(str, "ADD; vreg_1(%x) + vreg_2(%x)", vreg_1, vreg_2);
            logger.log_opcode(opcode, str);
            this->V_reg[vreg_1] += this->V_reg[vreg_2];
            break;
        case 0x0005:
            // Vx = Vx - Vy ; VF = NOT Borrow (Set to 1 if Vx > Vy)
            sprintf(str, "SUB; vreg_1(%x) - vreg_2(%x)", vreg_1, vreg_2);
            logger.log_opcode(opcode, str);
            if (this->V_reg[vreg_1] > this->V_reg[vreg_2])
            {
                this->V_reg[0xf] = 1;
            }
            this->V_reg[vreg_1] -= this->V_reg[vreg_2];
            break;
        case 0x0006:
            //Shift right Instruction; If LSB of Vx is 1, VF set to 1, otherwise 0; Vx divided by 2, Vx >> Vy
            // Unsure
            sprintf(str, "SHR; vreg_1(%x) >> vreg_2(%x)", vreg_1, vreg_2);
            logger.log_opcode(opcode, str);
            if (this->V_reg[vreg_1] & 1 == 1)
            {
                this->V_reg[0xf] = 1;
            }
            this->V_reg[vreg_1] = this->V_reg[vreg_1] >> 1;
            break;
        case 0x0007:
            // SUBN ; Vx = Vy - Vx ; VF is 1 if Vy > Vx
            sprintf(str, "SUBN; vreg_1(%x) | vreg_2(%x)", vreg_1, vreg_2);
            logger.log_opcode(opcode, str);
            if (this->V_reg[vreg_1] < this->V_reg[vreg_2])
            {
                this->V_reg[0xf] = 1;
            }
            this->V_reg[vreg_1] = this->V_reg[vreg_2] - this->V_reg[vreg_1];
            break;
        case 0x000E:
            // Shift left; If MSB of Vx is 1, VF set to 1, otherwise 0; Vx multiplied by 2; Vx << Vy
            // Unsure
            sprintf(str, "SHL; vreg_1(%x) << vreg_2(%x)", vreg_1, vreg_2);
            logger.log_opcode(opcode, str);
            if (this->V_reg[vreg_1] & 0x80 == 0x80)
            {
                this->V_reg[0xf] = 1;
            }
            this->V_reg[vreg_1] = this->V_reg[vreg_1] << 1;
            break;
        }
        break;
    case 0x9000:
        // Skip next instruction if Vx != Vy. If not equal, PC incremented by 2.
        vreg_1 = (opcode & 0x0F00) >> 8; // x
        vreg_2 = (opcode & 0x00F0) >> 4; // y
        sprintf(str, "Skip if not equal; vreg_1 : %x ; vreg_2 : %x", vreg_1, vreg_2);
        logger.log_opcode(opcode, str);
        if (this->V_reg[vreg_1] != this->V_reg[vreg_2])
        {
            this->pc_reg += 2;
        }
        break;
    case 0xA000:
        // Value of register i set to addr;
        addr = opcode & 0x0FFF;
        sprintf(str, "Setting I reg to addr: %x", addr);
        logger.log_opcode(opcode, str);
        this->I_reg = addr;
        break;
    case 0xB000:
        // Jump to location NNN + V0
        addr = opcode & 0x0FFF;
        sprintf(str, "Jump to %x + V0", addr);
        logger.log_opcode(opcode, str);
        this->pc_reg = this->V_reg[0] + addr;
        break;
    case 0xC000:
        // Random 8 bit number & byte; stored in Vx
        vreg = (opcode & 0x0F00) >> 8; // x
        byte = opcode & 0x00FF;        // kk
        sprintf(str, "Random : vreg %x : RND & %x", vreg, byte);
        logger.log_opcode(opcode, str);
        this->V_reg[vreg] = rand() % 256 & byte;
        break;
    case 0xD000:
        // Display n-byte sprite starting at memory location stored in I register at point (Vx, Vy);
        // set VF = 1 in case pixel needs to be erased.
        vreg_1 = (opcode & 0x0F00) >> 8; // x
        vreg_2 = (opcode & 0x00F0) >> 4; // y
        nibble = (opcode & 0x000F);
        sprintf(str, "Draw : nibble(%x) at point(%x, %x)", nibble, vreg_1, vreg_2);
        logger.log_opcode(opcode, str);
        for (int i = 0; i < nibble; i++)
        {
            int sprite = this->memory[this->I_reg + i];
            for (int j = 7; j >= 0; j--)
            {
                int pos = this->V_reg[vreg_1] + i + this->gui->pixels_y * (j + this->V_reg[vreg_2]);
                if (this->gui->display[pos] == 1)
                    this->V_reg[0xF] = 1;
                switch ((sprite & 1))
                {
                case 0:
                    this->gui->display[pos] = 0;
                    break;
                case 1:
                    this->gui->display[pos] = 1;
                    break;
                }
                sprite = sprite >> 1;
            }
        }
        break;
    case 0xE000:
        vreg = (opcode & 0x0F00) >> 8; // x
        switch (opcode & 0x00FF)
        {
        case 0x009E:
            // Skip next instruction if key with value Vx is pressed.
            sprintf(str, "SKP if keypress == (%x)", vreg);
            logger.log_opcode(opcode, str);
            break;

        case 0x00A1:
            // Skip next instruction if key with value Vx is not pressed.
            sprintf(str, "SKNP if keypress != (%x)", vreg);
            logger.log_opcode(opcode, str);
            break;
        }
    case 0xF000:
        vreg = (opcode & 0x0F00) >> 8;
        switch (opcode & 0x00FF)
        {
        case 0x0007:
            // Load delay timer
            sprintf(str, "LD [%x], DT", vreg);
            logger.log_opcode(opcode, str);
            break;
        case 0x000A:
            // Wait for keypress, store value of key in Vx. Halt till keypress
            sprintf(str, "LD [%x], K", vreg);
            logger.log_opcode(opcode, str);
            break;
        case 0x0015:
            // Set delay timer = Vx
            sprintf(str, "LD DT, [%x]", vreg);
            logger.log_opcode(opcode, str);
            break;
        case 0x0018:
            // Set sound timer = Vx
            sprintf(str, "LD ST, [%x]", vreg);
            logger.log_opcode(opcode, str);
            break;
        case 0x001E:
            // Set I = I + Vx
            sprintf(str, "ADD I, [%x]", vreg);
            logger.log_opcode(opcode, str);
            break;
        case 0x0029:
            // Set I = location of sprite for digit Vx
            // Moar research pls halp
            sprintf(str, "LD F, [%x]", vreg);
            logger.log_opcode(opcode, str);
            break;

        // INCOMPLETE / NOT UNDERSTOOD FULLY
        case 0x0033:
            // Store BCD representation of Vx in memory locations I, I+1, I+2
            sprintf(str, "LD F, [%x]", vreg);
            logger.log_opcode(opcode, str);
            break;
        case 0x0055:
            sprintf(str, "LD F, [%x]", vreg);
            logger.log_opcode(opcode, str);
            break;
        case 0x0065:
            sprintf(str, "LD F, [%x]", vreg);
            logger.log_opcode(opcode, str);
            break;
        }
    }
    this->pc_reg += 2;
}

void CHIP8::load_ROM(const char *filename)
{
    uint8 rom[MEM_SIZE - 512] = {0};
    std::ifstream rom_fd(filename, std::ios::binary);
    if (!rom_fd)
    {
        perror("ERROR: Could not read ROM file");
        return;
    }
    rom_fd.read((char *)rom, MEM_SIZE - 512);
    rom_fd.close();
    for (int i = 0; i < (MEM_SIZE - 512); i++)
    {
        this->memory[i + 512] = rom[i];
    }
}