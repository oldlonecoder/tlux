#include <tlux/argc.h>
#include <tlux/strbrk.h>




namespace tux::cmd
{


arg::arg(const std::string &opt_name_, char letter_, uint8_t opt_, int require_narg)
{
    _required_args = require_narg;
    _opt = opt_;
    _opt_name = opt_name_;
    _c = letter_;
}



env_args::env_args(int argc, char** argv)
{
    for (int c = 1; c < argc; c++) _argv.push_back(argv[c]);
    
}

env_args::~env_args()
{
    _args.clear();
    _argv.clear();
}



arg& env_args::operator += (arg&& _arg)
{
    _args.push_back(std::move(_arg));
    return _args.back();

}

arg& env_args::add(arg&& _arg)
{
    _args.push_back(std::move(_arg));
    return _args.back();
}

expect<> env_args::compile()
{
    
    // For now let's put it locally into this stack...
    //-------------------------------------------------
    //std::map<"--",
    //...
    //-------------------------------------------------


    diagnostic::debug(sfnll) << code::test << " dump argv:";
    diagnostic::output() << code::begin << ":";
    for (auto const& A : _argv) diagnostic::output() << A;
    diagnostic::output() << code::end << ";";
    arg::iterator argit = _args.begin();
    std::vector<arg::iterator> parsed_args;
    int c = 0;
    while(c < _argv.size())
    {
        strbrk brk{ _argv[c] };
        strbrk::token_t::list tokens;
        auto n = brk(tokens);
        if (!n) 
            return diagnostic::error() << code::failed << " argument as no data or parse error";


    }





    return code::notimplemented;
}

expect<> env_args::execute()
{
    return code::notimplemented;
}

}