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

/*
	no harm in explicitly defining token string laterals here,
	using macros below is bad enough figuring out what is happening
	in the code.
*/
CPF_API const cpf::type::string_vector cpf::intern::std_tokens = {
	/*
		dim text colour no background
	*/
	L"r", L"g", L"b", L"y", L"m", L"c", L"w",

	/*
		dim background
	*/
	L"r#", L"g#", L"b#", L"y#", L"m#", L"c#", L"w#",

	/*
		bright text colour no background
	*/
	L"r*", L"g*", L"b*", L"y*", L"m*", L"c*", L"w*",

	/*
		bright background
	*/
	L"r*#", L"g*#", L"b*#", L"y*#", L"m*#", L"c*#", L"w*#",

	/*
		dim text and background colour
	*/
	L"rr", L"rb", L"rg", L"ry", L"rm", L"rc", L"rw", /*red*/
	L"gg", L"gb", L"gr", L"gy", L"gm", L"gc", L"gw", /*green*/
	L"bb", L"br", L"bg", L"by", L"bm", L"bc", L"bw", /*blue*/
	L"yy", L"yb", L"yg", L"yr", L"ym", L"yc", L"yw", /*yellow*/
	L"mm", L"mr", L"mg", L"my", L"mb", L"mc", L"mw", /*magenta*/
	L"cc", L"cr", L"cg", L"cy", L"cm", L"cb", L"cw", /*cyan*/
	L"ww", L"wr", L"wg", L"wy", L"wm", L"wc", L"wb", /*white*/

	/*
		bright text colour and dim background colour
	*/
	L"r*r", L"r*b", L"r*g", L"r*y", L"r*m", L"r*c", L"r*w", /*red*/
	L"g*g", L"g*b", L"g*r", L"g*y", L"g*m", L"g*c", L"g*w", /*green*/
	L"b*b", L"b*r", L"b*g", L"b*y", L"b*m", L"b*c", L"b*w", /*blue*/
	L"y*y", L"y*b", L"y*g", L"y*r", L"y*m", L"y*c", L"y*w", /*yellow*/
	L"m*m", L"m*r", L"m*g", L"m*y", L"m*b", L"m*c", L"m*w", /*magenta*/
	L"c*c", L"c*r", L"c*g", L"c*y", L"c*m", L"c*b", L"c*w", /*cyan*/
	L"w*w", L"w*r", L"w*g", L"w*y", L"w*m", L"w*c", L"w*b", /*white*/

	/*
		dim text colour and bright background colour
	*/
	L"rr*", L"rb*", L"rg*", L"ry*", L"rm*", L"rc*", L"rw*", /*red*/
	L"gg*", L"gb*", L"gr*", L"gy*", L"gm*", L"gc*", L"gw*", /*green*/
	L"bb*", L"br*", L"bg*", L"by*", L"bm*", L"bc*", L"bw*", /*blue*/
	L"yy*", L"yb*", L"yg*", L"yr*", L"ym*", L"yc*", L"yw*", /*yellow*/
	L"mm*", L"mr*", L"mg*", L"my*", L"mb*", L"mc*", L"mw*", /*magenta*/
	L"cc*", L"cr*", L"cg*", L"cy*", L"cm*", L"cb*", L"cw*", /*cyan*/
	L"ww*", L"wr*", L"wg*", L"wy*", L"wm*", L"wc*", L"wb*", /*white*/

	/*
		bright text and background colour
	*/
	L"r*r*", L"r*b*", L"r*g*", L"r*y*", L"r*m*", L"r*c*", L"r*w*", /*red*/
	L"g*g*", L"g*b*", L"g*r*", L"g*y*", L"g*m*", L"g*c*", L"g*w*", /*green*/
	L"b*b*", L"b*r*", L"b*g*", L"b*y*", L"b*m*", L"b*c*", L"b*w*", /*blue*/
	L"y*y*", L"y*b*", L"y*g*", L"y*r*", L"y*m*", L"y*c*", L"y*w*", /*yellow*/
	L"m*m*", L"m*r*", L"m*g*", L"m*y*", L"m*b*", L"m*c*", L"m*w*", /*magenta*/
	L"c*c*", L"c*r*", L"c*g*", L"c*y*", L"c*m*", L"c*b*", L"c*w*", /*cyan*/
	L"w*w*", L"w*r*", L"w*g*", L"w*y*", L"w*m*", L"w*c*", L"w*b*" /*white*/

#ifndef _WIN32 
	/*
		linux only text attribute tokens:

		bold, dim, blink, reverse, hidden
	*/
	, L"bld", L"dim", L"uln", L"blk",	L"rvs",	L"hid",	
	L"?bld",	L"?dim",	L"?uln",	L"?blk",	L"?rvs",	L"?hid",
#endif /*#ifndef _WIN32*/
};

/*
	The macro "REG_COLOUR_VALUES" is used to reduce code repetition. 
	Though this does hinder readability a little.

	To understand what this macro expands to please make note of the fact that 
	"cpf::intern::std_token_vals" is a std::map. The [key] is a string lateral for tokens i.e "r" or "bw*"
	The [value] represents an OS-specific value used to configure the system terminal 
	i.e on windows this would be a value like "FOREGROUND_GREEN" and on linux it would 
	be some arbtrary escape sequence corresponding to the [key] token.

	macro expansion is slightly different for Unix and Windows
*/

#ifdef _WIN32

#define REG_COLOUR_VALUES(c, val_f, val_b) \
{ L"" L ## c L"#", (val_b) },\
{ L"" L ## c L"*#", (val_b | _cpf_bgi) },\
{ L"" L ## c, (val_f) },\
{ L"" L ## c L"r", (val_f | _cpf_Rb) },\
{ L"" L ## c L"b", (val_f | _cpf_Bb) },\
{ L"" L ## c L"g", (val_f | _cpf_Gb) },\
{ L"" L ## c L"y", (val_f | _cpf_Yb) },\
{ L"" L ## c L"m", (val_f | _cpf_Mb) },\
{ L"" L ## c L"c", (val_f | _cpf_Cb) },\
{ L"" L ## c L"w", (val_f | _cpf_Wb) },\
{ L"" L ## c L"*", (val_f | _cpf_fgi) },\
{ L"" L ## c L"*r", ((val_f | _cpf_fgi) | _cpf_Rb) },\
{ L"" L ## c L"*b", ((val_f | _cpf_fgi) | _cpf_Bb) },\
{ L"" L ## c L"*g", ((val_f | _cpf_fgi) | _cpf_Gb) },\
{ L"" L ## c L"*y", ((val_f | _cpf_fgi) | _cpf_Yb) },\
{ L"" L ## c L"*m", ((val_f | _cpf_fgi) | _cpf_Mb) },\
{ L"" L ## c L"*c", ((val_f | _cpf_fgi) | _cpf_Cb) },\
{ L"" L ## c L"*w", ((val_f | _cpf_fgi) | _cpf_Wb) },\
{ L"" L ## c L"r*", (val_f | _cpf_Rb_i) },\
{ L"" L ## c L"b*", (val_f | _cpf_Bb_i) },\
{ L"" L ## c L"g*", (val_f | _cpf_Gb_i) },\
{ L"" L ## c L"y*", (val_f | _cpf_Yb_i) },\
{ L"" L ## c L"m*", (val_f | _cpf_Mb_i) },\
{ L"" L ## c L"c*", (val_f | _cpf_Cb_i) },\
{ L"" L ## c L"w*", (val_f | _cpf_Wb_i) },\
{ L"" L ## c L"*r*", ((val_f | _cpf_fgi) | _cpf_Rb_i) },\
{ L"" L ## c L"*b*", ((val_f | _cpf_fgi) | _cpf_Bb_i) },\
{ L"" L ## c L"*g*", ((val_f | _cpf_fgi) | _cpf_Gb_i) },\
{ L"" L ## c L"*y*", ((val_f | _cpf_fgi) | _cpf_Yb_i) },\
{ L"" L ## c L"*m*", ((val_f | _cpf_fgi) | _cpf_Mb_i) },\
{ L"" L ## c L"*c*", ((val_f | _cpf_fgi) | _cpf_Cb_i) },\
{ L"" L ## c L"*w*", ((val_f | _cpf_fgi) | _cpf_Wb_i) }

const std::map<const cpf::type::str, cpf::type::colour> cpf::intern::std_token_vals{
	/*
		red
	*/
	REG_COLOUR_VALUES("r", _cpf_Rf, _cpf_Rb),

	/*
		green
	*/
	REG_COLOUR_VALUES("g", _cpf_Gf, _cpf_Gb),

	/*
		blue
	*/
	REG_COLOUR_VALUES("b", _cpf_Bf, _cpf_Bb),

	/*
		yellow
	*/
	REG_COLOUR_VALUES("y", _cpf_Yf, _cpf_Yb),

	/*
		magenta
	*/
	REG_COLOUR_VALUES("m", _cpf_Mf, _cpf_Mb),

	/*
		cyan
	*/
	REG_COLOUR_VALUES("c", _cpf_Cf, _cpf_Cb),

	/*
		white
	*/
	REG_COLOUR_VALUES("w", _cpf_Wf, _cpf_Wb)
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
{	L"" #c "*w*",	L"\x1B[0;9"#i";107m" }

extern const std::map<const cpf::type::str, cpf::type::colour> cpf::intern::std_token_vals{

	/*attributes specifiers*/
	{	L"bld", 	L"\x1B[1m"},
	{	L"dim", 	L"\x1B[2m"},
	{	L"uln", 	L"\x1B[4m"},
	{	L"blk", 	L"\x1B[5m"},
	{	L"rvs", 	L"\x1B[7m"},
	{	L"hid", 	L"\x1B[8m"},
	{	L"?bld", 	L"\x1B[21m"},
	{	L"?dim", 	L"\x1B[22m"},
	{	L"?uln", 	L"\x1B[24m"},
	{	L"?blk", 	L"\x1B[25m"},
	{	L"?rvs", 	L"\x1B[27m"},
	{	L"?hid", 	L"\x1B[28m"},

	/*
		red
	*/
	REG_COLOUR_VALUES(r, 1),

	/*
		green
	*/
	REG_COLOUR_VALUES(g, 2),

	/*
		blue
	*/
	REG_COLOUR_VALUES(b, 4),

	/*
		yellow
	*/
	REG_COLOUR_VALUES(y, 3),

	/*
		magenta
	*/
	REG_COLOUR_VALUES(m, 5),

	/*
		cyan
	*/
	REG_COLOUR_VALUES(c, 6),

	/*
		white
	*/
	REG_COLOUR_VALUES(w, 7)
};

#endif /*#ifdef _WIN32*/


