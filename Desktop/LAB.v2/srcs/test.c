#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "complex.h"
#include "globals.h"
#include "io.h"
#include "lineform.h"
#include "operations.h"

#define ASSERT(expr) \
    do { \
        if (!(expr)) { \
            fprintf(stderr, "Assertion failed: (%s) в файле %s в строке %d\n", #expr, __FILE__, __LINE__); \
            exit(1); \
        } \
    } while(0)

static void reset_error(void) {
    set_error(NO_ERROR);
}

static int cmplx_eq(Complex a, Complex b, float eps) {
    return (fabsf(a.Re - b.Re) < eps) && (fabsf(a.Im - b.Im) < eps);
}

static int cmplx_arr_eq(Complex* a, Complex* b, unsigned n, float eps) {
    for (unsigned i = 0; i < n; i++)
        if (!cmplx_eq(a[i], b[i], eps)) return 0;
    return 1;
}

static int flt_arr_eq(float* a, float* b, unsigned n, float eps) {
    for (unsigned i = 0; i < n; i++)
        if (fabsf(a[i] - b[i]) > eps) return 0;
    return 1;
}

static int str_eq(const char* s1, const char* s2) {
    if (!s1 || !s2) return s1 == s2;
    while (*s1 && *s2 && *s1 == *s2) { s1++; s2++; }
    return (*s1 == '\0' || *s1 == '\n') && (*s2 == '\0' || *s2 == '\n');
}

static void test_add_cmplx(void) {
    Complex a = {1.0f, 2.0f};
    Complex b = {3.0f, 4.0f};
    Complex res = add_cmplx(a, b);
    ASSERT(fabsf(res.Re - 4.0f) < 1e-5);
    ASSERT(fabsf(res.Im - 6.0f) < 1e-5);

    a = (Complex){-1.0f, -2.0f};
    b = (Complex){1.0f, 2.0f};
    res = add_cmplx(a, b);
    ASSERT(fabsf(res.Re) < 1e-5 && fabsf(res.Im) < 1e-5);
    printf("test_add_cmplx OK\n");
}

static void test_dif_cmplx(void) {
    Complex a = {5.0f, 7.0f};
    Complex b = {2.0f, 3.0f};
    Complex res = dif_cmplx(a, b);
    ASSERT(fabsf(res.Re - 3.0f) < 1e-5);
    ASSERT(fabsf(res.Im - 4.0f) < 1e-5);

    a = (Complex){2.0f, 2.0f};
    b = (Complex){2.0f, 2.0f};
    res = dif_cmplx(a, b);
    ASSERT(fabsf(res.Re) < 1e-5 && fabsf(res.Im) < 1e-5);
    printf("test_dif_cmplx OK\n");
}

static void test_mul_cmplx(void) {
    Complex a = {1.0f, 2.0f};
    Complex b = {3.0f, 4.0f};
    Complex res = mul_cmplx(a, b);
    ASSERT(fabsf(res.Re + 5.0f) < 1e-5);
    ASSERT(fabsf(res.Im - 10.0f) < 1e-5);

    a = (Complex){0.0f, 1.0f};
    b = (Complex){0.0f, 1.0f};
    res = mul_cmplx(a, b);
    ASSERT(fabsf(res.Re + 1.0f) < 1e-5);
    ASSERT(fabsf(res.Im) < 1e-5);
    printf("test_mul_cmplx OK\n");
}

static void test_result_size(void) {
    ASSERT(result_size(5, 3) == 5);
    ASSERT(result_size(2, 7) == 7);
    ASSERT(result_size(0, 0) == 0);
    printf("test_result_size OK\n");
}

static void test_get_cmplx_sum(void) {
    Complex a[] = {{1,1}, {2,2}, {3,3}};
    Complex b[] = {{10,10}, {20,20}};
    unsigned sa = 3, sb = 2;
    void* res = get_cmplx_sum(a, b, sa, sb);
    ASSERT(res != NULL);
    Complex* cres = (Complex*)res;
    Complex expected[] = {{11,11}, {22,22}, {3,3}};
    ASSERT(cmplx_arr_eq(cres, expected, 3, 1e-5));
    free(res);
    printf("test_get_cmplx_sum OK\n");
}

static void test_get_flt_sum(void) {
    float a[] = {1.0f, 2.0f, 3.0f};
    float b[] = {10.0f, 20.0f};
    void* res = get_flt_sum(a, b, 3, 2);
    ASSERT(res != NULL);
    float* fres = (float*)res;
    float expected[] = {11.0f, 22.0f, 3.0f};
    ASSERT(flt_arr_eq(fres, expected, 3, 1e-5));
    free(res);
    printf("test_get_flt_sum OK\n");
}

static void test_get_cmplx_dif(void) {
    Complex a[] = {{5,5}, {4,4}, {3,3}};
    Complex b[] = {{1,1}, {2,2}};
    void* res = get_cmplx_dif(a, b, 3, 2);
    ASSERT(res != NULL);
    Complex* cres = (Complex*)res;
    Complex expected[] = {{4,4}, {2,2}, {3,3}};
    ASSERT(cmplx_arr_eq(cres, expected, 3, 1e-5));
    free(res);
    printf("test_get_cmplx_dif OK\n");
}

static void test_get_flt_dif(void) {
    float a[] = {5.0f, 4.0f, 3.0f};
    float b[] = {1.0f, 2.0f};
    void* res = get_flt_dif(a, b, 3, 2);
    ASSERT(res != NULL);
    float* fres = (float*)res;
    float expected[] = {4.0f, 2.0f, 3.0f};
    ASSERT(flt_arr_eq(fres, expected, 3, 1e-5));
    free(res);
    printf("test_get_flt_dif OK\n");
}

static void test_get_cmplx_mul(void) {
    Complex lnf[] = {{1,0}, {0,1}, {2,2}};
    Complex factor = {2,0};
    void* res = get_cmplx_mul(lnf, &factor, 3);
    ASSERT(res != NULL);
    Complex* cres = (Complex*)res;
    Complex expected[] = {{2,0}, {0,2}, {4,4}};
    ASSERT(cmplx_arr_eq(cres, expected, 3, 1e-5));
    free(res);
    printf("test_get_cmplx_mul OK\n");
}

static void test_get_flt_mul(void) {
    float lnf[] = {1.0f, 2.0f, 3.0f};
    float factor = 2.5f;
    void* res = get_flt_mul(lnf, &factor, 3);
    ASSERT(res != NULL);
    float* fres = (float*)res;
    float expected[] = {2.5f, 5.0f, 7.5f};
    ASSERT(flt_arr_eq(fres, expected, 3, 1e-5));
    free(res);
    printf("test_get_flt_mul OK\n");
}

static void test_get_cmplx_calc(void) {
    Complex lnf[] = {{1,0}, {2,0}, {3,0}};
    Complex vars[] = {{2,0}, {3,0}};
    void* res = get_cmplx_calc(lnf, vars, 3);
    ASSERT(res != NULL);
    Complex* cres = (Complex*)res;
    ASSERT(fabsf(cres->Re - 14.0f) < 1e-5);
    ASSERT(fabsf(cres->Im) < 1e-5);
    free(res);
    printf("test_get_cmplx_calc OK\n");
}

static void test_get_flt_calc(void) {
    float lnf[] = {1.0f, 2.0f, 3.0f};
    float vars[] = {2.0f, 3.0f};
    void* res = get_flt_calc(lnf, vars, 3);
    ASSERT(res != NULL);
    float* fres = (float*)res;
    ASSERT(fabsf(*fres - 14.0f) < 1e-5);
    free(res);
    printf("test_get_flt_calc OK\n");
}

static void test_check_float(void) {
    ASSERT(check_float("123") == 3);
    ASSERT(check_float("12.34") == 5);
    ASSERT(check_float("-45.67") == 0);
    ASSERT(check_float("abc") == 0);
    printf("test_check_float OK\n");
}

static void test_check_cmplx(void) {
    C_info ci = check_cmplx("3+4*i");
    ASSERT(ci.count == 2 && ci.len == 5);
    ci = check_cmplx("5");
    ASSERT(ci.count == 1);
    ci = check_cmplx("7*i");
    ASSERT(ci.count == -1);
    ci = check_cmplx("invalid");
    ASSERT(ci.count == 0);
    printf("test_check_cmplx OK\n");
}

static void test_cmplx_output(void) {
    char buf[100];
    Complex c = {1.23f, 4.56f};
    int len = cmplx_output(buf, c);
    ASSERT(len > 0);
    ASSERT(strstr(buf, "1.23") && strstr(buf, "4.56"));
    c = (Complex){0,0};
    len = cmplx_output(buf, c);
    ASSERT(len == 0);
    c = (Complex){5,0};
    len = cmplx_output(buf, c);
    ASSERT(strstr(buf, "5.00") && !strstr(buf, "i"));
    printf("test_cmplx_output OK\n");
}

static void test_get_lnf_float(void) {
    char str[] = "1.5 2.7 3.0\n";
    Lnf l = get_lnf(str, 'f');
    ASSERT(l.lnf != NULL && l.size == 3);
    float* arr = (float*)l.lnf;
    ASSERT(fabsf(arr[0] - 1.5f) < 1e-5);
    ASSERT(fabsf(arr[1] - 2.7f) < 1e-5);
    ASSERT(fabsf(arr[2] - 3.0f) < 1e-5);
    free(l.lnf);
    printf("test_get_lnf_float OK\n");
}

static void test_get_lnf_complex(void) {
    char str[] = "1+2*i 3*i 5\n";
    Lnf l = get_lnf(str, 'c');
    ASSERT(l.lnf != NULL && l.size == 3);
    Complex* arr = (Complex*)l.lnf;
    ASSERT(fabsf(arr[0].Re - 1.0f) < 1e-5 && fabsf(arr[0].Im - 2.0f) < 1e-5);
    ASSERT(fabsf(arr[1].Re) < 1e-5 && fabsf(arr[1].Im - 3.0f) < 1e-5);
    ASSERT(fabsf(arr[2].Re - 5.0f) < 1e-5 && fabsf(arr[2].Im) < 1e-5);
    free(l.lnf);
    printf("test_get_lnf_complex OK\n");
}

static void test_get_factor(void) {
    void* f = get_factor("12.5", 'f');
    ASSERT(f != NULL);
    ASSERT(fabsf(*(float*)f - 12.5f) < 1e-5);
    free(f);

    f = get_factor("2+3*i", 'c');
    ASSERT(f != NULL);
    Complex* c = (Complex*)f;
    ASSERT(fabsf(c->Re - 2.0f) < 1e-5 && fabsf(c->Im - 3.0f) < 1e-5);
    free(f);
    printf("test_get_factor OK\n");
}

static void test_output_lnf(void) {
    Complex carr[] = {{1,1}, {2,0}, {0,3}};
    char* s = output_lnf(carr, 3, 'c');
    ASSERT(s != NULL);
    ASSERT(strstr(s, "1.00 + 1.00*i") != NULL);
    ASSERT(strstr(s, "2.00*x1") != NULL);
    ASSERT(strstr(s, "3.00*i*x2") != NULL);
    free(s);

    float farr[] = {1.5f, 2.5f, 3.5f};
    s = output_lnf(farr, 3, 'f');
    ASSERT(s != NULL);
    ASSERT(strstr(s, "1.50") && strstr(s, "2.50*x1") && strstr(s, "3.50*x2"));
    free(s);
    printf("test_output_lnf OK\n");
}

static void test_output_num(void) {
    Complex c = {1.23f, 4.56f};
    char* s = output_num(&c, 'c');
    ASSERT(s != NULL);
    ASSERT(strstr(s, "1.23") && strstr(s, "4.56"));
    free(s);

    float f = 3.1415f;
    s = output_num(&f, 'f');
    ASSERT(s != NULL);
    ASSERT(strstr(s, "3.14"));
    free(s);
    printf("test_output_num OK\n");
}

static void test_error_massage(void) {
    set_error(NO_ERROR);
    ASSERT(str_eq(error_massage(), "Неизвестная ошибка."));
    set_error(MEMORY_ERROR);
    ASSERT(str_eq(error_massage(), "Ошибка выделения памяти."));
    set_error(INVAL_TYPE);
    ASSERT(str_eq(error_massage(), "Ошибка: неизвестный тип."));
    printf("test_error_massage OK\n");
}

static void test_set_for_dif_add(void) {
    float a[] = {1.0f}, b[] = {2.0f};
    dif_add da = set_for_dif_add(a, b, 'f', 1, 1);
    ASSERT(da.calculations.add_lnf != NULL);
    ASSERT(da.calculations.dif_lnf != NULL);
    reset_error();
    da = set_for_dif_add(NULL, b, 'f', 1, 1);
    ASSERT(get_error() == INVAL_LNF_PARAM);
    reset_error();
    da = set_for_dif_add(a, b, 'x', 1, 1);
    ASSERT(get_error() == INVAL_TYPE);
    printf("test_set_for_dif_add OK\n");
}

static void test_set_for_mul_calc(void) {
    float lnf[] = {1.0f}, vars[] = {2.0f};
    mul_calc mc = set_for_mul_calc(lnf, vars, 'f', 1);
    ASSERT(mc.calculations.mul_lnf != NULL);
    ASSERT(mc.calculations.calc_lnf != NULL);
    reset_error();
    mc = set_for_mul_calc(NULL, vars, 'f', 1);
    ASSERT(get_error() == INVAL_LNF_PARAM);
    reset_error();
    mc = set_for_mul_calc(lnf, vars, 'x', 1);
    ASSERT(get_error() == INVAL_TYPE);
    printf("test_set_for_mul_calc OK\n");
}

static void test_globals_error(void) {
    reset_error();
    ASSERT(get_error() == NO_ERROR);
    set_error(MEMORY_ERROR);
    ASSERT(get_error() == MEMORY_ERROR);
    set_error(INVAL_TYPE);
    ASSERT(get_error() == INVAL_TYPE);
    printf("test_globals_error OK\n");
}

int main() {
    printf("========== RUNNING TESTS ==========\n");

    test_add_cmplx();
    test_dif_cmplx();
    test_mul_cmplx();

    test_result_size();
    test_get_cmplx_sum();
    test_get_flt_sum();
    test_get_cmplx_dif();
    test_get_flt_dif();
    test_get_cmplx_mul();
    test_get_flt_mul();
    test_get_cmplx_calc();
    test_get_flt_calc();

    test_check_float();
    test_check_cmplx();
    test_cmplx_output();
    test_get_lnf_float();
    test_get_lnf_complex();
    test_get_factor();
    test_output_lnf();
    test_output_num();
    test_error_massage();

    test_set_for_dif_add();
    test_set_for_mul_calc();

    test_globals_error();

    printf("========== ALL TESTS PASSED ==========\n");
    return 0;
}