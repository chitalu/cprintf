#include <gtest/gtest.h>
#include <cprintf/cprintf.h>

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

TEST(Arg, printing_std_string)
{
	std::wstring ws = L"l love cprintf";
	ASSERT_NO_THROW(cwprintf(L"cpf test :: print std::wstring :: %s\n", ws));

	std::string s = "l love cprintf";
	ASSERT_NO_THROW(cprintf("cpf test :: print std::string :: %s\n", s));
}

int main(int argc, char **argv)
{
	cfprintf(stdout, "test hello 0\n");
	cfwprintf(stdout, L"test hello 1\n");
	struct{ int x; }ff;
	auto x = std::stringstream();
	cprintf("foo %s", ff);
	cfprintf_dbg(stdout, "debug-print test hello 0\n");
	cfprintf_dbg(stdout, "debug-print test hello 1\n");
	::testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();
	std::getchar();
	return ret;
}