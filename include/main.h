#pragma once


#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

#define ErrMem          0x01
#define NoArgs          { 0x00, 0x00 }

typedef unsigned char int8;
typedef unsigned short int16;
typedef unsigned int int32;
typedef unsigned long long int int64;

#define $1 (int8 *)
#define $2 (int16)
#define $4 (int32)
#define $8 (int64)
#define $c (char *)
#define $i (int)

typedef unsigned short int Reg;

struct s_registers{
    Reg ax;
    Reg bx;
    Reg cx;
    Reg dx;
    Reg sp;
    Reg ip;
};
typedef struct s_registers Registers;

struct s_cpu{
    Registers r;
};
typedef struct s_cpu CPU;

typedef int8 *Memory;

enum e_opcode {
    mov = 0x01,
    nop = 0x02,
    hlt = 0x03
};
typedef enum e_opcode Opcode;

struct s_instrmap{
    Opcode o;
    int8 size;
};
typedef struct s_instrmap IM;

typedef int8 Args;

struct s_instruction {
    // an instruction is a command to the CPU
    Opcode o;
    Args a[]; // 0 to 2 bytes
};

typedef struct s_instruction Instruction;

typedef int8 Program;

struct s_vm {
    CPU c;
    Memory m;
    int16 brk;
};

typedef struct s_vm VM;

typedef Memory *Stack;

static IM instrmap[] = {
    { mov, 0x03 },
    { nop, 0x01 },
    { hlt, 0x01 }
};
#define IMs (sizeof(instrmap) / sizeof(struct s_instrmap))

void execute(VM*);
 Program *examplePorgram(VM*);
int8 map(Opcode o);
VM *virtualMachine();
int main(int, char**);