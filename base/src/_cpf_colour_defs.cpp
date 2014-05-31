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

#include "_cpf_colour_defs.h"
#include <assert.h>

#ifdef _WIN32 //attribute
const auto default_foreground_colour = [&]()->_cpf_type::colour
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	auto a = csbi.wAttributes;
	return static_cast<_cpf_type::colour>(a % 16);
}();
#else

#endif

extern "C" const _cpf_type::str_vec _cpf_std_tokens = {

	/*default*/
	"!",

	/*dim text colour no background*/
	"r", "g", "b", "y", "m", "c", "w",

	/*dim background*/
	"#r", "#g", "#b", "#y", "#m", "#c", "#w",

	/*bright text colour no background*/
	"r*", "g*", "b*", "y*", "m*", "c*", "w*",

	/*bright background*/
	"#r*", "#g*", "#b*", "#y*", "#m*", "#c*", "#w*",

	/*dim text and background colour*/
	"rr", "rb", "rg", "ry", "rm", "rc", "rw", /*red*/
	"gg", "gb", "gr", "gy", "gm", "gc", "gw", /*green*/
	"bb", "br", "bg", "by", "bm", "bc", "bw", /*blue*/
	"yy", "yb", "yg", "yr", "ym", "yc", "yw", /*yellow*/
	"mm", "mr", "mg", "my", "mb", "mc", "mw", /*magenta*/
	"cc", "cr", "cg", "cy", "cm", "cb", "cw", /*cyan*/
	"ww", "wr", "wg", "wy", "wm", "wc", "wb", /*white*/

	/*bright text colour and dim background colour*/
	"r*r", "r*b", "r*g", "r*y", "r*m", "r*c", "r*w", /*red*/
	"g*g", "g*b", "g*r", "g*y", "g*m", "g*c", "g*w", /*green*/
	"b*b", "b*r", "b*g", "b*y", "b*m", "b*c", "b*w", /*blue*/
	"y*y", "y*b", "y*g", "y*r", "y*m", "y*c", "y*w", /*yellow*/
	"m*m", "m*r", "m*g", "m*y", "m*b", "m*c", "m*w", /*magenta*/
	"c*c", "c*r", "c*g", "c*y", "c*m", "c*b", "c*w", /*cyan*/
	"w*w", "w*r", "w*g", "w*y", "w*m", "w*c", "w*b", /*white*/

	/*dim text colour and bright background colour*/
	"rr*", "rb*", "rg*", "ry*", "rm*", "rc*", "rw*", /*red*/
	"gg*", "gb*", "gr*", "gy*", "gm*", "gc*", "gw*", /*green*/
	"bb*", "br*", "bg*", "by*", "bm*", "bc*", "bw*", /*blue*/
	"yy*", "yb*", "yg*", "yr*", "ym*", "yc*", "yw*", /*yellow*/
	"mm*", "mr*", "mg*", "my*", "mb*", "mc*", "mw*", /*magenta*/
	"cc*", "cr*", "cg*", "cy*", "cm*", "cb*", "cw*", /*cyan*/
	"ww*", "wr*", "wg*", "wy*", "wm*", "wc*", "wb*", /*white*/

	/*bright text and background colour*/
	"r*r*", "r*b*", "r*g*", "r*y*", "r*m*", "r*c*", "r*w*", /*red*/
	"g*g*", "g*b*", "g*r*", "g*y*", "g*m*", "g*c*", "g*w*", /*green*/
	"b*b*", "b*r*", "b*g*", "b*y*", "b*m*", "b*c*", "b*w*", /*blue*/
	"y*y*", "y*b*", "y*g*", "y*r*", "y*m*", "y*c*", "y*w*", /*yellow*/
	"m*m*", "m*r*", "m*g*", "m*y*", "m*b*", "m*c*", "m*w*", /*magenta*/
	"c*c*", "c*r*", "c*g*", "c*y*", "c*m*", "c*b*", "c*w*", /*cyan*/
	"w*w*", "w*r*", "w*g*", "w*y*", "w*m*", "w*c*", "w*b*" /*white*/

#ifndef _WIN32 //attributes
	, "bld", "dim", "uln", "blk",	"rvs",	"hid",	
	"!bld",	"!dim",	"!uln",	"!blk",	"!rvs",	"!hid",
#endif /*#ifndef _WIN32*/
};

#ifdef _WIN32

const std::map<const _cpf_type::str, _cpf_type::colour> _cpf_std_token_vals{
	/*default*/
	{ "!",	[&](void)->_cpf_type::colour
			{
				CONSOLE_SCREEN_BUFFER_INFO csbi;
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
				auto a = csbi.wAttributes;
				return static_cast<_cpf_type::colour>(a % 16);
			}()
	},

	/*red*/
	{ "#r", (default_foreground_colour | _cpf_Rb) },
	{ "#r*", (default_foreground_colour | _cpf_Rb | _cpf_bgi) },

	{ "r", (_cpf_Rf) },
	{ "rr", (_cpf_Rf | _cpf_Rb) },
	{ "rb", (_cpf_Rf | _cpf_Bb) },
	{ "rg", (_cpf_Rf | _cpf_Gb) },
	{ "ry", (_cpf_Rf | _cpf_Rb | _cpf_Gb) },
	{ "rm", (_cpf_Rf | _cpf_Rb | _cpf_Bb) },
	{ "rc", (_cpf_Rf | _cpf_Gb | _cpf_Bb) },
	{ "rw", (_cpf_Rf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "r*", (_cpf_Rf | _cpf_fgi) },
	{ "r*r", ((_cpf_Rf | _cpf_fgi) | _cpf_Rb) },
	{ "r*b", ((_cpf_Rf | _cpf_fgi) | _cpf_Bb) },
	{ "r*g", ((_cpf_Rf | _cpf_fgi) | _cpf_Gb) },
	{ "r*y", ((_cpf_Rf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "r*m", ((_cpf_Rf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "r*c", ((_cpf_Rf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "r*w", ((_cpf_Rf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "rr*", ((_cpf_Rf) | (_cpf_Rb | _cpf_bgi)) },
	{ "rb*", ((_cpf_Rf) | (_cpf_Bb | _cpf_bgi)) },
	{ "rg*", (_cpf_Rf | (_cpf_Gb | _cpf_bgi)) },
	{ "ry*", (_cpf_Rf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "rm*", (_cpf_Rf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "rc*", (_cpf_Rf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "rw*", (_cpf_Rf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "r*r*", ((_cpf_Rf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "r*b*", ((_cpf_Rf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "r*g*", ((_cpf_Rf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "r*y*", ((_cpf_Rf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "r*m*", ((_cpf_Rf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "r*c*", ((_cpf_Rf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "r*w*", ((_cpf_Rf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	/*green*/
	{ "#g", (default_foreground_colour | _cpf_Gb) },
	{ "#g*", (default_foreground_colour | _cpf_Gb | _cpf_bgi) },

	{ "g", (_cpf_Gf) },
	{ "g*", (_cpf_Gf | _cpf_fgi) },

	{ "gr", (_cpf_Gf | _cpf_Rb) },
	{ "gb", (_cpf_Gf | _cpf_Bb) },
	{ "gg", (_cpf_Gf | _cpf_Gb) },
	{ "gy", (_cpf_Gf | _cpf_Rb | _cpf_Gb) },
	{ "gm", (_cpf_Gf | _cpf_Rb | _cpf_Bb) },
	{ "gc", (_cpf_Gf | _cpf_Gb | _cpf_Bb) },
	{ "gw", (_cpf_Gf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "g*r", ((_cpf_Gf | _cpf_fgi) | _cpf_Rb) },
	{ "g*b", ((_cpf_Gf | _cpf_fgi) | _cpf_Bb) },
	{ "g*g", ((_cpf_Gf | _cpf_fgi) | _cpf_Gb) },
	{ "g*y", ((_cpf_Gf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "g*m", ((_cpf_Gf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "g*c", ((_cpf_Gf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "g*w", ((_cpf_Gf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "gr*", ((_cpf_Gf) | (_cpf_Rb | _cpf_bgi)) },
	{ "gb*", ((_cpf_Gf) | (_cpf_Bb | _cpf_bgi)) },
	{ "gg*", (_cpf_Gf | (_cpf_Gb | _cpf_bgi)) },
	{ "gy*", (_cpf_Gf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "gm*", (_cpf_Gf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "gc*", (_cpf_Gf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "gw*", (_cpf_Gf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "g*r*", ((_cpf_Gf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "g*b*", ((_cpf_Gf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "g*g*", ((_cpf_Gf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "g*y*", ((_cpf_Gf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "g*m*", ((_cpf_Gf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "g*c*", ((_cpf_Gf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "g*w*", ((_cpf_Gf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	/*blue*/
	{ "#b", (default_foreground_colour | _cpf_Gb) },
	{ "#b*", (default_foreground_colour | _cpf_Gb | _cpf_bgi) },

	{ "b", (_cpf_Bf) },
	{ "b*", (_cpf_Bf | _cpf_fgi) },

	{ "br", (_cpf_Bf | _cpf_Rb) },
	{ "bb", (_cpf_Bf | _cpf_Bb) },
	{ "bg", (_cpf_Bf | _cpf_Gb) },
	{ "by", (_cpf_Bf | _cpf_Rb | _cpf_Gb) },
	{ "bm", (_cpf_Bf | _cpf_Rb | _cpf_Bb) },
	{ "bc", (_cpf_Bf | _cpf_Gb | _cpf_Bb) },
	{ "bw", (_cpf_Bf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "b*r", ((_cpf_Bf | _cpf_fgi) | _cpf_Rb) },
	{ "b*b", ((_cpf_Bf | _cpf_fgi) | _cpf_Bb) },
	{ "b*g", ((_cpf_Bf | _cpf_fgi) | _cpf_Gb) },
	{ "b*y", ((_cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "b*m", ((_cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "b*c", ((_cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "b*w", ((_cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "br*", ((_cpf_Bf) | (_cpf_Rb | _cpf_bgi)) },
	{ "bb*", ((_cpf_Bf) | (_cpf_Bb | _cpf_bgi)) },
	{ "bg*", (_cpf_Bf | (_cpf_Gb | _cpf_bgi)) },
	{ "by*", (_cpf_Bf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "bm*", (_cpf_Bf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "bc*", (_cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "bw*", (_cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "b*r*", ((_cpf_Bf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "b*b*", ((_cpf_Bf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "b*g*", ((_cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "b*y*", ((_cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "b*m*", ((_cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "b*c*", ((_cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "b*w*", ((_cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	/*yellow*/
	{ "#y", (default_foreground_colour | (_cpf_Rb | _cpf_Gb)) },
	{ "#y*", (default_foreground_colour | (_cpf_Rb | _cpf_Gb) | _cpf_bgi) },

	{ "y", (_cpf_Rf | _cpf_Gf) },
	{ "y*", ((_cpf_Rf | _cpf_Gf) | _cpf_fgi) },

	{ "yr", ((_cpf_Rf | _cpf_Gf) | _cpf_Rb) },
	{ "yb", ((_cpf_Rf | _cpf_Gf) | _cpf_Bb) },
	{ "yg", ((_cpf_Rf | _cpf_Gf) | _cpf_Gb) },
	{ "yy", ((_cpf_Rf | _cpf_Gf) | _cpf_Rb | _cpf_Gb) },
	{ "ym", ((_cpf_Rf | _cpf_Gf) | _cpf_Rb | _cpf_Bb) },
	{ "yc", ((_cpf_Rf | _cpf_Gf) | _cpf_Gb | _cpf_Bb) },
	{ "yw", ((_cpf_Rf | _cpf_Gf) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "y*r", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Rb) },
	{ "y*b", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Bb) },
	{ "y*g", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Gb) },
	{ "y*y", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "y*m", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "y*c", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "y*w", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "yr*", (((_cpf_Rf | _cpf_Gf)) | (_cpf_Rb | _cpf_bgi)) },
	{ "yb*", (((_cpf_Rf | _cpf_Gf)) | (_cpf_Bb | _cpf_bgi)) },
	{ "yg*", ((_cpf_Rf | _cpf_Gf) | (_cpf_Gb | _cpf_bgi)) },
	{ "yy*", ((_cpf_Rf | _cpf_Gf) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "ym*", ((_cpf_Rf | _cpf_Gf) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "yc*", ((_cpf_Rf | _cpf_Gf) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "yw*", ((_cpf_Rf | _cpf_Gf) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "y*r*", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "y*b*", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "y*g*", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "y*y*", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "y*m*", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "y*c*", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "y*w*", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	/*magenta*/
	{ "#m", (default_foreground_colour | (_cpf_Rb | _cpf_Bb)) },
	{ "#m*", (default_foreground_colour | (_cpf_Rb | _cpf_Bb) | _cpf_bgi) },

	{ "m", (_cpf_Rf | _cpf_Bf) },
	{ "m*", (_cpf_Rf | _cpf_Bf | _cpf_fgi) },

	{ "mr", (_cpf_Rf | _cpf_Bf | _cpf_Rb) },
	{ "mb", (_cpf_Rf | _cpf_Bf | _cpf_Bb) },
	{ "mg", (_cpf_Rf | _cpf_Bf | _cpf_Gb) },
	{ "my", (_cpf_Rf | _cpf_Bf | _cpf_Rb | _cpf_Gb) },
	{ "mm", (_cpf_Rf | _cpf_Bf | _cpf_Rb | _cpf_Bb) },
	{ "mc", (_cpf_Rf | _cpf_Bf | _cpf_Gb | _cpf_Bb) },
	{ "mw", (_cpf_Rf | _cpf_Bf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "m*r", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Rb) },
	{ "m*b", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Bb) },
	{ "m*g", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Gb) },
	{ "m*y", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "m*m", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "m*c", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "m*w", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "mr*", ((_cpf_Rf | _cpf_Bf) | (_cpf_Rb | _cpf_bgi)) },
	{ "mb*", ((_cpf_Rf | _cpf_Bf) | (_cpf_Bb | _cpf_bgi)) },
	{ "mg*", (_cpf_Rf | _cpf_Bf | (_cpf_Gb | _cpf_bgi)) },
	{ "my*", (_cpf_Rf | _cpf_Bf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "mm*", (_cpf_Rf | _cpf_Bf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "mc*", (_cpf_Rf | _cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "mw*", (_cpf_Rf | _cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "m*r*", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "m*b*", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "m*g*", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "m*y*", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "m*m*", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "m*c*", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "m*w*", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	/*cyan*/
	{ "#c", (default_foreground_colour | (_cpf_Gb | _cpf_Bb)) },
	{ "#c*", (default_foreground_colour | (_cpf_Gb | _cpf_Bb) | _cpf_bgi) },

	{ "c", (_cpf_Gf | _cpf_Bf) },
	{ "c*", (_cpf_Gf | _cpf_Bf | _cpf_fgi) },

	{ "cr", (_cpf_Gf | _cpf_Bf | _cpf_Rb) },
	{ "cb", (_cpf_Gf | _cpf_Bf | _cpf_Bb) },
	{ "cg", (_cpf_Gf | _cpf_Bf | _cpf_Gb) },
	{ "cy", (_cpf_Gf | _cpf_Bf | _cpf_Rb | _cpf_Gb) },
	{ "cm", (_cpf_Gf | _cpf_Bf | _cpf_Rb | _cpf_Bb) },
	{ "cc", (_cpf_Gf | _cpf_Bf | _cpf_Gb | _cpf_Bb) },
	{ "cw", (_cpf_Gf | _cpf_Bf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "c*r", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb) },
	{ "c*b", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Bb) },
	{ "c*g", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb) },
	{ "c*y", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "c*m", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "c*c", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "c*w", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "cr*", ((_cpf_Gf | _cpf_Bf) | (_cpf_Rb | _cpf_bgi)) },
	{ "cb*", ((_cpf_Gf | _cpf_Bf) | (_cpf_Bb | _cpf_bgi)) },
	{ "cg*", (_cpf_Gf | _cpf_Bf | (_cpf_Gb | _cpf_bgi)) },
	{ "cy*", (_cpf_Gf | _cpf_Bf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "cm*", (_cpf_Gf | _cpf_Bf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "cc*", (_cpf_Gf | _cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "cw*", (_cpf_Gf | _cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "c*r*", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "c*b*", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "c*g*", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "c*y*", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "c*m*", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "c*c*", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "c*w*", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	/*white*/
	{ "#w", (default_foreground_colour | (_cpf_Rb | _cpf_Gb | _cpf_Bb)) },
	{ "#w*", (default_foreground_colour | (_cpf_Rb | _cpf_Gb | _cpf_Bb) | _cpf_bgi) },

	{ "w", ((_cpf_Rf | _cpf_Gf | _cpf_Bf)) },
	{ "w*", ((_cpf_Rf | _cpf_Gf | _cpf_Bf) | _cpf_fgi) },

	{ "wr", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb) },
	{ "wb", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Bb) },
	{ "wg", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb) },
	{ "wy", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "wm", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "wc", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "ww", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "w*r", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Rb) },
	{ "w*b", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Bb) },
	{ "w*g", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Gb) },
	{ "w*y", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "w*m", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "w*c", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "w*w", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "wr*", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi)) | (_cpf_Rb | _cpf_bgi)) },
	{ "wb*", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi)) | (_cpf_Bb | _cpf_bgi)) },
	{ "wg*", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "wy*", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "wm*", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "wc*", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "ww*", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "w*r*", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "w*b*", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "w*g*", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "w*y*", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "w*m*", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "w*c*", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "w*w*", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) }

};

#else /*#ifdef _WIN32*/
//http://www.linuxhomenetworking.com/forums/showthread.php/1095-Linux-console-Colors-And-Other-Trick-s
//http://stackoverflow.com/questions/3506504/c-code-changes-terminal-text-color-how-to-restore-defaults-linux
//http://linuxgazette.net/issue65/padala.html
//http://misc.flogisoft.com/bash/tip_colors_and_formatting

/*<ESC>[{attr};{fg};{bg}m*/
#define COLOUR_BLOCK(c, i) \
{	"#"#c"",	"\x1B[0;0;4"#i"m" },\
{	"#"#c"*",	"\x1B[0;0;10"#i"m" },\
{	#c "",		"\x1B[0;0;3"#i"m" },\
{	#c "r",		"\x1B[0;3"#i";41m" },\
{	#c "g",		"\x1B[0;3"#i";42m" },\
{	#c "b",		"\x1B[0;3"#i";44m" },\
{	#c "y",		"\x1B[0;3"#i";43m" },\
{	#c "m",		"\x1B[0;3"#i";45m" },\
{	#c "c",		"\x1B[0;3"#i";46m" },\
{	#c "w",		"\x1B[0;3"#i";47m" },\
\
{	#c "*",		"\x1B[0;0;9"#i"m" },\
{	#c "*r",	"\x1B[0;9"#i";41m" },\
{	#c "*g",	"\x1B[0;9"#i";42m" },\
{	#c "*b",	"\x1B[0;9"#i";44m" },\
{	#c "*y",	"\x1B[0;9"#i";43m" },\
{	#c "*m",	"\x1B[0;9"#i";45m" },\
{	#c "*c",	"\x1B[0;9"#i";46m" },\
{	#c "*w",	"\x1B[0;9"#i";47m" },\
\
{	#c "r*",	"\x1B[0;3"#i";101m" },\
{	#c "g*",	"\x1B[0;3"#i";102m" },\
{	#c "b*",	"\x1B[0;3"#i";104m" },\
{	#c "y*",	"\x1B[0;3"#i";103m" },\
{	#c "m*",	"\x1B[0;3"#i";105m" },\
{	#c "c*",	"\x1B[0;3"#i";106m" },\
{	#c "w*",	"\x1B[0;3"#i";107m" },\
\
{	#c "*r*",	"\x1B[0;9"#i";101m" },\
{	#c "*g*",	"\x1B[0;9"#i";102m" },\
{	#c "*b*",	"\x1B[0;9"#i";104m" },\
{	#c "*y*",	"\x1B[0;9"#i";103m" },\
{	#c "*m*",	"\x1B[0;9"#i";105m" },\
{	#c "*c*",	"\x1B[0;9"#i";106m" },\
{	#c "*w*",	"\x1B[0;9"#i";107m" },\

extern const std::map<const _cpf_type::str, _cpf_type::colour> _cpf_std_token_vals{

	/*attributes specifiers*/
	{	"bld", 		"\x1B[1m"},
	{	"dim", 		"\x1B[2m"},
	{	"uln", 		"\x1B[4m"},
	{	"blk", 		"\x1B[5m"},
	{	"rvs", 		"\x1B[7m"},
	{	"hid", 		"\x1B[8m"},
	{	"!bld", 	"\x1B[21m"},
	{	"!dim", 	"\x1B[22m"},
	{	"!uln", 	"\x1B[24m"},
	{	"!blk", 	"\x1B[25m"},
	{	"!rvs", 	"\x1B[27m"},
	{	"!hid", 	"\x1B[28m"},

	/*colours*/
	
	/*default (reset all colours and attribs)*/
	{ 	"!", 	"\x1B[0;0;0m"},

	COLOUR_BLOCK(r, 1)
	COLOUR_BLOCK(g, 2)
	COLOUR_BLOCK(b, 4)
	COLOUR_BLOCK(y, 3)
	COLOUR_BLOCK(m, 5)
	COLOUR_BLOCK(c, 6)
	COLOUR_BLOCK(c, 7)
};

#endif /*#ifdef _WIN32*/


