#!
import os
import sys
import itertools
import string
import time

_module_abs_path = os.path.abspath(__file__)
_module_real_path = os.path.realpath(_module_abs_path)
_module_dir_name = os.path.dirname(_module_real_path)

output_dir = str()

_templates_dir = os.path.join(_module_dir_name, "template")

def _load_template(filename):
    data = None
    with open(os.path.join(_templates_dir, filename)) as f:
        lines = f.readlines()
        if len(lines) == 0:
            raise IOError("Invalid template file content: ", filename)
        file_string = "".join(lines)
        data = string.Template(file_string)
    
    return data

def _gen_and_pipe_fdata():
    global output_dir 
    
    filename_we = "cprintf_c_api.{0}"
    header_file_basename = filename_we.format("h")
    source_file_basename = filename_we.format("cpp")
    header_file_path = os.path.join(output_dir, header_file_basename)
    source_file_path = os.path.join(output_dir, source_file_basename)

    header_file_template = _load_template("header_file.template")
    source_file_template = _load_template("source_file.template")
    function_body_template = _load_template("function_body.template")
    
    permitted_types = ("int", "long", "float", "const char*", "const wchar_t*")
    ptype_combinations = list()
    for n in range(1, len(permitted_types) + 1):
        combs = itertools.combinations_with_replacement(permitted_types, n)
        perms = itertools.permutations(permitted_types, n)
        x = set(list(combs) + list(perms))
        ptype_combinations.extend(list(x))

    format_str_types = ("const char*", "const wchar_t*")
    stream_type = ("int", )
    (STREAM_TYPE, FORMAT_TYPE, FORMATTED_ARGS_TYPES) = tuple(range(0, 3))
    
    time_str = time.strftime("%Y-%m-%d %H:%M:%S", time.gmtime())

    header_file_metadata = {  "protocals" : str(), "timestamp" :  time_str }
    
    source_file_metadata = {    "source" : str(), 
                                "hfile_name" : header_file_basename, 
                                "timestamp" :  time_str } 
    
    function_body_metadata = {  "signature" : str(), 
                                "invoke_with" : str() }

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
            # if the format string is wide then any formatted args must be of the
            # same type/ encoding and vice-versa
            if( (Ts[FORMAT_TYPE] == "const char*" and "const wchar_t*" in Ts[FORMATTED_ARGS_TYPES])) or \
                (Ts[FORMAT_TYPE] == "const wchar_t*" and "const char*" in Ts[FORMATTED_ARGS_TYPES]):
                continue
        
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
        source_file_signature = "int _cprintf_{0}({1})".format(unique_id, params) 
        # signature used in header file
        header_file_signature = "int _cprintf_{0}({1})".format(unique_id, ", ".join([t for t in argtypes]))
      
        # skip first argument and use n+1 to prevent generated code from
        # passing x0 as first argument
        invoke_with = ", ".join(["_{0}".format(n+1) for n,v in enumerate(tmp[1:])])
         
        header_file_metadata["protocals"] = header_file_metadata["protocals"] + "CPF_API {0};\n".format(header_file_signature);
       
        function_body_metadata["signature"] = source_file_signature
        function_body_metadata["invoke_with"] = invoke_with
 
        source_file_metadata["source"] = source_file_metadata["source"] + function_body_template.safe_substitute(function_body_metadata)
   
    with open(header_file_path, "w") as header_file:
        print("writing file: {0}".format(header_file_path))
        header_file.write(header_file_template.safe_substitute(header_file_metadata))

    with open(source_file_path, "w") as source_file: 
        print("writing file: {0}".format(source_file_path))
        source_file.write(source_file_template.safe_substitute(source_file_metadata)) 

if __name__ == '__main__':
    if len(sys.argv) == 1:
        print("please specify an output directory!")
        sys.exit(1)

    output_dir = sys.argv[1]
    if not os.path.isdir(output_dir):
        print("making dir: {0}".format(output_dir))
        os.makedirs(output_dir)

    _gen_and_pipe_fdata()

