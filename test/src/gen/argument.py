"""
Copyright (C) 2014 Floyd Mulenga Chitalu jnr									

Permission is hereby granted, free of charge, to obtain a copy					
of this software, to deal in the Software without restriction, including		
without limitation the rights to [use], [copy], [modify], [merge], [publish],	
[distribute], [sublicense], and/or [sell] copies of the Software, and to		
permit persons to whom the Software is furnished to do so, subject to			
the following conditions:														
																				
The above copyright notice and this permission notice shall be included in		
all copies or substantial portions of the Software.								
																				
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR		
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,		
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE		
AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM(S), DAMAGE(S) OR OTHER		
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,	
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN		
THE SOFTWARE.	

"""

import 	sys, os, itertools

DTYPE, FSPEC, INITVAL = tuple(range(0, 3))
print "DTYPE"

floats = ("float", "double", "long double")
chars = ("char", "wchar")
integrals = ["int", "long", "long long", "short"]
quals = ("signed", "unsigned")
integrals.extend([" ".join([q, i]) for q in quals for i in integrals if i != "wchar"])

char_ptrs = [(" ".join([q, c]) + "*") for q in quals for c in chars if i != "wchar"]
char_ptrs.extend([i+"*" for i in chars])
strings = ("std::string", "std::wstring") + tuple(char_ptrs)
types = (tuple(integrals) + floats + strings)
types = types + tuple(" ".join(["const", t]) for t in types)

print types

fspec_chars = ("c", )
fspec_floats = ("f", "g", "e", "a")
fspec_ints = ("d", "i")
fspec_uints = ("u", "o", "x")
fspec_strings = ("s", )

frmt_specs = fspec_chars + fspec_floats + fspec_ints + fspec_uints + fspec_strings

print frmt_specs

init_values = ("teststr", 101, 1.01)

def _gen_permutations():
	return itertools.product(types, frmt_specs, init_values)

def _discard_perm(perm):
	fpreds = (
		lambda e: e[DTYPE] in strings and e[FSPEC] not in fspec_strings,
		lambda e: e[DTYPE] in floats and e[FSPEC] not in fspec_floats,
		#signed integrals need not always be prefixed with "signed" i.e "signed int"
		lambda e: (e[DTYPE] in integrals and ("signed" in e[DTYPE] and "unsigned" not in e[DTYPE])) and e[FSPEC] not in fspec_ints,
		lambda e: (e[DTYPE] in integrals and "unsigned" in e[DTYPE]) and e[FSPEC] not in fspec_uints
		)

	if any([p(perm) for p in fpreds]):
		return True

	return False

def _generate():

	permutations = list()

	return tuple (filter( _discard_perm, _gen_permutations()))

file_header = """
#include <cprintf/cprintf.hpp>
#include <gtest/gtest.h>

#define TEST_ARG0(case_name_, type_, data_value_, fmt_spec_)\\
TEST(argument, case_name_)\\
{\\
	type_ data = data_value_;\\
	ASSERT_NO_THROW(cprintf("%" fmt_spec_ "\\n", data));\\
	ASSERT_NO_THROW(cfprintf(stdout, "%" fmt_spec_ "\\n", data));\\
	ASSERT_NO_THROW(cfprintf(stderr, "%" fmt_spec_ "\\n", data));\\
	ASSERT_NO_THROW(cwprintf(L"%" fmt_spec_ "\\n", data));\\
	ASSERT_NO_THROW(cwfprintf(stdout, L"%" fmt_spec_ "\\n", data));\\
	ASSERT_NO_THROW(cfwprintf(stderr, L"%" fmt_spec_ "\\n", data));\\
	std::tuple<type_> arg_tup = {data, data};\\
	ASSERT_NO_THROW(cprintf_t("%" fmt_spec_ " %" fmt_spec_ "\\n", arg_tup));\\
	ASSERT_NO_THROW(cfprintf_t(stdout, "%" fmt_spec_ " %" fmt_spec_ "\\n", arg_tup));\\
	ASSERT_NO_THROW(cfprintf_t(stderr, "%" fmt_spec_ " %" fmt_spec_ "\\n", arg_tup));\\
	ASSERT_NO_THROW(cwprintf_t(L"%" fmt_spec_ " %" fmt_spec_ "\\n", arg_tup);\\
	ASSERT_NO_THROW(cfwprintf_t(stdout, L"%" fmt_spec_ " %" fmt_spec_ "\\n", arg_tup));\\
	ASSERT_NO_THROW(cfwprintf_t(stderr, L"%" fmt_spec_ " %" fmt_spec_ "\\n", arg_tup));\\
}
"""

def make_tests():
	raw_vals = _generate()
	saved_stdout = sys.stdout

	sys.stdout = open("argument.cpp", "w")

	print(file_header);

	for test in raw_vals:
		
		instance_str = """TEST_ARG0({0}, {1}, {2}, {3});"""

		str_vars = ["".join(test[DTYPE].split()), str(test[INITVAL]), test[FSPEC]]
		case_name = "_".join(str_vars) 
		str_vars[0] = test[DTYPE]
		instance_str = instance_str.format(case_name, *str_vars)

		print(instance_str)

	sys.stdout.close()

	sys.stdout = saved_stdout

if __name__ == '__main__':
	make_tests()