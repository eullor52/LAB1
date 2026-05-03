#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "lineform.h"
#include "complex.h"

dynamic_array get_complex_sum(dynamic_array left, dynamic_array right);
dynamic_array get_double_sum(dynamic_array left, dynamic_array right);

dynamic_array get_complex_difference(dynamic_array left, dynamic_array right);
dynamic_array get_double_difference(dynamic_array left, dynamic_array right);

dynamic_array get_complex_multiplication(dynamic_array lineform, void* factor);
dynamic_array get_double_multiplication(dynamic_array lineform, void* factor);

void* get_complex_calculation(dynamic_array lineform, dynamic_array variables);
void* get_double_calculation(dynamic_array lineform, dynamic_array variables);

#endif 
