# tlux
####[<span style="color : brown;">tux</span> penguin & a cup of <span style="color : green;">tea</span> + lux (light)]

* My personal legacy, writting and learning <span style="color : blue;">C++</span> in 1988-esque code-style and some gems from modern c++-11/20 easiness of STL goodies.

This project, exclusively under linux, is a set of basic tools for my other projects that depend on these.
I try ==not== to depend on third party tools, hence this repository.
***
GCC minimum version requirement for g++ is 11.2 (c++17 and early c++20 support)
***
*"tux" is the root namespace of this repository, though I know that the name "tux" is used everywhere, so in this project as well ;)
***

#### By order of dependancy:
utf
: Basic glyph support. ( beware: not utf-* standard. Using hardcoded const char* ...litteraly) For now it is french accents and some (growing list of) glyphs from the (nerd) fonts.

textattr
: Define and manage everything about text attributes and color-pairs
I am in the way to support ansi256 (console/terminal) and html encoding.

stracc
: "Composition class", holds instance of a std::string as (input convertion goodies) accumulator

object
: OOP... classic object->parent->children relational design. (std::smart pointers are NOT very welcome here)

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

database
: Very specific and basic sqlite3  support
    * field class
    * table class

