#include <gtest/gtest.h>
#include <cprintf/cprintf.h>

#ifdef _WIN32

TEST(BadToken, using_bitmap_fg_tok_on_windows)
{
	ASSERT_THROW(	cprintf("cpf test :: $64f xterm bitmap colour is an error\n"), 
					cpf::type::except);
	printf("\n");
}

TEST(BadToken, using_bitmap_bg_tok_on_windows)
{
	ASSERT_THROW(	cprintf("cpf test :: $64b xterm bitmap colour is an error\n"),
					cpf::type::except);
	printf("\n");
}

TEST(BadToken, using_bitmap_fgbg_tok_on_windows)
{
	ASSERT_THROW(	cprintf("cpf test :: $128f.16b xterm bitmap colour is an error\n"),
					cpf::type::except);
	printf("\n");
}

TEST(BadToken, using_bitmap_ampersand_fgbg_tok_on_windows)
{
	ASSERT_THROW(	cprintf("cpf test :: $64& xterm bitmap colour is an error\n"),
					cpf::type::except);
	printf("\n");
}

#endif

TEST(Arg, printing_std_string)
{
	std::string s = "l love cprintf";
	ASSERT_NO_THROW(cprintf("cpf test :: print std::string :: %s\n", s));
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();
	std::getchar();
	return ret;
}