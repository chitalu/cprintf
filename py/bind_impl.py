import sys
import os

# names for some object types that are used by the standard 
# Python interpreter, but not for the types defined by 
# various extension modules
import types
import ctypes

_CPF_STDO = 0x1
_CPF_STDE = 0X2

_STREAM = 0
_FORMAT = 1

MIN_ARGS_COUNT = 2
MAX_ARGS_COUNT = 6

# given a particular variable this function returns the variable's ctypes' 
# equivalent type for caller to correctly convert arguments before invoking
# "C" shared library's symbols
def _typename(var):

    if isinstance(var, types.IntType): return ctypes.c_int32
    elif isinstance(var, types.LongType): return ctypes.c_long
    elif isinstance(var, types.BooleanType): return ctypes.c_bool
    elif isinstance(var, types.FloatType): return ctypes.c_float
    elif isinstance(var, types.StringType): return ctypes.c_char_p
    elif isinstance(var, types.UnicodeType): return ctypes.c_wchar_p
    elif isinstance(var, sys.stdout): return ctypes.c_uint8
    elif isinstance(var, sys.stderr): return ctypes.c_uint8
    
    raise ValueError("CPF-RT-ERR: Invalid Type")


def promote_fn_args(*args):
    return (_typename(a)(a) for a in args) 

for i in promote_fn_args(*(2, 4, "ssss")):
	print i
	print type(i)

def resolve_lib_fn_name(args):
    frmt_type_str_id = "ws" if type(args[_FORMAT]) is ctypes.c_wchar_p else "s"
    fn_name = "_cprintf_" #+ frmt_type_str_id

    modified_typenames = [type(a).__name__[2:] for a in args]
    modified_typenames = map(lambda e: e.replace("_", ""), modified_typenames)

    fn_name = fn_name + "_".join(modified_typenames)

    print fn_name

    # function args list
    argdecl = "{0} arg{1}"
    argtypes = [(type(a).__name__[2:]) for a in args]

    def replace_frmt_str_type_repr(v):
        if("wchar_p" in v):
            v = "const wchar_t*"    
        elif ("char_p" in v):
            v = "const char*"
        return v

    argtypes = map(replace_frmt_str_type_repr, argtypes)
    argtypes = ["{0} x{1}".format(v, str(k)) for k, v in enumerate(argtypes)]
    argtypes = ", ".join(argtypes)

    print fn_name + "(" + argtypes + ")"


myargs = tuple(promote_fn_args(*(_CPF_STDE, 2, 4.4, "ssss", u"ask")))
resolve_lib_fn_name(myargs)


import itertools
import string
import time

fname_we = "py_cprintf"
generated_hfile = fname_we + ".h"
generated_cppfile = fname_we + ".cpp"

hfile = string.Template("""
#pragma once
#ifndef __PY_CPRINTF_H__
#define __PY_CPRINTF_H__

#include <cprintf/cprintf.hpp>

// THIS FILE IS GENERATED!
// $timestamp

extern "C" {

$protocals
};

#endif // __PY_CPRINTF_H__
""")

cppfile = string.Template("""
#include <$hfile_name>

// THIS FILE IS GENERATED!
// $timestamp

$source

""")

fn_body = string.Template("""
$signature{
    if (x0 == CPF_STDO) {
        return (cprintf<(CPF_ATOMIC | CPF_STDO)>($invoke_with))->c;
    } else if (x0 == CPF_STDE) {
        return (cprintf<(CPF_ATOMIC | CPF_STDE)>($invoke_with))->c;
    }
  return -1;
}
""")

def gen_and_pipe_fdata():
    permitted_types = ("int", "long", "float", "const char*", "const wchar_t*")
    ptype_combinations = list()
    for n in range(1, len(permitted_types)):
        ptype_combinations.extend(list(itertools.combinations_with_replacement(permitted_types, n)))

    format_str_types = ("const char*", "const wchar_t*")
    stream_type = ("int", )
    (STREAM_TYPE, FORMAT_TYPE, FORMATTED_ARGS_TYPES) = tuple(range(0, 3))
    
    time_str = time.strftime("%Y-%m-%d %H:%M:%S", time.gmtime())
    hfile_meta = {  "protocals" : str(), "timestamp" :  time_str}
    
    cppfile_meta = {"source" : str(), 
                    "hfile_name" : generated_hfile, 
                    "timestamp" :  time_str
                    } 
    
    fn_def_meta = { "signature" : str(), 
                    "invoke_with" : str()
                }

    # generation data for API with only stream and format string args
    gdata = tuple(itertools.product(stream_type, format_str_types))
     
    # generation data for all possible combination of arguments to insure
    # full API overloading coverage 
    gdata = gdata + tuple(itertools.product(stream_type, 
                                            format_str_types, 
                                            ptype_combinations)) 

    # for every API variant
    for Ts in gdata:
        # sum the required arguments needed to uniquely define a protocal
        argtypes = (Ts[STREAM_TYPE], ) + (Ts[FORMAT_TYPE], )
        if(len(Ts) > 2):
            argtypes = argtypes + Ts[FORMATTED_ARGS_TYPES]
        
        #API parameter list
        arglist = ["{0} x{1}".format(v, str(k)) for k, v in enumerate(argtypes)]
        params = ", ".join(arglist)
                
        def replace_frmt_type(s):
            if("const " in s):
                s = s.replace("const ", "")     
            return s
        
        # string to create unique symbol name
        tmp = map(replace_frmt_type, argtypes)
        unique_id = "".join(map(lambda e: e[0], tmp))

        # signature used in the source file
        fn_cpp_signature = "int _cprintf_{0}({1})".format(unique_id, params)
        
        # signature used in header file
        fn_h_signature = "int _cprintf_{0}({1})".format(unique_id, ", ".join([t for t in argtypes]))
       
        invoke_with = ", ".join(["x{0}".format(n) for n,v in enumerate(tmp)])
         
        hfile_meta["protocals"] = hfile_meta["protocals"] + "CPF_API {0};\n".format(fn_h_signature);
       
        fn_def_meta["signature"] = fn_cpp_signature
        fn_def_meta["invoke_with"] = invoke_with
 
        cppfile_meta["source"] = cppfile_meta["source"] + fn_body.safe_substitute(fn_def_meta)
    
    with open(generated_hfile, "w") as header_file:
        print("writing file: {0}".format(generated_hfile))
        header_file.write(hfile.safe_substitute(hfile_meta))

    with open(generated_cppfile, "w") as source_file: 
        print("writing file: {0}".format(generated_cppfile))
        source_file.write(cppfile.safe_substitute(cppfile_meta)) 

gen_and_pipe_fdata()

def impl_detail(lib_hndl, *args):
    assert(len(args) >= MIN_ARGS_COUNT and len(args) <= MAX_ARGS_COUNT)

    
    #lib_func_args = 
    lib_func = getattr(lib_hndl, deduced_name)
    
    return lib_func(*lib_func_args)
