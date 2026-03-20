#ifndef LINEFORM_H
#define LINEFORM_H



typedef struct 
{
    void* a;
    unsigned size_a;
    void* b;
    unsigned size_b;
    void* (*add_lnf)(void* a, void* b, unsigned size_a, unsigned size_b);
    void* (*dif_lnf)(void* a, void* b, unsigned size_a, unsigned size_b);
}dif_add;

typedef struct
{
    void* lnf;
    void* vrlbs;
    unsigned size;
    void* (*mul_lnf)(void* a, void* b, unsigned size);
    void* (*calc_lnf)(void* lnf, void* vrlbs, unsigned size);
}mul_calc;

#define UNCOR_LNF_ERR (dif_add){0, 301, 0, 301, 0, 0}
#define UNCOR_TYPE_ERR (dif_add){0, 302, 0, 302, 0, 0}


dif_add set_for_dif_add(void* a, void* b, char type, unsigned size_a, unsigned size_b);
mul_calc set_for_mul_calc(void* lnf, void* vrlbs, char type, unsigned size);

#endif