#include "globals.h"
//#include "operations.h"
#include "lineform.h"

enum error error_info = NO_ERROR;

void set_error(enum error err_code)
{
    error_info = err_code;
}

enum error get_error()
{
    return error_info;
}

void clear_error() {
    set_error(NO_ERROR);
}