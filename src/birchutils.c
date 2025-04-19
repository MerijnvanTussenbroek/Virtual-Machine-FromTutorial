#include "birchutils.h"

//All the functions here have been made by Dr Birch himself, not by me
//I am just following his tutorial

//the same as memset
void zero(int8 *str, int16 size) {
    int8 *p;
    int16 n;

    for (n=0, p=str; n<size; n++, p++)
        *p = 0;

    return;
}

//the same as memcpy
void copy(int8 *dst, int8 *src, int16 size) {
    int8 *d, *s;
    int16 n;

    for (n=size, d=dst, s=src; n; n--, d++, s++)
        *d = *s;

    return;
}

void printhex(int8 *str, int16 size, int8 delim) {
    int8 *p;
    int16 n;

    for (p=str, n=size; n; n--, p++) {
        printf("%.02x", *p);
        if (delim)
            printf("%c", delim);
        fflush(stdout);
    }
    printf("\n");

    return;
}

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