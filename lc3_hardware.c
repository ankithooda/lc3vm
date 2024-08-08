#include <stdint.h>
#include "lc3_hardware.h"

/*
  The special register regs[ COND ] is updated
  after any register value is updated.
 */
void update_flags( REG r ) {
  if ( regs[ r ] == 0 ) {
    regs[ REG.COND ] = FLAG.ZRO;

  } else if ( regs[ r ] > 0 ) {
    regs[ REG.COND ] = FLAG.POS;

  } else {
    regs[ REG.COND ] = FLAG.NEG;

  }
}

/* Extend Sign */
int16_t extend_sign( int16_t value, uint8_t bit_count ) {

  // if this is a negative number
  if ( ( value >> ( bit_count - 1 ) ) & 0x1 ) {
    // Extend 1s.
    value |= 0xFFFF << bit_count;
  }
  return value;
}

/////////////////////////////// Instructions ////////////////////////////

/* The ADD instruction has two modes */
/* Mode bit is inst[5] */
/* Register Mode - inst[0-2] are Second source register */
/* Immediate Mode - inst[0-5] is an integer value */

void add_inst( uint16_t inst ) {
  uint16_t dr, sr1, sr2, mode;
  int16_t imm_value;

  dr  = (inst >> 9) & 0x7;
  sr1 = (inst >> 6) & 0x7;

  mode = (inst >> 5) & 0x1;

  if ( mode ) {

    // Immediate mode
    imm_value = extend_sign( inst & 0x1F );
    regs[ dr ] = regs[ sr1 ] + imm_value;
    update_flags( regs[ dr ] );

  } else {

    // Register Mode.
    sr2 = inst & 0x7;
    regs[ dr ] = regs[ sr1 ] + regs[ sr2 ];
    update_flags( dr );
  }

}
