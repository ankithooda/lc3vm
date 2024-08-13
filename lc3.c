#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "lc3_hardware.h"

int main( int argc, char **argv ) {

  uint16_t pc_start = 10;
  void *memory_load_location;

  // Initialize hardware
  initialize_hardware();
  fprintf( stdout, "Debug: Hardware Initialized\n" );
  debug_hardware();

  memory_load_location = get_memory_offset(pc_start);

  if ( argc != 2 ) {
    fprintf( stdout, "lc3-vm <image>\n" );
    exit( 1 );
  }

  FILE *program = fopen(argv[1], "rb");

  if (!program) {
    fprintf(stderr, "Error: Could not open file - %s\n", strerror(errno));
    exit(2);
  }

  size_t bytes_read = fread((void *)memory_load_location, 2, 1000, program);

  if ( bytes_read == -1 ) {
    fprintf(stderr, "Error: Could not read file - %s\n", strerror(errno));
    exit(3);
  } else {
    fprintf(stdout, "Program - %ld Byte(s) loaded\n", bytes_read);
  }


  // Load PC and start machine
  set_pc(pc_start);

  run_machine();

  debug_hardware();

  return 0;
}
