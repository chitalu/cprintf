#include <gtest/gtest.h>
#include <cprintf/cprintf.hpp>
//
//#ifdef _WIN32
//
//TEST(BadToken, using_bitmap_fg_tok_on_windows)
//{
//	ASSERT_THROW(	cwprintf(L"cpf test :: $64f xterm bitmap colour is an error\n"), 
//					cpf::type::except);
//	printf("\n");
//}
//
//TEST(BadToken, using_bitmap_bg_tok_on_windows)
//{
//	ASSERT_THROW(	cwprintf(L"cpf test :: $64b xterm bitmap colour is an error\n"),
//					cpf::type::except);
//	printf("\n");
//}
//
//TEST(BadToken, using_bitmap_fgbg_tok_on_windows)
//{
//	ASSERT_THROW(	cwprintf(L"cpf test :: $128f.16b xterm bitmap colour is an error\n"),
//					cpf::type::except);
//	printf("\n");
//}
//
//TEST(BadToken, using_bitmap_ampersand_fgbg_tok_on_windows)
//{
//	ASSERT_THROW(	cwprintf(L"cpf test :: $64& xterm bitmap colour is an error\n"),
//					cpf::type::except);
//	printf("\n");
//}
//
//#endif
//
TEST(Arg, std_string)
{
	std::wstring ws = L"c++";
	EXPECT_EQ(ws.size(), 3u);
	ASSERT_NO_THROW(cprintf(L"cpf test :: print std::wstring :: %s\n", ws));
}

TEST(Arg, std_wstring)
{
	std::string s = "c++";
	EXPECT_EQ(s.size(), 3u);
	ASSERT_NO_THROW(cprintf("cpf test :: print std::string :: %s\n", s));
}

TEST(Arg, fmt_specifier_to_arg_count_mismatch)
{
	auto r = cprintf(L"cpf test :: %d\n", 1001, "test");
	r;
}

TEST(Arg, fmt_specifier_to_arg_type_mismatch)
{
	cprintf("cpf test :: print std::wstring :: %d %s\n", 1001, "test");
}

TEST(Format_String, print_foo_string)
{
	cprintf("cpf test :: foo\n");
}

TEST(Format_String, fmt_specifier_with_no_arg)
{
	cprintf("cpf test :: %d\n");;
}

TEST(Token, foo)
{
	cprintf("cpf test :: $y`red\n");
}

TEST(Colour, red)
{
	cprintf("cpf test :: $r`red\n");
}


int main(int argc, char **argv)
{
	//ru << "foo";
	auto r = cprintf("test hello %d %s\n", 8, "ljgvlvlv");
	//cprintx(std::string("").c_str());
	char c = 123; short s = 1234; int i = 123456;
	//cprintf_t("", std::make_tuple(0));
	//null pointers not allowed
	cprintf("TEST BINARY\n%d = %b\n%d = %b\n%d = %b\n%p = %b\n", 
			c, c, s, s, i, i, std::string().data(), std::string().data());

	cprintf_dbg("foos the bars");

	//cprintf("test hello 0\n", "8LL");
	//cprintf("test hello 0\n", "8LL", 4, f);
	/*cprintf(L"test hello 1\n");
	cprintf("debug-print test hello 0\n");
	cprintf_dbg("debug-print test hello 1\n");*/
	::testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();
	std::getchar();
	return ret;
}
