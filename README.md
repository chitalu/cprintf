# cprintf #

Welcome to the **cprintf** Project

cprintf is an umbrella name used to encapsulate a group of C++(11/0x) 
language functions for extended formatted-printing. 
The purpose to provide a thin auxiallary layer atop that of (f)printf.
Most functionality works, in much a similar manner, to that of its patternal
counter-part(s)	i.e printf, fprintf etc. 

Aside from guarranteed type-safety (unlike that of it predecesors) 
cprintf also introduces the feature of text-colour specification ($...) for output 
directed to a terminal. 
This functionality effectively permits users the ability to specify--as part of 
the format string--the colour of all (or some) of the format-string's characters.

In taking advantage of variadic templates, the library's interface also provides
functionality that enables users to opt for the use of a Tuple object as function 
input. This Tuple then acts as the container for the to-be-printed arguments who's 
elements are ordered in a corresponding layout with format specifiers (%...) in 
the format string.  

**Note**

This project is still in early stages of development and as such it should be 
expected that changes (sometimes drastic) will be made to the current implementation.

The feature-set available on the Microsoft Windows Operating System is limited in 
comparison to that of the Linux Operating System (subject to terminal). Users wishing
to target older hardware interfaces may be required to limit their use of features 
available for compatilibity.

For more info, refer to: http://en.tldp.org/HOWTO/Text-Terminal-HOWTO.html

**See project Wiki for samples of use**
