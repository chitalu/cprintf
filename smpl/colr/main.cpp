#include "cprintf/cprintf.h"

auto tokens = {
	/*default*/
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
};

int main(void)
{
	bool exec_ok = true;

	try
	{
		

		cprintf("bar $r*red\n");
		cprintf("bar $r*r*ed\n");
		cprintf("bar $r#ed\n");
		cprintf("bar $r*#ed\n");
		cprintf("quux$r.b blue\n");
		cprintf("quux$b*.r red\n");
		cprintf("quux$b*.r* red\n");
		cprintf("quux$r.b* blue\n");

#ifdef _WIN32
		cprintf("quux $64f xterm bitmap colour is ignored\n");
		cprintf("quux $64b xterm bitmap colour is ignored\n");
		cprintf("quux $128f.16b xterm bitmap colour is ignored\n");
		cprintf("quux $64& xterm bitmap colour is ignored\n");

		cprintf("note that $bld this text is blue\n");
#else
		cprintf("quux $64f xterm bitmap colour\n");
		cprintf("quux $64b xterm bitmap colour\n");
		cprintf("quux $128f.64b xterm bitmap colour\n");
		cprintf("quux $64& xterm bitmap colour\n");

		cprintf("note that $bld this text is bold\n");
#endif

		std::getchar();

		int count = 0;
		for (auto i : tokens)
		{
			auto s = std::string(i);
			cprintf(("$" + s + "%s" + "$?\t").c_str(), i);
			if ((++count % 8) == 0)
			{
				printf("\n");
			}
		}
	}
	catch (_cpf_type::error e)
	{
		fprintf(stderr, "cprintf sample failed\n%s\n", e.what());
		exec_ok = false;
	}

	return exec_ok ? EXIT_SUCCESS : EXIT_FAILURE;
}