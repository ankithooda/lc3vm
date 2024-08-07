#include <stdint.h>

#define R_COUNT 10
#define MEMORY_MAX (1 << 16)

enum OPCODE {
  BR,
  ADD,
  LD,
  ST,
  JSR,
  AND,
  LDR,
  STR,
  RTI,
  NOT,
  LDI,
  STI,
  JMP,
  RES,
  LEA,
  TRAP
};

enum {
  R0,
  R1,
  R2,
  R3,
  R4,
  R5,
  R6,
  R7,
  PC,
  COND
};

enum {
  FL_POS = 0,
  FL_ZRO = 1,
  FL_NEG = 2
};

uint16_t regs[R_COUNT];
uint16_t memory[MEMORY_MAX];

int main(int argc, char **argv) {
  return 0;
}
