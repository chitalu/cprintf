import sys
import os
import ctypes
import types
import collections

import bind_impl as API

_module_abs_path = os.path.abspath(__file__)
_module_real_path = os.path.realpath(_module_abs_path)
_module_dir_name = os.path.dirname(_module_real_path)

_CPF_STDO = 0x1
_CPF_STDE = 0X2

_STREAM = 0
_FORMAT = 1

_MIN_ARGS_COUNT = 1
_MAX_ARGS_COUNT = 6

# TODO: in the future this script should expect the shared lib file
# to be in the same directory as itself by default. 
# If other scripts decide to include this then it should be possible
# to override the value of "lib_path" to point elsewhere

# path to the shared library with exported cprintf symbols 
# client scripts may change this variable to indicate a different directory 
# The default search path is within the script's directory
lib_path = os.path.join(_module_dir_name, "../build/py/libpycpf.so")

# handle to the shared library
_lib = None

def cprintf(*VA_ARGS):
    """
    TODO: add doc string 
    """
    global _lib
    global lib_path

    status = None
   
    arg_cnt = len(VA_ARGS)
    if not (arg_cnt >= _MIN_ARGS_COUNT and arg_cnt <= _MAX_ARGS_COUNT):
        raise ValueError("CPF-RT-ERR: Invalid argument count: ", len(VA_ARGS))
 
    is_str = lambda e: isinstance(e, types.StringType) or isinstance(e, types.UnicodeType)
    
    arg0 = VA_ARGS[0]
    print type(arg0)
    if not (isinstance(arg0, file) and ((arg0 is sys.stdout) or (arg0 is sys.stderr))) and \
        not is_str(arg0):
        raise ValueError("CPF-RT-ERR: First argument is of an invalid type:", type(arg0).__name__ )
   
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
            raise ValueError("CPF-RT-ERR: Invalid expandable argument", fmtspec_arg)
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
            forwarded_args = (stream, ) + VA_ARGS[1:]

        if (_lib is None):
            _lib = ctypes.CDLL(lib_path)

        print forwarded_args
        status = API.invoke(_lib, *forwarded_args)

    return status

if __name__ == '__main__':
    status0 = cprintf("$y*hello world!\n")
    print("status0: ", status0)
    
    status1 = cprintf(sys.stderr, "write to $r*stderr$?`!\n")
    print("status1: ", status1)
    
    status2 = cprintf(sys.stdout, "$m*formatted$? arg: %d!\n", 101)
    print("status2: ", status2)

    my_tup = (107, 3.142)
    status3 = cprintf("$m*formatted$? args: $g*%d$? %f!\n", my_tup)
    print("status3: ", status3)
   
    my_list = ["cprintf", 99, 2.5]
    status4 = cprintf(sys.stdout, "$w*formatted$? args: $c%s %d %f!\n", my_list)
    print("status4: ", status4)
