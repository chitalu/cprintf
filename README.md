# c_printf #

Welcome to the **c_printf** Project

c_printf is an umbrella name used to encapsulate a group of C++(11/0x) 
language functions for extended formatted-printing. 
The project aims to provide a thin auxiallary layer atop that of fprintf.
Most functionality works, in much a similar manner to that of its patternal
counter-part(s)	i.e printf, fprintf etc. 

Aside from guarranteed type-safety (unlike that of it predecesors) 
c_printf also introduces the feature of colour token specification. 
With this, users are able to specify, as part of the format string, the 
colour of all or some of the format-string-text within console output. 
Alongside this is also the addition of map tokens which enable users the 
ability to map specific strings with certain token value(s).

Extending on the use of variadic arguments, c_printf also permits users
the ability to opt for the use of a Tuple object which then acts as a 
container for the to-be-printed arguments ordered in correspondence with 
the argument layout as specified within the format string.  

For numerous reasons the functionailty available on the Windows operating 
system is limited in comparison to that of the *nix family of operating 
systems (subject to terminal). This includes specifying text attributes with the following traits:

* boldness 
* blinking
* extended dim colour specification
* 256 colour support
* and more ...   


Thank you and I hope you enjoy using it in your projects.

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

- *Floyd Chitalu*