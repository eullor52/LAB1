#include "io.h"
#include <stdio.h>
#include "globals.h"

void map(void func(void*), int count, ...)
{
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) func(va_arg(args, void*));

    va_end(args);
}

void clean_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int check_float_number(char* str)
{
    int num_len = 0;
    int i = 0;

    while (str[i] > 47 && str[i] < 58) 
    {
        ++i; 
        ++num_len;
    }
    
    if (str[i] == ',' || str[i] == '.')
    {
        ++i;
        ++num_len;

        while(str[i] > 47 && str[i] < 58)
        {
            ++i;
            ++num_len;
        }

        if (num_len <= MAX_NUM_LEN) return num_len;
        else return -1;
    }
    else if (num_len <= MAX_NUM_LEN) return num_len;
    else return -1;
}

C_info check_complex_number(char* str)
{
    C_info result, err = {0,0};
    int tmp, i = 0;

    if (*str == '-') ++i;

    tmp = check_float_number(str + i);

    if (tmp <= 0) return err;
    else i += tmp;

    if (str[i] == '-' || str[i] == '+')
    {
        ++i;
        tmp = check_float_number(str + i);

        if(tmp <= 0) return err;
        else i += tmp;
        
        if (str[i] == '*' && str[i + 1] == 'i' && (str[i + 2] == 0 || str[i + 2] == ' ' || str[i + 2] == '\n'))
        {
            result.len = i + 2;
            result.count = 2;
            return result;
        }
        else return err;
    }
    else if (str[i] == ' ' || str[i] == '\n' || str[i] == 0)
    {
        result.len = i;
        result.count = 1;
        return result;
    }
    else if (str[i] == '*' && str[i + 1] == 'i' && (str[i + 2] == ' ' || str[i + 2] == '\n' || str[i + 2] == 0))
    {
        result.len = i + 2;
        result.count = -1;
        return result;
    }
    else return err;
}

int complex_number_output(char* str, Complex num)
{
    int i;

    if (num.Re == 0 && num.Im == 0) return 0;

    if (num.Im != 0 && num.Re != 0)
    {
        i = sprintf(str, "%.2f + %.2f*i", num.Re, num.Im);

        return i;
    }
    else if (num.Im == 0)
    {
        i = sprintf(str, "%.2f", num.Re);

        return i;
    }
    else
    {
        i = sprintf(str, "%.2f*i", num.Im);

        return i;
    }
}

char* complex_lineform_output(Complex* lnf, unsigned size)
{
    char* buffer = malloc(sizeof(char) * (MAX_NUM_LEN * 2 + 3) * size);

    if(!buffer) return NULL;

    int  p = complex_number_output(buffer, lnf[0]);

    if (p != 0)
    {
        sprintf(buffer + p, "%s", " + ");
        p += 3;
    }

    for (int i = 1; i < size; i++)
    {
        if (lnf[i].Re == 0 && lnf[i].Im == 0) continue;

        if (lnf[i].Re != 0 && lnf[i].Im != 0)
        {
            buffer[p++] = '(';
            p += complex_number_output(buffer + p, lnf[i]);
            buffer[p++] = ')';
        }
        else p += complex_number_output(buffer + p, lnf[i]);

        p += sprintf(buffer + p, "*x%d", i);

        if (i < size - 1) p += sprintf(buffer + p, " + ");
    }

    buffer[p++] = '\0';
    return buffer;
}

char* float_lineform_output(float* lnf, unsigned size)
{
    char* buffer = malloc(sizeof(char) * MAX_NUM_LEN * size);

    if (!buffer)  return NULL;

    char tmp[100];
    int p = 0;

    if (lnf[0] != 0) 
    {
        sprintf(tmp, "%.2f", lnf[0]);

        for (int i = 0; tmp[i] != '\0'; i++)
        {
            buffer[p] = tmp[i];
            ++p;
        }
        buffer[p++] = ' ';
        buffer[p++] = '+';
        buffer[p++] = ' ';
    }

    for (int i = 1; i < size; i++)
    {
        char vrbl[10];
        
        sprintf(tmp, "%.2f", lnf[i]);
        
        for (int k = 0; tmp[k] != '\0'; k++)
        {
            buffer[p++] = tmp[k];
        }

        buffer[p++] = '*';
        buffer[p++] = 'x';

        sprintf(vrbl, "%d", i);

        for (int x = 0; vrbl[x] != '\0'; x++)
        {
            buffer[p++] = vrbl[x];
        }
        if (i != size - 1)
        {
            buffer[p++] = ' ';
            buffer[p++] = '+';
            buffer[p++] = ' ';
        }
    }
    buffer[p++] = '\0';
    return buffer;
}

char* lineform_output(void* lnf, unsigned size, char type)
{
    if (type == 'c')
    {
        Complex* c_lnf = (Complex*)lnf;
        char* result = complex_lineform_output(c_lnf, size);
        if (!result) 
        {
            set_error(MEMORY_ERROR);
            return NULL;
        }
        else return result;

    }
    else if (type == 'f')
    {
        float* f_lnf = (float*)lnf;
        char* result = float_lineform_output(f_lnf, size);  
        if (!result)
        {
            set_error(MEMORY_ERROR);
            return NULL;
        }
        else return result;
    }
    else
    {
        set_error(INVAL_TYPE);
        return NULL;
    }
}

Lnf get_lineform(char* str, char type)
{
    Lnf result;

    if (type == 'c')
    {
        Complex* lnf = NULL;
        unsigned k = 0;

        int i = 0;
        while (str[i] != '\n' && str[i] != 0)
        {
            while (str[i] == ' ') ++i;

            C_info tmp = check_complex_number(str + i);
            if (tmp.count == 0) 
            {
                if (lnf) free(lnf);
                set_error(INVAL_INPUT_STR);
                return ERRO;
            }

            Complex* p = realloc(lnf, sizeof(Complex) * (k + 1));
            if (!p) 
            {
                if (lnf) free(lnf);
                set_error(MEMORY_ERROR);
                return ERRO;
            }
            lnf = p;

            float im, real;

            if (tmp.count == 2)
            {
                if (sscanf(str + i, "%f%f*i", &real, &im) != 2) 
                {
                    if (lnf) free(lnf);
                    set_error(INVAL_INPUT_STR);
                    return ERRO;
                }
                lnf[k].Im = im;
                lnf[k].Re = real;
                ++k;
            }
            else if(tmp.count == 1)
            {
                if (sscanf(str + i, "%f", &real) != 1) 
                {
                    if (lnf) free(lnf);
                    set_error(INVAL_INPUT_STR);
                    return ERRO;
                }
                lnf[k].Im = 0;
                lnf[k].Re = real;
                ++k;
            }
            else if(tmp.count == -1)
            {
                if (sscanf(str + i, "%f*i", &im) != 1)
                {
                    if (lnf) free(lnf);
                    set_error(INVAL_INPUT_STR);
                    return ERRO;
                }
                lnf[k].Im = im;
                lnf[k].Re = 0;
                ++k;
            }
            i += tmp.len;
        }

        result.lnf = (void*)lnf;
        result.size = k;
    }
    else if (type == 'f')
    {
        float* lnf = NULL;
        int i = 0;
        unsigned k = 0;

        while (str[i] != '\n' && str[i] != 0)
        {
            while (str[i] == ' ') ++i;
            float tmp;
            int count = check_float_number(str+i);

            if (sscanf(str + i, "%f", &tmp) != 1) 
            {
                set_error(INVAL_INPUT_STR);
                return ERRO;
            }

            float* p = realloc(lnf, sizeof(float) * (k+1));
            if (!p) 
            {
                if (lnf) free(lnf);
                set_error(MEMORY_ERROR);
                return ERRO;
            }
            lnf = p;

            i += count;
            lnf[k] = tmp;
            ++k;
        }
        result.lnf = (void*)lnf;
        result.size = k;
    }
    else
    {
        set_error(INVAL_TYPE);
        return ERRO;
    }
    return result;
}

void* get_factor(char* str, char type)
{
    if (type == 'c')
    {
        Complex* result = malloc(sizeof(Complex));
        if (!result)
        {
            set_error(MEMORY_ERROR);
            return NULL;
        }

        result->Im = 0;
        result->Re = 0;

        C_info c = check_complex_number(str);

        if (c.count == 0)
        {
            free(result);
            set_error(INVAL_INPUT_STR);
            return NULL;
        }

        if (c.count == 2) sscanf(str, "%f%f*i", &result->Re, &result->Im);
        else if (c.count == 1) sscanf(str, "%f", &result->Re);
        else sscanf(str, "%f*i", &result->Im);

        return (void*)result;
    }
    else if (type == 'f')
    {
        float* result = malloc(sizeof(float));
        
        if (check_float_number(str) <= 0)
        {
            set_error(INVAL_INPUT_STR);
            return NULL;
        }

        sscanf(str, "%f", result);

        return (void*)result;
    }
    else return NULL;
}

char* number_output(void* num, char type)
{
    if (type == 'c')
    {
        char* result = malloc(sizeof(char)* (MAX_NUM_LEN * 2 + 5));

        if (!result) 
        {
            set_error(MEMORY_ERROR);
            return NULL;
        }

        Complex* c_num = (Complex*)num;

        int i = complex_number_output(result, *c_num);

        if ( !i ) 
        {
            set_error(OUTPUT_ERROR);
            free(result);
            return NULL;
        }
        result[i] = 0;

        return result;
    }
    else if (type == 'f')
    {
        char* result = malloc(sizeof(char) * MAX_NUM_LEN);

        if (!result) 
        {
            set_error(MEMORY_ERROR);
            return NULL;
        }

        float* f_num = (float*)num;

        int i = sprintf(result, "%.2f", *f_num);

        if ( !i ) 
        {
            set_error(OUTPUT_ERROR);
            free(result);
            return NULL;
        }
        result[i] = 0;

        return result;
    }
    else 
    {
        set_error(INVAL_TYPE);
        return NULL;
    }
}

char* error_massage()
{
    enum error err_code = get_error();
    
    switch(err_code)
    {
        case NO_ERROR:
            return "Неизвестная ошибка.";
        case MEMORY_ERROR:
            return "Ошибка выделения памяти.";
        case INVAL_TYPE:
            return "Ошибка: неизвестный тип.";
        case INVAL_LNF_PARAM:
            return "Ошибка: неверные параметры.";
        case INVAL_INPUT_STR:
            return "Ошибка: неверная строка ввода.";
        case OUTPUT_ERROR:
            return "Ошибка в работе функций stdio.";
        case INVAL_INPUT_TYPE:
            return "Неизвестный формат вввода.";
        default:
            return NULL;
    }
}