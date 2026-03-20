#ifndef COMPLEX_H
#define COMPLEX_H

typedef struct
{
    float Re;
    float Im;
}Complex;

extern error_treaker;

Complex add_cmplx(Complex a, Complex b);
Complex dif_cmplx(Complex a, Complex b);
Complex mul_cmplx(Complex a, Complex b);

#endif