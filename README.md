# tuxic
* C++ ancient 1988-esque "tuxic" code using some gems from modern c++-11/20 easiness and other minimal amount of STL goodies.
    * And I have to admit that std::function, however, is a gem I can't live without


#### By order of dependancy:
* utf       : Basic glyph support. ( beware: not utf-* standard, using litteraly ...hardcoded const char* )
              For now it is french accents and some glyphs from the awesome (nerd) fonts.
* textattr  : Define and manage everything about text attributes
    * color-pairs -  I try to support ansi256 and html encoding.
* stracc    : Holds instance of a std::string as (input) accumulator
* object    : OOP... classical object->parent->children relational design. (std::(not so)smart pointers are NOT welcome here!)
* strbrk    : Using string_view and creates array of tokens using provided or default separators
* diagnostic: Diagnostic (and log) messaging using the above tools, used by the tools enumerated below;
* smtl::text: Basic simple markup text utility using the stuff provided by this project.
* tux_signal: Basic signal-slots, from https://schneegans.github.io/tutorials/2015/09/20/signal-slot,
              adapted to this project specifics.

--- I am an old lone coder.

-- more to come...