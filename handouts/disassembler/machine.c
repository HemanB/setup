
#include "facile.h"

static char whoami[] = "facileCPU";

/*
  Execute program loaded to code[] on input loaded to input[].
  Execute instructions one at a time: begin at the beginning, and follow
  the program wherever it leads.  Raise a fault if the program tries to
  do something that is impossible or if it runs on too long.
 */

int
facile_machine()
{
  int pc = 0;
  int executed = 0;
  int nextpc = 0;
  int src = 0, dest = 0;
  int oidx = 0;

  r[15] = insize;  // Pass insize to program in r15.

  for (executed = 0; executed < MAXRUN && !cpu_fault && !cpu_halt; executed++) {

    nextpc = pc + 1;

    /*
     * Execute instruction at code[pc].
     */
    src = code[pc].src;
    dest = code[pc].dest;
    //    facile_asm_showline(code[pc].opcode, src, dest);
      
    switch (code[pc].opcode) {
    case NOP:
      break;
    case LDI:
      if ((r[src] < 0) || (r[src] >= insize)) {
	fprintf(stderr, "%s: input address out of range: %d.\n", whoami, r[src]);
	cpu_fault++;
	break;
      }
      r[dest] = input[r[src]];
      break;
    case LDD:
      if ((r[src] < 0) || (r[src] >= MAXSEGSIZE)) {
	fprintf(stderr, "%s: data address out of range: %d.\n", whoami, r[src]);
	cpu_fault++;
	break;
      }
      r[dest] = data[r[src]];
      break;
    case STD:
      if ((r[dest] < 0) || (r[dest] >= MAXSEGSIZE)) {
	fprintf(stderr, "%s: data address out of range: %d.\n", whoami, r[dest]);
	cpu_fault++;
	break;
      }
      data[r[dest]] = r[src];
      break;
    case STO:
      oidx = r[dest];
      if ((oidx < 0) || (oidx >= MAXSEGSIZE)) {
	fprintf(stderr, "%s: input address out of range: %d.\n", whoami, oidx);
	cpu_fault++;
	break;
      } else if (oidx >= outsize) {
	outsize = oidx+1;
      }
      output[oidx] = r[src];
      break;      
    case SET:
      r[dest] = src;
      break;
    case MOV:
      r[dest]= r[src];
      break;
    case ADD:
      r[dest] += r[src];
      break;
    case SUB: 
      r[dest] -= r[src];
      break;
    case BEZ:
      if (r[src] == 0)  
	nextpc = dest;  // immediate
      break;
    case BNZ: 
      if (r[src] != 0) 
	nextpc = dest;   // immediate
      break;
    case BGZ: 
      if (r[src] > 0) 
	nextpc = dest;   // immediate
      break;
    case BLZ: 
      if (r[src] < 0) 
	nextpc = dest;   // immediate
      break;      
    case JMP: 
	nextpc = dest;   // immediate
      break;
    case FIN:
      cpu_halt++;
      break;
    default:
      assert(false);
    }

    if (cpu_halt || cpu_fault)
      break;
    executed++;

    if ((nextpc >= 0) && (nextpc < codesize)) {
      pc = nextpc;
    } else if (nextpc == codesize) {
      // Fell off the end!  Halt sweetly.
      cpu_halt++;
    } else {
      fprintf(stderr, "%s: program counter out of range: %d->%d.\n", whoami, pc, nextpc);
      cpu_fault++;
    } 

  }
  
  if (executed >= MAXRUN) {
    fprintf(stderr, "%s: cycle limit exceeded.\n", whoami);
    cpu_fault++;
  }

  return executed;
}

