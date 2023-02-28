
#include <tlux/application.h>
#include <tlux/cc_arg.h>


namespace tux
{

class tests : public application
{

    cc_arg<tests>::list _args;
public:

    tests() = default;
    ~tests() override;

    code::T init(int argc, char** argv) override;

private:
    code::T setup_db(cc_arg<tests>& arg);
};


class ttt
{
public:
    code::T setup_db(cc_arg<tests>& en);

};

ttt te;

code::T tests::init(int argc, char **argv)
{
    application::init(argc,argv);
    _args.push_back(
        cc_arg(this, &tests::setup_db, "dbname",(char)0,0,0 )
    );
    return code::notimplemented;
}

code::T tests::setup_db(cc_arg<tests> &arg)
{
    return code::notimplemented;
}


tests::~tests()
{}
}




auto main(int argc, char** argv) -> int
{
    tux::tests tests;
    auto r = tests.init(argc, argv);


    return 0;
}
