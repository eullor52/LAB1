#ifndef IO_H
#define IO_H

#include "complex.h"
#include <stdlib.h>
#include <stdarg.h>

#define  MAX_NUM_LEN 50

typedef struct 
{
    void* lnf;
    int size;
}Lnf;

typedef struct 
{
    unsigned len;
    int count;
}C_info;

#define ERRO (Lnf){NULL, 0}

void clean_input_buffer();

void map(void func(void*), int count, ...);
C_info check_complex_number(char* str);
int check_float_number(char* str);
int get_complex_number(char* str, Complex* num);
int complex_number_output(char* str, Complex num);
char* complex_lineform_output(Complex* lnf, unsigned size);
char* float_lineform_output(float* lnf, unsigned size);
char* lineform_output(void* lnf, unsigned size, char type);
Lnf get_lineform(char* str, char type);
void* get_factor(char* str, char type);
char* number_output(void* num, char type);
char* error_massage();

#endif
