#ifndef OPERATIONS_H
#define OPERATIONS_H


unsigned result_size(unsigned a, unsigned b);

void* get_cmplx_sum(void* a, void* b, unsigned size_a, unsigned size_b);
void* get_flt_sum(void* a, void* b, unsigned size_a, unsigned size_b);

void* get_cmplx_dif(void* a, void* b, unsigned size_a, unsigned size_b);
void* get_flt_dif(void* a, void* b, unsigned size_a, unsigned size_b);

void* get_cmplx_mul(void* a, void* b, unsigned size);
void* get_flt_mul(void* a, void* b, unsigned size);

void* get_cmplx_calc(void* a, void* b, unsigned size);
void* get_flt_calc(void* a, void* b, unsigned size);

#endif