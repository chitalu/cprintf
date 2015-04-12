#include "cprintf/cprintf.hpp"

int main(void)
{
	/*
		The use cprintf tokens means that some characters cannot just be printed without 
		being escaped. More generally, only the Pesa �$� needs to be escaped. All other "secondary"
		characters (see below) need only be escaped if they are placed/located immediately after 
		the occurance of a cprintf token.
		Escaping is achieved by the using the backtick �`�. Standard escape sequences such as newline, 
		tab etc. are the same as the stardard functions.

		primary character, signifies the occurance of a cprintf token in a given format string

		"`$"

		secondary characters, these are positioned immidietley after the pesa. If their occurance
		is after a user-specified token and the user wishes for them not to interfere with this
		preceeding tokens representative value, they must be escaped.

		"`r", "`g", "`b", "`y",
		"`m", "`c", "`w", "`.",
		"`*", "``", "`?" "`!",
		"`~", "`|", "`f", "`l"
	*/
	
	//last two characters are printed with bright red foreground colour 
	//and a dim red background
	cprintf(L"bar $r*red\n");

	//last two characters are printed with bright red foreground alone 
	//with the background colour being whatever console settings are.
	//in effect, the background colour remains unmodified
	cprintf(L"bar $r*`red\n");

	//"ed" printed with bright red applied to both foreground and background.
	//no escaping needed after the last asterisk (*) because "e" in not a 
	//recognised "secondary" token defining character value.
	cprintf(L"bar $r*r*ed\n");

	//another example of foreground and background colour setting
	//notice that we dont need to escape "Red" like $r*w*`Red? well this is because
	//the aforementioned "secondary" characters are parsed in a case senstive manner.
	//only those given above will need to be escaped if the occur immidiately after a
	//token. Example: cprintf("bar $r*w*red on white\n"); would confuse the parser
	//but cprintf("bar $r*w*`red on white\n"); would give desired results.
	cprintf(L"bar $r*w*Red on white\n");

	//simply dim red background 
	cprintf(L"bar $r#ed\n");

	//simply bright red background 
	cprintf(L"bar $r*#ed\n");

	//secondary character "#" is escaped, meaning it is printed as normal
	//using bright red as foreground along with all character [after] token.
	cprintf(L"bar $r*`#ed\n");

	//dot notation
	//remember cprintf token configurations function like a state machine (for some tokens),
	//the last colour set below (blue) takes effect eleminating the red that was set prior
	cprintf(L"quux$r.b blue\n");

	//same as above but with brightness involved
	cprintf(L"quux$b*.r red\n");
	cprintf(L"quux$b*.r* red\n");
	cprintf(L"quux$r.b* blue\n");

	//raw string literals can also be used
	auto s = R"rsl(raw $ystring lateral$? %s
)rsl";
	cprintf(s, "foo");
	cprintf("as argument: %s", s);

	//wide character
	auto ws = LR"rsl(raw $ystring lateral$? %s
)rsl";
	cprintf(ws, L"foo");
	cprintf(L"as argument: %s", ws);

	//printing types std::string and std::wstring in colour.
	//again, same as before
	std::string std_str = "yey! $g*Go me!! %d\n";
	cprintf(std_str, std_str.size());
	cprintf("as argument: %s", std_str);

	std::wstring std_wstr = L"yey! $g*Go me!! %d\n";
	cprintf(std_wstr, std_wstr.size());
	cprintf(L"as argument: %s", std_wstr);

	/*
		linux only text-format attribute tokens:

		bold, dim, blink, reverse, hidden
	*/
#ifdef _WIN32
	//it is not an error to use text format tokens on windows
	//they are simply ignored by the internal parser
	//in this case "bld" prints the text in blue because $b
	//is obviously recognised as a token for dim blue foreground
	//on linux this printf following text in bold
	cprintf(L"note that $bld this text is blue\n");
#else
	//the following are bitmap colour tokens available on XTERM
	//as can be imagine there are alot more options for decorating 
	//text on linux than windows
	cprintf(L"quux $64f xterm bitmap colour\n");
	cprintf(L"quux $64b xterm bitmap colour\n");

	//in this respect the dot notation does not affect internal 
	//console state, as such bitmap-colour foreground and background 
	//may be specified together.
	//alternative would be: "quux $128f$64b xterm..."
	//dot notation means you could also add other tokens like this:
	//cprintf("quux $m.64b.bld.8,16 xterm\n");
	//setting colour, text format and cursor position at the same time!
	cprintf(L"quux $128f.64b xterm bitmap colour\n");

	//applies bitmap colour to both foreground and background
	cprintf(L"quux $64& xterm bitmap colour\n");

	cprintf(L"note that $bld this text is bold\n");//unlike on windows :P

	//notice the difference with the call directly above, to print "ld" in 
	//blue, 'l' must be escape to prevent ambiguity in which text-format
	//tokens take precedence over colour
	cprintf(L"note that $b`ld this text is blue\n");

	//reverting text format token effects mid format string
	cprintf(L"drive forward and $rvs reverse $?rvsand the back to normal\n");
#endif
	//finally, here we simply display all available cross platform colours
	//have a look at the colourful console output! Better than the dullness you 
	//get everytime isnt it? (yeah)
	int count = 0;
	for (auto i : cpf::intern::std_tokens)
	{
		auto s = cpf::type::str(i);

		cprintf((L"$" + s + L"%s" + L"$?\t").c_str(), i);

		if ((++count % 8) == 0)
			printf("\n");
	}

	return 0;
}