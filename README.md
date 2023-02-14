# tlux
####[<span style="color : brown;">tux</span> penguin & a cup of <span style="color : green;">tea</span> + lux (light)]

* My personal legacy, writting and learning <span style="color : blue;">C++</span> in 1988-esque code-style and some gems from modern c++-11/20 easiness of STL goodies.

This project, exclusively under linux, is a set of basic tools for my other projects that depend on these.
I try ==not== to depend on third party tools, hence this repository.
GCC minimum version requirement for g++ is 11.2 (c++17 and early c++20 support)
***
* "tux" is the root namespace of this repository, though I know that the name ==tux== is used everywhere, so in this project as well ;)
***

#### By order of dependancy:
utf
: Basic glyph support. ( beware: not utf-* standard, using litteraly ...hardcoded const char* )
              For now it is french accents and some glyphs from the awesome (nerd) fonts.

textattr
: Define and manage everything about text attributes
    * color-pairs -  I try to support ansi256 and html encoding.

stracc
: "Composition class", holds instance of a std::string as (input) accumulator

object
: OOP... classical object->parent->children relational design. (std::smart pointers are NOT welcome here)

strbrk
: Using string_view and creates array of tokens using provided or default separators

diagnostic
: Diagnostic (and log) messaging using the above tools, used by the tools enumerated below;

smtl::text
: Basic simple markup text utility using the stuff provided by this project.

tux_signal
: Basic signal-slots, from https://schneegans.github.io/tutorials/2015/09/20/signal-slot,
              adapted to this project specifics.

...More to come:

application
: Base application class for
    * threads management
    * strings/text output encodings {ansi256; html, ...} management
    * async IO (starting in linux epoll, will check about the "not so better" io_submit)
    * messaging
    * central loop, ...

databse
: Very specific and basic sqlite3  support
    * field class
    * table class

