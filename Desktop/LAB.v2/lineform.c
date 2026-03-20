#include "lineform.h"
#include "operations.h"


dif_add set_for_dif_add(void* a, void* b, char type, unsigned size_a, unsigned size_b)
{
    dif_add lnf;

    if (!a || !b || !size_a || !size_b) return UNCOR_LNF_ERR;

    lnf.a = a;
    lnf.b = b;
    lnf.size_a = size_a;
    lnf.size_b = size_b;
    
    if (type == 'c')
    {
        lnf.add_lnf =  get_cmplx_sum;
        lnf.dif_lnf = get_cmplx_dif;
    }
    else if (type == 'f')
    {
        lnf.add_lnf = get_flt_sum;
        lnf.dif_lnf = get_flt_dif;
    }
    else return UNCOR_TYPE_ERR;

    return lnf;
}

mul_calc set_for_mul_calc(void* lnf, void* vrlbs, char type, unsigned size)
{
    mul_calc result;

    result.lnf = lnf;
    result.size = size;
    result.vrlbs = vrlbs;

    if (type == 'c')
    {
        result.calc_lnf = get_cmplx_calc;
        result.mul_lnf = get_cmplx_mul;
    }
    else
    {
        result.calc_lnf = get_flt_calc;
        result.mul_lnf = get_flt_mul;
    }
    return result;
}