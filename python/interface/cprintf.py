from __future__ import print_function

import sys
import os
import ctypes
import types
import collections
import fnmatch

import cpf_binding as binding

_module_abs_path = os.path.abspath(__file__)
_module_real_path = os.path.realpath(_module_abs_path)
_module_dir_name = os.path.dirname(_module_real_path)

_CPF_STDO = 0x1
_CPF_STDE = 0X2

_STREAM = 0
_FORMAT = 1

_MIN_ARGS_COUNT = 1
_MAX_ARGS_COUNT = 6

# path to the shared library with exported cprintf symbols 
# client scripts may change this variable to indicate a different directory 
# The default search path is within the script's directory
lib_path = os.path.join(_module_dir_name, "libcprintf.so")
cpf_base_path = os.path.join(_module_dir_name, "../../incl/cprintf/internal/cpf_base.h")

_lib_status_codes = None

def _scan_status_codes():
    global _lib_status_codes

    global _lib
    global lib_path
    _lib_status_codes = dict()
    with open(cpf_base_path, "r") as cpf_base_file:
        lines = cpf_base_file.readlines()
        for line in lines:
            if fnmatch.fnmatch(line, "*CPF_*_ERR*"):
                spos = line.rfind(' ') + 1
                radix = 16
                code = int(line[spos : ], radix)
                idx = lines.index(line)
                diag_msg = lines[idx - 1]

                _lib_status_codes[code] = diag_msg[2:].strip()

# handle to the shared library
_lib = None

def _init_once():
    global _lib
    global lib_path

    if _lib is not None and _lib_status_codes is not None:
        return
    
    if (_lib is None):
        _lib = ctypes.CDLL(lib_path, mode=ctypes.RTLD_GLOBAL)

    if _lib_status_codes is None:
        _scan_status_codes()

def _validate(status):
    if ( status not in _lib_status_codes):
        raise RuntimeError("CPF-RT-ERR: Unknown {0}".format(status))
    
    if status != 0:
        raise RuntimeError("CPF-RT-ERR: {0}".format(_lib_status_codes[status]))

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
        raise ValueError("CPF-RT-ERR: Invalid argument count: {0}".format(arg_cnt))
 
    is_str = lambda e: isinstance(e, types.StringType) or isinstance(e, types.UnicodeType) 
    arg0 = VA_ARGS[0]

    # emulate standard "print" function
    if not (isinstance(arg0, file) and ((arg0 is sys.stdout) or (arg0 is sys.stderr))) and \
        not is_str(arg0):
        print(*VA_ARGS)
        return
           
    stream = None

    # the first argument after the format string
    fmtspec_arg = None
    if isinstance(arg0, file):
        stream = _CPF_STDO if (arg0 is sys.stdout) else _CPF_STDE
        if arg_cnt > 2:
            fmtspec_arg = VA_ARGS[2]
    elif arg_cnt > 1:
        fmtspec_arg = VA_ARGS[1]

    is_iterable = lambda e: isinstance(e, collections.Iterable) and not is_str(e)
    is_list_or_tup = lambda e: (isinstance(e, tuple) or isinstance(e, list))
    
    if not (fmtspec_arg is None) and is_iterable(fmtspec_arg) and is_list_or_tup(fmtspec_arg):
        if any([is_iterable(x) for x in fmtspec_arg]):
            raise ValueError("CPF-RT-ERR: Invalid expandable argument: {0}".format(fmtspec_arg))
        a_ = None
        if not (stream is None):
            a_ = (VA_ARGS[_STREAM], VA_ARGS[_FORMAT]) + tuple(fmtspec_arg)
        else:
            a_ = (arg0, ) + fmtspec_arg
        status = cprintf(*a_)
    else:
        forwarded_args = None
        if stream is None:
            forwarded_args = (_CPF_STDO, ) + VA_ARGS
        else:
            forwarded_args = (stream, ) + VA_ARGS[1: ]
        _init_once()
        status = binding.invoke(_lib, *forwarded_args)
        _validate(status)

if __name__ == '__main__':
    pass
    # TODO
    # import doctest
    # doctest.testmod()
