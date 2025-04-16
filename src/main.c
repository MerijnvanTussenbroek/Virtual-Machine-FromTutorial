#include <stdio.h>

#include "main.h"
#include "birchutils.h"

int8 map(Opcode o)
{
    int8 n, ret;
    IM *p;

    ret = 0;
    for(n = IMs, p = instrmap; n; n--, p++)
    {
        if(p->o == o)
        {
            ret = p->size;
            break;
        }
    }

    return ret;
}

VM *virtualMachine()
{
    VM *p;
    int16 size;


    size = $2 sizeof(struct s_vm);
    p = (VM *)malloc($i size);
    if(!p)
    {
        errno = ErrMem;
        return (VM *)0;
    }
    zero($1 p, size);

    p->m = malloc(65535);
    if(!(p->m))
    {
        free(p);
        errno = ErrMem;
        return NULL;
    }
    zero(p->m,65535);

    return p;
}

void examplePorgram(VM *vm)
{
    Program *program;
    Instruction *i1, *i2, *i3;
    int16 s1, s2, s3;

    s1 = map(mov);
    s2 = map(nop);
    s3 = map(hlt);

    i1 = (Instruction *)malloc($i s1);
    i2 = (Instruction *)malloc($i s2);
    i3 = (Instruction *)malloc($i s3);

    assert(i1 && i2 && i3);

    zero($1 i1, s1);
    zero($1 i2, s2);
    zero($1 i3, s3);

    i1->o = mov;
    i1->a[0] = 0x00;
    i1->a[1] = 0x05;

    program = vm->m;
    copy($1 program, $1 i1, s1);
    program += s1;

    i2->o = nop;
    copy($1 program, $1 i2, s2);
    program += s2;

    i3->o = hlt;
    copy($1 program, $1 i3, s3);
    program += s3;

    vm->brk = (s1 + s2 + s3);
    vm $ip = (Reg)(vm->m);
    vm $sp = (Reg) -1;

    free(i1);
    free(i2);
    free(i3);
}

void movInstr(VM* vm, Args a1, Args a2)
{
    vm $ax = (Reg) (a1 << 4) | a2;

    return;
}

void execInstr(VM* vm, Instruction* instruction)
{

    Args a1, a2;
    int16 size = map(instruction->o);

    a1 = a2 = 0;

    if(size == 0x03){
        a1 = instruction->a[0];
        a2 = instruction->a[1];
    }

    switch(instruction->o)
    {
        case mov:
            movInstr(vm, a1, a2);
            break;
        case nop:
            //nopInstr(vm);
            break;
        case hlt:
            //hltInstr(vm);
            printf("\nProgram has finished\n");
            break;
    }

    return;
}

void execute(VM* vm)
{

    Program* programPointer;
    Instruction *instrPointer;

    Program* beginning = programPointer;

    int16 size;

    assert(vm && *vm->m);

    programPointer = vm->m;
    do{
        instrPointer = (Instruction *)programPointer;
        size = map(instrPointer->o);
        execInstr(vm, instrPointer);

        vm $ip += size;
        programPointer += size;
    }
    while(*programPointer != (Opcode)hlt);

    instrPointer = (Instruction *)programPointer;
    execInstr(vm, instrPointer);

    if(programPointer > vm->brk + beginning + 1)
    {
        segFault(vm);
    }
}

void error(VM* vm, errorCode e)
{
    int8 exitCode = -1;

    if(vm){
        free(vm->m);
        free(vm);
    }

    switch(e)
    {
        case ErrSegV:
            fprintf(stderr, "VM Segmentation fault\n");
            break;
        case SysHlt:
            fprintf(stderr, "VM finished program via hlt instruction\n");
            exitCode = 0;
            break;
        default:
            break;
    }

    getchar();
    exit(exitCode);
}

int main(int argc, char **argv)
{
    (void)argc;
    (void) argv;
    VM *virtualmachine;

    virtualmachine = virtualMachine();
    examplePorgram(virtualmachine);

    printf("vm = %p\n", virtualmachine);
    printf("\nprogram in vm = %p\n", virtualmachine->m);
    printf("brk in vm = %d\n", virtualmachine->brk);
    printhex($1 virtualmachine->m, 3 + 1 + 1, ' ');

    execute(virtualmachine);

    printf("\nax = %.16hx\n",$i virtualmachine $ax);
    printf("bx = %.16hx\n",$i virtualmachine $bx);
    printf("cx = %.16hx\n",$i virtualmachine $cx);
    printf("dx = %.16hx\n",$i virtualmachine $dx);
    printf("ip = %.16hx\n",$i virtualmachine $ip);
    printf("sp = %.16hx\n",$i virtualmachine $sp);

    getchar();


    return 0;
}