#include "io.h"
#include <stdio.h>

#define NUM_ERROR (Lnf){NULL, -1}
#define MEMORY_ERROR (Lnf){NULL, -2}
#define TYPE_ERROR (Lnf){NULL, -3}

void clean_input_buffer()
{
    int c;
    while(c = getchar() != '\n' && c != EOF);
}

int check_float(char* str)
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

C_info check_cmplx(char* str)
{
    C_info result, err = {0,0};
    int tmp, i = 0;

    if (*str == '-') ++i;

    tmp = check_float(str + i);

    if (tmp <= 0) return err;
    else i += tmp;

    if (str[i] == '-' || str[i] == '+')
    {
        ++i;
        tmp = check_float(str + i);

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

int cmplx_output(char* str, Complex num)
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

char* cmplx_lnf_output(Complex* lnf, unsigned size)
{
    char* buffer = malloc(sizeof(char) * (MAX_NUM_LEN * 2 + 3) * size);

    int  p = cmplx_output(buffer, lnf[0]);

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
            p += cmplx_output(buffer + p, lnf[i]);
            buffer[p++] = ')';
        }
        else p += cmplx_output(buffer + p, lnf[i]);

        p += sprintf(buffer + p, "*x%d", i);

        if (i < size - 1) p += sprintf(buffer + p, " + ");
    }

    buffer[p++] = '\0';
    return buffer;
}

char* flt_lnf_output(float* lnf, unsigned size)
{
    char* buffer = malloc(sizeof(char) * MAX_NUM_LEN * size);
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

char* output_lnf(void* lnf, unsigned size, char type)
{
    if (type == 'c')
    {
        Complex* c_lnf = (Complex*)lnf;
        return cmplx_lnf_output(c_lnf, size);
    }
    else if (type == 'f')
    {
        float* f_lnf = (float*)lnf;
        return flt_lnf_output(f_lnf, size);
    }
    else return NULL;
}

Lnf get_lnf(char* str, char type)
{
    Lnf result;

    if (type == 'c')
    {
        Complex* lnf = NULL;
        unsigned k = 0;

        int i = 0;
        while (str[i] != '\n' && str[i] != 0)
        {
            if (i) ++i;

            C_info tmp = check_cmplx(str + i);
            if (tmp.count == 0) 
            {
                if (lnf) free(lnf);
                return NUM_ERROR;
            }

            Complex* p = realloc(lnf, sizeof(Complex) * (k + 1));
            if (!p) 
            {
                if (lnf) free(lnf);
                return MEMORY_ERROR;
            }
            lnf = p;

            float im, real;

            if (tmp.count == 2)
            {
                if (sscanf(str + i, "%f%f*i", &real, &im) != 2) 
                {
                    if (lnf) free(lnf);
                    return NUM_ERROR;
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
                    return NUM_ERROR;
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
                    return NUM_ERROR;
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
            if (i) ++i;
            float tmp;
            int count = check_float(str+i);

            if (sscanf(str + i, "%f", &tmp) != 1) 
            {
                if (lnf) free(lnf);
                return NUM_ERROR;
            }

            float* p = realloc(lnf, sizeof(float) * (k+1));
            if (!p) 
            {
                if (lnf) free(lnf);
                return MEMORY_ERROR;
            }
            lnf = p;

            i += count;
            lnf[k] = tmp;
            ++k;
        }
        result.lnf = (void*)lnf;
        result.size = k;
    }
    else return TYPE_ERROR;
    return result;
}

void* get_factor(char* str, char type)
{
    if (type == 'c')
    {
        Complex* result = malloc(sizeof(Complex));
        result->Im = 0;
        result->Re = 0;

        C_info c = check_cmplx(str);

        if (c.count == 0) return NULL;

        if (c.count == 2) sscanf(str, "%f%f*i", &result->Re, &result->Im);
        else if (c.count == 1) sscanf(str, "%f", &result->Re);
        else sscanf(str, "%f*i", &result->Im);

        return (void*)result;
    }
    else if (type == 'f')
    {
        float* result = malloc(sizeof(float));
        
        if (check_float(str) <= 0) return NULL;

        sscanf(str, "%f", result);

        return (void*)result;
    }
    else return NULL;
}

char* output_num(void* num, char type)
{
    if (type == 'c')
    {
        char* result = malloc(sizeof(char)* (MAX_NUM_LEN * 2 + 5));

        if (!result) return NULL;

        Complex* c_num = (Complex*)num;

        int i = cmplx_output(result, *c_num);

        if ( !i ) return NULL;

        result[i] = 0;

        return result;
    }
    else if (type == 'f')
    {
        char* result = malloc(sizeof(char) * MAX_NUM_LEN);

        float* f_num = (float*)num;

        int i = sprintf(result, "%.2f", *f_num);

        if ( !i ) return NULL;

        result[i] = 0;

        return result;
    }
    else return NULL;
}