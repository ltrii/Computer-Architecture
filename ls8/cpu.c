#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  FILE *fptr;
  char line[1024];
  unsigned char address = 0;
  fptr = fopen(file, "r");

  if (fptr == NULL)
  {
    fprintf(stderr, "Error opening file: %s\n", file);
    exit(2);
  }

  while (fgets(line, sizeof(line), fptr) != NULL)
  {
    char *endptr;
    unsigned char bininstr;

    bininstr = strtoul(line, &endptr, 2);

    if (endptr == line)
    {
      continue;
    }

    cpu_ram_write(cpu, address++, bininstr);
  }

  fclose(fptr);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char opA;
  unsigned char opB;
  unsigned char ir = cpu_ram_read(cpu, cpu->pc);

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.

    if (ir >= 80)
    {
      opA = cpu_ram_read(cpu, cpu->pc + 1);
      opB = cpu_ram_read(cpu, cpu->pc + 2);
      cpu->pc += 3;
    }
    else if (ir >= 50)
    {
      opA = cpu_ram_read(cpu, cpu->pc + 1);
      cpu->pc += 2;
    }
    switch (ir)
    {
    case LDI:
      cpu_ram_write(cpu, opA, opB);
      break;
    case PRN:
      printf("%d\n", opB);
      break;
    case MUL:
      cpu->registers[opA] = (cpu->registers[0]) * (cpu->registers[1]);
      cpu->pc += 3;
      break;
    case HLT:
      exit(1);
      break;
    default:
      break;
    }

  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->registers, 0, sizeof(cpu->registers));
}

unsigned char cpu_ram_read(struct cpu *cpu, char loc)
{
  return cpu->ram[loc];
}

void cpu_ram_write(struct cpu *cpu, char loc, unsigned char val)
{
  cpu->ram[loc] = val;
}
