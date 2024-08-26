
#include "facile.h"

instruction_t code[MAXSEGSIZE];
int input[MAXSEGSIZE];
int output[MAXSEGSIZE];
int data[MAXSEGSIZE];
int r[NUMREGS];

int insize = 0;
int outsize = 0;
int codesize = 0;

int cpu_fault = 0;
int cpu_halt = 0;

instruction_template_t op_templates[] = {
  NOP, "nop", NO_OPERAND, NO_OPERAND,  
  LDI, "ldi", INDIRECT, REGISTER,
  LDD, "ldd", INDIRECT, REGISTER,
  STD, "std", REGISTER, INDIRECT,
  STO, "sto", REGISTER, INDIRECT,
  SET, "set", IMMEDIATE, REGISTER,
  MOV, "mov", REGISTER, REGISTER,
  ADD, "add", REGISTER, REGISTER,
  SUB, "sub", REGISTER, REGISTER,
  BEZ, "bez", REGISTER, IMMEDIATE,
  BNZ, "bnz", REGISTER, IMMEDIATE,
  BGZ, "bgz", REGISTER, IMMEDIATE,
  BLZ, "blz", REGISTER, IMMEDIATE,
  JMP, "jmp", NO_OPERAND, IMMEDIATE,
  FIN, "fin", NO_OPERAND, NO_OPERAND
  /* Warning!  Order matches enum order.  FIN must be last. */
};

static char whoami[] = "facile";

void
check_templates() {
  for (int i =0; i< FIN; i++) {
    if (op_templates[i].opcode != i) {
      fprintf(stderr, "%s: fix facile.h: opcode enumerator value should match template table index (slot %d does not).\n", whoami, i);
      exit(EXIT_FAILURE);
    }
    // todo: check all mnemonics are exactly 3 characters?   (asm fails if not)
  }
}

static char linebuf[MAXLINECHARS];

int
fill_input() {
  int count = 0;
  int i = 0;
  int bytes = 0;
  /*
    Read/scan input from stdin one line at a time until there are no more
    input lines (EOF), then stop.   Each line is an input int > 0, nothing more.
  */
  char *sin = fgets(linebuf, MAXLINECHARS, stdin);
  while ((sin != NULL) && (count < MAXSEGSIZE)) {
    if ((sscanf(sin, "%d\n%n", &i, &bytes) != 1) || i<0 || sin[bytes]!=0) {
      fprintf(stderr, "%s: malformed input for run at line %d: %s.\n",
	      whoami, count, sin);
      exit(EXIT_FAILURE);
    }
    input[count] = i;
    count += 1;
    sin = fgets(linebuf, MAXLINECHARS, stdin);
  }
  return count;
}

int
facile_assembler(FILE *progfile)
{
  int count = 0;
  int instrs;
  /*
    Read/scan input from progfile one line at a time until there are no more
    input lines (EOF), then stop.  Each line is a comment or ASM for one instruction.
    Ignore comment lines and blank lines.  Scan/count everything else.
  */
  char *sin = fgets(linebuf, MAXLINECHARS, progfile);
  while ((sin != NULL) && (count < MAXSEGSIZE-1)) {
    // printf("in: %s", sin);
    if ((sin[0] != '#') && (sin[0] != '\n')) {
      if ((instrs = facile_asm_scanline(sin, &code[count])) != -1)
	count += instrs;
    }
    sin = fgets(linebuf, MAXLINECHARS, progfile);
  }

  return count;
}


int
facile_disassembler()
{
  int count = 0;
  for (int loc = 0; loc < codesize; loc++) {
    if (facile_asm_showline(code[loc].opcode, code[loc].src, code[loc].dest) != 1)
      break;
    count++;
  }
  return count;
}

void usage(char* name) {
  fprintf(stderr, "Usage: %s [-x or -d] <program file> (provide input on stdin)\n", name);
  exit(EXIT_FAILURE);
}


int
main (int argc, char **argv) 
{ 
  char opt;
  bool runit = false;
  bool showit = false;
  FILE *progfile = NULL;

  if (argc != 3)
    usage(argv[0]);
  while ((opt = getopt(argc, argv, "x:d:")) != -1) {
    switch (opt) {
    case 'x':
      runit = true;
      progfile = fopen(optarg, "r");
      break;
    case 'd':
      showit = true;
      progfile = fopen(optarg, "r");
      break;
    default:
      usage(argv[0]);	
    }
  }

  if ((runit && showit) || (!runit && !showit))
    usage(argv[0]);	

  if (progfile == NULL) {
    perror("Failed open on program file (or no file)");
    usage(argv[0]);
  }

  check_templates();

  codesize = facile_assembler(progfile);
  if (codesize <= 0) {
    fprintf(stderr, "%s: no good code.\n", whoami);
    exit(EXIT_FAILURE);
  }

  if (showit) {
    if (facile_disassembler() != codesize) {
      fprintf(stderr, "%s: disassembler gave up unexpectedly.\n", whoami);
      exit(EXIT_FAILURE);
    }
  }

  if (runit) {
    /*
     * Remember: if your program launch is JUST SITTING THERE, give it stdin
     * or an EOF (type a ctrl-d).   Or use facile -x ... <noinput
     */
    if (isatty(fileno(stdin)))
      printf("Don't leave me hanging!  Type inputs and then ctrl-d.\n");
    insize = fill_input();  
    if (insize < 0) {
      fprintf(stderr, "%s: malformed input for program.\n", whoami);
      exit(EXIT_FAILURE);
    }

    int executed = facile_machine();

    if (outsize == 0)
      fprintf(stderr, "No output from program.\n");
    else {
      // printf("Program output:\n");
      for (int i=0; i < outsize; i++)
	printf("%d\n", output[i]);
    }
	
    if (cpu_fault) {
      fprintf(stderr, "%s: executed %d instructions before abnormal exit with CPU fault.\n",
	      whoami, executed);
    } else if (cpu_halt) {
      /*
       * This fac program executed normally.   Say so.   Here we choose to write the message
       * on stderr, even though it is status and not an error message.  Why stderr?   If we just
       * use printf it writes to stdout, which receives the output of the fac program.  We choose
       * not to pollute the output with chatter, so users can redirect their program output to
       * a file with '>' on the command line and get exactly what the fac program wrote.
       */
      fprintf(stderr, "%s: executed %d instructions before normal exit with CPU halt.\n",
	     whoami, executed);
    } else {
      /* "Never happens" */
      fprintf(stderr, "%s: executed %d instructions before derailing somehow.\n",
	      whoami, executed);      
    }

  }

  if (cpu_fault)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
