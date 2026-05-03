#ifndef IO_H
#define IO_H

#include "lineform.h"
#include "complex.h"

typedef struct {
    int len;
    int count;
    char sign;
} C_info;

typedef struct {
    dynamic_array (*parse_lineform)(const char* str);
    char* (*format_lineform)(dynamic_array* arr);
    void* (*parse_factor)(const char* str);
    char* (*format_number)(void* num);
}InputOutput;

int complex_to_string(char* buf, Complex c);
C_info check_complex_number(const char* str);
int check_double_number(const char* str);

dynamic_array parse_complex_lineform(const char* str);
dynamic_array parse_double_lineform(const char* str);

char* format_complex_lineform(dynamic_array* arr);
char* format_double_lineform(dynamic_array* arr);

void* parse_complex_factor(const char* str);
void* parse_double_factor(const char* str);

char* format_complex_number(void* num);
char* format_double_number(void* num);

void clean_input_buffer(void);

extern InputOutput complex_input_output;
extern InputOutput double_input_output;

#endif