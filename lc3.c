#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "lc3_hardware.h"

void run_machine() {
  bool running = true;
  uint16_t curr_instruction;
  while ( running ) {
    curr_instruction = regs[PC] >> 12;

    switch( curr_instruction ) {
    case OPCODE.ADD:
      fprintf( stdout, "ADD Instruction\n" );
      break;
    default:
      return;
    }
  }
}

void halt_machine() {
  fprintf( stdout, "Halting\n" );
  return;
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
