#include "test.h"



using tux::diagnostic;
using tux::code;

class MyCall
{
    std::string str{"Allo"};

    


public:

    MyCall() = default;
    MyCall(test& tt);
    ~MyCall() {}

    tux::expect<> Fini(int)
    {
        diagnostic::info(sfnll) << code::success;
        return code::ok;
    }

};



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

tux::code::M test::init()
{
    application::init(); // -> Never-ever forget anymore! ...
    //...

    diagnostic::test(sfnll) << "on windows:terminal - " << code::ok;
    return code::ok;
}

tux::code::M test::run()
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

tux::code::M test::terminate()
{
    auto R = Fini(0);
    return code::ok;
}




MyCall::MyCall(test& tt)
{
    tt.connect_term(this, &MyCall::Fini);

}
