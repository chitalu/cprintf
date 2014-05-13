#ifndef CPF_TEST_COMMON_H
#define CPF_TEST_COMMON_H

#include "gtest/gtest.h"
#include "c_printf.h"

#define _CPF_OSTR_PRINT(pf_call) \
printf("\nORIGINAL\n@====|====@\n"); \
pf_call; \
printf("\nTRANSFORMED\n@=====|=====@\n");

#endif
