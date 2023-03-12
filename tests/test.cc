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
    MyCall M{ the_test };

    try{
        the_test.run();
    }
    catch(const char* ){}
    diagnostic::clear([](diagnostic::log_entry& e) {
        std::cerr << diagnostic::cc(e);
        }
    );

    return 0;
}

test::test(int argc, char** argv):tux::application(argc,argv)
{
    Fini = { "Test Application & MyCall :",acc };
}

test::~test()
{
    std::cerr << '\n'  << __PRETTY_FUNCTION__ << " clear Fini::accumulator...";
    acc.clear();
}

tux::code::M test::init()
{
   

    application::init(); // -> Never-ever forget anymore! ...
    //...
    diagnostic::debug() << " building args processor:";
    /*
     arg(const std::string& opt_name_, char letter_, uint8_t opt_ = 0, int require_narg = 0);
    
    */
    auto text_delegate =
        (args() += {"text", 't', tux::cmd::arg::Required | tux::cmd::arg::ValRequired, 1 })
            .descriptions("Test the \"<stml>\" text  processor.")
            .connect(this, &test::text);
    
    diagnostic::test(sfnll) << "on windows:terminal - " << code::ok;
    return code::ok;
}

tux::code::M test::run()
{
    init();
    std::string str;
    //auto c = tux::color::BlueViolet;
    str = text_str << "<fg:Yellow; Icon:ArrowLeft;>Allo<fg:Aquamarine3;>! <Fg:BlueViolet; Icon:Windows;>";
    diagnostic::test() << code::success << str;
    //...
    (void)args().execute();
    //...
    (void)terminate();
    
    return code::ok;
}



tux::code::M test::terminate()
{
    auto R = Fini(0);
    return code::ok;
}



/*!
 * \brief test::text
 * \param a
 * \param ctx
 * \return
 */
tux::expect<> test::text(tux::cmd::arg& a, tux::cmd::arg::context& ctx)
{
    return tux::code::ok;
}


MyCall::MyCall(test& tt)
{
    tt.connect_term(this, &MyCall::Fini);

}
