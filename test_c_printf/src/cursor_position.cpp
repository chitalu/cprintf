#include "cpf_test_common.h"
#include <sstream>

TEST(Cursor_position, setting_position_basic)
{
	c_printf("hello world /15,15]and again");

	//c_printf("all /15,7]over /4,7]the/10,10]place");
}

TEST(Cursor_position, setting_multiple_positions)
{
	c_printf("all /15,7]over /4,7]the/10,10]place");
}

TEST(Cursor_position, setting_position_and_colour)
{
	c_printf("hello world /15,15]and /r*]again");
}

TEST(Cursor_position, setting_multiple_positions_with_colour)
{
	std::srand((int)time(NULL));

	for (auto i(0); i < 250000; ++i)
	//while (1)
	{
		unsigned char h = std::rand()%50;
		unsigned char w = std::rand()%25;
		char charachter = std::rand()%10 + 5;

		//do{ charachter = std::rand() % 100; } while (charachter == '\"');

		char buf[512];
		try
		{
			sprintf(buf, "/g*]/%d,%d] %c", h, w, charachter);
		}
		catch (_cpf_type::error e)
		{
		}
			c_printf(buf);
	}
		//c_printf("/c*]%c", std::rand() % 100);
}

TEST(Cursor_position, map_string_to_pos_and_colour)
{
	c_printf("/$over;the|15,20.r*y*;y*]it is not all over the place");
}

TEST(Cursor_position, printing_value_at_same_location)
{
	for (int i = 0; i < 100000; ++i)
	{
		c_printf("/0,10]/c*]%d/!]/m*] <- awesome huh", i);
	}
}

TEST(Cursor_position, map_str_to_pos_and_fcst)
{
	c_printf("/$colour;positioning|32&.bld;15,12]i am testing colour and positioning");
}