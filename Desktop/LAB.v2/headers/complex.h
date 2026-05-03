#ifndef COMPLEX_H
#define COMPLEX_H

typedef struct
{
    double Re;
    double Im;
} Complex;

Complex add_complex(Complex a, Complex b);
Complex dif_complex(Complex a, Complex b);
Complex mul_complex(Complex a, Complex b);

#endif