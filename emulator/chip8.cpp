#include "chip8.h"

void CHIP8::decodeInstructions(uint16 opcode)
{
    char * str;
    uint16 addr;
    uint8 vreg;
    uint8 vreg_1;
    uint8 vreg_2;
    uint8 byte;
    uint8 nibble;
    switch (opcode & 0xF000)
    {

    case 0x0000:
        switch (opcode)
        {
        case 0x00E0:
            // Clear the display
            logger.log_opcode(opcode, "CLS");
            break;
        case 0x00EE:
            // Return from function
            logger.log_opcode(opcode, "RET");
            break;
        default:
            // Deprecated instruction; Jump to machine code at addr
            addr = opcode & 0x0FFF;
            sprintf(str, "SYS to addr %u", addr);
            logger.log_opcode(opcode, str);
        }
        break;
    case 0x1000:
        // Jump to addr
        addr = opcode & 0x0FFF;
        sprintf(str, "JP to addr %u", addr);
        logger.log_opcode(opcode, str);
        break;
    case 0x2000:
        // Call function at addr, after pushing return pointer to stack
        addr = opcode & 0x0FFF;
        sprintf(str, "CALL to addr %u", addr);
        logger.log_opcode(opcode, str);
        break;
    case 0x3000:
        // Skip instruction if vreg == byte
        vreg = (opcode & 0x0F00) >> 8; // x
        byte = (opcode & 0x00FF);      // kk
        sprintf(str, "Skip if equal; vreg : %u ; byte : %u", vreg, byte);
        logger.log_opcode(opcode, str);
        break;
    case 0x4000:
        // Skip instruction if vreg != byte
        vreg = (opcode & 0x0F00) >> 8; // x
        byte = (opcode & 0x00FF);      // kk
        sprintf(str, "Skip if not equal; vreg : %u ; byte : %u", vreg, byte);
        logger.log_opcode(opcode, str);
        break;
    case 0x5000:
        // Skip instruction if vreg_1 != vreg_2
        vreg_1 = (opcode & 0x0F00) >> 8; // x
        vreg_2 = (opcode & 0x00F0) >> 4; // y
        sprintf(str, "Skip if equal; vreg_1 : %u ; vreg_2 : %u", vreg_1, vreg_2);
        logger.log_opcode(opcode, str);
        break;
    case 0x6000:
        // Set vreg = byte
        vreg = (opcode & 0x0F00) >> 8; // x
        byte = (opcode & 0x00FF);      // kk
        sprintf(str, "Load; vreg(%u) = byte(%u)", vreg, byte);
        logger.log_opcode(opcode, str);
        break;
    case 0x7000:
        // Add Vx and byte
        vreg = (opcode & 0x0F00) >> 8; // x
        byte = (opcode & 0x00FF);      // kk
        sprintf(str, "ADD; vreg(%u) + byte(%u)", vreg, byte);
        logger.log_opcode(opcode, str);
        break;
    case 0x8000:
        // Load from register
        vreg_1 = (opcode & 0x0F00) >> 8; // x
        vreg_2 = (opcode & 0x00F0) >> 4; // y
        switch (opcode & 0x000F)
        {
        case 0x0000:
            // Vx=Vy
            sprintf(str, "Load; vreg_1 : %u = vreg_2 : %u", vreg_1, vreg_2);
            logger.log_opcode(opcode, str);
            break;
        case 0x0001:
            // Vx = Vx | Vy
            sprintf(str, "OR; vreg_1(%u) | vreg_2(%u)", vreg_1, vreg_2);
            logger.log_opcode(opcode, str);
            break;
        case 0x0002:
            // Vx = Vx & Vy
            sprintf(str, "AND; vreg_1(%u) & vreg_2(%u)", vreg_1, vreg_2);
            logger.log_opcode(opcode, str);
            break;
        case 0x0003:
            // Vx = Vx ^ Vy
            sprintf(str, "XOR; vreg_1(%u) ^ vreg_2(%u)", vreg_1, vreg_2);
            logger.log_opcode(opcode, str);
            break;
        case 0x0004:
            // Vx = Vx+Vy ; If result > 8 bits, VF set to 1 (carry)
            sprintf(str, "ADD; vreg_1(%u) + vreg_2(%u)", vreg_1, vreg_2);
            logger.log_opcode(opcode, str);
            break;
        case 0x0005:
            // Vx = Vx - Vy ; VF = NOT Borrow (Set to 1 if Vx > Vy)
            sprintf(str, "SUB; vreg_1(%u) - vreg_2(%u)", vreg_1, vreg_2);
            logger.log_opcode(opcode, str);
            break;
        case 0x0006:
            //Shift right Instruction; If LSB of Vx is 1, VF set to 1, otherwise 0; Vx divided by 2, Vx >> Vy
            // Unsure
            sprintf(str, "SHR; vreg_1(%u) >> vreg_2(%u)", vreg_1, vreg_2);
            logger.log_opcode(opcode, str);
            break;
        case 0x0007:
            // SUBN ; Vx = Vy - Vx ; VF is 1 if Vy > Vx
            sprintf(str, "SUBN; vreg_1(%u) | vreg_2(%u)", vreg_1, vreg_2);
            logger.log_opcode(opcode, str);
            break;
        case 0x000E:
            // Shift left; If MSB of Vx is 1, VF set to 1, otherwise 0; Vx multiplied by 2; Vx << Vy
            // Unsure
            sprintf(str, "SHL; vreg_1(%u) << vreg_2(%u)", vreg_1, vreg_2);
            logger.log_opcode(opcode, str);
            break;
        }
        break;
    case 0x9000:
        // Skip next instruction if Vx != Vy. If not equal, PC incremented by 2.
        vreg_1 = (opcode & 0x0F00) >> 8; // x
        vreg_2 = (opcode & 0x00F0) >> 4; // y
        sprintf(str, "Skip if not equal; vreg_1 : %u ; vreg_2 : %u", vreg_1, vreg_2);
        logger.log_opcode(opcode, str);
        break;
    case 0xA000:
        // Value of register i set to addr;
        addr = opcode & 0x0FFF;
        sprintf(str, "Setting I reg to addr: %u", addr);
        logger.log_opcode(opcode, str);
        break;
    case 0xB000:
        // Jump to location NNN + V0
        addr = opcode & 0x0FFF;
        sprintf(str, "Jump to %u + V0", addr);
        logger.log_opcode(opcode, str);
        break;
    case 0xC000:
        // Random 8 bit number & byte; stored in Vx
        vreg = (opcode & 0x0F00) >> 8; // x
        byte = opcode & 0x00FF;        // kk
        sprintf(str, "Random : vreg %u : RND & %u", vreg, byte);
        logger.log_opcode(opcode, str);
        break;
    case 0xD000:
        // Display n-byte sprite starting at memory location stored in I register at point (Vx, Vy);
        // set VF = 1 in case pixel needs to be erased.
        vreg_1 = (opcode & 0x0F00) >> 8; // x
        vreg_2 = (opcode & 0x00F0) >> 4; // y
        nibble = (opcode & 0x000F);
        sprintf(str, "Draw : nibble(%u) at point(%u, %u)", nibble, vreg_1, vreg_2);
        logger.log_opcode(opcode, str);
        break;
    case 0xE000:
        vreg = (opcode & 0x0F00) >> 8; // x
        switch (opcode & 0x00FF)
        {
        case 0x009E:
            // Skip next instruction if key with value Vx is pressed.
            sprintf(str, "SKP if keypress == (%u)", vreg);
            logger.log_opcode(opcode, str);
            break;

        case 0x00A1:
            // Skip next instruction if key with value Vx is not pressed.
            sprintf(str, "SKNP if keypress != (%u)", vreg);
            logger.log_opcode(opcode, str);
            break;
        }
    case 0xF000:
        vreg = (opcode & 0x0F00) >> 8;
        switch (opcode & 0x00FF)
        {
        case 0x0007:
            // Load delay timer
            sprintf(str, "LD [%u], DT", vreg);
            logger.log_opcode(opcode, str);
            break;
        case 0x000A:
            // Wait for keypress, store value of key in Vx. Halt till keypress
            sprintf(str, "LD [%u], K", vreg);
            logger.log_opcode(opcode, str);
            break;
        case 0x0015:
            // Set delay timer = Vx
            sprintf(str, "LD DT, [%u]", vreg);
            logger.log_opcode(opcode, str);
            break;
        case 0x0018:
            // Set sound timer = Vx
            sprintf(str, "LD ST, [%u]", vreg);
            logger.log_opcode(opcode, str);
            break;
        case 0x001E:
            // Set I = I + Vx
            sprintf(str, "ADD I, [%u]", vreg);
            logger.log_opcode(opcode, str);
            break;
        case 0x0029:
            // Set I = location of sprite for digit Vx
            // Moar research pls halp
            sprintf(str, "LD F, [%u]", vreg);
            logger.log_opcode(opcode, str);
            break;


        // INCOMPLETE / NOT UNDERSTOOD FULLY
        case 0x0033:
            // Store BCD representation of Vx in memory locations I, I+1, I+2
            sprintf(str, "LD F, [%u]", vreg);
            logger.log_opcode(opcode, str);
            break;
        case 0x0055:
            sprintf(str, "LD F, [%u]", vreg);
            logger.log_opcode(opcode, str);
            break;
        case 0x0065:
            sprintf(str, "LD F, [%u]", vreg);
            logger.log_opcode(opcode, str);
            break;
        }
    }
}
