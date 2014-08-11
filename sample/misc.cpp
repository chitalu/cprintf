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
		if(((((int)progress/ 100) * int(m)))  % 5) == 0)
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
		sprintf(buf, "$%c*.$%d,%d %c", colours[c_(gen)], w, h, charachter);
		cprintf(buf);
	}
}

int main(void)
{
	bool exec_ok = true;

	try
	{
		progress_bar();
		std::getchar();
		rand_colours();
	}
	catch (_cpf_type::error &e)
	{
		std::cout << e.what() << std::endl;
		exec_ok = false;
	}
	return exec_ok ? EXIT_SUCCESS : EXIT_FAILURE;
}