import sys
import os
import ctypes

import bind_impl as detail

_module_abs_path = os.path.abspath(__file__)
_module_real_path = os.path.realpath(_module_abs_path)
_module_dir_name = os.path.dirname(_module_real_path)

# TODO: in the future this script should expect the shared lib file
# to be in the same directory as itself

# path to the shared library with exported cprintf symbols 
# client scripts may change this variable to indicate a different directory 
# The default search path is within the script's directory
lib_path = os.path.join(_module_dir_name, "../build/py/libpycpf.so")

# handle to the shared library
_lib = None

def cfprintf(stream, format):
    assert(type(format) is str)

    assert((stream is sys.stdout) or (stream is sys.stderr))

    if (_lib is None):
        _lib = ctypes.CDLL(_lib_path)

    return detail.impl_detail(_lib, stream, format)

def cprintf(format):
    """
    Writes the given "format" string to system standard output. If additional
    format specifiers as well as arguments arguments are supplied then they too
    are written to the standard output stream.
    """
    return cfprintf(sys.stdout, format)
   
if __name__ == '__main__':
    # test call
    ret = cprintf("$y*hello world!\n")
    print(">> ", ret)
   
