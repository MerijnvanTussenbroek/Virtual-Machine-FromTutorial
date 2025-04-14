#include <stdio.h>

#include "main.h"
#include "birchutils.h"

/*
    16 bit
        AX general registers
        BX
        CX
        DX
        SP -- stack pointer
        IP -- instruction pointer
    65 KB memory
    (Serial COM port)
    (Floppy drive)
*/

/*
    (0000 0000) instruction
    0000 0000
    0000 0000

    3 bytes

    the rest is the value of the instruction
*/

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

Program *examplePorgram(VM *vm)
{
    Program *program;
    Instruction *i1, *i2;
    int16 s1, s2;

    s1 = map(mov);
    s2 = map(nop);

    i1 = (Instruction *)malloc($i s1);
    i2 = (Instruction *)malloc($i s2);

    assert(i1 && i2);

    zero($1 i1, s1);
    zero($1 i2, s2);

    i1->o = mov;
    i1->a[0] = 0x00;
    i1->a[1] = 0x05;

    program = vm->m;
    copy($1 program, $1 i1, 1);
    program += s1;

    i2->o = nop;
    copy($1 program, $1 i2, 1);
    program += s2;

    vm->brk = (s1 + s2);

    free(i1);
    free(i2);

    return (Program *)&vm->m;
}

void execute(VM* vm)
{

}

int main(int argc, char **argv)
{
    Program *prog;
    VM *virtualmachine;

    virtualmachine = virtualMachine();
    prog = examplePorgram(virtualmachine);

    printf("vm = %p\n", virtualmachine);
    printf("program = %p\n", prog);

    getchar();


    return 0;
}