#ifndef GLOBALS_H
#define GLOBALS_H
#include "lineform.h"

enum error
{
    NO_ERROR,
    MEMORY_ERROR,
    INVAL_TYPE,
    INVAL_LINEFORM_PARAMETES,
    INVAL_INPUT_STR,
    OUTPUT_ERROR,
    INVAL_INPUT_TYPE,
    OUT_OF_RANGE
};

extern enum error error_info;

void set_error(enum error err_code);
enum error get_error();
void clear_error();

#define IS_ALRIGHT(return_value)  \
    do { \
        if (get_error() != NO_ERROR) { \
            return return_value; \
        } \
    } while(0)

#endif