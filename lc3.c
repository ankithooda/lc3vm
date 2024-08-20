#include "lc3_hardware.h"
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

uint16_t
swap16 (uint16_t value)
{
  return (value >> 8) | (value << 8);
}

int
load_image_file (FILE *program)
{
  uint16_t origin, program_sz;
  uint16_t *load_location;

  if (fread ((void *)&origin, sizeof (uint16_t), 1, program) == -1)
    {
      return -1;
    }
  origin = swap16 (origin);

  program_sz = MEMORY_MAX - origin;
  load_location = get_memory_location (origin);

  size_t words_read
      = fread ((void *)load_location, sizeof (uint16_t), program_sz, program);

  if (words_read == -1)
    {
      return -1;
    }
  else
    {
      // Swap each byte within each LC3 word.
      for (uint16_t i = 0; i < words_read; i++)
        {
          *load_location = swap16 (*load_location);
          load_location++;
        }

      set_pc (origin);
      fprintf (stdout,
               "Program - %ld LC3 Machine Word(s) loaded at location 0x%x\n",
               words_read, origin);

      fprintf (stdout, "Program Counter set to 0x%x\n", origin);
      return 0;
    }
}

int
main (int argc, char **argv)
{

  // Initialize hardware
  initialize_hardware ();
  fprintf (stdout, "Debug: Hardware Initialized\n");
  debug_hardware ();

  // Load program
  FILE *program = fopen (argv[1], "rb");
  if (!program)
    {
      fprintf (stderr, "Error: Could not open file - %s\n", strerror (errno));
      exit (2);
    }
  if (load_image_file (program) == -1)
    {
      fprintf (stderr, "Error Could not load image in memory - %s\n",
               strerror (errno));
      exit (3);
    }

  if (argc != 2)
    {
      fprintf (stdout, "lc3-vm <image>\n");
      exit (1);
    }

  run_machine ();

  // debug_hardware();

  return 0;
}
