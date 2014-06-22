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

For various reasons, the feature-set available on the Microsoft Windows Operating 
System is limited in comparison to that of the Linux Operating System (subject to 
terminal).

For more info, refer to: http://en.tldp.org/HOWTO/Text-Terminal-HOWTO.html


**Go to project Wiki for samples of use**
