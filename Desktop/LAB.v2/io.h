#ifndef IO_H
#define IO_H

#include "complex.h"
#include <stdlib.h>

#define  MAX_NUM_LEN 50

typedef struct 
{
    void* lnf;
    int size;
}Lnf;

#define NUM_ERROR (Lnf){NULL, -1}
#define MEMORY_ERROR (Lnf){NULL, -2}
#define TYPE_ERROR (Lnf){NULL, -3}

typedef struct 
{
    unsigned len;
    int count;
}C_info;


void clean_input_buffer();

C_info check_cmplx(char* str);
int check_float(char* str);
int get_cmplx(char* str, Complex* num);
int cmplx_output(char* str, Complex num);
char* cmplx_lnf_output(Complex* lnf, unsigned size);
char* flt_lnf_output(float* lnf, unsigned size);
char* output_lnf(void* lnf, unsigned size, char type);
Lnf get_lnf(char* str, char type);
void* get_factor(char* str, char type);
char* output_num(void* num, char type);

#endif
