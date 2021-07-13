#include "log.h"

void LOG::log_opcode(uint16 opcode, const char * log_string){
    printf("[LOG_OPCODE] %u : %s\n", opcode, log_string);
}