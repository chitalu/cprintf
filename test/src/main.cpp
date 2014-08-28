#include <cprintf/cprintf.h>
#include <gtest/gtest.h>

TEST(BadToken, using_bitmap_fg_tok_on_windows)
{
	ASSERT_THROW(	cprintf("quux $64f xterm bitmap colour is an error\n"), 
					_cpf_type::error);
	cprintf("$!");
}

TEST(BadToken, using_bitmap_bg_tok_on_windows)
{
	ASSERT_THROW(	cprintf("quux $64b xterm bitmap colour is an error\n"),
					_cpf_type::error);
	cprintf("$!");
}

TEST(BadToken, using_bitmap_fgbg_tok_on_windows)
{
	ASSERT_THROW(	cprintf("quux $128f.16b xterm bitmap colour is an error\n"),
					_cpf_type::error);
	cprintf("$!");
}

TEST(BadToken, using_bitmap_ampersand_fgbg_tok_on_windows)
{
	ASSERT_THROW(	cprintf("quux $64& xterm bitmap colour is an error\n"),
					_cpf_type::error);
	cprintf("$!");
}

TEST(Arg, printing_stl_string)
{
	std::string s = "l love cprintf";
	ASSERT_NO_THROW(cprintf("print std::string %s", s));
	cprintf("$!");
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();
	return ret;
}