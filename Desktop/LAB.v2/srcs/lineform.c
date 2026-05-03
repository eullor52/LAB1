#include "lineform.h"
#include "operations.h"
#include "globals.h"

dynamic_array create_double(size_t length) {
    dynamic_array new_array = {NULL, 0};
    if (length == 0) {
        return new_array;
    }
    void* tmp = malloc(sizeof(double)*length);
    if (!tmp) {
        set_error(MEMORY_ERROR);
        return new_array;
    }
    new_array.data = tmp;
    return new_array;
}

dynamic_array create_complex(size_t length) {
    dynamic_array new_array = {NULL, 0};
    if (length == 0) {
        return new_array;
    }
    void* tmp = malloc(sizeof(Complex)*length);
    if (!tmp) {
        set_error(MEMORY_ERROR);
        return new_array;
    }
    new_array.data = tmp;
    return new_array;
}

dynamic_array create_and_init_double(size_t length, ...) {
    dynamic_array new_array = {NULL, 0};
    if (length ==  0) {
        return new_array;
    }
    double* tmp = (double*)malloc(sizeof(double) * length);
    if (!tmp) {
        set_error(MEMORY_ERROR);
        return new_array;
    }
    va_list args;
    va_start(args, length);
    for (size_t i = 0; i < length; ++i) {
        tmp[i] = va_arg(args, double);
    }
    va_end(args);
    new_array.data = (void*)tmp;
    new_array.size = length;
    return new_array;
}

dynamic_array create_and_init_complex(size_t length, ...) {
    dynamic_array new_array = {NULL, 0};
    if (length ==  0) {
        return new_array;
    }
    Complex* tmp = (Complex*)malloc(sizeof(Complex) * length);
    if (!tmp) {
        set_error(MEMORY_ERROR);
        return new_array;
    }
    va_list args;
    va_start(args, length);
    for (size_t i = 0; i < length; ++i) {
        tmp[i] = va_arg(args, Complex);
    }
    va_end(args);
    new_array.data = (void*)tmp;
    new_array.size = length;
    return new_array;
}

void append_double(dynamic_array* array, void* item) {
    size_t new_size = array->size + 1;
    double* tmp = (double*)realloc(array->data, new_size * sizeof(double));
    if (!tmp) {
        set_error(MEMORY_ERROR);
        return;
    }
    tmp[new_size - 1] = *(double*)item;
    array->size = new_size;
    array->data = tmp;
}

void append_complex(dynamic_array* array, void* item) {
    size_t new_size = array->size + 1;
    Complex* tmp = (Complex*)realloc(array->data, new_size * sizeof(Complex));
    if (!tmp) {
        set_error(MEMORY_ERROR);
        return;
    }
    tmp[new_size - 1] = *((Complex*)item);
    array->size = new_size;
    array->data = tmp;
}

void delete(dynamic_array* array) {
    if (!array->data) {
        return;
    }
    free(array->data);
    array->data = NULL;
    array->size = 0;
}

void insertat_complex(dynamic_array array, void* item, size_t index) {
    if (index > array.size) {
        set_error(OUT_OF_RANGE);
        return;
    }
    Complex* tmp = (Complex*)array.data;
    Complex* elem = (Complex*)item;
    *(tmp + index) = *elem;
}

void insertat_double(dynamic_array array, void* item, size_t index) {
    if (index > array.size) {
        set_error(OUT_OF_RANGE);
        return;
    }
    double* tmp = (double*)array.data;
    double* elem = (double*)item;
    *(tmp + index) = *elem;
}

void* get_complex(dynamic_array array, size_t index) {
    Complex* tmp = (Complex*)array.data;
    return tmp + index;
}

void* get_double(dynamic_array array, size_t index) {
    double* tmp = (double*)array.data;
    return tmp + index;
}

Utils double_array = {
    .append = append_double,
    .create_and_init = create_and_init_double,
    .create = create_double,
    .delete = delete,
    .insertat = insertat_double,
    .get = get_double
};

Utils complex_array = {
    .append = append_complex,
    .create_and_init = create_and_init_complex,
    .create = create_complex,
    .delete = delete,
    .insertat = insertat_complex,
    .get = get_complex
};

Type_info double_operations = {
    .get_sum = get_double_sum,
    .get_difference = get_double_difference,
    .calculation = get_double_calculation,
    .multiplication = get_double_multiplication
};

Type_info complex_operations = {
    .get_sum = get_complex_sum,
    .get_difference = get_complex_difference,
    .calculation = get_complex_calculation,
    .multiplication = get_complex_multiplication
};