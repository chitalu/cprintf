#ifndef CPF_TEST_COMMON_H
#define CPF_TEST_COMMON_H

#include "gtest/gtest.h"
#include "c_printf.h"

/*
	note: cant used c_printf to test c_printf that is why
	printf is used.
*/
#define TEST_PERIMETER_EMPLACE_BEGIN \
auto pstr_start = R"pstr_s(\\
\\
================================================================\\
TEST START\\
================================================================\\
\\
)pstr_s";\
\
printf("%s", pstr_start);

#define TEST_PERIMETER_EMPLACE_END \
	auto pstr_end = R"pstr_e(\\
\\
================================================================\\
TEST END					\\
================================================================\\
\\
)pstr_e"; \
\
printf("%s", pstr_end);

#endif