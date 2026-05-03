#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "headers/complex.h"
#include "headers/globals.h"
#include "headers/io.h"
#include "headers/lineform.h"
#include "headers/operations.h"

#define EPS 1e-5f

void print_complex(Complex c) {
    char buf[100];
    complex_to_string(buf, c);
    printf("%s", buf);
}

int double_eq(double a, double b) {
    return fabs(a - b) < EPS;
}

int complex_eq(Complex a, Complex b) {
    return fabs(a.Re - b.Re) < EPS && fabs(a.Im - b.Im) < EPS;
}

int double_arr_eq(double* a, double* b, size_t n) {
    for (size_t i = 0; i < n; i++)
        if (!double_eq(a[i], b[i])) return 0;
    return 1;
}

int complex_arr_eq(Complex* a, Complex* b, size_t n) {
    for (size_t i = 0; i < n; i++)
        if (!complex_eq(a[i], b[i])) return 0;
    return 1;
}

#define CHECK_COND(cond, msg, passed) \
    do { \
        if (!(cond)) { \
            fprintf(stderr, "Ошибка в %s (%s:%d): %s\n", __func__, __FILE__, __LINE__, msg); \
            *(passed) = 0; \
        } \
    } while(0)

#define CHECK_INT_EQ(expected, actual, test_name, passed) \
    do { \
        if ((expected) != (actual)) { \
            fprintf(stderr, "Ошибка в %s (%s:%d): %s: ожидалось %d, получено %d\n", \
                    __func__, __FILE__, __LINE__, (test_name), (expected), (actual)); \
            *(passed) = 0; \
        } \
    } while(0)

#define CHECK_DOUBLE_EQ(expected, actual, test_name, passed) \
    do { \
        if (!double_eq((expected), (actual))) { \
            fprintf(stderr, "Ошибка в %s (%s:%d): %s: ожидалось %.6f, получено %.6f\n", \
                    __func__, __FILE__, __LINE__, (test_name), (expected), (actual)); \
            *(passed) = 0; \
        } \
    } while(0)

#define CHECK_COMPLEX_EQ(expected, actual, test_name, passed) \
    do { \
        if (!complex_eq((expected), (actual))) { \
            fprintf(stderr, "Ошибка в %s (%s:%d): %s: ожидалось ", \
                    __func__, __FILE__, __LINE__, (test_name)); \
            print_complex(expected); \
            fprintf(stderr, ", получено "); \
            print_complex(actual); \
            fprintf(stderr, "\n"); \
            *(passed) = 0; \
        } \
    } while(0)

#define CHECK_NON_NULL(ptr, name, passed) \
    do { \
        if ((ptr) == NULL) { \
            fprintf(stderr, "Ошибка в %s (%s:%d): %s вернул NULL\n", \
                    __func__, __FILE__, __LINE__, (name)); \
            *(passed) = 0; \
        } \
    } while(0)

int test_add_complex() {
    int passed = 1;
    Complex a = {1.0, 2.0};
    Complex b = {3.0, 4.0};
    Complex res = add_complex(a, b);
    Complex expected = {4.0, 6.0};
    CHECK_COMPLEX_EQ(expected, res, "add_complex (1+2i)+(3+4i)", &passed);

    a = (Complex){-1.0, -2.0};
    b = (Complex){1.0, 2.0};
    res = add_complex(a, b);
    expected = (Complex){0.0, 0.0};
    CHECK_COMPLEX_EQ(expected, res, "add_complex (-1-2i)+(1+2i)", &passed);
    if (passed) printf("  test_add_complex OK\n");
    return passed;
}

int test_dif_complex() {
    int passed = 1;
    Complex a = {5.0, 7.0};
    Complex b = {2.0, 3.0};
    Complex res = dif_complex(a, b);
    Complex expected = {3.0, 4.0};
    CHECK_COMPLEX_EQ(expected, res, "dif_complex (5+7i)-(2+3i)", &passed);

    a = (Complex){2.0, 2.0};
    b = (Complex){2.0, 2.0};
    res = dif_complex(a, b);
    expected = (Complex){0.0, 0.0};
    CHECK_COMPLEX_EQ(expected, res, "dif_complex (2+2i)-(2+2i)", &passed);
    if (passed) printf("  test_dif_complex OK\n");
    return passed;
}

int test_mul_complex() {
    int passed = 1;
    Complex a = {1.0, 2.0};
    Complex b = {3.0, 4.0};
    Complex res = mul_complex(a, b);
    Complex expected = {-5.0, 10.0};
    CHECK_COMPLEX_EQ(expected, res, "mul_complex (1+2i)*(3+4i)", &passed);

    a = (Complex){0.0, 1.0};
    b = (Complex){0.0, 1.0};
    res = mul_complex(a, b);
    expected = (Complex){-1.0, 0.0};
    CHECK_COMPLEX_EQ(expected, res, "mul_complex i*i", &passed);
    if (passed) printf("  test_mul_complex OK\n");
    return passed;
}

int test_globals_error() {
    int passed = 1;
    set_error(MEMORY_ERROR);
    clear_error();
    CHECK_COND(get_error() == NO_ERROR, "начальная ошибка не NO_ERROR", &passed);
    set_error(MEMORY_ERROR);
    CHECK_COND(get_error() == MEMORY_ERROR, "MEMORY_ERROR не установилась", &passed);
    set_error(INVAL_INPUT_STR);
    CHECK_COND(get_error() == INVAL_INPUT_STR, "INVAL_INPUT_STR не установилась", &passed);
    clear_error();
    CHECK_COND(get_error() == NO_ERROR, "сброс ошибки не сработал", &passed);
    if (passed) printf("  test_globals_error OK\n");
    return passed;
}

int test_check_double_number() {
    int passed = 1;
    CHECK_COND(check_double_number("123") > 0, "123 не распознано", &passed);
    CHECK_COND(check_double_number("12.34") > 0, "12.34 не распознано", &passed);
    CHECK_COND(check_double_number("-45.67") > 0, "-45.67 не распознано", &passed);
    CHECK_COND(check_double_number("+3.14") > 0, "+3.14 не распознано", &passed);
    CHECK_COND(check_double_number("abc") <= 0, "abc ошибочно распознано", &passed);
    CHECK_COND(check_double_number("") <= 0, "пустая строка ошибочно распознана", &passed);
    if (passed) printf("  test_check_double_number OK\n");
    return passed;
}

int test_check_complex_number() {
    int passed = 1;
    C_info ci = check_complex_number("3+4*i");
    CHECK_COND(ci.count == 2 && ci.len > 0, "3+4*i не распознано", &passed);
    ci = check_complex_number("5");
    CHECK_COND(ci.count == 1, "5 не распознано как действительное", &passed);
    ci = check_complex_number("-7*i");
    CHECK_COND(ci.count == -1, "-7*i не распознано как мнимое", &passed);
    ci = check_complex_number("invalid");
    CHECK_COND(ci.count == 0, "invalid ошибочно распознано", &passed);
    if (passed) printf("  test_check_complex_number OK\n");
    return passed;
}

int test_complex_to_string() {
    int passed = 1;
    char buf[100];
    Complex c = {1.23, 4.56};
    int len = complex_to_string(buf, c);
    CHECK_COND(len > 0, "не удалось преобразовать 1.23+4.56i", &passed);
    CHECK_COND(strstr(buf, "1.23") != NULL && strstr(buf, "4.56") != NULL,
               "неправильный формат вывода", &passed);
    c = (Complex){0,0};
    len = complex_to_string(buf, c);
    CHECK_COND(len == 0, "0 дал ненулевую строку", &passed);
    c = (Complex){5.0, 0};
    len = complex_to_string(buf, c);
    CHECK_COND(strstr(buf, "5.00") != NULL && strstr(buf, "i") == NULL,
               "чисто действительное число выводится с i", &passed);
    if (passed) printf("  test_complex_to_string OK\n");
    return passed;
}

int test_parse_double_lineform() {
    int passed = 1;
    clear_error();
    dynamic_array arr = parse_double_lineform("1.5 2.7 3.0");
    CHECK_COND(get_error() == NO_ERROR, "ошибка парсинга", &passed);
    CHECK_COND(arr.size == 3, "неправильное количество чисел", &passed);
    double* data = (double*)arr.data;
    CHECK_DOUBLE_EQ(1.5, data[0], "parse_double_lineform[0]", &passed);
    CHECK_DOUBLE_EQ(2.7, data[1], "parse_double_lineform[1]", &passed);
    CHECK_DOUBLE_EQ(3.0, data[2], "parse_double_lineform[2]", &passed);
    double_array.delete(&arr);
    if (passed) printf("  test_parse_double_lineform OK\n");
    return passed;
}

int test_parse_complex_lineform() {
    int passed = 1;
    clear_error();
    dynamic_array arr = parse_complex_lineform("1+2*i 3*i 5");
    CHECK_COND(get_error() == NO_ERROR, "ошибка парсинга комплексных чисел", &passed);
    CHECK_COND(arr.size == 3, "неправильное количество комплексных чисел", &passed);
    Complex* data = (Complex*)arr.data;
    Complex expected1 = {1,2};
    CHECK_COMPLEX_EQ(expected1, data[0], "parse_complex_lineform[0]", &passed);
    Complex expected2 = {0,3};
    CHECK_COMPLEX_EQ(expected2, data[1], "parse_complex_lineform[1]", &passed);
    Complex expected3 = {5,0};
    CHECK_COMPLEX_EQ(expected3, data[2], "parse_complex_lineform[2]", &passed);
    complex_array.delete(&arr);
    if (passed) printf("  test_parse_complex_lineform OK\n");
    return passed;
}

int test_format_double_lineform() {
    int passed = 1;
    dynamic_array arr = double_array.create_and_init(3, 1.0, 2.0, 3.0);
    char* str = format_double_lineform(&arr);
    CHECK_NON_NULL(str, "format_double_lineform", &passed);
    if (str) {
        CHECK_COND(strstr(str, "1.00") != NULL, "нет первого коэффициента", &passed);
        CHECK_COND(strstr(str, "2.00*x1") != NULL, "нет второго коэффициента", &passed);
        CHECK_COND(strstr(str, "3.00*x2") != NULL, "нет третьего коэффициента", &passed);
        free(str);
    }
    double_array.delete(&arr);
    if (passed) printf("  test_format_double_lineform OK\n");
    return passed;
}

int test_format_complex_lineform() {
    int passed = 1;
    Complex c1 = {1,1}, c2 = {2,0}, c3 = {0,3};
    dynamic_array arr = complex_array.create_and_init(3, c1, c2, c3);
    char* str = format_complex_lineform(&arr);
    CHECK_NON_NULL(str, "format_complex_lineform", &passed);
    if (str) {
        CHECK_COND(strstr(str, "1.00 + 1.00*i") != NULL, "нет первого коэффициента", &passed);
        CHECK_COND(strstr(str, "2.00*x1") != NULL, "нет второго коэффициента", &passed);
        CHECK_COND(strstr(str, "3.00*i*x2") != NULL, "нет третьего коэффициента", &passed);
        free(str);
    }
    complex_array.delete(&arr);
    if (passed) printf("  test_format_complex_lineform OK\n");
    return passed;
}

int test_parse_double_factor() {
    int passed = 1;
    clear_error();
    void* f = parse_double_factor("12.5");
    CHECK_COND(get_error() == NO_ERROR, "ошибка при 12.5", &passed);
    if (f) {
        CHECK_DOUBLE_EQ(12.5, *(double*)f, "parse_double_factor 12.5", &passed);
        free(f);
    }
    f = parse_double_factor("-3.14");
    CHECK_COND(get_error() == NO_ERROR, "ошибка при -3.14", &passed);
    if (f) {
        CHECK_DOUBLE_EQ(-3.14, *(double*)f, "parse_double_factor -3.14", &passed);
        free(f);
    }
    parse_double_factor("abc");
    CHECK_COND(get_error() != NO_ERROR, "abc не вызвал ошибку", &passed);
    clear_error();
    if (passed) printf("  test_parse_double_factor OK\n");
    return passed;
}

int test_parse_complex_factor() {
    int passed = 1;
    clear_error();
    void* f = parse_complex_factor("2+3*i");
    CHECK_COND(get_error() == NO_ERROR, "ошибка при 2+3i", &passed);
    if (f) {
        Complex expected = {2,3};
        CHECK_COMPLEX_EQ(expected, *(Complex*)f, "parse_complex_factor 2+3i", &passed);
        free(f);
    }
    f = parse_complex_factor("-5*i");
    CHECK_COND(get_error() == NO_ERROR, "ошибка при -5i", &passed);
    if (f) {
        Complex expected = {0,-5};
        CHECK_COMPLEX_EQ(expected, *(Complex*)f, "parse_complex_factor -5i", &passed);
        free(f);
    }
    parse_complex_factor("invalid");
    CHECK_COND(get_error() != NO_ERROR, "invalid не вызвал ошибку", &passed);
    clear_error();
    if (passed) printf("  test_parse_complex_factor OK\n");
    return passed;
}

int test_format_double_number() {
    int passed = 1;
    double val = 3.14159;
    char* s = format_double_number(&val);
    CHECK_NON_NULL(s, "format_double_number", &passed);
    if (s) {
        CHECK_COND(strstr(s, "3.14") != NULL, "неправильное форматирование", &passed);
        free(s);
    }
    if (passed) printf("  test_format_double_number OK\n");
    return passed;
}

int test_format_complex_number() {
    int passed = 1;
    Complex c = {1.23, 4.56};
    char* s = format_complex_number(&c);
    CHECK_NON_NULL(s, "format_complex_number", &passed);
    if (s) {
        CHECK_COND(strstr(s, "1.23") != NULL && strstr(s, "4.56") != NULL,
                   "неправильное форматирование комплексного числа", &passed);
        free(s);
    }
    if (passed) printf("  test_format_complex_number OK\n");
    return passed;
}

int test_double_array_utils() {
    int passed = 1;
    clear_error();
    dynamic_array arr = double_array.create(0);
    CHECK_COND(arr.data == NULL && arr.size == 0, "create(0) не создал пустой массив", &passed);
    double val = 1.5;
    double_array.append(&arr, &val);
    CHECK_COND(arr.size == 1 && double_eq(*(double*)arr.data, 1.5),
              "append не добавил элемент", &passed);
    val = 2.7;
    double_array.append(&arr, &val);
    CHECK_COND(arr.size == 2, "append не увеличил размер", &passed);
    double* data = (double*)arr.data;
    CHECK_COND(double_eq(data[0], 1.5) && double_eq(data[1], 2.7),
              "неправильные значения после append", &passed);
    val = 3.5;
    double_array.insertat(arr, &val, 0);
    CHECK_COND(data[0] == 3.5, "insertat не установил значение", &passed);
    data[0] = 4.3;
    double* tmp = double_array.get(arr, 0);
    CHECK_COND(*tmp == 4.3, "get вернул непрвильное значение", &passed);
    double_array.delete(&arr);
    CHECK_COND(arr.data == NULL && arr.size == 0, "delete не очистил массив", &passed);
    if (passed) printf("  test_double_array_utils OK\n");
    return passed;
}

int test_complex_array_utils() {
    int passed = 1;
    clear_error();
    dynamic_array arr = complex_array.create(0);
    CHECK_COND(arr.data == NULL && arr.size == 0, "create(0) не создал пустой массив", &passed);
    Complex c = {1,2};
    complex_array.append(&arr, &c);
    CHECK_COND(arr.size == 1 && complex_eq(*(Complex*)arr.data, c),
              "append не добавил комплексный элемент", &passed);
    c = (Complex){3,4};
    complex_array.append(&arr, &c);
    CHECK_COND(arr.size == 2, "append не увеличил размер", &passed);
    Complex* data = (Complex*)arr.data;
    CHECK_COND(complex_eq(data[0], (Complex){1,2}) && complex_eq(data[1], (Complex){3,4}),
              "неправильные комплексные значения после append", &passed);
    c = (Complex){5,6};
    complex_array.insertat(arr, &c, 0);
    CHECK_COND(complex_eq(data[0], (Complex){5,6}), "insert at установил неправильные комплексные значения", &passed);
    data[0] = (Complex){6,7};
    Complex* tmp = double_array.get(arr, 0);
    CHECK_COND(complex_eq(*tmp, (Complex){6,7}), "get вернул непрвильное комплексное значение", &passed);
    complex_array.delete(&arr);
    CHECK_COND(arr.data == NULL && arr.size == 0, "delete не очистил массив", &passed);
    if (passed) printf("  test_complex_array_utils OK\n");
    return passed;
}

int test_double_sum() {
    int passed = 1;
    dynamic_array a = double_array.create_and_init(3, 1.0, 2.0, 3.0);
    dynamic_array b = double_array.create_and_init(2, 10.0, 20.0);
    dynamic_array res = get_double_sum(a, b);
    CHECK_COND(res.size == 3, "неправильный размер результата суммы", &passed);
    double* data = (double*)res.data;
    double expected[] = {11.0, 22.0, 3.0};
    CHECK_COND(double_arr_eq(data, expected, 3), "неправильная сумма double", &passed);
    double_array.delete(&a);
    double_array.delete(&b);
    double_array.delete(&res);
    if (passed) printf("  test_double_sum OK\n");
    return passed;
}

int test_complex_sum() {
    int passed = 1;
    Complex exp[] = {{11,11},{22,22},{3,3}};
    dynamic_array a = complex_array.create_and_init(3, (Complex){1,1}, (Complex){2,2}, (Complex){3,3});
    dynamic_array b = complex_array.create_and_init(2, (Complex){10,10}, (Complex){20,20});
    dynamic_array res = get_complex_sum(a, b);
    CHECK_COND(res.size == 3, "неправильный размер результата суммы комплексов", &passed);
    Complex* data = (Complex*)res.data;
    CHECK_COND(complex_arr_eq(data, exp, 3), "неправильная сумма комплексных чисел", &passed);
    complex_array.delete(&a);
    complex_array.delete(&b);
    complex_array.delete(&res);
    if (passed) printf("  test_complex_sum OK\n");
    return passed;
}

int test_double_difference() {
    int passed = 1;
    dynamic_array a = double_array.create_and_init(3, 5.0, 4.0, 3.0);
    dynamic_array b = double_array.create_and_init(2, 1.0, 2.0);
    dynamic_array res = get_double_difference(a, b);
    CHECK_COND(res.size == 3, "неправильный размер разности", &passed);
    double* data = (double*)res.data;
    double expected[] = {4.0, 2.0, 3.0};
    CHECK_COND(double_arr_eq(data, expected, 3), "неправильная разность double", &passed);
    double_array.delete(&a);
    double_array.delete(&b);
    double_array.delete(&res);
    if (passed) printf("  test_double_difference OK\n");
    return passed;
}

int test_complex_difference() {
    int passed = 1;
    Complex exp[] = {{4,4},{2,2},{3,3}};
    dynamic_array a = complex_array.create_and_init(3, (Complex){5,5}, (Complex){4,4}, (Complex){3,3});
    dynamic_array b = complex_array.create_and_init(2, (Complex){1,1}, (Complex){2,2});
    dynamic_array res = get_complex_difference(a, b);
    CHECK_COND(res.size == 3, "неправильный размер результата разности", &passed);
    Complex* data = (Complex*)res.data;
    CHECK_COND(complex_arr_eq(data, exp, 3), "неправильная разность комплексных линейных форм", &passed);
    complex_array.delete(&a);
    complex_array.delete(&b);
    complex_array.delete(&res);
    if (passed) printf("  test_complex_difference OK\n");
    return passed;
}

int test_double_multiplication() {
    int passed = 1;
    dynamic_array a = double_array.create_and_init(3, 1.0, 2.0, 3.0);
    double factor = 2.5;
    dynamic_array res = get_double_multiplication(a, &factor);
    CHECK_COND(res.size == 3, "неправильный размер произведения действительной линейной формы", &passed);
    double* data = (double*)res.data;
    double expected[] = {2.5, 5.0, 7.5};
    CHECK_COND(double_arr_eq(data, expected, 3), "неправильное умножение действительной линейной формы", &passed);
    double_array.delete(&a);
    double_array.delete(&res);
    if (passed) printf("  test_double_multiplication OK\n");
    return passed;
}

int test_complex_multiplication() {
    int passed = 1;
    Complex exp[] = {{2,0},{0,2},{4,4}};
    dynamic_array a = complex_array.create_and_init(3, (Complex){1,0}, (Complex){0,1}, (Complex){2,2});
    Complex factor = {2,0};
    dynamic_array res = get_complex_multiplication(a, &factor);
    CHECK_COND(res.size == 3, "неправильный размер произведения комплексной линейной формы", &passed);
    Complex* data = (Complex*)res.data;
    CHECK_COND(complex_arr_eq(data, exp, 3), "неправильное умножение комплексных чисел", &passed);
    complex_array.delete(&a);
    complex_array.delete(&res);
    if (passed) printf("  test_complex_multiplication OK\n");
    return passed;
}

int test_double_calculation() {
    int passed = 1;
    dynamic_array coeffs = double_array.create_and_init(3, 1.0, 2.0, 3.0);
    dynamic_array vars = double_array.create_and_init(3, 2.0, 3.0, 4.0);
    void* res = get_double_calculation(coeffs, vars);
    CHECK_NON_NULL(res, "get_double_calculation", &passed);
    if (res) {
        CHECK_DOUBLE_EQ(20.0, *(double*)res, "double_calculation 1*2+2*3+3*4", &passed);
        free(res);
    }
    double_array.delete(&coeffs);
    double_array.delete(&vars);
    if (passed) printf("  test_double_calculation OK\n");
    return passed;
}

int test_complex_calculation() {
    int passed = 1;
    dynamic_array coeffs = complex_array.create_and_init(3, (Complex){1,0}, (Complex){2,0}, (Complex){3,0});
    dynamic_array vars = complex_array.create_and_init(3, (Complex){2,0}, (Complex){3,0}, (Complex){4,0});
    void* res = get_complex_calculation(coeffs, vars);
    CHECK_NON_NULL(res, "get_complex_calculation", &passed);
    if (res) {
        Complex val = *(Complex*)res;
        Complex expected = {20,0};
        CHECK_COMPLEX_EQ(expected, val, "complex_calculation", &passed);
        free(res);
    }
    complex_array.delete(&coeffs);
    complex_array.delete(&vars);
    if (passed) printf("  test_complex_calculation OK\n");
    return passed;
}

/* ---------- Главная функция запуска тестов ---------- */
int main() {
    printf("========== ЗАПУСК ТЕСТОВ ==========\n\n");

    int (*tests[])() = {
        test_add_complex,
        test_dif_complex,
        test_mul_complex,
        test_globals_error,
        test_check_double_number,
        test_check_complex_number,
        test_complex_to_string,
        test_parse_double_lineform,
        test_parse_complex_lineform,
        test_format_double_lineform,
        test_format_complex_lineform,
        test_parse_double_factor,
        test_parse_complex_factor,
        test_format_double_number,
        test_format_complex_number,
        test_double_array_utils,
        test_complex_array_utils,
        test_double_sum,
        test_complex_sum,
        test_double_difference,
        test_complex_difference,
        test_double_multiplication,
        test_complex_multiplication,
        test_double_calculation,
        test_complex_calculation
    };
    int num_tests = sizeof(tests) / sizeof(tests[0]);
    int passed = 0;

    for (int i = 0; i < num_tests; i++) {
        if (tests[i]()) {
            passed++;
        }
        printf("\n");
    }

    printf("========== РЕЗУЛЬТАТ ==========\n");
    printf("Пройдено тестов: %d из %d\n", passed, num_tests);
    return 0;
}