#ifndef GLOBALS_H
#define GLOBALS_H

enum error
{
    NO_ERROR,
    MEMORY_ERROR,
    INVAL_TYPE,
    UNDEF_LNF_ARGS,
    INVAL_INPUT_STR,
    OUTPUT_ERROR
};

extern enum error error_info;

void set_error(enum error err_code);
enum error get_error();

extern Type_info1 func1_cmplx;

extern Type_info1 func1_flt;

extern Type_info2 func2_cmplx;

extern Type_info2 func2_flt;


#endif