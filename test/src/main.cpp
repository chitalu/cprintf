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
//TEST(Arg, print_std_string)
//{
//	std::wstring ws = L"c++";
//	EXPECT_EQ(ws.size(), 3u);
//	ASSERT_NO_THROW(cwprintf(L"cpf test :: print std::wstring :: %s\n", ws));
//}
//
//TEST(Arg, print_std_wstring)
//{
//	std::string s = "c++";
//	EXPECT_EQ(s.size(), 3u);
//	ASSERT_NO_THROW(cprintf("cpf test :: print std::string :: %s\n", s));
//}
//
//TEST(Arg, fmt_specifier_to_arg_count_mismatch)
//{
//	ASSERT_THROW(	cwprintf(L"cpf test :: %d\n", 1001, "test"),
//					cpf::type::except);
//}
//
//TEST(Arg, fmt_specifier_to_arg_type_mismatch)
//{
//	ASSERT_THROW(	cwprintf(L"cpf test :: print std::wstring :: %d %s\n", 1001, "test"),
//					cpf::type::except);
//}
//
//TEST(Format_String, print_foo_string)
//{
//	ASSERT_NO_THROW(cwprintf(L"cpf test :: foo\n"));
//}
//
//TEST(Format_String, fmt_specifier_with_no_arg)
//{
//	ASSERT_THROW(	cwprintf(L"cpf test :: %d\n"),
//					cpf::type::except);
//}
//
//TEST(Token, foo)
//{
//	ASSERT_NO_THROW(cwprintf(L"cpf test :: $y`red\n"));
//}
//
//TEST(Colour, red)
//{
//	ASSERT_NO_THROW(cwprintf(L"cpf test :: $r`red\n"));
//}
//

const char *byte_to_binary(__int64 x)
{
	static char b[64];
	b[0] = '\0';

	auto z = 1LL << 62LL; // 63LL; = neg
	for (; z > 0; z >>= 1)
	{
		auto h = (x & z);
		strcat(b, ((x & z) == z) ? "1" : "0");
	}

	return b;
}

int main(int argc, char **argv)
{
	// cprintf() << "foo: " << 2 << "\n"; 

	{
		/* binary string to int */

		char *tmp;
		char *b = "0101";

		printf("%d\n", strtol(b, &tmp, 2));
	}

	{
		/* byte to binary string */

		printf("%s\n", byte_to_binary(4LL));
	}

	cprintf_x(8LL);

	auto r = cprintf("test hello 0\n");
	/*cprintf(L"test hello 1\n");
	cprintf("debug-print test hello 0\n");
	cprintf_dbg("debug-print test hello 1\n");*/
	::testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();
	std::getchar();
	return ret;
}