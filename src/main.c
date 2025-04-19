#include "main.h"
#include "birchutils.h"

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

Program* instr(Instruction *instr)
{
    Program* program;

    int8 size = map(instr->o);

    program = (Program* )malloc($i size);
    assert(program);

    copy(program, $1 instr, size);

    return program;
}

Program* loadProgram(VM* vm, ...)
{
    Program *p, *instr;

    va_list ap;

    Opcode opc;

    int8 size;

    va_start(ap, vm);
    p = vm->m;
    do{
        instr = va_arg(ap, Program*);
        opc = (Opcode)*instr;
        size = map(opc);

        copy($1 p, $1 instr, size);

        p += size;

    }while(opc != hlt);


    va_end(ap);

    return p;
}

void examplePorgram(VM *vm)
{
    Program *program;
    Instruction *i1, *i2, *i3, *i4, *i5, *i6, *i7;
    int16 s1, s2, s3;

    s1 = map(mov);
    s2 = map(nop);
    s3 = map(hlt);

    i1 = (Instruction *)malloc($i s1);
    i2 = (Instruction *)malloc($i s2);
    i3 = (Instruction *)malloc($i s3);

    i4 = (Instruction *)malloc($i s2);
    i5 = (Instruction *)malloc($i s2);
    i6 = (Instruction *)malloc($i s2);
    i7 = (Instruction *)malloc($i s2);

    assert(i1 && i2 && i3 && i4 && i5 && i6 && i7);

    zero($1 i1, s1);
    zero($1 i2, s2);
    zero($1 i3, s3);
    zero($1 i4, s1);
    zero($1 i5, s2);
    zero($1 i6, s3);
    zero($1 i7, s3);

    i1->o = mov;
    i1->a[0] = 0x00;
    i1->a[1] = 0x05;

    program = vm->m;

    i2->o = nop;
    i3->o = hlt;

    i4->o = stg;
    i5->o = clg;
    i6->o = stl;

    i7->o = ste;

    Program* p = loadProgram(vm, i1, i2, i4, i5, i6, i7, i3);

    vm->brk = (s1 + s2 + s3 + s2 + s2 + s2 + s2);
    vm $ip = (Reg)(vm->m);
    vm $sp = (Reg) -1;

    free(i1);
    free(i2);
    free(i3);
    free(i4);
    free(i5);
    free(i6);
    free(i7);
}

void movInstr(VM* vm, Opcode* opc, Args a1, Args a2)
{
    int16 dst = (a1 << 8) | a2;

    switch(*opc)
    {
        case mov: // mov to ax
            vm $ax = (Reg) dst;
            break;
        case 0x09: // mov to bx
            vm $bx = (Reg) dst;
            break;
        case 0x0a: // move to cx
            vm $cx = (Reg) dst;
            break;
        case 0x0b: // move to dx
            vm $dx = (Reg) dst;
            break;
        case 0x0c: // move to sp
            vm $sp = (Reg) dst;
            break;
        case 0x0d: // nothing

            break;
        case 0x0e: // nothing

            break;
        case 0x0f: // nothing

            break;
        default:
        error(vm, BadInstr);
            break;
    }

    return;
}

void steInstr(VM* vm)
{
    vm $fl |= 0x04;
}

void stgInstr(VM* vm)
{
    vm $fl |= 0x02;
}

void stlInstr(VM* vm)
{
    vm $fl |= 0x01;
}

void cleInstr(VM* vm)
{

    vm $fl &= ~(1 << 2);
}

void clgInstr(VM* vm)
{
    vm $fl &= ~(1 << 1);
}

void cllInstr(VM* vm)
{
    vm $fl &= ~(1 << 0);
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
        case mov ... 0x0f:
            movInstr(vm, &(instruction->o), a1, a2);
            break;
        case nop:
            //nopInstr(vm);
            break;
        case hlt:
            //hltInstr(vm);
            printf("\nProgram has finished\n");
            return;
            break;
        case ste:
            steInstr(vm);
            break;
        case stg:
            stgInstr(vm);
            break;
        case stl:
            stlInstr(vm);
            break;
        case cle:
            cleInstr(vm);
            break;
        case clg:
            clgInstr(vm);
            break;
        case cll:
            cllInstr(vm);
            break;
    }

    return;
}

void execute(VM* vm)
{

    Program* programPointer;
    Instruction *instrPointer;


    int16 size;

    assert(vm && *vm->m);

    programPointer = vm->m;

    vm $ip = $2 programPointer;

    Program* beginning = programPointer;

    do{
        instrPointer = (Instruction *)programPointer;
        size = map(instrPointer->o);
        execInstr(vm, instrPointer);

        vm $ip += size;
        programPointer += size;
    }
    while(*programPointer != (Opcode)hlt && programPointer <= vm->brk + beginning + 1);

    instrPointer = (Instruction *)programPointer;
    execInstr(vm, instrPointer);
    programPointer++;

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
        case BadInstr:
            fprintf(stderr, "Bad Instruction\n");
        default:
            break;
    }

    getchar();
    exit(exitCode);
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    
    VM *virtualmachine;

    virtualmachine = virtualMachine();
    examplePorgram(virtualmachine);

    printf("vm = %p\n", virtualmachine);
    printf("program in vm = %p\n", virtualmachine->m);
    printf("brk in vm = %d\n", virtualmachine->brk);
    printhex($1 virtualmachine->m, 3 + 1 + 1, ' ');

    execute(virtualmachine);

    printf("\nax = %.16hx\n",$i virtualmachine $ax);
    printf("bx = %.16hx\n",$i virtualmachine $bx);
    printf("cx = %.16hx\n",$i virtualmachine $cx);
    printf("dx = %.16hx\n",$i virtualmachine $dx);
    printf("fl = %.16hx\n", $i virtualmachine $fl);
    printf("ip = %.16hx\n",$i virtualmachine $ip);
    printf("sp = %.16hx\n",$i virtualmachine $sp);

    getchar();


    return 0;
}