#ifndef LINEFORM_H
#define LINEFORM_H

#include <stdlib.h>
#include <stdarg.h>
#include "complex.h"


typedef struct {
    void* data;
    size_t size;
}dynamic_array;

typedef struct {
    void (*append)(dynamic_array* array, void* item);
    dynamic_array (*create_and_init)(size_t length, ...);
    dynamic_array (*create)(size_t lenght);
    void (*delete)(dynamic_array* array);
    void (*insertat)(dynamic_array array, void* item, size_t index);
    void* (*get)(dynamic_array array, size_t index);
}Utils;

typedef struct {
    dynamic_array  (*get_sum)(dynamic_array left, dynamic_array right);
    dynamic_array (*get_difference)(dynamic_array left, dynamic_array right);
    dynamic_array (*multiplication)(dynamic_array lineform, void* factor);
    void* (*calculation)(dynamic_array lineform, dynamic_array variables);
}Type_info;

extern Utils complex_array;
extern Utils double_array;
extern Type_info double_operations;
extern Type_info complex_operations;

#endif