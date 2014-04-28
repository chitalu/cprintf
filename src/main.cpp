#include "c_printf.h"

int main(int argc, char const *argv[])
{
	/* test code */
	/*TODO ADD SUPPORT FOR STRING CONTAINING NO BLOCK SPACES*/
	//c_printf(stdout, "test foo %d ${c!} %f ${y!b!} %s ${g!c!} done.", 10, 10.0, "hello world!");
	

	auto test_str = 
R"test_foo(

${|60w!}

${b!}#include${c!}<iostream>${!}

${b!}using namespace${!} std;

${b!}int${!} main(${b!}int${!} argc, ${b!}char${!} argv**)
{
    ${b!}int${!} num, factorial = %d;

    cout << ${r!}" Enter Number To Find Its Factorial:  "${!};

    cin >> num;

    ${b!}for${!}(${b!}int${!} a=1; a <= num; a++)
    {
        factorial = factorial * a;
    }

	cout << ${r!}"Factorial of Given Number is = "${!} << factorial <<endl;

    ${b!}return${!} 0;
}

${|60y!}

${w!m!}this is a demo string: %s${!}

)test_foo";

	auto x = static_cast<unsigned char>(ext_ASCII::latin_lcase_f);
	c_printf(stdout, test_str, 21, "LOOK MA' IT WORKS!");

	return 0;
}