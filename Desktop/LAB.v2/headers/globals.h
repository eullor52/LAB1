#ifndef GLOBALS_H
#define GLOBALS_H
#include "lineform.h"

enum error
{
    NO_ERROR,
    MEMORY_ERROR,
    INVAL_TYPE,
    INVAL_LNF_PARAM,
    INVAL_INPUT_STR,
    OUTPUT_ERROR,
    INVAL_INPUT_TYPE
};

extern enum error error_info;

void set_error(enum error err_code);
enum error get_error();

extern Type_info1 func1_cmplx;

extern Type_info1 func1_flt;

extern Type_info2 func2_cmplx;

extern Type_info2 func2_flt;


#endif