#!
import os
import sys
import itertools
import string
import time

fname_we = "pycpf"
generated_hfile = fname_we + ".h"
generated_cppfile = fname_we + ".cpp"
output_dir = str()

hfile = string.Template("""
#pragma once
#ifndef __PYCPF_H__
#define __PYCPF_H__

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
    if (_0 == CPF_STDO) {
        return (cprintf<(CPF_ATOMIC | CPF_STDO)>($invoke_with))->c;
    } else if (_0 == CPF_STDE) {
        //return (cprintf<(CPF_ATOMIC | CPF_STDE)>($invoke_with))->c;
    }
  return -1;
}
""")

def _gen_and_pipe_fdata():
    global generated_hfile
    global generated_cppfile
    global output_dir 

    permitted_types = ("int", "long", "float", "const char*", "const wchar_t*")
    ptype_combinations = list()
    for n in range(1, len(permitted_types) + 1):
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
        arglist = ["{0} _{1}".format(v, str(k)) for k, v in enumerate(argtypes)]
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
      
        # skip first argument and use n+1 to prevent generated code from
        # passing x0 as first argument
        invoke_with = ", ".join(["_{0}".format(n+1) for n,v in enumerate(tmp[1:])])
         
        hfile_meta["protocals"] = hfile_meta["protocals"] + "CPF_API {0};\n".format(fn_h_signature);
       
        fn_def_meta["signature"] = fn_cpp_signature
        fn_def_meta["invoke_with"] = invoke_with
 
        cppfile_meta["source"] = cppfile_meta["source"] + fn_body.safe_substitute(fn_def_meta)
   
    generated_hfile = os.path.join(output_dir, generated_hfile)
    with open(generated_hfile, "w") as header_file:
        print("writing file: {0}".format(generated_hfile))
        header_file.write(hfile.safe_substitute(hfile_meta))

    generated_cppfile = os.path.join(output_dir, generated_cppfile)
    with open(generated_cppfile, "w") as source_file: 
        print("writing file: {0}".format(generated_cppfile))
        source_file.write(cppfile.safe_substitute(cppfile_meta)) 

if __name__ == '__main__':
    if len(sys.argv) == 1:
        print("please specify an output directory!")
        sys.exit(1)

    output_dir = sys.argv[1]
    if not os.path.isdir(output_dir):
        os.makedirs(output_dir)

    _gen_and_pipe_fdata()

