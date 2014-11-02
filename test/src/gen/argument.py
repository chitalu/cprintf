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

floats = ("float", "double")
chars = ("char", "wchar")
integrals = ["int", "long", "short"]
quals = ("signed", "unsigned")
integrals.extend(" ".join([q, i]) for q in quals for i in integrals if i != "wchar")

char_ptrs = [(" ".join([q, c]) + "*") for q in quals for c in chars if i != "wchar"]
char_ptrs.extend([i+"*" for i in chars])
strings = ("std::string", "std::wstring") + tuple(char_ptrs)
types = (tuple(integrals) + floats + strings)
types = types + tuple(" ".join(["const", t]) for t in types)

print types

def _gen_permutations():
	pass

def _generate():
	permutations = list()

	for p in _gen_permutations():
		pass

	return tuple(permutations)

def make_tests():
	pass

def _write(permutations):
	file_header = """
#include <cprintf/cprintf.hpp>
#include <gtest/gtest.h>

#define TEST_ARG0(case_name_, type_, data_value_, fmt_spec_)
TEST(argument, case_name_)\
{\
	type_ data = data_value_;\
	ASSERT_NO_THROW(cprintf("%" fmt_spec_ "\n", data));\
	ASSERT_NO_THROW(cfprintf(stdout, "%" fmt_spec_ "\n", data));\
	ASSERT_NO_THROW(cfprintf(stderr, "%" fmt_spec_ "\n", data));\
	ASSERT_NO_THROW(cwprintf(L"%" fmt_spec_ "\n", data));\
	ASSERT_NO_THROW(cwfprintf(stdout, L"%" fmt_spec_ "\n", data));\
	ASSERT_NO_THROW(cfwprintf(stderr, L"%" fmt_spec_ "\n", data));\
	std::tuple<type_> arg_tup = {data, data};\
	ASSERT_NO_THROW(cprintf_t("%" fmt_spec_ " %" fmt_spec_ "\n", arg_tup));\
	ASSERT_NO_THROW(cfprintf_t(stdout, "%" fmt_spec_ " %" fmt_spec_ "\n", arg_tup));\
	ASSERT_NO_THROW(cfprintf_t(stderr, "%" fmt_spec_ " %" fmt_spec_ "\n", arg_tup));\
	ASSERT_NO_THROW(cwprintf_t(L"%" fmt_spec_ " %" fmt_spec_ "\n", arg_tup);\
	ASSERT_NO_THROW(cfwprintf_t(stdout, L"%" fmt_spec_ " %" fmt_spec_ "\n", arg_tup));\
	ASSERT_NO_THROW(cfwprintf_t(stderr, L"%" fmt_spec_ " %" fmt_spec_ "\n", arg_tup));\
}
"""