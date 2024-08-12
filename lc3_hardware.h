#include <stdint.h>

#define R_COUNT 10
#define MEMORY_MAX (1 << 16)

enum OPCODE {
  OP_BR     =  0,
  OP_ADD    =  1,
  OP_LD     =  2,
  OP_ST     =  3,
  OP_JSR    =  4,
  OP_AND    =  5,
  OP_LDR    =  6,
  OP_STR    =  7,
  OP_RTI    =  8,
  OP_NOT    =  9,
  OP_LDI    = 10,
  OP_STI    = 11,
  OP_JMP    = 12,
  OP_RES    = 13,
  OP_LEA    = 14,
  OP_TRAP   = 15
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
  RPC     = 8,
  RCOND   = 9
};

enum FLAGS {
  FL_ZRO = 0,
  FL_POS = 1,
  FL_NEG = 2
};

static uint16_t *regs;
static uint8_t *memory;


void         initialize_hardware     ( void );                       // Initialize Hardware
void         debug_hardware          ( void );
void         run_machine             ( void );
uint16_t     get_pc                  ( void );
void         set_pc                  ( uint16_t value );
uint64_t     get_memory_offset       ( uint16_t offset );

void         add_instruction         ( uint16_t instruction );           // ADD Instruction
