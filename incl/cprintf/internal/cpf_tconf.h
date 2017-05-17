#ifndef __CPF_CONFIG_H__
#define __CPF_CONFIG_H__

#include "cprintf/internal/cpf_cdef.h"
#include <cprintf/internal/cpf_type.h>

namespace _cprintf_
{

CPF_API void save_stream_state(file_stream_t ustream);

// boolean finished_cpf_exec is to indicate whether this is the last call to
// before cprintf finishes execution to return by default this function executes
// only if save_stream_state has been previous called (in the current call to
// cprintf) else it returns immidiately. This behaviour can be overridden via
// force_restore.
CPF_API void restore_stream_state(file_stream_t ustream,
                                  bool finished_cpf_exec = false);

// configure system terminal settings
//        @ustream 	- output stream
//        @attr - text attribute vector
CPF_API void configure(file_stream_t ustream,
                       const attribute_group_t &attr);

}

#endif
