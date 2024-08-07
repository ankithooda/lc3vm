#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

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


void run_machine() {
  bool running = true;
  uint16_t curr_instruction;
  while ( running ) {
    curr_instruction = regs[PC];


  }
}

void halt_machine() {
  ;
}

int main( int argc, char **argv ) {

  uint16_t pc_start = 3000;

  if ( argc != 2 ) {
    fprintf( stdout, "lc3-vm <image>\n" );
    exit( 1 );
  }

  int fd = open( argv[1], O_RDONLY );

  if ( fd == -1 ) {
    fprintf( stderr, "Error: Could not open file\n" );
    exit( 2 );
  }

  int bytes_read;

  bytes_read = read( fd, (void *)(memory + pc_start), MEMORY_MAX - pc_start );

  if ( bytes_read == -1 ) {
    fprintf( stderr, "Error: Could not read file\n" );
    exit( 3 );
  }

  // Load PC and start machine
  regs[PC] = pc_start;

  run_machine();
  halt_machine();

  return 0;
}
