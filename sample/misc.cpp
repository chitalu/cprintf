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
		cprintf_t("/0,10]filename: /c*]%s/!]\ndownloaded: %.2ff (%.2f%%)\nremaining: %.2f\nprogress: /g*g*]%s", data);
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
		std::uniform_int_distribution<> w_(0, 80);
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

void standard_tokens(void)
{
	auto tokens = {
		/*default*/
		"?",

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
	};
	int count = 0;
	for (auto i : tokens)
	{
		cprintf(std::string("$").append(i).append("%s").append("$?\t").c_str(), i);
	    if((++count % 8) == 0)
	    {
	        printf("\n");
	    }
	}
	printf("\n\n");
}

int main(void)
{
	bool exec_ok = true;

	try
	{
		standard_tokens();
		std::getchar();
		args_and_colours();
		std::getchar();
		progress_bar();
		std::getchar();
		rand_colours();
	}
	catch (_cpf_type::error &e)
	{
		fprintf(stderr, "cprintf sample failed\n%s\n", e.what());
		exec_ok = false;
	}
	return exec_ok ? EXIT_SUCCESS : EXIT_FAILURE;
}