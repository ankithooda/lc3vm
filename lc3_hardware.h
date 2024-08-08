#include <stdint.h>


#define R_COUNT 10
#define MEMORY_MAX (1 << 16)

enum OPCODE {
  BR     =  0,
  ADD    =  1,
  LD     =  2,
  ST     =  3,
  JSR    =  4,
  AND    =  5,
  LDR    =  6,
  STR    =  7,
  RTI    =  8,
  NOT    =  9,
  LDI    = 10,
  STI    = 11,
  JMP    = 12,
  RES    = 13,
  LEA    = 14,
  TRAP   = 15
};

enum REG {
  R0     = 0,
  R1     = 1,
  R2     = 2,
  R3     = 3,
  R4     = 4,
  R5     = 5,
  R6     = 6,
  R7     = 7,
  PC     = 8,
  COND   = 9
};

enum FLAG {
  ZRO = 0,
  POS = 1,
  NEG = 2
};

uint16_t regs[ R_COUNT ];
uint16_t memory[ MEMORY_MAX ];

void add_inst( uint16_t );           // ADD Instruction
