#include "c_printf.h"

int main(int argc, char const *argv[])
{

	/* test code */
	c_printf(stdout, "\n\ntest foo %d ^r|%f ^g|%s ^b|done.\n\n", 10, 10.0, "hello world!");
	
/*
	auto test_str = 
R"test_foo(

^_60w!|

^b!|#include^c!|<iostream>^!|

^b!|using namespace^!| std;

^b!|int^!| main(^b!|int^!| argc, ^b!|char^!| argv**)
{
    ^b!|int^!| num, factorial = %d;

    cout << ^r!|" Enter Number To Find Its Factorial:  "^!|;

    cin >> num;

    ^b!|for^!|(^b!|int^!| a=1; a <= num; a++)
    {
        factorial = factorial * a;
    }

	cout << ^r!|"Factorial of Given Number is = "^!| << factorial <<endl;

    ^b!|return^!| 0;
}

^_60y!|

^w!m!|this is a demo string: ^!|

)test_foo";
    
	c_printf(stdout, test_str, 21);
*/
	return 0;
}
