#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "globals.h"
#include "complex.h"

#define MAX_NUM_LEN 64

int check_double_number(const char* str) {
    int len = 0;
    const char* p = str;
    if (*p == '-' || *p == '+') {
        p++;
        len++; 
    }
    while (isdigit(*p)) {
        ++p;
        ++len;
    }
    if (*p == '.' || *p == ',') {
        ++p; 
        ++len;
        while (isdigit(*p)) {
            p++; len++;
        }
        return (len <= MAX_NUM_LEN) ? len : -1;
    }
    return (len <= MAX_NUM_LEN) ? len : -1;
}

C_info check_complex_number(const char* str) {
    C_info err = {0, 0, 0};
    int i = 0;
    if (str[0] == '-' || str[0] == '+') ++i;
    int tmp = check_double_number(str + i);
    if (tmp <= 0) return err;
    i += tmp;
    if (str[i] == '-' || str[i] == '+') {
        char sign = str[i];
        ++i;
        tmp = check_double_number(str + i);
        if (tmp <= 0) return err;
        i += tmp;
        if (str[i] == '*' && str[i+1] == 'i' && (!str[i+2] || str[i+2]==' ' || str[i+2]=='\n')) {
            return (C_info){i+2, 2, sign};
        }
        return err;
    } else if (str[i] == ' ' || str[i] == '\n' || str[i] == 0) {
        return (C_info){i, 1, 0};
    } else if (str[i] == '*' && str[i+1] == 'i' && (!str[i+2] || str[i+2]==' ' || str[i+2]=='\n')) {
        return (C_info){i+2, -1, str[0]};
    }
    return err;
}

int complex_to_string(char* buf, Complex c) {
    if (c.Re == 0 && c.Im == 0) return 0;
    if (c.Im != 0 && c.Re != 0) {
        return sprintf(buf, "%.2f %c %.2f*i", c.Re, (c.Im >= 0) ? '+' : '-', fabs(c.Im));
    }
    else if (c.Im == 0) {
        return sprintf(buf, "%.2f", c.Re);
    }
    else {
        return sprintf(buf, "%.2f*i", c.Im);
    }
}

dynamic_array parse_complex_lineform(const char* str) {
    dynamic_array err = {NULL, 0};
    dynamic_array result = complex_array.create(0);
    if (get_error() != NO_ERROR) return err;
    
    int i = 0;
    while (str[i] != '\0') {
        while (isspace(str[i])) i++;
        if (!str[i]) break;
        
        C_info info = check_complex_number(str + i);
        if (info.count == 0) {
            complex_array.delete(&result);
            set_error(INVAL_INPUT_STR);
            return err;
        }
        
        double real = 0, imag = 0;
        if (info.count == 2) {
            char sign;
            if (sscanf(str + i, "%lf%c%lf*i", &real, &sign, &imag) != 3) {
                complex_array.delete(&result);
                set_error(INVAL_INPUT_STR);
                return err;
            }
            if (sign == '-') imag = -imag;
        } else if (info.count == 1) {
            if (sscanf(str + i, "%lf", &real) != 1) {
                complex_array.delete(&result);
                set_error(INVAL_INPUT_STR);
                return err;
            }
        } else if (info.count == -1) {
            if (sscanf(str + i, "%lf*i", &imag) != 1) {
                complex_array.delete(&result);
                set_error(INVAL_INPUT_STR);
                return err;
            }
        }
        
        Complex c = {real, imag};
        complex_array.append(&result, &c);
        if (get_error() != NO_ERROR) {
            complex_array.delete(&result);
            return err;
        }
        i += info.len;
    }
    return result;
}

dynamic_array parse_double_lineform(const char* str) {
    dynamic_array err = {NULL, 0};
    dynamic_array result = double_array.create(0);
    if (get_error() != NO_ERROR) return err;
    
    int i = 0;
    while (str[i] != '\0') {
        while (isspace(str[i])) i++;
        if (!str[i]) break;
        
        int len = check_double_number(str + i);
        if (len <= 0) {
            double_array.delete(&result);
            set_error(INVAL_INPUT_STR);
            return err;
        }
        
        double val;
        if (sscanf(str + i, "%lf", &val) != 1) {
            double_array.delete(&result);
            set_error(INVAL_INPUT_STR);
            return err;
        }
        
        double_array.append(&result, &val);
        if (get_error() != NO_ERROR) {
            double_array.delete(&result);
            return err;
        }
        i += len;
    }
    return result;
}

char* format_complex_lineform(dynamic_array* arr) {
    if (!arr->data || arr->size == 0) {
        set_error(INVAL_LINEFORM_PARAMETES);
        return NULL;
    }
    Complex* coeffs = (Complex*)arr->data;
    size_t size = arr->size;
    char* buffer = malloc(sizeof(char) * (MAX_NUM_LEN * 2 + 10) * (size + 1));
    if (!buffer) {
        set_error(MEMORY_ERROR);
        return NULL;
    }
    int p = 0;
    if (coeffs[0].Re != 0 || coeffs[0].Im != 0)
        p += complex_to_string(buffer + p, coeffs[0]);
    for (size_t i = 1; i < size; i++) {
        if (coeffs[i].Re == 0 && coeffs[i].Im == 0) continue;
        if (p > 0) p += sprintf(buffer + p, " + ");
        if (coeffs[i].Re != 0 && coeffs[i].Im != 0) {
            buffer[p++] = '(';
            p += complex_to_string(buffer + p, coeffs[i]);
            buffer[p++] = ')';
        } else {
            p += complex_to_string(buffer + p, coeffs[i]);
        }
        p += sprintf(buffer + p, "*x%zu", i);
    }
    if (p == 0) p += sprintf(buffer + p, "0");
    buffer[p] = '\0';
    return buffer;
}

char* format_double_lineform(dynamic_array* arr) {
    if (!arr->data || arr->size == 0) {
        set_error(INVAL_LINEFORM_PARAMETES);
        return NULL;
    }
    double* coeffs = (double*)arr->data;
    size_t size = arr->size;
    char* buffer = malloc(sizeof(char) * MAX_NUM_LEN * (size + 1));
    if (!buffer) {
        set_error(MEMORY_ERROR);
        return NULL;
    }
    int p = 0;
    if (coeffs[0] != 0) p += sprintf(buffer + p, "%.2f", coeffs[0]);
    for (size_t i = 1; i < size; i++) {
        if (coeffs[i] == 0) continue;
        if (p > 0) p += sprintf(buffer + p, " + ");
        p += sprintf(buffer + p, "%.2f*x%zu", coeffs[i], i);
    }
    if (p == 0) p += sprintf(buffer + p, "0");
    buffer[p] = '\0';
    return buffer;
}

void* parse_complex_factor(const char* str) {
    Complex* res = malloc(sizeof(Complex));
    if (!res) {
        set_error(MEMORY_ERROR);
        return NULL;
    }
    C_info info = check_complex_number(str);
    if (info.count == 0) {
        free(res);
        set_error(INVAL_INPUT_STR);
        return NULL;
    }
    double real = 0, imag = 0;
    if (info.count == 2) {
        char sign;
        if (sscanf(str, "%lf%c%lf*i", &real, &sign, &imag) != 3) {
            free(res);
            set_error(INVAL_INPUT_STR);
            return NULL;
        }
        if (sign == '-') imag = -imag;
    } else if (info.count == 1) {
        if (sscanf(str, "%lf", &real) != 1) {
            free(res);
            set_error(INVAL_INPUT_STR);
            return NULL;
        }
    } else {
        if (sscanf(str, "%lf*i", &imag) != 1) {
            free(res);
            set_error(INVAL_INPUT_STR);
            return NULL;
        }
    }
    res->Re = real;
    res->Im = imag;
    return res;
}

void* parse_double_factor(const char* str) {
    void* void_res;
    double* res = malloc(sizeof(double));
    if (!res) {
        set_error(MEMORY_ERROR);
        return NULL;
    }
    if (check_double_number(str) <= 0) {
        free(res);
        set_error(INVAL_INPUT_STR);
        return NULL;
    }
    sscanf(str, "%lf", res);
    void_res = (void*)res;
    return void_res;
}

char* format_complex_number(void* num) {
    char* buf = malloc(MAX_NUM_LEN * 2 + 5);
    if (!buf) {
        set_error(MEMORY_ERROR);
        return NULL;
    }
    Complex* complex_num = (Complex*)num;
    int len = complex_to_string(buf, *complex_num);
    if (len == 0) {
        free(buf);
        set_error(OUTPUT_ERROR);
        return NULL;
    }
    buf[len] = '\0';
    return buf;
}

char* format_double_number(void* num) {
    char* buf = malloc(MAX_NUM_LEN);
    if (!buf) {
        set_error(MEMORY_ERROR);
        return NULL;
    }
    double* double_num = (double*)num;
    int len = sprintf(buf, "%.2f", *double_num);
    if (len == 0) {
        free(buf);
        set_error(OUTPUT_ERROR);
        return NULL;
    }
    buf[len] = '\0';
    return buf;
}

void clean_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

InputOutput complex_input_output = {
    .format_lineform = format_complex_lineform,
    .format_number = format_complex_number,
    .parse_lineform = parse_complex_lineform,
    .parse_factor = parse_complex_factor
};

InputOutput double_input_output = {
    .format_lineform = format_double_lineform,
    .format_number = format_double_number,
    .parse_lineform = parse_double_lineform,
    .parse_factor = parse_double_factor
};