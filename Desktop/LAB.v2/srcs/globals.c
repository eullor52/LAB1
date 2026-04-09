#include "globals.h"
#include "operations.h"
#include "lineform.h"

enum error error_info = NO_ERROR;


Type_info1 func1_cmplx = {
    .add_lnf = get_cmplx_sum,
    .dif_lnf = get_cmplx_dif
};

Type_info1 func1_flt = {
    .add_lnf = get_flt_sum,
    .dif_lnf = get_flt_dif
};

Type_info2 func2_cmplx = {
    .mul_lnf = get_cmplx_mul,
    .calc_lnf = get_cmplx_calc
};

Type_info2 func2_flt = {
    .mul_lnf = get_flt_mul,
    .calc_lnf = get_flt_calc
};

void set_error(enum error err_code)
{
    error_info = err_code;
}

enum error get_error()
{
    return error_info;
}