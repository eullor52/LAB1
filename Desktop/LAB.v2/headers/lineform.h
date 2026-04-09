#ifndef LINEFORM_H
#define LINEFORM_H

#include "operations.h"

typedef struct
{
    void* (*add_lnf)(void* a, void* b, unsigned size_a, unsigned size_b);
    void* (*dif_lnf)(void* a, void* b, unsigned size_a, unsigned size_b);
}Type_info1;

typedef struct
{
    void* (*mul_lnf)(void* a, void* b, unsigned size);
    void* (*calc_lnf)(void* lnf, void* vrlbs, unsigned size);
}Type_info2;


typedef struct 
{
    void* a;
    unsigned size_a;
    void* b;
    unsigned size_b;
    Type_info1 calculations;
}dif_add;

typedef struct
{
    void* lnf;
    void* vrlbs;
    unsigned size;
    Type_info2 calculations;
}mul_calc;

dif_add set_for_dif_add(void* a, void* b, char type, unsigned size_a, unsigned size_b);
mul_calc set_for_mul_calc(void* lnf, void* vrlbs, char type, unsigned size);

#endif