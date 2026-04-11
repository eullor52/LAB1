#include <stdio.h>
#include "io.h"
#include "lineform.h"
#include <stdlib.h>
#include <conio.h>
#include "globals.h"


unsigned result_size(unsigned a, unsigned b);

char* get_input(int mode)
{
    if (mode == 1) puts("Коэффициенты через пробел:");
    else if (mode == 2) puts("Mножитель:");
    else if (mode == 3) puts("Значения переменных через пробел:");
    else 
    {
        set_error(INVAL_INPUT_TYPE);
        return NULL;
    }
    char* buffer = malloc(sizeof(char));
    int i = 0;
    buffer[i] = getchar();
    while (buffer[i] != '\n')
    {
        ++i;
        char* p = realloc(buffer, sizeof(char) * (i + 1));
        if (!p) 
        {
            set_error(MEMORY_ERROR);
            free(buffer);
            return NULL;
        }
        buffer = p;
        buffer[i] = getchar();
    }
    buffer[i] = '\0';

    return buffer;
}

int main()
{
    for(;;)
    {
        set_error(NO_ERROR);
        char choise = 0, type;
        puts("Выберите действие:\n   1) - Сложение\n   2) - Вычитание\n   3) - Умножение\n   4) - Вычисление");
        choise = getchar();
        clean_input_buffer();

        if (choise == '1')
        {
            puts("Тип значений:");
            char type = getchar();
            clean_input_buffer();

            char* str1 = get_input(4);
            if (!str1)
            {
                puts(error_massage());
                clean_input_buffer();
                continue;
            }

            char* str2 = get_input(1);
            if (!str2)
            {
                free(str1);
                puts(error_massage());
                clean_input_buffer();
                continue;
            }
            
            Lnf a = get_lnf(str1, type);
            if (!a.lnf)
            {
                map(free, 2, str1, str2);
                puts(error_massage());
                clean_input_buffer();
                continue;
            }

            Lnf b = get_lnf(str2, type);
            if (!b.lnf)
            {
                map(free, 3, str1, str2, a.lnf);
                puts(error_massage());
                clean_input_buffer();
                continue;
            }

            dif_add lnf = set_for_dif_add(a.lnf, b.lnf, type, a.size, b.size);

            if (get_error() != NO_ERROR)
            {
                map(free, 4, str1, str2, a.lnf, b.lnf);
                puts(error_massage());
                clean_input_buffer();
                continue;
            }

            void* result  = lnf.calculations.add_lnf(lnf.a, lnf.b, lnf.size_a, lnf.size_b);
            if (!result)
            {
                map(free, 4, str1, str2, a.lnf, b.lnf);
                puts(error_massage());
                clean_input_buffer();
                continue;
            }

            char* output = output_lnf(result, result_size(a.size, b.size), type);
            {
                map(free, 4, str1, str2, a.lnf, b.lnf, result);
                puts(error_massage());
                clean_input_buffer();
                continue;
            }

            printf("%s\n", output);

            map(free, 6, result, output, str1, str2, a.lnf, b.lnf);
        }
        else if (choise == '2')
        {
            puts("Тип значений:");
            char type = getchar();
            clean_input_buffer();

            char* str1 = get_input(1);
            char* str2 = get_input(1);

            Lnf a = get_lnf(str1, type);
            Lnf b = get_lnf(str2, type);

            dif_add lnf = set_for_dif_add(a.lnf, b.lnf, type, a.size, b.size);

            void* result  = lnf.calculations.dif_lnf(lnf.a, lnf.b, lnf.size_a, lnf.size_b);

            char* output = output_lnf(result, result_size(a.size, b.size), type);

            printf("%s\n", output);

            free(result);
            free(output);
            free(str1);
            free(str2);
            free(a.lnf);
            free(b.lnf);
        }
        else if (choise == '3')
        {
            puts("Тип значений:");
            char type = getchar();
            clean_input_buffer();

            char* str1 = get_input(1);
            char* str2 = get_input(2);
            
            Lnf a = get_lnf(str1, type);
            void* b = get_factor(str2, type);

            mul_calc lnf = set_for_mul_calc(a.lnf, b, type, a.size);

            void* result = lnf.calculations.mul_lnf(lnf.lnf, lnf.vrlbs, lnf.size);

            char* output = output_lnf(result, a.size, type);

            printf("%s\n", output);

            free(result);
            free(output);
            free(str1);
            free(str2);
            free(a.lnf);
            free(b);
        }
        else if (choise == '4')
        {
            puts("Тип значений:");
            char type = getchar();
            clean_input_buffer();

            char* str1 = get_input(1);
            char* str2 = get_input(3);

            Lnf a = get_lnf(str1, type);
            Lnf b = get_lnf(str2, type);

            mul_calc lnf = set_for_mul_calc(a.lnf, b.lnf, type, a.size);

            void* result = lnf.calculations.calc_lnf(lnf.lnf, lnf.vrlbs, lnf.size);

            char* output = output_num(result, type);

            printf("%s\n", output);

            free(result);
            free(output);
            free(str1);
            free(str2);
            free(a.lnf);
            free(b.lnf);
        }
        else
        {
            puts("Ошибка ввода!");
            continue;
        }
    }

}
