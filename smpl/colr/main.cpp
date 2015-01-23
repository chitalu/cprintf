#include "cprintf/cprintf.hpp"

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
		cprintf("note that $bld this text is blue\n");
#else
		cprintf("quux $64f xterm bitmap colour\n");
		cprintf("quux $64b xterm bitmap colour\n");
		cprintf("quux $128f.64b xterm bitmap colour\n");
		cprintf("quux $64& xterm bitmap colour\n");

		cprintf("note that $bld this text is bold\n");
		cprintf("note that $b`ld this text is blue\n");
#endif

		int count = 0;
		for (auto i : cpf::intern::std_tokens)
		{
			auto s = cpf::type::str(i);

			cwprintf((L"$" + s + L"%s" + L"$?\t").c_str(), i);

			if ((++count % 8) == 0)
				printf("\n");
		}
	}
	catch (cpf::type::except &e)
	{
		fwprintf(stderr, L"cprintf sample failed\n%s\n", e.msg());
		exec_ok = false;
	}

	return exec_ok ? EXIT_SUCCESS : EXIT_FAILURE;
}