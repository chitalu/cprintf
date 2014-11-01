# Welcome to the cprintf Project #

_"What is this cprintf", you ask?_ 

_Well..._ 

First of all, "cprintf" is an umbrella name used to encapsulate a group of C++(11/0x) language 
functions for extended formatted-printing. 
The purpose to provide a thin auxiallary layer atop that of std::<f|w>printf. Most functionality 
works, in much a similar manner, to that of its maternal counter-part(s) i.e printf, fprintf 
etc. 

Aside from guarranteed type-safety (unlike that of it predecesors) cprintf also introduces 
the feature of text-colour specification ($...) for output directed to a terminal. 
This functionality effectively permits users the ability to specify--as part of the format 
string--the colour of all (or some) of the format-string's characters.

In taking advantage of variadic templates, the library's interface also provides functionality 
that enables users to opt for the use of a Tuple object as function input. This Tuple then acts 
as the container for the to-be-printed arguments who's elements are ordered in a corresponding 
layout with format specifiers (%...) in the format string.  

###Depedencies:

CMake 2.8
Git 1.8

**Window:**

* Microsoft (R) C/C++ Optimizing Compiler Version 18.00.21005.1 (Visual Studio 2013)
 
**Linux**

* GNU Compiler Collection 4.8

_Note_
Users are advised to keep note of the fact that this project is still in premature early 
stages of development, and as such it should be expected that changes (sometimes drastic) 
will be made to the current implementation.

Also, the feature-set available on the Microsoft Windows Operating System is limited in 
comparison to that of the Linux Operating Systems (subject to terminal). Users wishing
to target older hardware interfaces may be required to limit their use of features 
available for compatilibity.

The cross-platform feature set guarantees the availability of 7 colours that may be specified 
for text on both Windows and Linux terminals.

These are:
* RED
* GREEN
* BLUE
* YELLOW
* CYAN
* MAGENTA
* WHITE

What this means is that text colour (also known as foreground-colour) can be be set to any of 
these value and vice-versa with respect to background. Combinations may also be used i.e RED 
foreground on a BLUE background colour.

> Some--typically older--terminals may not support all of these colours and as such the user 
is advised to check documentation for best results. From basic tests, most (recent) consoles 
are fine.
For more info, refer to: http://en.tldp.org/HOWTO/Text-Terminal-HOWTO.html

**API Overview**

The following is be a brief walk-through on how to use library and its interface. We will explore 
the use Attribute Specifiers (i.e not just those beginning with %..), how to use them, and qlso 
some other helpful library features that may assist in productivity and other aspects.

## Cross-Platform Tokens

Before proceeding delving, the reader should note that a "token" in the context of cprintf is 
a _symbolic sub-string_ that forms an intrinsic sub-section of the format string and is one 
which is responsible for modifying all characters proceeding its positional offset in the format 
string (until the end or another is specified). 
These "symbolic token" values may also (for clarity) be referred to as _Attribute Specifiers (AS)_ 
since they, in-effect, are constructs where-by a text's appearance may-be distinguished, via 
their specification, from another.

Argument specification and processing is achieved in the same manner as std::<f>printf where
format specifiers are denoted by the percent sign (%). On the other hand, AS's are notably 
denoted--within the format-string--by a _peso_ ($) followed by a value representing the attribute(s) 
with-which subsequent characters following its occurrence are to be printed with.

### Colour Tokens

The following tokens are specified as part of a format string in order to modify colour of 
subsequent characters in a format string:

```
	/*default i.e used to revert last set AS to that which it was prior to invoking library API*/
	"?",

	/*dim text colour no background*/
	"r", "g", "b", "y", "m", "c", "w",

	/*dim background*/
	"r#", "g#", "b#", "y#", "m#", "c#", "w#",

	/*bright text colour no background*/
	"r*", "g*", "b*", "y*", "m*", "c*", "w*",

	/*bright background*/
	"r*#", "g*#", "b*#", "y*#", "m*#", "c*#", "w*#",

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
```

## Linux-Specific Tokens

### 256-Range Colour

It’s not commonly used, but most Linux terminals can support 256 colours. cprintf takes advantage 
of this by allowing users to attribute their terminal output with such. And if used effectively 
this can produce impressive results.
Once again, this covers foreground, background colour specification. Combinations are also permitted.

The Linux-specific colour token syntax is as follows:

```
"0-256f" "0-256b" & "0-256&"
```

The first indicates the foreground colour applied to the text; the second to the background; and 
the third applies the same colour to both fore and background colour for the subsequent characters 
in a format string.

Below are some examples:

```
cprintf("qwerty $24f baroo\n");
cprintf("ytrewq $75b foo\n");
cprintf("ewqrty $128& foo\n");

/*
  note: dot notation - used to combine multiple attributes...
*/
cprintf("hey hey hey $75f.56b bar\n");
```

### Font-Appearance Modifying Tokens

On Linux, cprintf also features the most commonly supported control sequences for modifying text 
font appearance. Of-course, their support depends on the used terminal (check dicumentation of yours). 

These are:
* Bold/Bright,
* Dim,
* Underlined,
* Blink, 
* Reverse,
* Hidden

Below are their symbolic string representations:
```
/*
  enablers
  [bold, dim, underline, reverse-colour, hidden]
*/
"bld", "dim", "uln", "blk", "rvs", "hid"	

/*
  disablers
*/
"?bld",	"?dim",	"?uln",	"?blk",	"?rvs",	"?hid",
```

Examples:

```
cprintf("foo $bld bar is bold!\n");
cprintf("foo $rvs bar $?rvs end reverse colour\n");
```

##Cursor Position

Aside from permitting users the ability to specify the format and colour of text printed 
within a terminal, cprintf also allows users (via the format string) to specify the cursor
position upon which the subsequent text shall be printed. The syntax adopts the 
Cartesian-coordinate form.

Below are examples:

```
/*
  note "hello world " will be printed at [current] cursor position
*/
cprintf("hello world $15,15and again");
cprintf("all $15,7over $4,7the$10,10place");

/*
  mixing colour and position
*/
cprintf("hello world $15,15 and $r*again");
```
## Escape Tokens

Where the standard printf functions, provide a mechanism to "escape" characters using the 
backslash "\", cprintf also offers this functionality to users. This is achieved by prefixing
[all pesa occurances] with a backtick "`" (that is tokens that need be escaped and treated as
normal by the internal parser).

```
cprintf("i want to escape this `$r* token");
cprintf("do stuff `$ back tick ``");

/*
  "special" characters immidietly proceeding a token need be escaped
  to prevent undesired out (and possible and error).
  the following would [otherwise] print yhe string "olor" in red on a
  cyan background.
*/
cprintf("hello $r*`color");
```

## Error Handling

All runtime errors are handled via exceptions of type ```cpf::type::except```.
This includes format string syntax errors.

### Argument verification.

In addition to the permitted arguments to std::printf, cprintf also supports user ability
to print types ```std::string``` and ```std::wtring```.

```
std::string s("C++0x");
cprintf("i love %s\n", s);

std::wstring ws("C++0x");
cwprintf("i love %s\n", ws);
```

Specification of arguments of a non-permitted type shall result in a compile-time error.

## Specifying Arguments to cprintf

This is very much the same as C++ standard std::<f|w>printf with added support for the use of 
a Tuple who elements are the used as the arguments to teh API.

Examples:

```
/*
  create tuple (saved args for later (re)use)
*/
auto args = std::make_tuple(23, "you", 'a', 21.8f);
cprintf_t(" all args go here:  %d %s %c %f", args);

/*
  modify single elemnt and reprint
*/
std::get<3>(args) = 1.8f;
cprintf_f(" all args go here:  %d %s %c %f", args);

/*
  print single argument
*/
cprintf(foos the bars %g\n", std::get<3>(args));

/*
  print tuple elements with embedded AS
  1.  apply dim green foreground colour on red
  2.  change cursor position to 4 character in terminal buffer
      on x-axis and 7 on y-axis
  3.  restore colour		
  4.  apply bright red and change position to origin before 
      printing last two arguments
*/
cprintf_f("all args $g.r`go here$?:  %d %s $4,7%c $r*.0,0%f", args);
```

## Debugging Assistants

The API also provides auxiliary macros for debugging purposes. All output is streamed to 
standard error. Users may use these just as they would cprintf, cfprintf, cprintf_t, and cfprintf_t. 
Permissions and limitations imposed reflect those of the aforementioned. Macro expansion will only 
occur in client debug builds (when NDEBUG is undefined) and non else. And as such, building in 
release mode results in the macros expanding to [nothing], rendering a client call impotent.

```
cfprintf_dbg(strm, format, ...) 
cprintf_dbg(format, ...) 
cfprintf_t_dbg(strm, format, tup) 
cprintf_t_dbg(format, tup) 
```

I hope this will be useful to you!

Please leave feedback and I will be more than happy to consider improvements to the library.
I may however take a while to implement new changes due to time constrains. 
CONTRIBUTORS WANTED!

Thank you

- Floyd
