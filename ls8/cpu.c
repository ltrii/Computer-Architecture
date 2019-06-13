#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6
#define sp 7

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
  unsigned char valueA = cpu->registers[regA];
  unsigned char valueB = cpu->registers[regB];


  switch (op)
  {
  case ALU_MUL:
    cpu->registers[regA] = valueA * valueB;
    break;
  case ALU_ADD:
    cpu->registers[regA] = valueA + valueB;
    break;
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
  unsigned char ir;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address pc).
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the pc to the next instruction.
    ir = cpu_ram_read(cpu, cpu->pc);

    opA = cpu_ram_read(cpu, cpu->pc + 1);
    opB = cpu_ram_read(cpu, cpu->pc + 2);

    switch (ir)
    {
    case LDI:
      cpu->registers[opA] = opB;
      break;
    case PRN:
      printf("%d\n", cpu->registers[opA]);
      break;
    case MUL:
      alu(cpu, ALU_MUL, opA, opB);
      break;
    case ADD:
      alu(cpu, ALU_ADD, opA, opB);
      break;
    case POP:
      cpu->registers[opA] = cpu->ram[(cpu->registers[sp])];
      cpu->registers[sp]++;
      break;
    case PUSH:
      cpu->registers[sp]--;
      cpu_ram_write(cpu, cpu->registers[sp], cpu->registers[opA]);
      break;
    case CALL:
      cpu->registers[sp]--;
      cpu_ram_write(cpu, cpu->registers[sp], cpu->pc + 2);
      cpu->pc = cpu->registers[opA];
      break;
    case RET:
      cpu->pc = cpu->ram[cpu->registers[sp]];
      cpu->registers[sp]++;
      break;
    case HLT:
      running = 0;
      break;
    default:
      printf("unexpected instruction 0x%02X at 0x%02X\n", ir, cpu->pc);
      exit(1);
      break;
    }

  }

}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the pc and other special registers
  cpu->pc = 0;
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->registers, 0, sizeof(cpu->registers));
  cpu->registers[7] = 0xf4;
}

unsigned char cpu_ram_read(struct cpu *cpu, char loc)
{
  return cpu->ram[loc];
}

void cpu_ram_write(struct cpu *cpu, char loc, unsigned char val)
{
  cpu->ram[loc] = val;
}
