import sys
import os

# names for some object types that are used by the standard 
# Python interpreter, but not for the types defined by 
# various extension modules
import types
import ctypes

# given a particular variable this function returns the variable's ctypes' 
# equivalent type for caller to correctly convert arguments before invoking
# "C" shared library's symbols
def _ctype_get(var):

    if isinstance(var, types.IntType): return ctypes.c_int32
    elif isinstance(var, types.LongType): return ctypes.c_long
    elif isinstance(var, types.BooleanType): return ctypes.c_bool
    elif isinstance(var, types.FloatType): return ctypes.c_float
    elif isinstance(var, types.StringType): return ctypes.c_char_p
    elif isinstance(var, types.UnicodeType): return ctypes.c_wchar_p
    elif isinstance(var, file): return ctypes.c_uint8
    
    raise TypeError("CPF-RT-ERR: Invalid Type -->", type(var).__name__)

def _promote_args(*args):
    return tuple(_ctype_get(a)(a) for a in args) 

def _infer_API_name(args):
    # first two arguments have predetermined types
    name = "_cprintf_{0}"
    # remove the "c_" part of type name
    stripped_ctype_names = [type(a).__name__[2:] for a in args]
    # identify type by first letter
    type_char0s = map(lambda e: e[0], stripped_ctype_names)
    # append to produce API name
    return name.format("".join(type_char0s))
    
def invoke(lib_hndl, *args):
    # convert user args to their ctypes equivalents
    promoted_form = _promote_args(*args)
    # deduce API name to call based on argument types
    API_name = _infer_API_name(promoted_form)
    if not hasattr(lib_hndl, API_name):
        raise ValueError("CPF-RT-ERR: Unsupported argument-type combination")

    # get specific API handle
    lib_func = getattr(lib_hndl, API_name) 
    return lib_func(*promoted_form)

