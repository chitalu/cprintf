from __future__ import print_function

import sys
import os
import ctypes
import types
import collections
import fnmatch

_CPF_STDO = 0x1
_CPF_STDE = 0X2

_STREAM = 0
_FORMAT = 1

_MIN_ARGS_COUNT = 1
_MAX_ARGS_COUNT = 2

# path to the shared library with exported cprintf symbols 
# client scripts may change this variable to indicate a different directory 
# The default search path is within the script's directory
lib_path = None

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
    
    raise TypeError("cprintf error: Invalid Type -->", type(var).__name__)

def _promote_args(*args):
    return tuple(_ctype_get(a)(a) for a in args) 
    
def invoke(lib_hndl, *args):
    # convert user args to their ctypes equivalents
    promoted_form = _promote_args(*args)
    # deduce API name to call based on argument types
    API_name = "cprintf_{0}".format(promoted_form)

    if not hasattr(lib_hndl, API_name):
        raise ValueError("cprintf error: Unsupported Type combination")

    # get specific API handle
    lib_func = getattr(lib_hndl, API_name)
    lib_func.restype = ctypes.c_int
    lib_func.argtypes = [type(arg) for arg in promoted_form]
    return lib_func(*promoted_form)

# handle to the shared library
_lib = None

def _init_once():
    global _lib
    global lib_path

    if _lib is not None:
        return
    
    if (_lib is None):
        if not os.path.exists(lib_path):
            raise RuntimeError("cprintf error: library path does not exist")
        _lib = ctypes.CDLL(lib_path, mode=ctypes.RTLD_GLOBAL)

def _validate(status):    
    if status != 0:
        raise RuntimeError("cprintf error: {0}".format(status))

def cprintf(*VA_ARGS):
    """
    TODO: add complete doc-string

    If targeting the underlying cprintf shared library instead of the emulated 
    standard python print function, then only integers, longs, floats, byte 
    strings and unicode strings can be used as parameters. All other types are
    not allowed.
    """
    global _lib

    status = None
    arg_cnt = len(VA_ARGS)
    if not (arg_cnt >= _MIN_ARGS_COUNT and arg_cnt <= _MAX_ARGS_COUNT):
        raise ValueError("cprintf error: Invalid argument count: {0}".format(arg_cnt))
 
    is_str = lambda e: isinstance(e, types.StringType) or isinstance(e, types.UnicodeType) 
    arg0 = VA_ARGS[0]

    # emulate standard "print" function
    if not (isinstance(arg0, type(sys.stdout)) and ((arg0 is sys.stdout) or (arg0 is sys.stderr))) and \
        not is_str(arg0):
        print(*VA_ARGS)
        return
           
    stream = None
    # the first argument after the format string
    fmtspec_arg = None
    if isinstance(arg0, type(sys.stdout)):
        stream = _CPF_STDO if (arg0 is sys.stdout) else _CPF_STDE
        if arg_cnt > 2:
            fmtspec_arg = VA_ARGS[2]
    elif arg_cnt > 1:
        fmtspec_arg = VA_ARGS[1]

    is_iterable = lambda e: isinstance(e, collections.Iterable) and not is_str(e)
    is_list_or_tup = lambda e: (isinstance(e, tuple) or isinstance(e, list))
    
    if not (fmtspec_arg is None) and is_iterable(fmtspec_arg) and is_list_or_tup(fmtspec_arg):
        if any([is_iterable(x) for x in fmtspec_arg]):
            raise ValueError("cprintf error: Invalid expandable argument: {0}".format(fmtspec_arg))
        a_ = None
        if not (stream is None):
            a_ = (VA_ARGS[_STREAM], VA_ARGS[_FORMAT]) + tuple(fmtspec_arg)
        else:
            a_ = (arg0, ) + fmtspec_arg
        status = invoke(_lib, *a_)
    else:
        forwarded_args = None
        if stream is None:
            forwarded_args = (_CPF_STDO, ) + VA_ARGS
        else:
            forwarded_args = (stream, ) + VA_ARGS[1: ]
        _init_once()
        status = invoke(_lib, *forwarded_args)
        _validate(status)

if __name__ == '__main__':
    pass
