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

from sys import stdout, stderr

def basic_samples(): 
    # NOTE: The following emulates the behaviour of the standard python 
    # "print()" function. Nothing new
    cprintf(75)
    cprintf(5.9)
    cprintf(["this is NOT a format string %d\n", 73, "foo"], 0.2)
    cprintf({1 : "foo", 2.4 : 2, "bar" : 75})
    cprintf({1 : "foo", 2.4 : 2, "bar" : 75}, "cprintf")

    # NOTE: This is the "cprintf" way of doing things!
    cprintf(stderr, u"this IS a format string\n") 
    cprintf(stdout, u"$y*hello world!\n")
    cprintf(u"print pi: %f\n", 3.14)
    cprintf(stderr, u"write to $r*stderr$?`!\n")
    cprintf(stderr, u"$m*`formatted$? arg: %d!\n", 101)
    my_tup = (107, 3.142)
    cprintf(stderr, u"$m*`formatted$? args: $g*%d$? %f!\n", my_tup)
    my_list = [u"cprintf", 99, 2.5]
    cprintf(stderr, u"$w*`formatted$? args: $c%s %d %f!\n", my_list)
   
def display_tokens():
    token_types = (bg, ibg, fg, ifg, fgbg, ifgbg, fgibg, ifgibg) = tuple(range(0, 8))
    tokens = ("r", "g", "b", "y", "m", "c", "w")
    l = list()
    for ct in token_types:
        for t in tokens:
            if ct == bg:
                l.append( t+"#")
            elif ct == ibg:
                l.append(t+"*#")
            elif ct == fg:
                l.append(t)
            elif ct == ifg:
                l.append(t+"*")
            elif ct == fgbg:
                for t_ in tokens:
                    l.append(t+t_)
            elif ct == ifgbg:
                for t_ in tokens:
                    l.append(t+"*"+t_)
            elif ct == fgibg:
                for t_ in tokens:
                    l.append(t + t_+"*")       
            elif ct == ifgibg:
                for t_ in tokens:
                    l.append(t+"*"+t_+"*")
    for token in l:
        frmt = "$" + token + " " + str(l.index(token)) + "$?\t"
        cprintf(stderr, unicode(frmt, "utf-8")) 
        if (l.index(token) % 16)+1 == 1:    
            print("")
    print("")

    for i in range(0, 256):
        frmt =  "$" + str(i) + "f " + str(i) + "$?\t"
        cprintf(stderr, unicode(frmt, "utf-8"))
        if (i % 16)+1 == 1:
            print("")
    print("")

    for i in range(0, 256):
        frmt =  "$" + str(i) + "b " + str(i) + "$?\t"
        cprintf(stderr, unicode(frmt, "utf-8"))
        if (i % 16)+1 == 1:
            print("")
    print("")

def main():
    basic_samples()
    display_tokens()

if __name__ == '__main__':
    main()
