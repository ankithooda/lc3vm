#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "lc3_hardware.h"

int main( int argc, char **argv ) {

  uint16_t pc_start = 3000;
  uint64_t memory_load_location = get_memory_offset( pc_start );

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

  bytes_read = read( fd, (void *)memory_load_location, MEMORY_MAX - pc_start );

  if ( bytes_read == -1 ) {
    fprintf( stderr, "Error: Could not read file\n" );
    exit( 3 );
  }

  // Initialize hardware
  initialize_hardware();
  fprintf( stdout, "Debug: Hardware Initialized\n" );

  debug_hardware();
  fprintf( stdout, "Debug: Hardware Printed\n" );

  // Load PC and start machine
  set_pc( pc_start );

  run_machine();

  debug_hardware();

  return 0;
}
