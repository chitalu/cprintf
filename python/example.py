import sys, os

_module_abs_path = os.path.abspath(__file__)
_module_real_path = os.path.realpath(_module_abs_path)
_module_dir_name = os.path.dirname(_module_real_path)

# location of cprintf script
sys.path.insert(0, os.path.join(_module_dir_name, "./interface"))

import cprintf 

# notify cprintf module that you want to use this lib
# by default the script expects to find the libcprintf.so/cprintf.dll
# file in the same directory as itself
cprintf.lib_path = os.path.join(_module_dir_name, "../build/python/libcprintf.so")

from cprintf import cprintf

def main():
    # NOTE: The following emulates the behaviour of the standard python 
    # "print()" function. Nothing new
    cprintf(75)
    cprintf(5.9)
    cprintf(["this is NOT a format string %d\n", 73, "foo"], 0.2)
    cprintf({1 : "foo", 2.4 : 2, "bar" : 75})
    cprintf({1 : "foo", 2.4 : 2, "bar" : 75}, "cprintf")

    # NOTE: This is the "cprintf" i.e. std::printf or std::fprintf syntax way 
    # of printing data.
    cprintf("this IS a format string")
    cprintf("$y*hello world!\n")
    cprintf("print pi: %f\n", 3.14)
    cprintf(sys.stderr, "write to $r*stderr$?`!\n")
    cprintf(sys.stdout, "$m*formatted$? arg: %d!\n", 101)
    my_tup = (107, 3.142)
    cprintf("$m*formatted$? args: $g*%d$? %f!\n", my_tup)
    my_list = ["cprintf", 99, 2.5]
    cprintf(sys.stdout, "$w*formatted$? args: $c%s %d %f!\n", my_list)
    
if __name__ == '__main__':
    main()
