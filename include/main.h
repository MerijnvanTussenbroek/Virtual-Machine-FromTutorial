#pragma once


#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <stdarg.h>

#define noErr           0x00
#define ErrMem          0x01 
#define ErrSegV         0x02 
#define SysHlt          0x03
#define BadInstr        0x04

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

#define $fl ->c.r.flags
#define equal(x)       (!!((x $fl & 0x04) >> 2))
#define gt(x)          (!!((x $fl & 0x02) >> 1))
#define lt(x)          (!!(x $fl & 0x01))


#define segFault(x)     error((x), ErrSegV);

typedef unsigned short int Reg;

struct s_registers{
    Reg ax;
    Reg bx;
    Reg cx;
    Reg dx;
    Reg sp;
    Reg ip;
    Reg flags;
};
typedef struct s_registers Registers;

/*

Reg flags:
Zero flag - if the result of an operation is zero
Carry flag - if the result of a math equation needs you to carry the number
CMP/comparison flag - for storing the result of a comparison

*/

struct s_cpu{
    Registers r;
};
typedef struct s_cpu CPU;

typedef int8 *Memory;

enum e_opcode {
    nop = 0x01, // no instruction
    hlt = 0x02, // halt program
    mov = 0x08, // 0x08 - 0x0f, so 0000 1000 to 0000 1111
    ste = 0x10, //set equal flag
    cle = 0x11, // clear equal flag
    stg = 0x12, // set greater flag
    clg = 0x13, // clear greater flag
    stl = 0x14, // set less flag
    cll = 0x15, // clear less flag
    push = 0x16,// push command
    pop = 0x17 // pop command
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

typedef Memory Stack;

static IM instrmap[] = {
    { nop, 0x01 },
    { hlt, 0x01 },
    { mov, 0x03 },{0x09,0x03},{0x0a,0x03},{0x0b,0x03},
    { 0x0c,0x03 },{0x0d,0x03},{0x0e,0x03},{0x0f,0x03},
    { ste, 0x01 },
    { cle, 0x01 },
    { stg, 0x01 },
    { clg, 0x01 },
    { stl, 0x01 },
    { cll, 0x01 },
    { pop, 0x03 }, // for the push and pop instructions
    { push, 0x03 } // only 1 byte of the 2 available are used

};
#define IMs (sizeof(instrmap) / sizeof(struct s_instrmap))

typedef unsigned char errorCode;

void error(VM*, errorCode); // In the event an error occurs

void movInstr(VM*, Opcode*, Args, Args); // function for the move instruction
void pushInstr(VM*, Args, Args);
void popInstr(VM*, Args, Args);
void steInstr(VM*);
void stgInstr(VM*);
void stlInstr(VM*);
void cleInstr(VM*);
void clgInstr(VM*);
void cllInstr(VM*);

void execInstr(VM*, Instruction*); // executes a single instruction
void execute(VM*); // executes the program loaded into the memory of the VM
 void examplePorgram(VM*);
Program* loadProgram(VM* vm, ...);
Program* instr(Instruction*);
VM *virtualMachine(); // Constructor function
int main(int, char**);