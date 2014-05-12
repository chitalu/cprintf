#include "cpf_test_common.h"
#include "colour_repr.h"

TEST(Tag_Map_token, fg_bg_colour_test)
{
	TEST_PERIMETER_EMPLACE_BEGIN
	{
		int count = 0;
		for(auto i : _cpf_colour_token_vals )
		{
			ASSERT_NO_THROW(c_printf(stdout,
		    std::string("/").append(i.first).append("]").append("%s").append("/!]\t").c_str(), 
		    i.first.c_str()));
		    if((++count % 4) == 0)
		    {
		        printf("\n");
		    }
		}
	}
	TEST_PERIMETER_EMPLACE_END
}