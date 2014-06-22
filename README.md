# c_printf #

Welcome to the **c_printf** Project

c_printf is an umbrella name used to encapsulate a group of C++(11/0x) 
language functions for extended formatted-printing. 
The purpose to provide a thin auxiallary layer atop that of (f)printf.
Most functionality works, in much a similar manner, to that of its patternal
counter-part(s)	i.e printf, fprintf etc. 

Aside from guarranteed type-safety (unlike that of it predecesors) 
c_printf also introduces the feature of colour format specification for output 
directed to a terminal. 
This functionality effectively permits users the ability to specify--as part of 
the format string--the colour of all (or some) of the format-string's characters.

In taking advantage of variadic templates, the library's interface also provides
functionality that enables users to opt for the use of a Tuple object as function 
input. This Tuple then acts as the container for the to-be-printed arguments who's 
elements are ordered in a corresponding layout with format specifiers (%...) in 
the format string.  

**Note**

For numerous reasons, the feature-set available on the Microsoft Windows Operating 
System is limited in comparison to that of the Linux Operating System (subject to 
terminal).

For more info, refer to: http://en.tldp.org/HOWTO/Text-Terminal-HOWTO.html
_

Floyd Mulenga Chitalu.

Below is some sample code:


```
#!C++

c_printf("%d foo /y*]bar %s baz", 28, "yellow");

c_printf("left hand side /r*] right hand side");

c_printf_dbg("this is a debugging message");

c_printf("/$s3;s1|b*] test s1 foo s2 bar also s3 baz\n");

auto mytuple = std::make_tuple("c++", "awesome", 101);
c_printf_t("%s is %s: %d", mytuple);

std::get<0>(mytuple) = "You";
c_printf_t("%s is %s: %d", mytuple);
```

Below are more examples taken from [http://www.cplusplus.com/reference/cstdio/printf/](http://www.cplusplus.com/reference/cstdio/printf/)


```
#!C++

c_printf("Characters:/y*] %c %c \n", 'a', 65);
c_printf("Decimals: %d %ld\n", 1977, 650000L);
c_printf("Preceding with blanks: %10d \n", 1977);
c_printf("Preceding with zeros: %010d \n", 1977);
auto args_of_100 = std::make_tuple(100, 100, 100, 100, 100);
c_printf_t("Some different radices:/c*] %d %x %o %#x %#o \n", args_of_100);
c_printf("floats: %4.2f %+.0e %E \n", 3.1416, 3.1416, 3.1416);
c_printf("%s \n", "A string");
c_printf("%-5s", "ab");

c_printf("tuple /#y*]element/!] test: %d", std::get<0>(args_of_100));
```

Results:
![cpf.png](https://bitbucket.org/repo/aRe8Gp/images/2141127256-cpf.png)