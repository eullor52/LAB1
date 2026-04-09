#include "lineform.h"
#include "operations.h"
#include "globals.h"


dif_add set_for_dif_add(void* a, void* b, char type, unsigned size_a, unsigned size_b)
{
    dif_add lnf;

    if (!a || !b || !size_a || !size_b) 
    {
        set_error(UNDEF_LNF_ARGS);
        return lnf;
    }

    lnf.a = a;
    lnf.b = b;
    lnf.size_a = size_a;
    lnf.size_b = size_b;
    
    if (type == 'c')
    {
        lnf.calculations = func1_cmplx;
    }
    else if (type == 'f')
    {
        lnf.calculations = func1_flt;
    }
    else
    {
        set_error(INVAL_TYPE);
        return lnf;
    }

    return lnf;
}

mul_calc set_for_mul_calc(void* lnf, void* vrlbs, char type, unsigned size)
{
    mul_calc result;

    if (!lnf || !vrlbs || !size) 
    {
        set_error(UNDEF_LNF_ARGS);
        return result;
    }

    result.lnf = lnf;
    result.size = size;
    result.vrlbs = vrlbs;

    if (type == 'c')
    {
        result.calculations = func2_cmplx;
    }
    else if (type == 'f')
    {
        result.calculations = func2_flt;
    }
    else 
    {
        set_error(INVAL_TYPE);
        return result;
    }
    
    return result;
}