#include "cpf_test_common.h"

TEST(Tag_Map_token, one_to_one_tag_map_at_beginning_of_format_string)
{
		ASSERT_NO_THROW(
		c_printf(
		stdout, "/$s1|(b!)] test s1 foo s2 bar also s3 baz\n")
		);
}

TEST(Tag_Map_token, using_two_to_one_tag_map)
{
		ASSERT_NO_THROW(c_printf(stdout, "/$s3:s1|(b!)] test s1 foo s2 bar also s3 baz\n"));
}

TEST(Tag_Map_token, using_two_to_two_tag_map)
{
	ASSERT_NO_THROW(
	c_printf(stdout, "quux bax /$s1:s3|(r!)(b!)] example s1 foo s2 bar food s3 baz\n")
	);
}

TEST(Tag_Map_token, using_tag_map_with_tag_token_inhibitor)
{
		ASSERT_NO_THROW(
		c_printf(
		stdout,
		"quux bax /$s1:s2:s3|(r!)(b!)(yw)] ex s1 foo s2 bar this is also s3 baz /$]the rest s2 foo s3 \n")
		);
	
}

TEST(Tag_Map_token, using_only_tag_token_inhibitor)
{
	ASSERT_NO_THROW(
		c_printf(stdout, "quux bax /$]the rest s2 foo s3 \n"));
}

TEST(Tag_Map_token, basic_cplusplus_code_tag)
{
		auto code_str =
		R"code_str(
/$int:char:float:unsigned:return|(b!)]
int main(int argc, char** argv)
{
	float x = 0;
	for(unsigned int i(0); i < 10; ++i)
	{
		x *= i;
	}
	return 0;
}
)code_str";

		ASSERT_NO_THROW(c_printf(stdout, code_str));
}

TEST(Tag_Map_token, using_tag_map_at_end_of_format_string)
{
		ASSERT_NO_THROW(
		c_printf(
		stdout,
		"quux 666 ex s1 foo s2 bar this is also s3 baz the rest s2 foo s3 \n/$s1:s2:s3|(r!)(b!)(yw)]")
		);
}

TEST(Tag_Map_token, basic_cplusplus_code_tag_with_arg)
{
		auto tag_token_test_string1 =
R"test_string(
	/$int:while:true:%d|(b!)]
	int c = %d;
	while(true)
	{
		c++; 
	}
)test_string";

		ASSERT_NO_THROW(c_printf(stdout, tag_token_test_string1, 10));
}