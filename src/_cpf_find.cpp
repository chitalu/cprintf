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

#include "_cpf_find.h"

cpf::type::size cpf::search_for(const cpf::type::str& _what, 
								const cpf::type::str& _where,
								const cpf::type::size _offset,
								const char& _esc_char)
{
	bool found = false;
	auto _Off = _offset;
	cpf::type::size position = 0;

	while((position = _where.find(_what, _Off)) != _where.npos)
	{
		if(position == 0)
		{
			return position;
		}
		else //position > 0 
		{
			/*verify that position - 1 is not equal to an escape character*/
			if(_where[position - 1] == _esc_char)
			{
				_Off = position + 1;
				continue;
			}
			else
			{
				return position;
			}
		}
	}

	return position;
}