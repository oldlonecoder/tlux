#include <tlux/cc_arg.h>




namespace tux {


namespace cmd
{
arg::arg(const std::string &opt_name_, char letter_, uint8_t opt_, int require_narg)
{
    _required_args = require_narg;
    _opt = opt_;
    _opt_name = opt_name_;
    _c = letter_;
}

}

cc_args::~cc_args()
{
    _args.clear();
}

code::T cc_args::process_args(int argc, char **argv)
{
    return code::T();
}


cc_args &cc_args::operator<<(cmd::arg* _arg)
{
    _args.push_back(_arg);
    return *this;
}

}
