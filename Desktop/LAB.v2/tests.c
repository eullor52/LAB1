#include "tests.h"
#include "io.h"
#include <stdio.h>

int test_1()
{
    int assert = 9;
    char str[] = "1234,2323";

    if (check_float(str) != assert) 
    {
        fprintf(stderr, "Test #1 crashed.\n");
        return 1;
    }
    else
    {
        fprintf(stderr, "Test #1 succes.\n");
        return 0;
    }
}

int test_2()
{
    C_info assert = {9, 2};
    char str[] = "12+2,31*i\n";

    C_info res = check_cmplx(str);
    if (res.count == assert.count && res.len == assert.len)
    {
        fprintf(stderr, "Test #2 succes.\n");
        return 0;
    }
    else
    {
        fprintf(stderr, "Test #2 crashed.\n");
        return 1;
    }
}

int test_3_1()
{
    Complex ass_lnf[] = {{12,2.31}};
    int size = 1;
    char str[] = "12+2.31*i\n";

    Lnf result = get_lnf(str, 'c');
    Complex* res = (Complex*)result.lnf;

    if (result.size == NUM_ERROR.size) 
    {
        fprintf(stderr, "Test #3.1 crashed with error code -1.\n");
        return -1;
    }
    else if (result.size == MEMORY_ERROR.size)
    {
        fprintf(stderr, "Test #3.1 crashed with error code -2.\n");
        return -2;
    }
    else if ( result.size == TYPE_ERROR.size)
    {
        fprintf(stderr, "Test #3.1 crashed with error code -3.\n");
        return -3;
    }
    else if (res->Re == ass_lnf->Re && res->Im == ass_lnf->Im /*&& res[1].Re == ass_lnf[1].Re &&
    res[1].Im == ass_lnf[1].Im && res[2].Re == ass_lnf[2].Re && res[2].Im == ass_lnf[2].Im*/ && size == result.size)
    {
        fprintf(stderr, "Test #3.1 succes.\n");
        return 0;
    }
    else 
    {
        fprintf(stderr, "Test #3.1 crashed with unknown error.\n %f + %f*i size: %d\n", res->Re, res->Im, result.size);
        return 1;
    }
}

int test_3_2()
{
    float ass_lnf[] = {1,2,3};
    int ass_size = 3;
    char str[] = "1 2 3";

    Lnf result = get_lnf(str, 'f');

    float* res = (float*)result.lnf;

    if (result.size == NUM_ERROR.size) 
    {
        fprintf(stderr, "Test #3.2 crashed with error code -1.\n");
        return -1;
    }
    else if (result.size == MEMORY_ERROR.size)
    {
        fprintf(stderr, "Test #3.2 crashed with error code -2.\n");
        return -2;
    }
    else if (result.size == TYPE_ERROR.size)
    {
        fprintf(stderr, "Test #3.2 crashed with error code -3.\n");
        return -3;
    }
    else if (res[0] == ass_lnf[0] && res[1] == ass_lnf[1] && res[2] == ass_lnf[2] && result.size == ass_size)
    {
        fprintf(stderr, "Test #3.2 succes.\n");
        return 0;
    }
    else 
    {
        fprintf(stderr, "Test #3.2 crached with unknown error.\n");
        return 1;
    }
}

int test_4_1()
{
    Complex assert = {1.234,2.134};
    char str[] = "1.234+2.134*i";

    Complex* result = (Complex*)get_factor(str, 'c');

    if (!result)
    {
        fprintf(stderr, "Test #4.1 crashed with error code -1.");
        return -1;
    }
    else if (result->Re == assert.Re && result->Im == assert.Im)
    {
        fprintf(stderr, "Test #4.1 succes.");
        return 0;
    }
    else
    {
        fprintf(stderr, "Test #4.1 crashed with unknown error.");
        return 1;
    }
}