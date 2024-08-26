/*
Facile: Fake Assembly Code for an Instructional Language Environment

Facile is a primitive abstract machine with a dozen or so instruction opcodes.
It illustrates ISA/ASM concepts, but simplified to make it trivial to implement
---and also inconvenient to program.   This implementation includes a facile assembler,
emulator, and basic I/O.  It illustrates basic C programming.   
*/

/* Some useful stdlib header files needed anywhere/everywhere.   Including them here
   brings them in to any C source file that includes this header file facile.h.
*/
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

/*
 * These are the opcodes. Enums are small integers.
 */
typedef enum opcode {
  NOP,
  LDI,
  LDD,
  STD,
  STO,
  SET,
  MOV,
  ADD,
  SUB,
  BEZ,
  BNZ,
  BGZ,
  BLZ,
  JMP,
  FIN
} opcode_t;

typedef enum operand_mode { IMMEDIATE, REGISTER, INDIRECT, NO_OPERAND } operand_mode_t;
// See "syntax" in the Facile doc for explanation of syntax for operand modes.
// Syntax summary/example: immediate: $123; register: r0; indirect: (r0), none: 0.

typedef struct instruction_template {
  opcode_t opcode;
  char* mnemonic;
  operand_mode_t source_mode; 
  operand_mode_t dest_mode;
} instruction_template_t;

/*
 * Instruction template array defined in facile.c.
 * You can index op_templates[] with an opcode_t op to get op's mnemonic and operand modes.
 */
extern instruction_template_t op_templates[];

/*
= {
  NOP, "nop", NO_OPERAND, NO_OPERAND   
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
  FIN, "fin", NO_OPERAND, NO_OPERAND,
};
*/

/*
 * An "executable binary instruction" for code[].  "Just a bunch of numbers."
 */
typedef struct instruction {
  opcode_t opcode; 
  int src; 
  int dest; 
} instruction_t;

/*

Facile programs run with four memory segments: code, input, output, data.   Each of max size
MAXSEGSIZE.   Code and input are populated from source code (a program file) and integer inputs
(on stdin) supplied by user.   Facile tracks their sizes as codesize and insize.   Output
is populated by the program as it runs.   Facile tracks its size as outsize.   The machine has
16 registers.   All values and addresses are non-negative integers.

We keep all of this machine state as global variables, along with the table of instruction templates
and basic machine status indicators from the run.   They are public throughout the system.
Use with caution. 

 */

#define MAXLINECHARS 64
#define MAXSEGSIZE 1024
#define MAXRUN 1000
#define NUMREGS 16

extern instruction_t code[];
extern int input[];
extern int output[];
extern int data[];
extern int r[];


extern int insize;
extern int outsize;
extern int codesize;

/* Use as booleans: value 0 else 1 iff the CPU has halted or faulted. */
extern int cpu_fault;
extern int cpu_halt;

/*
These procedures are defined in separate source files and called from main in (facile.c).
They access the global machine state as you would expect.
scanline: the facileASM assembler in asm.c.
showline: the disassembler in dis.c
machine: in machine.c (facileCPU)
 */

int facile_asm_scanline(char *s, instruction_t *outinstr);
/* Return -1, 0, 1.   Error, no instruction, or a valid instruction. */

int facile_asm_showline(opcode_t op, int src, int dest);
/* Return 1 for success, else -1. */

int facile_machine();
/* RUN and return number of instructions executed---no more than MAXRUN.  */
