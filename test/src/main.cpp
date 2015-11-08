#include <gtest/gtest.h>
#include <cprintf/cprintf.hpp>
//
//#ifdef _WIN32
//
// TEST(BadToken, using_bitmap_fg_tok_on_windows)
//{
//	ASSERT_THROW(	cwprintf("$64f xterm bitmap colour is an
// error\n"),
//					cpf::type::except);
//	printf("\n");
//}
//
// TEST(BadToken, using_bitmap_bg_tok_on_windows)
//{
//	ASSERT_THROW(	cwprintf("$64b xterm bitmap colour is an
// error\n"),
//					cpf::type::except);
//	printf("\n");
//}
//
// TEST(BadToken, using_bitmap_fgbg_tok_on_windows)
//{
//	ASSERT_THROW(	cwprintf("$128f.16b xterm bitmap colour
// is an error\n"),
//					cpf::type::except);
//	printf("\n");
//}
//
// TEST(BadToken, using_bitmap_ampersand_fgbg_tok_on_windows)
//{
//	ASSERT_THROW(	cwprintf("$64& xterm bitmap colour is an
// error\n"),
//					cpf::type::except);
//	printf("\n");
//}
//
//#endif
//
TEST(Args, std_string) {
  std::string ws = "c++";
  EXPECT_EQ(ws.size(), 3u);
  ASSERT_NO_THROW(cprintf("std::string = %s\n", ws));
}

TEST(Args, std_wstring) {
  std::wstring s = L"c++";
  EXPECT_EQ(s.size(), 3u);
  ASSERT_NO_THROW(cprintf("std::wstring = %S\n", s));
}

TEST(Args, fmt_specifier_to_arg_count_mismatch) {
  auto r = cprintf("%d\n", 1001, "test");
}

TEST(Args, fmt_specifier_to_arg_type_mismatch) {
   auto r = cprintf("print std::wstring :: %d %s\n", 1001, "test");
}

TEST(FormatString, print_foo_string) {  auto r = cprintf("foo\n"); }

TEST(FormatString, fmt_specifier_with_no_arg) {
   auto r = cprintf("%d\n");
  ;
}

TEST(Token, foo) { auto r = cprintf("$y`red\n"); }

TEST(BinaryFormatSpecifier, type_int32)
{
  int value = 0b1111000011110000;
   auto r = cprintf("value = %b\n", value);
}

TEST(BinaryFormatSpecifier, type_uint32)
{
  unsigned int value = 0b1111000011110000;
   auto r = cprintf("value = %b\n", value);
}

int main(int argc, char **argv) { 
  ::testing::InitGoogleTest(&argc, argv);
  int ret = RUN_ALL_TESTS();
  std::getchar();
  return ret;
}
