#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "lc3_hardware.h"

uint16_t *regs;
uint16_t *memory;


/*
  The special register regs[ COND ] is updated
  after any register value is updated.
 */
void update_flags(enum REG r)
{
  if (regs[r] == 0) {
    regs[RCOND] = FL_ZRO;

  } else if (regs[r] > 0) {
    regs[RCOND] = FL_POS;

  } else {
    regs[RCOND] = FL_NEG;

  }
}

/* Extend Sign */
int16_t extend_sign(int16_t value, uint8_t bit_count)
{

  // if this is a negative number
  if ((value >> (bit_count - 1)) & 0x1) {
    // Extend 1s.
    value |= 0xFFFF << bit_count;
  }
  return value;
}

/* Initialize Hardware */
void initialize_hardware(void)
{
  regs    = (uint16_t *)malloc(R_COUNT * sizeof(uint16_t));
  memory  = (uint16_t *)malloc(MEMORY_MAX * sizeof(uint16_t));
  regs[R0] = 12;
  regs[R1] = 13;
  regs[R3] = 7;
}

/* PC Counter and Memory helper functions */
void set_pc(uint16_t value)
{
  regs[RPC] = value;
}

uint16_t get_pc()
{
  return regs[RPC];
}

void *get_memory_offset(uint16_t offset)
{
  return (void *)(memory + offset);
}

uint16_t read_memory(uint16_t address)
{
  uint16_t value = memory[address];

  return (value >> 8)|(value << 8);
}

/* Run Machine */
void run_machine()
{
  bool running = true;
  uint16_t curr_instruction;
  while (running) {
    curr_instruction = read_memory(regs[RPC]);
    fprintf(stdout, "Curr - %d\n",curr_instruction);

    switch(curr_instruction >> 12) {
    case OP_BR:
      branch_instruction(curr_instruction);
      fprintf(stdout, "BR Instruction\n");
      break;
    case OP_ADD:
      add_instruction(curr_instruction);
      fprintf(stdout, "ADD Instruction\n");
      break;
    default:
      return;
    }
    regs[RPC] = regs[RPC] + 1;
  }
}

void halt_machine()
{
  fprintf(stdout, "Halting\n");
  return;
}



void debug_hardware()
{
  for (uint8_t i = 0; i < R_COUNT - 2; i++) {
    printf("R%d    - %d\n", i, regs[ i ]);
  }
  printf("RPC   - %d\n", regs[ RPC ]);
  printf("RCOND - %d\n", regs[ RCOND ]);
}


/////////////////////////////// Instructions ////////////////////////////

/* The ADD instruction has two modes */
/* Mode bit is inst[5] */
/* Register Mode - inst[0-2] are Second source register */
/* Immediate Mode - inst[0-5] is an integer value */

void add_instruction(uint16_t inst)
{
  uint16_t dr, sr1, sr2, mode;
  int16_t imm_value;

  dr  = (inst >> 9) & 0x7;
  sr1 = (inst >> 6) & 0x7;

  mode = (inst >> 5) & 0x1;

  if (mode) {

    // Immediate mode
    imm_value = extend_sign((inst & 0x1F), 5);
    regs[dr] = regs[sr1] + imm_value;
    update_flags(regs[ dr ]);

  } else {

    // Register Mode.
    sr2 = inst & 0x7;
    regs[dr] = regs[sr1] + regs[sr2];
    update_flags(dr);
  }
}

void branch_instruction(uint16_t instruction)
{
  if (
      (((instruction >> 11) & 0x1) && regs[RCOND] == FL_NEG) ||
      (((instruction >> 10) & 0x1) && regs[RCOND] == FL_ZRO) ||
      (((instruction >>  9) & 0x1) && regs[RCOND] == FL_POS)) {
    regs[RPC] = regs[RPC] + extend_sign(instruction & 0x1FF, 9);
  }
}
