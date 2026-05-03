#include "complex.h"

Complex add_complex(Complex a, Complex b)
{
    Complex result;
    result.Re = a.Re + b.Re;
    result.Im = a.Im + b.Im;
    return result;
}

Complex dif_complex(Complex a, Complex b)
{
    Complex result;
    result.Re = a.Re - b.Re;
    result.Im = a.Im - b.Im;
    return result;
}

Complex mul_complex(Complex a, Complex b)
{
    Complex result;
    result.Re = a.Re * b.Re - a.Im * b.Im;
    result.Im = a.Re * b.Im + a.Im * b.Re;
    return result;
}
