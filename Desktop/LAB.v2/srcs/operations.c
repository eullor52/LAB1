#include "operations.h"
#include "complex.h"
#include <stdlib.h>
#include "globals.h"
#include "lineform.h"

dynamic_array get_complex_sum(dynamic_array left, dynamic_array right) {
    dynamic_array error = {NULL, 0};
    if (!(left.data && right.data)) {
        set_error(INVAL_LINEFORM_PARAMETES);
        return error;
    }
    dynamic_array result = complex_array.create(0);
    IS_ALRIGHT(error);
    Complex *a = (Complex*)left.data, *b = (Complex*)right.data;
    size_t min = left.size < right.size ? left.size : right.size;
    for (size_t i = 0; i < min; ++i) {
        Complex sum = add_complex(a[i], b[i]);
        complex_array.append(&result, &sum);
    }
    if (left.size > right.size) {
        for (size_t i = min; i < left.size; ++i)
            complex_array.append(&result, &a[i]);
    } else if (right.size > left.size) {
        for (size_t i = min; i < right.size; ++i)
            complex_array.append(&result, &b[i]);
    }
    return result;
}

dynamic_array get_double_sum(dynamic_array left, dynamic_array right) {
    dynamic_array error = {NULL, 0};
    if (!(left.data && right.data)) {
        set_error(INVAL_LINEFORM_PARAMETES);
        return error;
    }
    dynamic_array result = double_array.create(0);
    IS_ALRIGHT(error);
    double *a = (double*)left.data, *b = (double*)right.data;
    size_t min = left.size < right.size ? left.size : right.size;
    for (size_t i = 0; i < min; ++i) {
        double sum = a[i] + b[i];
        double_array.append(&result, &sum);
    }
    if (left.size > right.size) {
        for (size_t i = min; i < left.size; ++i)
            double_array.append(&result, &a[i]);
    } else if (right.size > left.size) {
        for (size_t i = min; i < right.size; ++i)
            double_array.append(&result, &b[i]);
    }
    return result;
}

dynamic_array get_complex_difference(dynamic_array left, dynamic_array right) {
    dynamic_array error = {NULL, 0};
    if (!(left.data && right.data)) {
        set_error(INVAL_LINEFORM_PARAMETES);
        return error;
    }
    dynamic_array result = complex_array.create(0);
    IS_ALRIGHT(error);
    Complex *a = (Complex*)left.data, *b = (Complex*)right.data;
    size_t min = left.size < right.size ? left.size : right.size;
    for (size_t i = 0; i < min; ++i) {
        Complex diff = dif_complex(a[i], b[i]);
        complex_array.append(&result, &diff);
    }
    if (left.size > right.size) {
        for (size_t i = min; i < left.size; ++i)
            complex_array.append(&result, &a[i]);
    } else if (right.size > left.size) {
        Complex zero = {0, 0};
        for (size_t i = min; i < right.size; ++i) {
            Complex neg = dif_complex(zero, b[i]);
            complex_array.append(&result, &neg);
        }
    }
    return result;
}

dynamic_array get_double_difference(dynamic_array left, dynamic_array right) {
    dynamic_array error = {NULL, 0};
    if (!(left.data && right.data)) {
        set_error(INVAL_LINEFORM_PARAMETES);
        return error;
    }
    dynamic_array result = double_array.create(0);
    IS_ALRIGHT(error);
    double *a = (double*)left.data, *b = (double*)right.data;
    size_t min = left.size < right.size ? left.size : right.size;
    for (size_t i = 0; i < min; ++i) {
        double diff = a[i] - b[i];
        double_array.append(&result, &diff);
    }
    if (left.size > right.size) {
        for (size_t i = min; i < left.size; ++i)
            double_array.append(&result, &a[i]);
    } else if (right.size > left.size) {
        for (size_t i = min; i < right.size; ++i) {
            double neg = -b[i];
            double_array.append(&result, &neg);
        }
    }
    return result;
}

dynamic_array get_complex_multiplication(dynamic_array lineform, void* factor) {
    dynamic_array error = {NULL, 0};
    if (!lineform.data || !factor) {
        set_error(INVAL_LINEFORM_PARAMETES);
        return error;
    }
    dynamic_array result = complex_array.create(lineform.size);
    IS_ALRIGHT(error);
    Complex *coeffs = (Complex*)lineform.data;
    Complex f = *(Complex*)factor;
    for (size_t i = 0; i < lineform.size; ++i) {
        Complex prod = mul_complex(coeffs[i], f);
        complex_array.append(&result, &prod);
    }
    return result;
}

dynamic_array get_double_multiplication(dynamic_array lineform, void* factor) {
    dynamic_array error = {NULL, 0};
    if (!lineform.data || !factor) {
        set_error(INVAL_LINEFORM_PARAMETES);
        return error;
    }
    dynamic_array result = double_array.create(lineform.size);
    IS_ALRIGHT(error);
    double *coeffs = (double*)lineform.data;
    double f = *(double*)factor;
    for (size_t i = 0; i < lineform.size; ++i) {
        double prod = coeffs[i] * f;
        double_array.append(&result, &prod);
    }
    return result;
}

void* get_complex_calculation(dynamic_array lineform, dynamic_array variables) {
    if (!lineform.data || !variables.data || lineform.size != variables.size) {
        set_error(INVAL_LINEFORM_PARAMETES);
        return NULL;
    }
    Complex *coeffs = (Complex*)lineform.data;
    Complex *vars = (Complex*)variables.data;
    Complex result = {0, 0};
    for (size_t i = 0; i < lineform.size; ++i) {
        Complex term = mul_complex(coeffs[i], vars[i]);
        result = add_complex(result, term);
    }
    Complex *out = malloc(sizeof(Complex));
    if (!out) {
        set_error(MEMORY_ERROR);
        return NULL;
    }
    *out = result;
    return out;
}

void* get_double_calculation(dynamic_array lineform, dynamic_array variables) {
    if (!lineform.data || !variables.data || lineform.size != variables.size) {
        set_error(INVAL_LINEFORM_PARAMETES);
        return NULL;
    }
    double *coeffs = (double*)lineform.data;
    double *vars = (double*)variables.data;
    double result = 0;
    for (size_t i = 0; i < lineform.size; ++i) {
        result += coeffs[i] * vars[i];
    }
    double *out = malloc(sizeof(double));
    if (!out) {
        set_error(MEMORY_ERROR);
        return NULL;
    }
    *out = result;
    return out;
}