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

uint16_t *get_memory_location(uint16_t offset)
{
  return memory + offset;
}

uint16_t read_memory(uint16_t address)
{
  return memory[address];
}

void write_memory(uint16_t address, uint16_t value)
{
  memory[address] = value;
}

/* Run Machine */
void run_machine()
{
  uint16_t curr_instruction;
  while (true) {
    // We are incrementing regs[RPC] as we are reading the value.
    // Instructions like LD add the offsets to the incremented PC.
    // Not the PC which contained the LD instruction.
    curr_instruction = read_memory(regs[RPC]++);

    /*
      Return if a ZERO instruction (where all bits are zero) is encountered.
      ZERO instruction is technically a BR instruction which has
      destination address as ZERO, it also does not specify any COND registers
      to be checked thus effecitvely making it a NOOP instruction.

      Instead of executign NOOPs and incrementing PC, we simply return and
      halt the machine.
     */
    if (curr_instruction == 0) {
      return;
    }

    fprintf(stdout, "Curr Instruction - %d\n",curr_instruction);

    switch(curr_instruction >> 12) {
    case OP_BR:
      branch_instruction(curr_instruction);
      fprintf(stdout, "BR Instruction\n");
      break;
    case OP_ADD:
      add_instruction(curr_instruction);
      fprintf(stdout, "ADD Instruction\n");
      break;
    case OP_LD:
      ld_instruction(curr_instruction);
      fprintf(stdout, "LD Instruction\n");
      break;
    case OP_ST:
      st_instruction(curr_instruction);
      fprintf(stdout, "ST Instruction\n");
      break;
    case OP_JSR:
      jsr_instruction(curr_instruction);
      fprintf(stdout, "JSR Instruction\n");
      break;
    case OP_AND:
      and_instruction(curr_instruction);
      fprintf(stdout, "AND Instruction\n");
      break;
    case OP_LDR:
      ldr_instruction(curr_instruction);
      fprintf(stdout, "LDR Instruction\n");
      break;
    case OP_STR:
      str_instruction(curr_instruction);
      fprintf(stdout, "STR Instruction\n");
      break;
    case OP_RTI:
      fprintf(stdout, "RTI Instruction\n");
      fprintf(stderr, "Invalid OPCODE RTI\n");
      return;
    case OP_NOT:
      not_instruction(curr_instruction);
      fprintf(stdout, "NOT Instruction\n");
      break;
    case OP_LDI:
      ldi_instruction(curr_instruction);
      fprintf(stdout, "LDI Instruction\n");
      break;
    case OP_STI:
      sti_instruction(curr_instruction);
      fprintf(stdout, "STI Instruction\n");
      break;
    case OP_JMP:
      jmp_instruction(curr_instruction);
      fprintf(stdout, "JMP Instruction\n");
      break;
    case OP_RES:
      fprintf(stdout, "RES Instruction\n");
      fprintf(stderr, "Invalid OPCODE RES\n");
      break;
    case OP_LEA:
      lea_instruction(curr_instruction);
      fprintf(stdout, "LEA Instruction\n");
      break;
    case OP_TRAP:
      trap_instruction(curr_instruction);
      fprintf(stdout, "TRAP Instruction\n");
      break;
    default:
      return;
    }
    debug_hardware();
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

void add_instruction(uint16_t instruction)
{
  uint16_t dr, sr1, sr2;
  int16_t imm_value;

  bool mode;

  dr  = (instruction >> 9) & 0x7;
  sr1 = (instruction >> 6) & 0x7;

  mode = (instruction >> 5) & 0x1;

  if (mode) {
    // Immediate mode
    imm_value = extend_sign((instruction & 0x1F), 5);
    regs[dr] = regs[sr1] + imm_value;
  } else {

    // Register Mode.
    sr2 = instruction & 0x7;
    regs[dr] = regs[sr1] + regs[sr2];
  }
  update_flags(dr);
}

void branch_instruction(uint16_t instruction)
{

  if (((instruction >> 9) & 0x7) & regs[RCOND]) {
    regs[RPC] = regs[RPC] + extend_sign(instruction & 0x1FF, 9);
  }

}

void ld_instruction(uint16_t instruction)
{
  uint16_t dr, pc9offset;

  dr = (instruction >> 9) & 0x7;
  pc9offset = extend_sign(instruction & 0x1FF, 9);

  regs[dr] = read_memory(regs[RPC] + pc9offset);

  update_flags(dr);
}

void st_instruction(uint16_t instruction)
{
  uint16_t sr, pc9offset;

  sr = (instruction >> 9) & 0x7;
  pc9offset = extend_sign(instruction & 0x1FF, 9);

  write_memory(regs[RPC] + pc9offset, regs[sr]);
}

void jsr_instruction(uint16_t instruction)
{
  uint16_t base_reg, pcoffset11;
  bool mode;

  // Save PC to R7
  regs[R7] = regs[RPC];

  mode = (instruction >> 11) & 0x1;

  // 11th bit is set -> PCOffset mode, otherwise base register mode.
  if (mode) {
    pcoffset11 = extend_sign(instruction & 0x7FF, 11);
    regs[RPC] = regs[RPC] + pcoffset11;
  } else {
    base_reg = (instruction >> 6) & 0x7;
    regs[RPC] = regs[base_reg];
  }
}

void and_instruction(uint16_t instruction)
{
  uint16_t dr, sr1, sr2;
  int16_t imm_value;

  bool mode;

  dr  = (instruction >> 9) & 0x7;
  sr1 = (instruction >> 6) & 0x7;

  mode = (instruction >> 5) & 0x1;

  if (mode) {

    // Immediate mode
    imm_value = extend_sign((instruction & 0x1F), 5);
    regs[dr] = regs[sr1] & imm_value;
  } else {

    // Register Mode.
    sr2 = instruction & 0x7;
    regs[dr] = regs[sr1] & regs[sr2];
  }
  update_flags(dr);
}


void not_instruction(uint16_t instruction)
{
  uint16_t dr, sr;

  dr = (instruction >> 9) & 0x7;
  sr = (instruction >> 6) & 0x7;

  regs[dr] = ~regs[sr];

  update_flags(dr);
}


void str_instruction(uint16_t instruction)
{
  uint16_t sr, base_r, offset6;

  sr       = (instruction >> 9) & 0x7;
  base_r   = (instruction >> 6) & 0x7;
  offset6  = extend_sign(instruction & 0x1F, 6);

  memory[regs[base_r] + offset6] = regs[sr];
}


void ldr_instruction(uint16_t instruction)
{
  uint16_t dr, base_r, offset6;

  dr       = (instruction >> 9) & 0x7;
  base_r   = (instruction >> 6) & 0x7;
  offset6  = extend_sign(instruction & 0x1F, 6);

  regs[dr] = memory[regs[base_r] + offset6];
  update_flags(dr);
}


void ldi_instruction(uint16_t instruction)
{
  uint16_t dr, pcoffset9;

  dr        = (instruction >> 9) & 0x7;
  pcoffset9 = extend_sign(instruction & 0x1FF, 9);
  regs[dr]  = memory[memory[regs[RPC] + pcoffset9]];

  update_flags(dr);
}


void sti_instruction(uint16_t instruction)
{
  uint16_t sr, pcoffset9;

  sr        = (instruction >> 9) & 0x7;
  pcoffset9 = extend_sign(instruction & 0x1FF, 9);
  memory[memory[regs[RPC] + pcoffset9]] = regs[sr];
}


void jmp_instruction(uint16_t instruction)
{
  uint16_t r;

  r = (instruction >> 6) & 0x7;
  regs[RPC] = regs[r];
}


void lea_instruction(uint16_t instruction)
{
  uint16_t dr, pcoffset9;

  dr        = (instruction >> 9) & 0x7;
  pcoffset9 = extend_sign(instruction & 0x1FF, 9);

  regs[dr]  = regs[RPC] + pcoffset9;

  update_flags(dr);
}


void trap_instruction(uint16_t instruction)
{
  uint16_t trap_vector;

  regs[RPC] = regs[R7];

  trap_vector = instruction & 0xFF;

  regs[RPC] = read_memory(trap_vector);
}
