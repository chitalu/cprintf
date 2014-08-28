#include "cprintf/cprintf.h"

#include <thread>
#include <chrono>

void progress_bar(void)
{
	std::size_t c = 0; float m = 20.0f;
	std::string bar;
	while(++c <= m)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		auto progress = (float)c/m * 100;
		if((((((int)progress/ 100) * int(m)))  % 5) == 0)
		{
			bar.append("#");
		}
		auto downloaded = ((float)c/m) * 512.0f;
		auto data = std::make_tuple("some-big-file", downloaded, progress, 512.0f - downloaded, bar.c_str());
		cprintf_t("$0,2`filename: $c*%s$?\ndownloaded: %.2f (%.2f%%)\nremaining: %.2f\nprogress: $gg%s", data);
	}
}

#include <random>

void rand_colours(void)
{
	std::vector<char> colours = {'r', 'g', 'b', 'y', 'm', 'c', 'w'};
	for (auto i(0); i < 500; ++i)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(40));

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(97, 122);
		std::uniform_int_distribution<> h_(0, 20);
		std::uniform_int_distribution<> w_(0, 20);
		std::uniform_int_distribution<> c_(0, 6);

		char charachter = dis(gen);
		unsigned char h = h_(gen);
		unsigned char w = w_(gen);

		char buf[512];
#ifdef _WIN32
		sprintf_s(
#else
		sprintf(
#endif
			buf, "$%c*.$%d,%d %c", colours[c_(gen)], w, h, charachter);
		cprintf(buf);
	}
}

void args_and_colours(void)
{
	cprintf("foo $r%s\n", "red");
	cprintf("bar $r*%s\n", "red");
	cprintf("quux$g %s\n", "green");
	cprintf("$b%s\n", "blue");
	cprintf("$b*%s\n", "blue");
}

void buffer_wipe(void)
{
	cprintf("screen$r buffer $?.!`clear and reset colour\n");
	cprintf("screen$r buffer $!`clear\n");
	cprintf("screen$r buffer $?` reset colour\n");

	cprintf("screen $r buffer $!~ clear but retain cursor position\n");
}

void lull_token(void)
{
	/*
	lull token

	this does not necesarilly modify the proceeding-character's
	attributes. Instead it halts printing of the format
	string until keyboard input is recieved.
	In effective, the format string (from first character to the lull
	token occurance) is printed as normal until a lull token is encountered,
	which is the point at which the cprintf waits until keypress/ input
	is recieved.
	*/

	cprintf("wait for keypress $|printing rest of string...\n");
	cprintf("wait for keypress$|.r`rest...\n");
	cprintf("wait for keypress $r.|`rest...\n");
}

int main(void)
{
	bool exec_ok = true;

	try
	{
		args_and_colours();
		std::getchar();
		cprintf("$!");
		progress_bar();
		std::getchar();
		cprintf("$!");
		rand_colours();
		std::getchar();
		cprintf("$!");
		lull_token();
	}
	catch (_cpf_type::error &e)
	{
		cfprintf(stderr, "$0,0`cprintf sample failed\n%s\n", e.what());
		exec_ok = false;
	}
	return exec_ok ? EXIT_SUCCESS : EXIT_FAILURE;
}