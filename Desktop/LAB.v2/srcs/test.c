#include <stdio.h>

#include "complex.h"
#include "globals.h"
#include "io.h"
#include "lineform.h"
#include "operations.h"

#define assert(statement, counter) \
    ((statement) ? (void)0 : (*(counter) += 1, error_notif(#statement, __FILE__, __LINE__, __func__)))

void error_notif(const char* statement, const char* file, int line, const char* func)
{
    printf("Ошибка: %s\n   В файле %s, функции %s, строке %d.\n", 
           statement, file, func, line);
}

float float_abs(float x)
{
    return (x < 0) ? -x : x;
}

int str_eq(const char* a, const char* b)
{
    while (*a && *b && *a == *b) {
        a++;
        b++;
    }
    return *a == *b;
}

int cmplx_comp(Complex a, Complex b)
{
    const float eps = 1e-5f;
    return float_abs(a.Re - b.Re) < eps && float_abs(a.Im - b.Im) < eps;
}

int flt_comp(float a, float b)
{
    const float eps = 1e-5f;
    return float_abs(a - b) < eps;
}

int test_complex()
{
    int counter = 0;

    Complex a = {1.0f, 2.0f};
    Complex b = {3.0f, 4.0f};
    Complex res;

    res = add_cmplx(a, b);
    assert(cmplx_comp(res, (Complex){4.0f, 6.0f}), &counter);

    res = dif_cmplx(a, b);
    assert(cmplx_comp(res, (Complex){-2.0f, -2.0f}), &counter);

    res = mul_cmplx(a, b);
    assert(cmplx_comp(res, (Complex){-5.0f, 10.0f}), &counter);

    Complex zero = {0.0f, 0.0f};
    res = mul_cmplx(a, zero);
    assert(cmplx_comp(res, zero), &counter);

    return counter;
}

int test_operations_complex()
{
    int counter = 0;

    Complex sum_a[] = {{1,1}, {2,2}, {3,3}};
    Complex sum_b[] = {{0,1}, {1,0}};
    unsigned size_a = 3, size_b = 2;
    void* res = get_cmplx_sum(sum_a, sum_b, size_a, size_b);
    assert(res != NULL, &counter);
    Complex* cres = (Complex*)res;
    assert(cmplx_comp(cres[0], (Complex){1,2}), &counter);
    assert(cmplx_comp(cres[1], (Complex){3,2}), &counter);
    assert(cmplx_comp(cres[2], (Complex){3,3}), &counter);
    free(res);

    Complex dif_a[] = {{5,5}, {3,3}, {1,1}};
    Complex dif_b[] = {{1,1}, {2,2}};
    res = get_cmplx_dif(dif_a, dif_b, 3, 2);
    assert(res != NULL, &counter);
    cres = (Complex*)res;
    assert(cmplx_comp(cres[0], (Complex){4,4}), &counter);
    assert(cmplx_comp(cres[1], (Complex){1,1}), &counter);
    assert(cmplx_comp(cres[2], (Complex){1,1}), &counter);
    free(res);

    Complex mul_lnf[] = {{1,0}, {0,1}, {2,3}};
    Complex factor = {2,2};
    res = get_cmplx_mul(mul_lnf, &factor, 3);
    assert(res != NULL, &counter);
    cres = (Complex*)res;
    assert(cmplx_comp(cres[0], (Complex){2,2}), &counter);
    assert(cmplx_comp(cres[1], (Complex){-2,2}), &counter);
    assert(cmplx_comp(cres[2], (Complex){-2,10}), &counter);
    free(res);

    Complex calc_lnf[] = {{1,2}, {3,4}, {5,6}};
    Complex factors[] = {{1,0}, {1,0}};
    res = get_cmplx_calc(calc_lnf, factors, 3);
    assert(res != NULL, &counter);
    cres = (Complex*)res;
    assert(cmplx_comp(*cres, (Complex){1+3+5, 2+4+6}), &counter);
    free(res);

    return counter;
}

int test_operations_float()
{
    int counter = 0;

    float sum_a[] = {1.5f, 2.5f, 3.5f};
    float sum_b[] = {0.5f, 1.5f};
    void* res = get_flt_sum(sum_a, sum_b, 3, 2);
    assert(res != NULL, &counter);
    float* fres = (float*)res;
    assert(flt_comp(fres[0], 2.0f), &counter);
    assert(flt_comp(fres[1], 4.0f), &counter);
    assert(flt_comp(fres[2], 3.5f), &counter);
    free(res);

    float dif_a[] = {10.0f, 20.0f};
    float dif_b[] = {1.0f, 2.0f, 3.0f};
    res = get_flt_dif(dif_a, dif_b, 2, 3);
    assert(res != NULL, &counter);
    fres = (float*)res;
    assert(flt_comp(fres[0], 9.0f), &counter);
    assert(flt_comp(fres[1], 18.0f), &counter);
    assert(flt_comp(fres[2], -3.0f), &counter);
    free(res);

    float mul_lnf[] = {1.5f, 2.0f, 3.5f};
    float factor = 2.0f;
    res = get_flt_mul(mul_lnf, &factor, 3);
    assert(res != NULL, &counter);
    fres = (float*)res;
    assert(flt_comp(fres[0], 3.0f), &counter);
    assert(flt_comp(fres[1], 4.0f), &counter);
    assert(flt_comp(fres[2], 7.0f), &counter);
    free(res);

    float calc_lnf[] = {1.0f, 2.0f, 3.0f};
    float calc_factors[] = {2.0f, 3.0f};
    res = get_flt_calc(calc_lnf, calc_factors, 3);
    assert(res != NULL, &counter);
    fres = (float*)res;
    assert(flt_comp(*fres, 14.0f), &counter);
    free(res);

    return counter;
}

int test_io_functions()
{
    int counter = 0;
    char buf[100];

    Complex c = {1.23f, 4.56f};
    int len = cmplx_output(buf, c);
    const char* expected1 = "1.23 + 4.56*i";
    int ok = 1;
    for (int i = 0; i < len && expected1[i]; i++) {
        if (buf[i] != expected1[i]) { ok = 0; break; }
    }
    assert(ok && buf[len] == '\0', &counter);
    assert(len > 0, &counter);

    c = (Complex){0, 5};
    len = cmplx_output(buf, c);
    const char* expected2 = "5.00*i";
    ok = 1;
    for (int i = 0; i < len && expected2[i]; i++) {
        if (buf[i] != expected2[i]) { ok = 0; break; }
    }
    assert(ok, &counter);

    c = (Complex){-3, 0};
    len = cmplx_output(buf, c);
    const char* expected3 = "-3.00";
    ok = 1;
    for (int i = 0; i < len && expected3[i]; i++) {
        if (buf[i] != expected3[i]) { ok = 0; break; }
    }
    assert(ok, &counter);

    void* f = get_factor("2+3*i", 'c');
    assert(f != NULL, &counter);
    Complex* cf = (Complex*)f;
    assert(cmplx_comp(*cf, (Complex){2,3}), &counter);
    free(f);

    f = get_factor("7*i", 'c');
    cf = (Complex*)f;
    assert(cmplx_comp(*cf, (Complex){0,7}), &counter);
    free(f);

    f = get_factor("3.14", 'f');
    assert(f != NULL, &counter);
    float* ff = (float*)f;
    assert(flt_comp(*ff, 3.14f), &counter);
    free(f);

    c = (Complex){2.5f, -1.5f};
    char* s = output_num(&c, 'c');
    assert(s != NULL, &counter);
    const char* expected4 = "2.50 + -1.50*i";
    ok = 1;
    for (int i = 0; expected4[i]; i++) {
        if (s[i] != expected4[i]) { ok = 0; break; }
    }
    assert(ok, &counter);
    free(s);

    float val = 123.456f;
    s = output_num(&val, 'f');
    assert(s != NULL, &counter);
    const char* expected5 = "123.46";
    ok = 1;
    for (int i = 0; expected5[i]; i++) {
        if (s[i] != expected5[i]) { ok = 0; break; }
    }
    assert(ok, &counter);
    free(s);

    assert(check_float("123") == 3, &counter);
    assert(check_float("12.34") == 5, &counter);
    assert(check_float("abc") == 0, &counter);

    C_info ci = check_cmplx("3+4*i");
    assert(ci.count == 2, &counter);
    ci = check_cmplx("7");
    assert(ci.count == 1, &counter);
    ci = check_cmplx("7*i");
    assert(ci.count == -1, &counter);

    return counter;
}

int test_lineform()
{
    int counter = 0;

    float a[] = {1.0f, 2.0f};
    float b[] = {3.0f};
    dif_add da = set_for_dif_add(a, b, 'f', 2, 1);
    assert(da.calculations.add_lnf != NULL, &counter);
    assert(da.calculations.dif_lnf != NULL, &counter);
    assert(da.size_a == 2 && da.size_b == 1, &counter);

    set_error(NO_ERROR);
    da = set_for_dif_add(a, b, 'x', 2, 1);
    assert(get_error() == INVAL_TYPE, &counter);

    float lnf[] = {1.0f, 2.0f};
    float vr[] = {3.0f, 4.0f};
    mul_calc mc = set_for_mul_calc(lnf, vr, 'f', 2);
    assert(mc.calculations.mul_lnf != NULL, &counter);
    assert(mc.calculations.calc_lnf != NULL, &counter);
    assert(mc.size == 2, &counter);

    return counter;
}

int test_globals()
{
    int counter = 0;

    set_error(NO_ERROR);
    assert(get_error() == NO_ERROR, &counter);
    set_error(MEMORY_ERROR);
    assert(get_error() == MEMORY_ERROR, &counter);
    set_error(INVAL_LNF_PARAM);
    assert(get_error() == INVAL_LNF_PARAM, &counter);

    return counter;
}

int main()
{
    int total_errors = 0;

    printf("Запуск тестов...\n\n");

    total_errors += test_complex();
    total_errors += test_operations_complex();
    total_errors += test_operations_float();
    total_errors += test_io_functions();
    total_errors += test_lineform();
    total_errors += test_globals();

    printf("\n========== РЕЗУЛЬТАТ ==========\n");
    if (total_errors == 0)
        printf("Все тесты пройдены успешно!\n");
    else
        printf("Обнаружено ошибок: %d\n", total_errors);

    return total_errors;
}