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

CPF_API const cpf::type::string_vector cpf::std_tokens = {

	/*dim text colour no background*/
	L"r", L"g", L"b", L"y", L"m", L"c", L"w",

	/*dim background*/
	L"r#", L"g#", L"b#", L"y#", L"m#", L"c#", L"w#",

	/*bright text colour no background*/
	L"r*", L"g*", L"b*", L"y*", L"m*", L"c*", L"w*",

	/*bright background*/
	L"r*#", L"g*#", L"b*#", L"y*#", L"m*#", L"c*#", L"w*#",

	/*dim text and background colour*/
	L"rr", L"rb", L"rg", L"ry", L"rm", L"rc", L"rw", /*red*/
	L"gg", L"gb", L"gr", L"gy", L"gm", L"gc", L"gw", /*green*/
	L"bb", L"br", L"bg", L"by", L"bm", L"bc", L"bw", /*blue*/
	L"yy", L"yb", L"yg", L"yr", L"ym", L"yc", L"yw", /*yellow*/
	L"mm", L"mr", L"mg", L"my", L"mb", L"mc", L"mw", /*magenta*/
	L"cc", L"cr", L"cg", L"cy", L"cm", L"cb", L"cw", /*cyan*/
	L"ww", L"wr", L"wg", L"wy", L"wm", L"wc", L"wb", /*white*/

	/*bright text colour and dim background colour*/
	L"r*r", L"r*b", L"r*g", L"r*y", L"r*m", L"r*c", L"r*w", /*red*/
	L"g*g", L"g*b", L"g*r", L"g*y", L"g*m", L"g*c", L"g*w", /*green*/
	L"b*b", L"b*r", L"b*g", L"b*y", L"b*m", L"b*c", L"b*w", /*blue*/
	L"y*y", L"y*b", L"y*g", L"y*r", L"y*m", L"y*c", L"y*w", /*yellow*/
	L"m*m", L"m*r", L"m*g", L"m*y", L"m*b", L"m*c", L"m*w", /*magenta*/
	L"c*c", L"c*r", L"c*g", L"c*y", L"c*m", L"c*b", L"c*w", /*cyan*/
	L"w*w", L"w*r", L"w*g", L"w*y", L"w*m", L"w*c", L"w*b", /*white*/

	/*dim text colour and bright background colour*/
	L"rr*", L"rb*", L"rg*", L"ry*", L"rm*", L"rc*", L"rw*", /*red*/
	L"gg*", L"gb*", L"gr*", L"gy*", L"gm*", L"gc*", L"gw*", /*green*/
	L"bb*", L"br*", L"bg*", L"by*", L"bm*", L"bc*", L"bw*", /*blue*/
	L"yy*", L"yb*", L"yg*", L"yr*", L"ym*", L"yc*", L"yw*", /*yellow*/
	L"mm*", L"mr*", L"mg*", L"my*", L"mb*", L"mc*", L"mw*", /*magenta*/
	L"cc*", L"cr*", L"cg*", L"cy*", L"cm*", L"cb*", L"cw*", /*cyan*/
	L"ww*", L"wr*", L"wg*", L"wy*", L"wm*", L"wc*", L"wb*", /*white*/

	/*bright text and background colour*/
	L"r*r*", L"r*b*", L"r*g*", L"r*y*", L"r*m*", L"r*c*", L"r*w*", /*red*/
	L"g*g*", L"g*b*", L"g*r*", L"g*y*", L"g*m*", L"g*c*", L"g*w*", /*green*/
	L"b*b*", L"b*r*", L"b*g*", L"b*y*", L"b*m*", L"b*c*", L"b*w*", /*blue*/
	L"y*y*", L"y*b*", L"y*g*", L"y*r*", L"y*m*", L"y*c*", L"y*w*", /*yellow*/
	L"m*m*", L"m*r*", L"m*g*", L"m*y*", L"m*b*", L"m*c*", L"m*w*", /*magenta*/
	L"c*c*", L"c*r*", L"c*g*", L"c*y*", L"c*m*", L"c*b*", L"c*w*", /*cyan*/
	L"w*w*", L"w*r*", L"w*g*", L"w*y*", L"w*m*", L"w*c*", L"w*b*" /*white*/

#ifndef _WIN32 //attributes
	, L"bld", L"dim", L"uln", L"blk",	L"rvs",	L"hid",	
	L"?bld",	L"?dim",	L"?uln",	L"?blk",	L"?rvs",	L"?hid",
#endif /*#ifndef _WIN32*/
};

#ifdef _WIN32

/*
	TODO: CLEAN UP WINDOWS MACROS, TOO MUCH REPETITION!
*/
const std::map<const cpf::type::str, cpf::type::colour> cpf::std_token_vals{
	/*red*/
	{ L"r#", (_cpf_Rb) },
	{ L"r*#", (_cpf_Rb | _cpf_bgi) },
	{ L"r", (_cpf_Rf) },
	{ L"rr", (_cpf_Rf | _cpf_Rb) },
	{ L"rb", (_cpf_Rf | _cpf_Bb) },
	{ L"rg", (_cpf_Rf | _cpf_Gb) },
	{ L"ry", (_cpf_Rf | _cpf_Rb | _cpf_Gb) },
	{ L"rm", (_cpf_Rf | _cpf_Rb | _cpf_Bb) },
	{ L"rc", (_cpf_Rf | _cpf_Gb | _cpf_Bb) },
	{ L"rw", (_cpf_Rf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },
	{ L"r*", (_cpf_Rf | _cpf_fgi) },
	{ L"r*r", ((_cpf_Rf | _cpf_fgi) | _cpf_Rb) },
	{ L"r*b", ((_cpf_Rf | _cpf_fgi) | _cpf_Bb) },
	{ L"r*g", ((_cpf_Rf | _cpf_fgi) | _cpf_Gb) },
	{ L"r*y", ((_cpf_Rf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ L"r*m", ((_cpf_Rf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ L"r*c", ((_cpf_Rf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ L"r*w", ((_cpf_Rf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },
	{ L"rr*", ((_cpf_Rf) | (_cpf_Rb | _cpf_bgi)) },
	{ L"rb*", ((_cpf_Rf) | (_cpf_Bb | _cpf_bgi)) },
	{ L"rg*", (_cpf_Rf | (_cpf_Gb | _cpf_bgi)) },
	{ L"ry*", (_cpf_Rf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ L"rm*", (_cpf_Rf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ L"rc*", (_cpf_Rf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ L"rw*", (_cpf_Rf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },
	{ L"r*r*", ((_cpf_Rf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ L"r*b*", ((_cpf_Rf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ L"r*g*", ((_cpf_Rf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ L"r*y*", ((_cpf_Rf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ L"r*m*", ((_cpf_Rf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ L"r*c*", ((_cpf_Rf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ L"r*w*", ((_cpf_Rf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	/*green*/
	{ L"g#", (_cpf_Gb) },
	{ L"g*#", (_cpf_Gb | _cpf_bgi) },
	{ L"g", (_cpf_Gf) },
	{ L"g*", (_cpf_Gf | _cpf_fgi) },
	{ L"gr", (_cpf_Gf | _cpf_Rb) },
	{ L"gb", (_cpf_Gf | _cpf_Bb) },
	{ L"gg", (_cpf_Gf | _cpf_Gb) },
	{ L"gy", (_cpf_Gf | _cpf_Rb | _cpf_Gb) },
	{ L"gm", (_cpf_Gf | _cpf_Rb | _cpf_Bb) },
	{ L"gc", (_cpf_Gf | _cpf_Gb | _cpf_Bb) },
	{ L"gw", (_cpf_Gf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },
	{ L"g*r", ((_cpf_Gf | _cpf_fgi) | _cpf_Rb) },
	{ L"g*b", ((_cpf_Gf | _cpf_fgi) | _cpf_Bb) },
	{ L"g*g", ((_cpf_Gf | _cpf_fgi) | _cpf_Gb) },
	{ L"g*y", ((_cpf_Gf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ L"g*m", ((_cpf_Gf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ L"g*c", ((_cpf_Gf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ L"g*w", ((_cpf_Gf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },
	{ L"gr*", ((_cpf_Gf) | (_cpf_Rb | _cpf_bgi)) },
	{ L"gb*", ((_cpf_Gf) | (_cpf_Bb | _cpf_bgi)) },
	{ L"gg*", (_cpf_Gf | (_cpf_Gb | _cpf_bgi)) },
	{ L"gy*", (_cpf_Gf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ L"gm*", (_cpf_Gf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ L"gc*", (_cpf_Gf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ L"gw*", (_cpf_Gf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },
	{ L"g*r*", ((_cpf_Gf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ L"g*b*", ((_cpf_Gf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ L"g*g*", ((_cpf_Gf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ L"g*y*", ((_cpf_Gf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ L"g*m*", ((_cpf_Gf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ L"g*c*", ((_cpf_Gf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ L"g*w*", ((_cpf_Gf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	/*blue*/
	{ L"b#", (_cpf_Bb) },
	{ L"b*#", (_cpf_Bb | _cpf_bgi) },
	{ L"b", (_cpf_Bf) },
	{ L"b*", (_cpf_Bf | _cpf_fgi) },
	{ L"br", (_cpf_Bf | _cpf_Rb) },
	{ L"bb", (_cpf_Bf | _cpf_Bb) },
	{ L"bg", (_cpf_Bf | _cpf_Gb) },
	{ L"by", (_cpf_Bf | _cpf_Rb | _cpf_Gb) },
	{ L"bm", (_cpf_Bf | _cpf_Rb | _cpf_Bb) },
	{ L"bc", (_cpf_Bf | _cpf_Gb | _cpf_Bb) },
	{ L"bw", (_cpf_Bf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },
	{ L"b*r", ((_cpf_Bf | _cpf_fgi) | _cpf_Rb) },
	{ L"b*b", ((_cpf_Bf | _cpf_fgi) | _cpf_Bb) },
	{ L"b*g", ((_cpf_Bf | _cpf_fgi) | _cpf_Gb) },
	{ L"b*y", ((_cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ L"b*m", ((_cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ L"b*c", ((_cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ L"b*w", ((_cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },
	{ L"br*", ((_cpf_Bf) | (_cpf_Rb | _cpf_bgi)) },
	{ L"bb*", ((_cpf_Bf) | (_cpf_Bb | _cpf_bgi)) },
	{ L"bg*", (_cpf_Bf | (_cpf_Gb | _cpf_bgi)) },
	{ L"by*", (_cpf_Bf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ L"bm*", (_cpf_Bf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ L"bc*", (_cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ L"bw*", (_cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },
	{ L"b*r*", ((_cpf_Bf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ L"b*b*", ((_cpf_Bf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ L"b*g*", ((_cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ L"b*y*", ((_cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ L"b*m*", ((_cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ L"b*c*", ((_cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ L"b*w*", ((_cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	/*yellow*/
	{ L"y#", (_cpf_Rb | _cpf_Gb) },
	{ L"y*#", ((_cpf_Rb | _cpf_Gb) | _cpf_bgi) },
	{ L"y", (_cpf_Rf | _cpf_Gf) },
	{ L"y*", ((_cpf_Rf | _cpf_Gf) | _cpf_fgi) },
	{ L"yr", ((_cpf_Rf | _cpf_Gf) | _cpf_Rb) },
	{ L"yb", ((_cpf_Rf | _cpf_Gf) | _cpf_Bb) },
	{ L"yg", ((_cpf_Rf | _cpf_Gf) | _cpf_Gb) },
	{ L"yy", ((_cpf_Rf | _cpf_Gf) | _cpf_Rb | _cpf_Gb) },
	{ L"ym", ((_cpf_Rf | _cpf_Gf) | _cpf_Rb | _cpf_Bb) },
	{ L"yc", ((_cpf_Rf | _cpf_Gf) | _cpf_Gb | _cpf_Bb) },
	{ L"yw", ((_cpf_Rf | _cpf_Gf) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },
	{ L"y*r", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Rb) },
	{ L"y*b", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Bb) },
	{ L"y*g", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Gb) },
	{ L"y*y", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ L"y*m", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ L"y*c", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ L"y*w", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },
	{ L"yr*", (((_cpf_Rf | _cpf_Gf)) | (_cpf_Rb | _cpf_bgi)) },
	{ L"yb*", (((_cpf_Rf | _cpf_Gf)) | (_cpf_Bb | _cpf_bgi)) },
	{ L"yg*", ((_cpf_Rf | _cpf_Gf) | (_cpf_Gb | _cpf_bgi)) },
	{ L"yy*", ((_cpf_Rf | _cpf_Gf) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ L"ym*", ((_cpf_Rf | _cpf_Gf) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ L"yc*", ((_cpf_Rf | _cpf_Gf) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ L"yw*", ((_cpf_Rf | _cpf_Gf) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },
	{ L"y*r*", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ L"y*b*", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ L"y*g*", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ L"y*y*", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ L"y*m*", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ L"y*c*", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ L"y*w*", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	/*magenta*/
	{ L"m#", (_cpf_Rb | _cpf_Bb) },
	{ L"m*#", ((_cpf_Rb | _cpf_Bb) | _cpf_bgi) },
	{ L"m", (_cpf_Rf | _cpf_Bf) },
	{ L"m*", (_cpf_Rf | _cpf_Bf | _cpf_fgi) },
	{ L"mr", (_cpf_Rf | _cpf_Bf | _cpf_Rb) },
	{ L"mb", (_cpf_Rf | _cpf_Bf | _cpf_Bb) },
	{ L"mg", (_cpf_Rf | _cpf_Bf | _cpf_Gb) },
	{ L"my", (_cpf_Rf | _cpf_Bf | _cpf_Rb | _cpf_Gb) },
	{ L"mm", (_cpf_Rf | _cpf_Bf | _cpf_Rb | _cpf_Bb) },
	{ L"mc", (_cpf_Rf | _cpf_Bf | _cpf_Gb | _cpf_Bb) },
	{ L"mw", (_cpf_Rf | _cpf_Bf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },
	{ L"m*r", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Rb) },
	{ L"m*b", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Bb) },
	{ L"m*g", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Gb) },
	{ L"m*y", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ L"m*m", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ L"m*c", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ L"m*w", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },
	{ L"mr*", ((_cpf_Rf | _cpf_Bf) | (_cpf_Rb | _cpf_bgi)) },
	{ L"mb*", ((_cpf_Rf | _cpf_Bf) | (_cpf_Bb | _cpf_bgi)) },
	{ L"mg*", (_cpf_Rf | _cpf_Bf | (_cpf_Gb | _cpf_bgi)) },
	{ L"my*", (_cpf_Rf | _cpf_Bf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ L"mm*", (_cpf_Rf | _cpf_Bf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ L"mc*", (_cpf_Rf | _cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ L"mw*", (_cpf_Rf | _cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },
	{ L"m*r*", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ L"m*b*", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ L"m*g*", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ L"m*y*", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ L"m*m*", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ L"m*c*", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ L"m*w*", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	/*cyan*/
	{ L"c#", (_cpf_Gb | _cpf_Bb) },
	{ L"c*#", ((_cpf_Gb | _cpf_Bb) | _cpf_bgi) },
	{ L"c", (_cpf_Gf | _cpf_Bf) },
	{ L"c*", (_cpf_Gf | _cpf_Bf | _cpf_fgi) },
	{ L"cr", (_cpf_Gf | _cpf_Bf | _cpf_Rb) },
	{ L"cb", (_cpf_Gf | _cpf_Bf | _cpf_Bb) },
	{ L"cg", (_cpf_Gf | _cpf_Bf | _cpf_Gb) },
	{ L"cy", (_cpf_Gf | _cpf_Bf | _cpf_Rb | _cpf_Gb) },
	{ L"cm", (_cpf_Gf | _cpf_Bf | _cpf_Rb | _cpf_Bb) },
	{ L"cc", (_cpf_Gf | _cpf_Bf | _cpf_Gb | _cpf_Bb) },
	{ L"cw", (_cpf_Gf | _cpf_Bf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },
	{ L"c*r", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb) },
	{ L"c*b", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Bb) },
	{ L"c*g", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb) },
	{ L"c*y", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ L"c*m", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ L"c*c", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ L"c*w", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },
	{ L"cr*", ((_cpf_Gf | _cpf_Bf) | (_cpf_Rb | _cpf_bgi)) },
	{ L"cb*", ((_cpf_Gf | _cpf_Bf) | (_cpf_Bb | _cpf_bgi)) },
	{ L"cg*", (_cpf_Gf | _cpf_Bf | (_cpf_Gb | _cpf_bgi)) },
	{ L"cy*", (_cpf_Gf | _cpf_Bf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ L"cm*", (_cpf_Gf | _cpf_Bf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ L"cc*", (_cpf_Gf | _cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ L"cw*", (_cpf_Gf | _cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },
	{ L"c*r*", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ L"c*b*", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ L"c*g*", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ L"c*y*", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ L"c*m*", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ L"c*c*", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ L"c*w*", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	/*white*/
	{ L"w#", (_cpf_Rb | _cpf_Gb | _cpf_Bb) },
	{ L"w*#", (_cpf_Rb | _cpf_Gb | _cpf_Bb) | _cpf_bgi },
	{ L"w", ((_cpf_Rf | _cpf_Gf | _cpf_Bf)) },
	{ L"w*", ((_cpf_Rf | _cpf_Gf | _cpf_Bf) | _cpf_fgi) },
	{ L"wr", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb) },
	{ L"wb", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Bb) },
	{ L"wg", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb) },
	{ L"wy", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ L"wm", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ L"wc", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ L"ww", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },
	{ L"w*r", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Rb) },
	{ L"w*b", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Bb) },
	{ L"w*g", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Gb) },
	{ L"w*y", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ L"w*m", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ L"w*c", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ L"w*w", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },
	{ L"wr*", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi)) | (_cpf_Rb | _cpf_bgi)) },
	{ L"wb*", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi)) | (_cpf_Bb | _cpf_bgi)) },
	{ L"wg*", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ L"wy*", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ L"wm*", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ L"wc*", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ L"ww*", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },
	{ L"w*r*", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ L"w*b*", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ L"w*g*", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ L"w*y*", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ L"w*m*", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ L"w*c*", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ L"w*w*", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) }

};

#else /*#ifdef _WIN32*/
//http://www.linuxhomenetworking.com/forums/showthread.php/1095-Linux-console-Colors-And-Other-Trick-s
//http://stackoverflow.com/questions/3506504/c-code-changes-terminal-text-color-how-to-restore-defaults-linux
//http://linuxgazette.net/issue65/padala.html
//http://misc.flogisoft.com/bash/tip_colors_and_formatting
//http://man7.org/linux/man-pages/man4/console_codes.4.html
//http://www.linuxquestions.org/questions/programming-9/get-cursor-position-in-c-947833/
//http://ascii-table.com/ansi-escape-sequences-vt-100.php
//http://invisible-island.net/xterm/ctlseqs/ctlseqs.html
//http://ispltd.org/mini_howto:ansi_terminal_codes

/*<ESC>[{attr};{fg};{bg}m*/
#define REG_COLOUR_VALUES(c, i) \
{	L"" #c,			L"\x1B[0;0;3"#i"m" },\
{	L"" #c"*",		L"\x1B[0;0;10"#i"m" },\
{	L"" #c"#",		L"\x1B[0;0;4"#i"m" },\
{	L"" #c"*#",		L"\x1B[0;30;10"#i"m" },\
{	L"" #c "r",		L"\x1B[0;3"#i";41m" },\
{	L"" #c "g",		L"\x1B[0;3"#i";42m" },\
{	L"" #c "b",		L"\x1B[0;3"#i";44m" },\
{	L"" #c "y",		L"\x1B[0;3"#i";43m" },\
{	L"" #c "m",		L"\x1B[0;3"#i";45m" },\
{	L"" #c "c",		L"\x1B[0;3"#i";46m" },\
{	L"" #c "w",		L"\x1B[0;3"#i";47m" },\
\
{	L"" #c "*",		L"\x1B[0;0;9"#i"m" },\
{	L"" #c "*r",	L"\x1B[0;9"#i";41m" },\
{	L"" #c "*g",	L"\x1B[0;9"#i";42m" },\
{	L"" #c "*b",	L"\x1B[0;9"#i";44m" },\
{	L"" #c "*y",	L"\x1B[0;9"#i";43m" },\
{	L"" #c "*m",	L"\x1B[0;9"#i";45m" },\
{	L"" #c "*c",	L"\x1B[0;9"#i";46m" },\
{	L"" #c "*w",	L"\x1B[0;9"#i";47m" },\
\
{	L"" #c "r*",	L"\x1B[0;3"#i";101m" },\
{	L"" #c "g*",	L"\x1B[0;3"#i";102m" },\
{	L"" #c "b*",	L"\x1B[0;3"#i";104m" },\
{	L"" #c "y*",	L"\x1B[0;3"#i";103m" },\
{	L"" #c "m*",	L"\x1B[0;3"#i";105m" },\
{	L"" #c "c*",	L"\x1B[0;3"#i";106m" },\
{	L"" #c "w*",	L"\x1B[0;3"#i";107m" },\
\
{	L"" #c "*r*",	L"\x1B[0;9"#i";101m" },\
{	L"" #c "*g*",	L"\x1B[0;9"#i";102m" },\
{	L"" #c "*b*",	L"\x1B[0;9"#i";104m" },\
{	L"" #c "*y*",	L"\x1B[0;9"#i";103m" },\
{	L"" #c "*m*",	L"\x1B[0;9"#i";105m" },\
{	L"" #c "*c*",	L"\x1B[0;9"#i";106m" },\
{	L"" #c "*w*",	L"\x1B[0;9"#i";107m" },\

extern const std::map<const cpf::type::str, cpf::type::colour> cpf::std_token_vals{

	/*attributes specifiers*/
	{	L"bld", 		L"\x1B[1m"},
	{	L"dim", 		L"\x1B[2m"},
	{	L"uln", 		L"\x1B[4m"},
	{	L"blk", 		L"\x1B[5m"},
	{	L"rvs", 		L"\x1B[7m"},
	{	L"hid", 		L"\x1B[8m"},
	{	L"?bld", 	L"\x1B[21m"},
	{	L"?dim", 	L"\x1B[22m"},
	{	L"?uln", 	L"\x1B[24m"},
	{	L"?blk", 	L"\x1B[25m"},
	{	L"?rvs", 	L"\x1B[27m"},
	{	L"?hid", 	L"\x1B[28m"},

	/*colours*/
	REG_COLOUR_VALUES(r, 1)
	REG_COLOUR_VALUES(g, 2)
	REG_COLOUR_VALUES(b, 4)
	REG_COLOUR_VALUES(y, 3)
	REG_COLOUR_VALUES(m, 5)
	REG_COLOUR_VALUES(c, 6)
	REG_COLOUR_VALUES(w, 7)

};

#endif /*#ifdef _WIN32*/


