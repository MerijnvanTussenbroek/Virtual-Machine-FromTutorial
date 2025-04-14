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
{ //Program *pr, int16 programSize
    VM *p;
    //Program *programPointer;
    int16 size;

    //assert((pr) && (programSize));

    size = $2 sizeof(struct s_vm);
    p = (VM *)malloc($i size);
    if(!p)
    {
        errno = ErrMem;
        return (VM *)0;
    }
    zero($1 p, size);

    /*
    programPointer = (Program *)malloc($i size);
    if(!programPointer)
    {
        free(p);
        errno = ErrMem;

        return (VM *)0;
    }
    copy(programPointer,pr, programSize);
    */

    return p;
}

Program *examplePorgram(VM *vm)
{
    Program *program;
    Instruction *i1, *i2;
    Args *a1;
    int16 s1, s2, sa1;

    s1 = map(mov);
    s2 = map(nop);

    i1 = (Instruction *)malloc($i s1);
    i2 = (Instruction *)malloc($i s2);

    assert(i1 && i2);

    zero($1 i1, s1);
    zero($1 i2, s2);

    i1->o = mov;
    sa1 = (s1 - 1);
    if(s1){
        a1 = (Args *)malloc($i sa1);
        assert(a1);
        zero(a1, sa1);
        *a1         = 0x00;
        *(a1 + 1)   = 0x05;
    }

    program = vm->s;
    copy($1 program,$1 i1, 1);
    program += 1;

    if(sa1 && a1){
        copy($1 program, $1 a1, sa1);
        program += sa1;
        free(a1);
    }

    i2->o = nop;
    copy($1 program, $1 i2, 1);

    free(i1);
    free(i2);

    return vm->s;
}

int main(int argc, char **argv)
{
    int8 size;
    Program *prog;
    VM *virtualmachine;

    size = (map(mov) + map(nop));
    virtualmachine = virtualMachine();
    prog = examplePorgram(virtualmachine);

    printf("vm = %p\n", virtualmachine);
    printf("program = %p\n", prog);
    getchar();


    return 0;
}