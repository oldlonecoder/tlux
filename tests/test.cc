#include "test.h"



using tux::diagnostic;
using tux::code;


auto main(int argc, char** argv) -> int
{
    test the_test(argc, argv);
    the_test.run();
}

test::test(int argc, char** argv)
{

}

test::~test()
{
}

tux::code::T test::init()
{
    application::init(); // -> Never-ever forget anymore! ...
    //...
    diagnostic::test(sfnll) << "on windows:terminal - " << code::ok;
    return code::ok;
}

tux::code::T test::run()
{
    init();
    //text = tux::text("<fg:Yellow Icon:ArrowLeft>Allo!");
    //text.compile();
    std::string str;
    //text >> str;
    auto c = tux::color::BlueViolet;
    str = text << "<fg:Yellow; Icon:ArrowLeft;>Allo<fg:Aquamarine3;>!<Fg:BlueViolet; Icon:Windows;>";
    diagnostic::test() << code::success << str;

    diagnostic::clear([](diagnostic::log_entry& e){
        std::cerr << diagnostic::cc(e);
        }
    );
    return code::ok;
}
