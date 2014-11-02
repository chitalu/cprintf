#include <gtest/gtest.h>
#include <cprintf/cprintf.hpp>

#ifdef _WIN32

TEST(BadToken, using_bitmap_fg_tok_on_windows)
{
	ASSERT_THROW(	cwprintf(L"cpf test :: $64f xterm bitmap colour is an error\n"), 
					cpf::type::except);
	printf("\n");
}

TEST(BadToken, using_bitmap_bg_tok_on_windows)
{
	ASSERT_THROW(	cwprintf(L"cpf test :: $64b xterm bitmap colour is an error\n"),
					cpf::type::except);
	printf("\n");
}

TEST(BadToken, using_bitmap_fgbg_tok_on_windows)
{
	ASSERT_THROW(	cwprintf(L"cpf test :: $128f.16b xterm bitmap colour is an error\n"),
					cpf::type::except);
	printf("\n");
}

TEST(BadToken, using_bitmap_ampersand_fgbg_tok_on_windows)
{
	ASSERT_THROW(	cwprintf(L"cpf test :: $64& xterm bitmap colour is an error\n"),
					cpf::type::except);
	printf("\n");
}

#endif

TEST(Arg, print_std_string)
{
	std::wstring ws = L"c++";
	EXPECT_EQ(ws.size(), 3u);
	ASSERT_NO_THROW(cwprintf(L"cpf test :: print std::wstring :: %s\n", ws));
}

TEST(Arg, print_std_wstring)
{
	std::string s = "c++";
	EXPECT_EQ(s.size(), 3u);
	ASSERT_NO_THROW(cprintf("cpf test :: print std::string :: %s\n", s));
}

TEST(Arg, fmt_specifier_to_arg_count_mismatch)
{
	ASSERT_THROW(	cwprintf(L"cpf test :: %d\n", 1001, "test"),
					cpf::type::except);
}

TEST(Arg, fmt_specifier_to_arg_type_mismatch)
{
	ASSERT_THROW(	cwprintf(L"cpf test :: print std::wstring :: %d %s\n", 1001, "test"),
					cpf::type::except);
}

TEST(Format_String, print_foo_string)
{
	ASSERT_NO_THROW(cwprintf(L"cpf test :: foo\n"));
}

TEST(Format_String, fmt_specifier_with_no_arg)
{
	ASSERT_THROW(	cwprintf(L"cpf test :: %d\n"),
					cpf::type::except);
}

TEST(Token, foo)
{
	ASSERT_NO_THROW(cwprintf(L"cpf test :: $y`red\n"));
}

TEST(Colour, red)
{
	ASSERT_NO_THROW(cwprintf(L"cpf test :: $r`red\n"));
}

int main(int argc, char **argv)
{
	cfprintf(stdout, "test hello 0\n");
	cfwprintf(stdout, L"test hello 1\n");
	cfprintf_dbg(stdout, "debug-print test hello 0\n");
	cfprintf_dbg(stdout, "debug-print test hello 1\n");
	::testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();
	std::getchar();
	return ret;
}