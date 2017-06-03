from __future__ import print_function

import sys
import os
import ctypes
import collections
import fnmatch

_CPF_STDO = 0x1
_CPF_STDE = 0X2
_STREAM = 0
_FORMAT = 1
_MIN_ARGS_COUNT = 1
_MAX_ARGS_COUNT = 2

# path to the shared library with exported cprintf symbol
_binary_filepath = None
# handle to the shared library
_binary_file_handle = None

def set_library_path(fpath):
    global _binary_filepath
    _binary_filepath = fpath

def _ctype_get(var):
    if isinstance(var, int): 
        return ctypes.c_int64
    elif isinstance(var, float): 
        return ctypes.c_double
    elif isinstance(var, str): 
        return ctypes.c_char_p
    raise TypeError("cprintf error: Invalid Type -->", type(var).__name__)

def _promote_va_arg(a):
    T = _ctype_get(a)
    casted = None
    if isinstance(a, str):
        casted = T(a.encode('utf-8'))
    else:
        casted = T(a)
    return casted
    
def invoke(lib_hndl, *args):
    API_name = "cprintf_c"
    if not hasattr(lib_hndl, API_name):
        raise ValueError("cprintf error: Unsupported Type combination")
    lib_func = getattr(lib_hndl, API_name)
    lib_func.restype = ctypes.c_int
    lib_func.argtypes = [ctypes.c_int32, ctypes.c_char_p]
    return lib_func(ctypes.c_int32(args[0]), ctypes.c_char_p(args[1].encode('utf-8')))

def _init_once():
    global _binary_file_handle
    global _binary_filepath
    if _binary_file_handle is not None:
        return
    if (_binary_file_handle is None):
        if not os.path.exists(_binary_filepath):
            raise RuntimeError("cprintf error: library path does not exist")
        _binary_file_handle = ctypes.CDLL(_binary_filepath, mode=ctypes.RTLD_GLOBAL) 

def cprintf(*VA_ARGS):
    """
    write colour formatted output to terminal
    """
    global _binary_file_handle
    status = None
    stream = None
    # the first argument after the format string
    fmtspec_arg = None
    arg_cnt = len(VA_ARGS)
    is_str = lambda e: isinstance(e, str) 
    arg0 = VA_ARGS[0]

    if not (arg_cnt >= _MIN_ARGS_COUNT and arg_cnt <= _MAX_ARGS_COUNT):
        raise ValueError("cprintf error: Invalid argument count: {0}".format(arg_cnt))

    # emulate standard "print" function
    if not (isinstance(arg0, type(sys.stdout)) and ((arg0 is sys.stdout) or (arg0 is sys.stderr))) and \
        not is_str(arg0):
        print(*VA_ARGS)
        return

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
        status = invoke(_binary_file_handle, *a_)
    else:
        forwarded_args = None
        if stream is None:
            forwarded_args = (_CPF_STDO, ) + VA_ARGS
        else:
            forwarded_args = (stream, ) + VA_ARGS[1: ]
        _init_once()
        status = invoke(_binary_file_handle, *forwarded_args)
        if status != 0:
            raise RuntimeError("cprintf error: {0}".format(status))
