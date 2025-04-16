#pragma once


#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

#define noErr           0x00
#define ErrMem          0x01 // 00 01
#define ErrSegV         0x02 // 00 10
#define SysHlt          0x03

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

#define $ax ->c.r.ax
#define $bx ->c.r.bx
#define $cx ->c.r.cx
#define $dx ->c.r.dx
#define $sp ->c.r.sp
#define $ip ->c.r.ip


#define segFault(x)     error((x), ErrSegV);

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
typedef int8 Opcode;

struct s_instrmap{
    Opcode o;
    int8 size;
};
typedef struct s_instrmap IM;

typedef int8 Args;

struct s_instruction {
    Opcode o;
    Args a[]; //either 0 or 2 bytes
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

typedef unsigned char errorCode;

void error(VM*, errorCode);
void movInstr(VM*, Args, Args);
void execInstr(VM*, Instruction*);
void execute(VM*);
 void examplePorgram(VM*);
int8 map(Opcode o);
VM *virtualMachine();
int main(int, char**);