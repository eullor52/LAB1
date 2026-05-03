#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/lineform.h"
#include "headers/io.h"
#include "headers/globals.h"
#include "headers/complex.h"


const char* error_message() {
    switch (get_error()) {
        case NO_ERROR:            return "Неизвестная ошибка.";
        case MEMORY_ERROR:        return "Ошибка выделения памяти.";
        case INVAL_TYPE:          return "Ошибка: неизвестный тип.";
        case INVAL_LINEFORM_PARAMETES: return "Ошибка: неверные параметры линейной формы.";
        case INVAL_INPUT_STR:     return "Ошибка: неверная строка ввода.";
        case OUTPUT_ERROR:        return "Ошибка в работе функций stdio.";
        case INVAL_INPUT_TYPE:    return "Неизвестный формат ввода.";
        default:                  return NULL;
    }
}

void print_error() {
    const char* msg = error_message();
    if (msg) printf("Ошибка: %s\n", msg);
    else printf("Неизвестная ошибка\n");
}

dynamic_array read_lineform(char type, const char* prompt) {
    dynamic_array result = {NULL, 0};
    char input[1024];
    while (1) {
        printf("%s", prompt);
        if (!fgets(input, sizeof(input), stdin)) {
            printf("Ошибка ввода, повторите.\n");
            continue;
        }
        input[strcspn(input, "\n")] = '\0';
        clear_error();
        if (type == 'c')
            result = complex_input_output.parse_lineform(input);
        else
            result = double_input_output.parse_lineform(input);
        if (get_error() == NO_ERROR && result.size > 0)
            return result;
        print_error();
        if (get_error() == MEMORY_ERROR) exit(1);
        clear_error();
        printf("Введите корректные коэффициенты.\n");
    }
}

void* read_factor(char type, const char* prompt) {
    char fact_str[256];
    while (1) {
        printf("%s", prompt);
        if (!fgets(fact_str, sizeof(fact_str), stdin)) {
            printf("Ошибка ввода, повторите.\n");
            continue;
        }
        fact_str[strcspn(fact_str, "\n")] = '\0';
        clear_error();
        void* factor = (type == 'c') ? complex_input_output.parse_factor(fact_str)
                                     : double_input_output.parse_factor(fact_str);
        if (get_error() == NO_ERROR && factor != NULL)
            return factor;
        print_error();
        if (get_error() == MEMORY_ERROR) exit(1);
        clear_error();
        printf("Введите корректный скаляр.\n");
    }
}

dynamic_array read_variables(char type, size_t expected_size) {
    dynamic_array vars = {NULL, 0};
    char var_buf[1024];
    while (1) {
        printf("Введите значения переменных (через пробел, количество = %zu): ", expected_size);
        if (!fgets(var_buf, sizeof(var_buf), stdin)) {
            printf("Ошибка ввода, повторите.\n");
            continue;
        }
        var_buf[strcspn(var_buf, "\n")] = '\0';
        clear_error();
        if (type == 'c')
            vars = complex_input_output.parse_lineform(var_buf);
        else
            vars = double_input_output.parse_lineform(var_buf);
        if (get_error() != NO_ERROR) {
            print_error();
            if (get_error() == MEMORY_ERROR) exit(1);
            clear_error();
            printf("Повторите ввод переменных.\n");
            continue;
        }
        if (vars.size == expected_size)
            return vars;
        printf("Ошибка: количество переменных (%zu) не совпадает с числом коэффициентов (%zu)\n",
               vars.size, expected_size);
        (type == 'c' ? complex_array.delete(&vars) : double_array.delete(&vars));
        printf("Повторите ввод переменных.\n");
    }
}

int main() {
    char type;
    printf("Выберите тип линейной формы (c - комплексная, f - вещественная): ");
    while (1) {
        if (scanf(" %c", &type) != 1) {
            printf("Неверный ввод. Повторите: ");
            clean_input_buffer();
            continue;
        }
        clean_input_buffer();
        if (type == 'c' || type == 'f') break;
        printf("Неверный тип. Повторите (c - комплексная, f - вещественная): ");
    }

    dynamic_array coeffs = read_lineform(type, "Введите коэффициенты (числа через пробел):\n");

    char* form_str = (type == 'c') ? complex_input_output.format_lineform(&coeffs)
                                   : double_input_output.format_lineform(&coeffs);
    if (form_str) {
        printf("Линейная форма: %s\n", form_str);
        free(form_str);
    }

    int choice;
    while (1) {
        printf("\n--- МЕНЮ ---\n");
        printf("1. Сложить с другой линейной формой\n");
        printf("2. Вычесть другую линейную форму\n");
        printf("3. Умножить на скаляр\n");
        printf("4. Вычислить значение (подставить переменные)\n");
        printf("5. Ввести новую линейную форму\n");
        printf("0. Выход\n");
        printf("Ваш выбор: ");
        if (scanf("%d", &choice) != 1) {
            printf("Неверный ввод. Введите число.\n");
            clean_input_buffer();
            continue;
        }
        clean_input_buffer();

        if (choice == 0) break;

        if (choice < 1 || choice > 5) {
            printf("Неверный номер. Повторите.\n");
            continue;
        }

        if (choice == 1 || choice == 2) {
            dynamic_array other = read_lineform(type, "Введите вторую линейную форму (коэффициенты): ");
            dynamic_array res = (type == 'c') ?
                (choice == 1 ? complex_operations.get_sum(coeffs, other)
                             : complex_operations.get_difference(coeffs, other))
              : (choice == 1 ? double_operations.get_sum(coeffs, other)
                             : double_operations.get_difference(coeffs, other));
            if (get_error() != NO_ERROR) {
                print_error();
                if (get_error() == MEMORY_ERROR) exit(1);
                clear_error();
                (type == 'c' ? complex_array.delete(&other) : double_array.delete(&other));
                continue;
            }
            char* out = (type == 'c') ? complex_input_output.format_lineform(&res)
                                      : double_input_output.format_lineform(&res);
            if (out) {
                printf("Результат: %s\n", out);
                free(out);
            } else print_error();
            (type == 'c' ? complex_array.delete(&res) : double_array.delete(&res));
            (type == 'c' ? complex_array.delete(&other) : double_array.delete(&other));
            clear_error();
        }
        else if (choice == 3) {
            void* factor = read_factor(type, "Введите скаляр: ");
            dynamic_array res = (type == 'c') ? complex_operations.multiplication(coeffs, factor)
                                              : double_operations.multiplication(coeffs, factor);
            free(factor);
            if (get_error() != NO_ERROR) {
                print_error();
                if (get_error() == MEMORY_ERROR) exit(1);
                clear_error();
                continue;
            }
            char* out = (type == 'c') ? complex_input_output.format_lineform(&res)
                                      : double_input_output.format_lineform(&res);
            if (out) {
                printf("Результат: %s\n", out);
                free(out);
            } else print_error();
            (type == 'c' ? complex_array.delete(&res) : double_array.delete(&res));
            clear_error();
        }
        else if (choice == 4) {
            dynamic_array vars = read_variables(type, coeffs.size);
            void* result = (type == 'c') ? complex_operations.calculation(coeffs, vars)
                                         : double_operations.calculation(coeffs, vars);
            if (get_error() != NO_ERROR) {
                print_error();
                if (get_error() == MEMORY_ERROR) exit(1);
                clear_error();
                (type == 'c' ? complex_array.delete(&vars) : double_array.delete(&vars));
                continue;
            }
            char* res_str = (type == 'c') ? complex_input_output.format_number(result)
                                          : double_input_output.format_number(result);
            if (res_str) {
                printf("Значение линейной формы = %s\n", res_str);
                free(res_str);
            } else print_error();
            free(result);
            (type == 'c' ? complex_array.delete(&vars) : double_array.delete(&vars));
            clear_error();
        }
        else if (choice == 5) {
            dynamic_array new_coeffs = read_lineform(type, "Введите новые коэффициенты: ");
            (type == 'c' ? complex_array.delete(&coeffs) : double_array.delete(&coeffs));
            coeffs = new_coeffs;
            char* new_str = (type == 'c') ? complex_input_output.format_lineform(&coeffs)
                                          : double_input_output.format_lineform(&coeffs);
            if (new_str) {
                printf("Новая линейная форма: %s\n", new_str);
                free(new_str);
            } else print_error();
        }
    }

    (type == 'c' ? complex_array.delete(&coeffs) : double_array.delete(&coeffs));
    printf("Выполнение завершено.\n");
    return 0;
}