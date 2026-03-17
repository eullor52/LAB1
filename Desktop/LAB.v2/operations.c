#include "operations.h"
#include "complex.h"
#include <stdlib.h>

unsigned result_size(unsigned a, unsigned b)
{
    if (a > b) return a;
    else return b;
}

void* get_cmplx_sum(void* a, void* b, unsigned size_a, unsigned size_b)
{
    unsigned r_size = result_size(size_a, size_b);
    Complex* result = malloc(sizeof(Complex) * r_size);
    Complex *left = (Complex*)a, *right = (Complex*)b;

    if (size_a > size_b)
    {
        for (int i = 0; i < size_b; i++) result[i] = add_cmplx(left[i], right[i]);
        for (int i = size_b; i < size_a; i++) result[i] = left[i];
    }
    else
    {
        for (int i = 0; i < size_a; i++) result[i] = add_cmplx(left[i], right[i]);
        for (int i = size_a; i < size_b; i++) result[i] = right[i];
    }

    return (void*)result;
}

void* get_flt_sum(void* a, void* b, unsigned size_a, unsigned size_b)
{
    unsigned r_size = result_size(size_a, size_b);
    float* result = malloc(sizeof(float) * r_size);
    float *left = (float*)a, *right = (float*)b;

    if (size_a > size_b)
    {
        for (int i = 0; i < size_b; i++) result[i] = left[i] + right[i];
        for (int i = size_b; i < size_a; i++) result[i] = left[i];
    }
    else
    {
        for (int i = 0; i < size_a; i++) result[i] = left[i] + right[i];
        for (int i = size_a; i < size_b; i++) result[i] = right[i];
    }

    return (void*)result;
}

void* get_cmplx_dif(void* a, void* b, unsigned size_a, unsigned size_b)
{
    unsigned r_size = result_size(size_a, size_b);
    Complex* result = malloc(sizeof(Complex) * r_size);
    Complex *left = (Complex*)a, *right = (Complex*)b;

    if (size_a > size_b)
    {
        for (int i = 0; i < size_b; i++) result[i] = dif_cmplx(left[i], right[i]);
        for (int i = size_b; i < size_a; i++) result[i] = left[i];
    }
    else
    {
        for (int i = 0; i < size_a; i++) result[i] = dif_cmplx(left[i], right[i]);
        for (int i = size_a; i < size_b; i++)
        {
            result[i] = right[i];
            result->Im *= -1;
            result->Re *= -1;
        }
    }

    return (void*)result;
}

void* get_flt_dif(void* a, void* b, unsigned size_a, unsigned size_b)
{
    unsigned r_size = result_size(size_a, size_b);
    float* result = malloc(sizeof(float) * r_size);
    float *left = (float*)a, *right = (float*)b;

    if (size_a > size_b)
    {
        for (int i = 0; i < size_b; i++) result[i] = left[i] - right[i];
        for (int i = size_b; i < size_a; i++) result[i] = left[i];
    }
    else
    {
        for (int i = 0; i < size_a; i++) result[i] = left[i] - right[i];
        for (int i = size_a; i < size_b; i++) result[i] = (-1) * right[i];
    }

    return (void*)result;
}

void* get_cmplx_mul(void* a, void* b, unsigned size)
{
    Complex* result = malloc(sizeof(Complex) * size);
    Complex *lnf = (Complex*)a, *factor = (Complex*)b;

    for (int i = 0; i < size; i++) result[i] = mul_cmplx(lnf[i], *factor);

    return (void*)result;
}

void* get_flt_mul(void* a, void* b, unsigned size)
{
    float* result = malloc(sizeof(float) * size);
    float *lnf = (float*)a, *factor = (float*)b;

    for (int i = 0; i < size; i++) result[i] = lnf[i] * (*factor);

    return (void*)result;
}

void* get_cmplx_calc(void* a, void* b, unsigned size)
{
    Complex* result  = malloc(sizeof(Complex));
    Complex *lnf  = (Complex*)a, *factors = (Complex*)b;

    *result  = lnf[0];

    for (int i = 1; i < size; i++)
    {
        Complex tmp = mul_cmplx(lnf[i], factors[i - 1]);
        *result = add_cmplx(*result, tmp);
    }
    return (void*)result;
}

void* get_flt_calc(void* a, void* b, unsigned size)
{
    float* result  = malloc(sizeof(float));
    float *lnf  = (float*)a, *factors = (float*)b;

    *result  = lnf[0];

    for (int i = 1; i < size; i++)
    {
        float tmp = lnf[i] * factors[i - 1];
        *result += tmp;
    }
    return (void*)result;
}