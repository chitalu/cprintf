/*

Copyright (C) 2014 Floyd Mulenga Chitalu jnr									

Permission is hereby granted, free of charge, to obtain a copy					
of this software, to deal in the Software without restriction, including		
without limitation the rights to [use], [copy], [modify], [merge], [publish],	
[distribute], [sublicense], and/or [sell] copies of the Software, and to		
permit persons to whom the Software is furnished to do so, subject to			
the following conditions:														
																				
The above copyright notice and this permission notice shall be included in		
all copies or substantial portions of the Software.								
																				
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR		
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,		
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE		
AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM(S), DAMAGE(S) OR OTHER		
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,	
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN		
THE SOFTWARE.	

*/

#ifndef __CPF_CONFIG_H__
#define __CPF_CONFIG_H__

#include "cprintf/internal/cpf_cdef.h"

namespace cpf
{
	namespace intern
	{
		CPF_API void save_stream_state(cpf::type::stream ustream);

		/*
			boolean finished_cpf_exec is to indicate whether this is the last call to
			before cprintf finishes execution to return

			by default this function executes only if save_stream_state
			has been previous called (in the current call to cprintf) else it returns
			immidiately. This behaviour can be overridden via force_restore.
		*/
		CPF_API void restore_stream_state(	cpf::type::stream ustream,
											bool finished_cpf_exec = false);

		/*
			configure system terminal settings

			@ustream 	- output stream
			@attr - text attribute vector
		*/
		CPF_API void configure(	cpf::type::stream ustream,
								const cpf::type::attribute_group& attr);
	}
}

#endif