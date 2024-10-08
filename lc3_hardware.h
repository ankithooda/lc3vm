#include <stdint.h>

#define R_COUNT 10
#define MEMORY_MAX (1 << 16)

enum OPCODE {
	OP_BR = 0,
	OP_ADD = 1,
	OP_LD = 2,
	OP_ST = 3,
	OP_JSR = 4,
	OP_AND = 5,
	OP_LDR = 6,
	OP_STR = 7,
	OP_RTI = 8,
	OP_NOT = 9,
	OP_LDI = 10,
	OP_STI = 11,
	OP_JMP = 12,
	OP_RES = 13,
	OP_LEA = 14,
	OP_TRAP = 15
};

enum REG {
	R0 = 0,
	R1 = 1,
	R2 = 2,
	R3 = 3,
	R4 = 4,
	R5 = 5,
	R6 = 6,
	R7 = 7,
	RPC = 8,
	RCOND = 9
};

/*
  The values 1, 2, 5 are chosen to simplify the implementation
  of BR instruction. See branch_instruction function
  lc3_hardware.c

  FL_POS - 001
  FL_ZRO - 010
  FL_NEG - 100
 */
enum FLAGS { FL_POS = 1, FL_ZRO = 2, FL_NEG = 4 };

enum TRAP_CODE {
	TRAP_GETC = 0x20,
	TRAP_OUT = 0x21,
	TRAP_PUTS = 0x22,
	TRAP_IN = 0x23,
	TRAP_PUTSP = 0x24,
	TRAP_HALT = 0x25
};

enum SPECIAL_REGS {
	KBSR = 0xFE00, /* Keyboard Status */
	KBDR = 0xFE02, /* Keyboard Data   */
	DSSR = 0xFE04, /* Display Status  */
	DSDR = 0xFE06, /* Display Data    */
	MCR = 0xFFFE /* Machine Control */
};

void initialize_hardware(void); // Initialize Hardware
void debug_hardware(void);
void run_machine(void);
uint16_t get_pc(void);
void set_pc(uint16_t value);
uint16_t *get_memory_location(uint16_t offset);

void add_instruction(uint16_t instruction); // ADD Instruction
void branch_instruction(uint16_t instruction);
void ld_instruction(uint16_t instruction);
void st_instruction(uint16_t instruction);
void jsr_instruction(uint16_t instruction);
void and_instruction(uint16_t instruction);
void not_instruction(uint16_t instruction);
void str_instruction(uint16_t instruction);
void ldr_instruction(uint16_t instruction);
void ldi_instruction(uint16_t instruction);
void sti_instruction(uint16_t instruction);
void jmp_instruction(uint16_t instruction);
void lea_instruction(uint16_t instruction);
void trap_instruction(uint16_t instruction);
