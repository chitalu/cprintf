#include "cpf_test_common.h"

TEST(Block_space_token, escape_blk_spc_token_prefix)
{
	auto t_str = "`/¬5<i am yellow>=y!¬]";
	_CPF_OSTR_PRINT(printf("%s", t_str));
	ASSERT_NO_THROW(c_printf(stdout, t_str));
}