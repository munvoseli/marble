This is meant to be a programming language, but graphical, like Scratch.
(it should be noted that i have tried to understand scratch but have failed to understand scratch, so it may not end up working like scratch)

I will now describe the language as if it has already been made.

Marble can be used as both an introductory language, and for more general-purpose programming.

It makes heavy use of different macro-like ways of visualizing a program - you can use real math expressions to express math, specialized struct/record IO has its own optional visual macro, and a sublanguage which serves the purpose that regex does compiles to a parser.  Macros also exist for bitfields and enums.

Function calls feature semantic arguments, where it is visible which arguments are mutated, and which arguments are read from.  Calls may also be laid out in different manners, and display different information.  There is multilingual support, and comments on how functions should be used can be language-specific, with fallback languages.

--

Right now, it is limited to x86, little endian, 64-bit integers, variable assignment to constants, and variable addition.  You cannot make your own functions.

Variable names are made of triangles.

The UI is very bad.  The controls are impossible to describe.
