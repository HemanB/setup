#include "facile.h"

/* Return 1 for success, else -1. */

int
facile_asm_showline(opcode_t op, int src, int dest)
{
  printf("%d %d %d\n", op, src, dest);

  return 1;
}

