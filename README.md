# tuxic
* C++ ancient 1988-esque tuxic code using some modern c++-11/20 easiness. Dot.


### I try to NOT using the STL/GNU stdc++. 


#### By order of dependancy:
* utf       : Basic glyph support. ( beware: not utf-* standard, using litteraly ...hardcoded const char* )
              For now it is french accents and some glyphs from the awesome (nerd) fonts.
* textattr  : Define and manage everything about text attributes (color/pairs, ansi256 and html enconding)
* stracc    : Holds instance of a std::string as (input) accumulator
* strbrk    : Using string_view and creates array of tokens using provided or default separators
* diagnostic: Diagnostic (and log) messaging using the above tools, used by the tools enumerated below;
* smtl      : Basic simple markup text utility using the stuff provided by this project.

* cnt : Set of experimental standalone containers to avoid the STL usage...

* In parts of the utils above, I try to support ansi256 and html encoding.
