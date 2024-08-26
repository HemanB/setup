
#include "facile.h"

static char whoami[] = "facileASM";

void
fatal(char *message, int position, char* line) {
    fprintf(stderr, "%s: %s at position %d in: %s\n",
	    whoami, message, position, line);
    exit(1);
}

int
expect(char *s, int i, char c) {
  if (s[i] != c)
    fatal("unexpected character", i, s);    
  i += 1;
  return i;
}

int
expect_int(char *s, int i, int *out) {
  int bytes = 0;
  if ((sscanf(&s[i], "%d%n", out, &bytes) != 1) || *out<0)
    fatal("expected a non-negative integer", i, s);
  i += bytes;
  return i;
}

int
expect_register(char *s, int i, int *out) {
  i = expect(s, i, 'r');
  i = expect_int(s, i, out);
  if (*out >= NUMREGS)
    fatal("register number out of range", i, s);
  return i;
}

int
expect_opcode(char* s, int i, opcode_t *op) {
  int j;
  char *opm = NULL;
  
  for (j=0; j<=FIN; j++) {
    opm = op_templates[j].mnemonic;
    /* match 3 char opcode */
    if ((s[i] == opm[0]) && (s[i+1] == opm[1]) && (s[i+2] == opm[2])) 
      break;
  }
  if (j > FIN)
    fatal("opcode not found", i, s);
  *op = op_templates[j].opcode;
  return i+3;
}

/*
  Scan one line of ASM input---one line of code from an ASM program.
  The line is a string.   Move down the string character by character.
  Expect the string to represent exactly one instruction:
  "<opcode> <left/source operand>,<right/dest operand>"
  The mode of each operand is dictated by the opcode, so expect compliance.
  Output: fill out the instruction_t passed by pointer.
  Return 1 for success: parsed one instruction.  (If error then program exits.)
 */

int
facile_asm_scanline(char *s, instruction_t *outinstr) {
  int i = 0; // current position (offset) in string
  opcode_t op;
  int src, dest;

  i = expect_opcode(s, i, &op);
  i = expect(s, i, ' ');

  // Source operand
  switch(op_templates[op].source_mode) {
  case IMMEDIATE: 
    i = expect(s, i, '$');
    i = expect_int(s, i, &src);
    break;
  case REGISTER: 
    i = expect_register(s, i, &src);
    break;
  case INDIRECT: 
    i = expect(s, i, '(');
    i = expect_register(s, i, &src);
    i = expect(s, i, ')');    
    break;
  case NO_OPERAND:
    i = expect(s, i, '0');
    src = 0;
    break;
  default:
    fatal("fix instruction template (source)", i, s); 
  };

  i = expect(s, i, ',');
  //  i = expect(s, i, ' ');

  // Destination operand
  switch(op_templates[op].dest_mode) {
  case IMMEDIATE: 
    i = expect(s, i, '$');
    i = expect_int(s, i, &dest);
    break;
  case REGISTER: 
    i = expect_register(s, i, &dest);
    break;
  case INDIRECT: 
    i = expect(s, i, '(');
    i = expect_register(s, i, &dest);
    i = expect(s, i, ')');    
    break;
  case NO_OPERAND:
    i = expect(s, i, '0');
    dest = 0;
    break;
  default:
    fatal("fix instruction template (dest)", i, s); 
  }

  outinstr->opcode = op;
  outinstr->src = src;
  outinstr->dest = dest;

  return 1;
}
